//lua_scripted_pattern.h

#pragma once

#include <lua.hpp>
#include <map>
#include <string>
#include <utility>
#include "enemy_act_pattern.h"

namespace MyGameProject
{
	template<typename Derived>
	struct LuaScripted : Pattern
	{
	private:
		using StatePtr = std::unique_ptr<lua_State, decltype(&lua_close)>;
		using FunctionBinderList = std::map<std::string, lua_CFunction>;

		//String type adaptors
		template<typename String>
		static const char* get_impl(String&& _str, ...) { return _str; }
		template<typename String>
		static auto get_impl(String&& _str, int) ->decltype(_str.c_str()) { return _str.c_str(); }
		template<typename String>
		static auto get(String&& _str) { return get_impl(_str, 0); }

		//Static lua modules
		static StatePtr L;
		static std::string coroutine_name;
		static FunctionBinderList enemy_memfunc_binders;
		static FunctionBinderList free_func_binders;

		//Unique lua modules
		lua_State* this_thread;

	protected:
		template<typename String>
		static void register_enemy_memfunc(lua_CFunction _cfunc, String&& _name)
		{
			enemy_memfunc_binders[_name] = _cfunc;
		}
		template<typename String>
		static void register_free_func(lua_CFunction _cfunc, String&& _name)
		{
			free_func_binders[_name] = _cfunc;
		}
		static Derived* to_this(lua_State* _state)
		{
			return static_cast<Derived*>(lua_touserdata(_state, lua_upvalueindex(1)));
		}
	public:
		template<typename String0, typename String1>
		static void apply_script(String0&& _file_name, String1&& _coroutine_name)
		{
			L = StatePtr(luaL_newstate(), &lua_close);

			luaL_openlibs(L.get());
			luaL_dofile(L.get(), get(std::forward<String0>(_file_name)));
			coroutine_name = std::forward<String1>(_coroutine_name);
		}

		LuaScripted(void) :this_thread(lua_newthread(L.get())) {}

		OVERRIDE_PARENTHESIS
		{
			lua_getglobal(this_thread, get(coroutine_name));
			lua_newtable(this_thread);
			lua_newtable(this_thread);
			lua_newtable(this_thread); 

			for (auto&& f : enemy_memfunc_binders)
			{
				lua_pushlightuserdata(this_thread, &_enemy);
				lua_pushcclosure(this_thread, f.second, 1);
				lua_setfield(this_thread, -2, get(f.first));
			}
			for (auto&& f : free_func_binders)
			{
				lua_pushlightuserdata(this_thread, static_cast<Derived*>(this));
				lua_pushcclosure(this_thread, f.second, 1);
				lua_setfield(this_thread, -2, get(f.first));
			}

			lua_setfield(this_thread, -2, "__index");
			lua_setmetatable(this_thread, -2);
			lua_resume(this_thread, nullptr, 1);
		}
	};

	template<typename D> typename LuaScripted<D>::StatePtr LuaScripted<D>::L(nullptr, &lua_close);
	template<typename D> std::string LuaScripted<D>::coroutine_name;
	template<typename D> typename LuaScripted<D>::FunctionBinderList LuaScripted<D>::enemy_memfunc_binders;
	template<typename D> typename LuaScripted<D>::FunctionBinderList LuaScripted<D>::free_func_binders;
}
