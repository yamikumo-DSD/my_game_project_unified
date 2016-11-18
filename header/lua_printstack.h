//lua_printstack.h

#pragma once

#include <lua.hpp>
#include <iostream>

inline void lua_printstack(lua_State* _state, std::ostream& _os)
{
	for (int i = lua_gettop(_state); i > 0; --i)
	{
		_os << '[' << i << ']' << ' ';
		switch (lua_type(_state, i))
		{
		case LUA_TNIL: _os << "nil" << std::endl;                                     break;
		case LUA_TBOOLEAN: _os << "bool" << ": " << lua_toboolean(_state, i) << std::endl; break;
		case LUA_TLIGHTUSERDATA: _os << "light user data" << std::endl;                                     break;
		case LUA_TNUMBER: _os << "number" << ": " << lua_tonumber(_state, i) << std::endl; break;
		case LUA_TSTRING: _os << "string" << ": " << lua_tostring(_state, i) << std::endl; break;
		case LUA_TTABLE: _os << "table" << std::endl;                                     break;
		case LUA_TFUNCTION: _os << "function" << std::endl;                                     break;
		case LUA_TUSERDATA: _os << "user data" << std::endl;                                     break;
		case LUA_TTHREAD: _os << "thread" << std::endl;                                     break;
		}
	}
}
