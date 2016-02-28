//pad.h

#ifndef __MYLIB_GP_PAD_H
#define __MYLIB_GP_PAD_H

#include "DxLib.h"
#include <filesystem>
#include <cstdio>
#include <string>

namespace gp
{
	const int PAD_BUTTON_NUM = 16;

	class PadConfig
	{
		private:
			int left,right,up,down,shot,bom,slow,start,change;
		public:
			void set_default(void);
			PadConfig(void);
			PadConfig(int const& _left,int const& _right,int const& _up,int const& _down,int const& _shot,int const& _bom,int const& _slow,int const& _start,int const& _change);
			void set_config(int const& _left,int const& _right,int const& _up,int const& _down,int const& _shot,int const& _bom,int const& _slow,int const& _start,int const& _change);
			const int get_left(void) const;
			const int get_right(void) const;
			const int get_up(void) const;
			const int get_down(void) const;
			const int get_shot(void) const;
			const int get_bom(void) const;
			const int get_slow(void) const;
			const int get_start(void) const;
			const int get_change(void) const;
			void load_pad_config(void);
			PadConfig& operator=(PadConfig const& _pad_config);
			PadConfig(PadConfig const& _pad_config);
	};

	extern PadConfig default_config_pad;

	class tagPadTime
	{
		private:
			int mul;
			int pad_input_flags;
			int pad_time[PAD_BUTTON_NUM];
		public:
			void update(void);
			const int operator[](int const& pad_macro);
			const int get(int const& pad_macro);
	};

	extern tagPadTime pad_time;
}

#endif

