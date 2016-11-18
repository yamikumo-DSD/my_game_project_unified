//pattern114.cpp

#pragma once

#include "lua_scripted_pattern.h"
#include "debug_value.h"

namespace MyGameProject
{
	struct Pattern114 final : LuaScripted<Pattern114>
	{
		int x{0};
		Pattern114(void) {}
		Pattern114(int _x):x(_x) {}
		void print_x(void) { debug_value(x); }
		static void preparation(void);
	};
}
