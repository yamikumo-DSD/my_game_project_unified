//color.h

#ifndef __MYLIB_GP_COLOR_H
#define __MYLIB_GP_COLOR_H

#include "DxLib.h"

namespace gp
{
	class Color
	{

		private:
			int r, g, b;
		public:
			Color(int _r, int _g, int _b);
			Color(void);
			Color(const Color& color);
			void set(int _r, int _g, int _b);
			const int get(void) const; //return integer value which returned by GetColor(int,int,int)
			const int red(void) const;
			const int green(void) const;
			const int blue(void) const;
			Color& operator=(const Color& _color);
	};

	extern Color red,green,blue,black,white,sky;
}

#endif
