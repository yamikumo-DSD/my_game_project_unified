//bindclass.h

#pragma once

#include <lua.hpp>
#include "common.h"

namespace MyGameProject
{
	namespace LuaUtilities
	{

		template<typename CXXClass>
		class RegisterCXXClass
		{
		private:
			lua_State* L;
			std::string name;
		public:
			RegisterCXXClass(lua_State* _state, const char* _class_name) :L(_state), name(_class_name)
			{
				lua_newtable(L); // Class body
				lua_newtable(L); // Metatable
				lua_newtable(L); // Method list
			}

			//Return 1 value.
			template<typename R, typename... Args>
			auto add_memfunc(R(CXXClass::*_memfunc)(Args...), const char* _name)->std::enable_if_t<detail::is_plain_type<R>::value>
			{
				using MemFunc = R(CXXClass::*)(Args...);
				auto memfunc{ static_cast<MemFunc*>(lua_newuserdata(L, sizeof(MemFunc))) };
				new(memfunc) MemFunc(_memfunc);
				lua_pushcclosure
				(
					L,
					[](lua_State* _state)->int
					{
						auto self{ static_cast<CXXClass*>(lua_touserdata(_state, 1)) };
						auto memfunc_ptr{ static_cast<MemFunc*>(lua_touserdata(_state, lua_upvalueindex(1))) };
						auto args{ detail::to_cvalues<Args...>(_state, 1) };
						auto result
						{
							detail::call_with_expanded_tuple
							(
								[self, memfunc_ptr](Args... _args) { return (self->*(*memfunc_ptr))(_args...); },
								args
							)
						};
						detail::push_cvalue(_state, result);
						return 1;
					},
					1
				);
				lua_setfield(L, -2, _name);
			}
			//Return 0 value.
			template<typename... Args>
			void add_memfunc(void(CXXClass::*_memfunc)(Args...), const char* _name)
			{
				using MemFunc = void(CXXClass::*)(Args...);
				auto memfunc{ static_cast<MemFunc*>(lua_newuserdata(L, sizeof(MemFunc))) };
				new(memfunc) MemFunc(_memfunc);
				lua_pushcclosure
				(
					L,
					[](lua_State* _state)->int
				{
					auto self{ static_cast<CXXClass*>(lua_touserdata(_state, 1)) };
					auto memfunc_ptr{ static_cast<MemFunc*>(lua_touserdata(_state, lua_upvalueindex(1))) };
					auto args{ detail::to_cvalues<Args...>(_state, 1) };
					detail::call_with_expanded_tuple
					(
						[self, memfunc_ptr](Args... _args) { return (self->*(*memfunc_ptr))(_args...); },
						args
					);
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
				using MemFunc = Struct<Rs...>(CXXClass::*)(Args...);
				auto memfunc{ static_cast<MemFunc*>(lua_newuserdata(L, sizeof(MemFunc))) };
				new(memfunc) MemFunc(_memfunc);
				lua_pushcclosure
				(
					L,
					[](lua_State* _state)->int
					{
						auto self{ static_cast<CXXClass*>(lua_touserdata(_state, 1)) };
						auto memfunc_ptr{ static_cast<MemFunc*>(lua_touserdata(_state, lua_upvalueindex(1))) };
						auto args{ detail::to_cvalues<Args...>(_state, 1) };
						auto result
						{
							detail::call_with_expanded_tuple
							(
								[self, memfunc_ptr](Args... _args) { return (self->*(*memfunc_ptr))(_args...); },
								args
							)
						};
						//Call push_cvalue for each values of the result.
						boost::fusion::for_each(result, [state = _state](auto&& _v) {detail::push_cvalue(state, std::forward<decltype(_v)>(_v));});
						//Return the number of elements of the result.
						return boost::fusion::size(result);
					},
					1
				);
				lua_setfield(L, -2, _name);
			}

			//After all member functions are registered, 
			//register new adaptor(allocator/constructor) and finalize the class.
			template<typename... Args>
			void finalize(void*(*_new)(Args...))
			{
				lua_setfield(L, -2, "__index");
				lua_pushlightuserdata(L, _new);

				lua_pushcclosure
				(
					L,
					[](lua_State* _state)->int
					{
						//Upvalue 1: metatable
						//Upvalue 2: new adaptor

						//Allocate and construct new object.
						auto new_{static_cast<void*(*)(Args...)>(lua_touserdata(_state, lua_upvalueindex(2)))};
						auto initializer{detail::to_cvalues<Args...>(_state, 1)};
						auto new_obj{detail::call_with_expanded_tuple(new_, initializer)};
						lua_pushlightuserdata(_state, new_obj);

						//Set the metatable.
						lua_pushvalue(_state, lua_upvalueindex(1));
						lua_setmetatable(_state, -2);

						//Return new object(table).
						return 1;
					},
					2
				);
				lua_setfield(L, -2, "new");

				//Finally, register the class to the global field.
				lua_setglobal(L, name.c_str());
			}
		};

	}
}
