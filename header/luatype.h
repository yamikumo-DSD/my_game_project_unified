//luatype.h

#pragma once

#include <lua.hpp>

namespace MyGameProject
{
	namespace LuaUtilities
	{
		inline const char* get_typename(lua_State* _state, int _idx)
		{
			return lua_typename(_state, lua_type(_state, _idx));
		}
	}
}
