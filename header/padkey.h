//padkey.h

#ifndef __MYLIB_GP_PAD_KEY_H
#define __MYLIB_GP_PAD_KEY_H

#include "keyboard.h"
#include "pad.h"

namespace gp
{
	class InputTime
	{
		private:
			gp::PadConfig pad_config;
			gp::KeyboardConfig keyboard_config;
		public:
			InputTime(PadConfig const& _pad_config,KeyboardConfig const& _keyboard_config);
			const int right(void) const;
			const int left(void) const;
			const int up(void) const;
			const int down(void) const;
			const int shot(void) const;
			const int bom(void) const;
			const int start(void) const;
			const int slow(void) const;
			const int change(void) const;
			int update(void);
			void load_config(void);
	};

	extern InputTime input_time_default;
}

#endif

