//dxsstream.cpp

#ifndef __MYLIB_GP_DX_SSTREAM_H
#define __MYLIB_GP_DX_SSTREAM_H

#include <sstream>
#include "DxLib.h"
#include "color.h"

namespace gp
{
	class End{};
	class NewLine{};

	extern End end;
	extern NewLine nline;

	const int HEIGHT_OF_CHARACTOR = 15;

	class DX_SStream
	{
		private:
			int x,y; //Coordinate
			int buf_y;
			Color color;
			std::stringstream buf;
		public:
			DX_SStream(int _x, int _y);
			template<class T> DX_SStream& operator<<(T const& arg)
			{
				buf << arg;
				return *this;
			}
			DX_SStream& operator<<(Color& _color);
			template<> DX_SStream& operator<<(NewLine const& nline)
			{
				static const std::string empty_str;
				auto str_buf = buf.str();
				DrawString(x, y, str_buf.c_str(), color.get());
				y += HEIGHT_OF_CHARACTOR;
				buf.str(empty_str);
				return *this;
			}
			void operator<<(End const& end);
			
	};

	typedef DX_SStream dxout;
}

#endif

