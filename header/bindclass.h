//bindclass.h

#pragma once

#include <boost/fusion/include/vector.hpp>
#include <boost/fusion/algorithm/iteration/for_each.hpp>
#include "common.h"

namespace MyGameProject
{
	namespace LuaUtilities
	{
		//Bind C++ class to Lua.
		//C++ object is weak-bound to lua object.
		template<typename CXXClass>
		class RegisterCXXClass
		{
		private:
			using Ref     = std::shared_ptr<CXXClass>;
			using WeakRef = std::weak_ptr  <CXXClass>;

			lua_State* L;
			const char* name;

			template<typename... Args, typename F>
			static auto call(lua_State* _state, F _functor, int _first_arg_index)
			{
				auto args{ detail::to_cvalues<Args...>(_state, _first_arg_index) };
				return detail::call_with_expanded_tuple
				(
					[f = _functor](Args... _args)
					{
						return f(_args...);
					}, args
				);
			}

			template<typename T, typename R, typename... Args>
			static auto bind(R(T::*_memfunc_ptr)(Args...), T* _obj)
			{
				return [memf = _memfunc_ptr, obj = _obj](Args... _args)
				{
					return (obj->*memf)(_args...);
				};
			}
		public:
			RegisterCXXClass(lua_State* _state, const char* _class_name) :L(_state), name(_class_name)
			{
				lua_newtable(L); // Class body
				lua_newtable(L); // Metatable
				lua_newtable(L); // Method list

				// Method 'expired' checks if the bound object is invalidated or not.
				lua_pushcfunction
				(
					L, 
					[](lua_State* _state)
					{
						lua_getfield(_state, 1, "weak_ref");
						auto weak_ref{ *static_cast<WeakRef*>(lua_touserdata(_state, -1)) };
						lua_settop(_state, 0);
						lua_pushboolean(_state, weak_ref.expired());
						return 1;
					}
				);
				lua_setfield(L, -2, "expired");
			}

			//Helper to get self pointer.
			//Doesn't work if self is poped before call of this method.
			//You should use this method to cash the self pointer before all in the glue functions.
			static CXXClass* self(lua_State* _state)
			{
				lua_getfield(_state, 1, "this");
				auto this_{ lua_touserdata(_state, -1) };
				return static_cast<CXXClass*>(this_);
			}
			
			//Return 1 value.
			template<typename R, typename... Args>
			auto add_memfunc(R            (CXXClass::*_memfunc)(Args...), const char* _name)->std::enable_if_t<detail::is_plain_type<R>::value>
			{
				using MemFunc = decltype(_memfunc);
				detail::construct_userdata(L, _memfunc);
				lua_pushcclosure
				(
					L,
					[](lua_State* _state)->int
					{
						auto memfunc_ptr{detail::upvalue_cast<MemFunc*>(_state, 1)};
						auto result{call<Args...>(_state, bind(*memfunc_ptr, self(_state)), 2)};
						detail::push_cvalue(_state, result);
						return 1;
					},
					1
				);
				lua_setfield(L, -2, _name);
			}
			//Return 0 value.
			template<typename... Args>
			void add_memfunc(void         (CXXClass::*_memfunc)(Args...), const char* _name)
			{
				using MemFunc = decltype(_memfunc);
				detail::construct_userdata(L, _memfunc);
				lua_pushcclosure
				(
					L,
					[](lua_State* _state)->int
					{
						auto memfunc_ptr{detail::upvalue_cast<MemFunc*>(_state, 1)};
						call<Args...>(_state, bind(*memfunc_ptr, self(_state)), 2);
						return 0;
					},
					1
				);
				lua_setfield(L, -2, _name);
			}
			//Return multiple values.
			template<typename... Rs, typename... Args>
			void add_memfunc(Struct<Rs...>(CXXClass::*_memfunc)(Args...), const char* _name)
			{
				using MemFunc = decltype(_memfunc);
				detail::construct_userdata(L, _memfunc);
				lua_pushcclosure
				(
					L,
					[](lua_State* _state)->int
					{
						auto memfunc_ptr{detail::upvalue_cast<MemFunc*>(_state, 1)};
						auto result{call<Args...>(_state, bind(*memfunc_ptr, self(_state)), 2)};
						//Call push_cvalue for each values of the result.
						boost::fusion::for_each(result, [state = _state](auto&& _v) {detail::push_cvalue(state, std::forward<decltype(_v)>(_v));});
						//Return the number of elements of the result.
						return boost::fusion::size(result);
					},
					1
				);
				lua_setfield(L, -2, _name);
			}

			//This will be needed if a returned value or arguments are of not lua compatible type.
			//
			//                 <<    EXAMPLE    >>
			//
			// RegisterCXXClass<Widget> w(lua_state, "Widget");
			// w.adapt_memfunc
			// (
			// 	[](lua_State* L)
			// 	{
			// 		auto self = self(L);
            // 
			//      //Process using self pointer.
			//      int result = self->method();
			//      lua_pushinteger(L, result);
            //
			//      //Number of retuned values.
			//      return 1;
			// 	}, "get" //Register this member of name "get"
			// );
			void adapt_memfunc(lua_CFunction _glue_func, const char* _name)
			{
				lua_pushcfunction(L, _glue_func);
				lua_setfield(L, -2, _name);
			}

			template<typename... Args>
			void finalize(std::shared_ptr<CXXClass>(*_make_shared)(Args...))
			{
				lua_setfield(L, -2, "__index");
				lua_pushlightuserdata(L, _make_shared);

				lua_pushcclosure
				(
					L,
					[](lua_State* _state)->int
					{
						//Upvalue 1: metatable
						//Upvalue 2: new adaptor

						//Allocate and construct new object.
						auto make_shared{static_cast<decltype(_make_shared)>(lua_touserdata(_state, lua_upvalueindex(2)))};
						auto initializer{detail::to_cvalues<Args...>(_state, 1)};
						auto ref{detail::call_with_expanded_tuple(make_shared, initializer)};

						//Clear all arguments.
						lua_settop(_state, 0);

						//Object structure:
						//{
						//    this = pointer to C++ object,
						//    weak_ref = weak reference to the object
				        //}
						lua_newtable(_state);
						auto userdata{ static_cast<WeakRef*>(lua_newuserdata(_state, sizeof(WeakRef)) )};
						new(userdata) WeakRef(ref);
						lua_setfield(_state, -2, "weak_ref");
						lua_pushlightuserdata(_state, ref.get());
						lua_setfield(_state, -2, "this");

						//Set the metatable.
						lua_pushvalue(_state, lua_upvalueindex(1));
						lua_setmetatable(_state, -2);

						//Return new object.
						return 1;
					},
					2
				);
				lua_setfield(L, -2, "new");

				//Finally, register the class to the global field.
				lua_setglobal(L, name);
			}
		};

	}
}