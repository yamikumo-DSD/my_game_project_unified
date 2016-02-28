//dxsstream.cpp

#include "dxsstream.h"

namespace gp
{
	End end;
	NewLine nline;
}

gp::DX_SStream::DX_SStream(int _x,int _y):x(_x),y(_y),buf_y(_y),color(gp::white){}
void gp::DX_SStream::operator<<(End const& end)
{
	static const std::string empty_str;
	auto str_buf = buf.str();
	DrawString(x,y,str_buf.c_str(),color.get());
	buf.str(empty_str);
	y = buf_y;
}
gp::DX_SStream& gp::DX_SStream::operator<<(Color& _color)
{
	color = _color;
	return *this;
}