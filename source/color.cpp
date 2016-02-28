//color.cpp

#include "color.h"

gp::Color::Color(int _r,int _g,int _b):r(_r),g(_g),b(_b){}
gp::Color::Color(const Color& color)
{
	this->r = color.red();
	this->g = color.green();
	this->b = color.blue();
}
void gp::Color::set(int _r,int _g,int _b){r = _r;g = _g;b = _b;}
const int gp::Color::get(void) const{ return GetColor(r,g,b); }
const int gp::Color::red(void) const { return r; }
const int gp::Color::green(void) const { return g; }
const int gp::Color::blue(void) const { return b; }
gp::Color& gp::Color::operator=(const Color& color)
{
	this->r = color.red();
	this->g = color.green();
	this->b = color.blue();
	return *this;
}

namespace gp
{
	Color red(255,0,0);
	Color green(0,255,0);
	Color blue(0,0,255);
	Color black(0,0,0);
	Color white(255,255,255);
	Color sky(120, 255, 255);
}