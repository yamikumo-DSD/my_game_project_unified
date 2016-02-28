//keyboard.h

#ifndef __MYLIB_GP_KEYBOARD_H
#define __MYLIB_GP_KEYBOARD_H

#include "DxLib.h"
#include <filesystem>
#include <cstdio>
#include <string>

namespace gp
{
	const int KEY_NUM = 256;

	class KeyboardConfig
	{
		private:
			int left,right,up,down,shot,bom,slow,start,change;
		public:
			void set_default(void);
			KeyboardConfig(void);
			KeyboardConfig(int const& _left, int const& _right, int const& _up, int const& _down, int const& _shot, int const& _bom, int const& _slow, int const& _start, int const& _change);
			void set_config(int const& _left, int const& _right, int const& _up, int const& _down, int const& _shot, int const& _bom, int const& _slow, int const& _start, int const& _change);
			const int get_left(void) const;
			const int get_right(void) const;
			const int get_up(void) const;
			const int get_down(void) const;
			const int get_shot(void) const;
			const int get_bom(void) const;
			const int get_slow(void) const;
			const int get_start(void) const;
			const int get_change(void) const;
			void load_keyboard_config(void);
			KeyboardConfig& operator=(KeyboardConfig const& _keyboard_config);
			KeyboardConfig(KeyboardConfig const& _keyboard_config);
	};

	extern KeyboardConfig default_config_keyboard;

	class tagKeyTime
	{
		private:
			char key_state[KEY_NUM];
			int key_time[KEY_NUM];
		public:
			int update(void);
			const int operator[](int key_macro) const;
			const int get(int key_macro) const;
	};

	extern tagKeyTime key_time;
}


#endif

