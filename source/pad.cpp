//pad.cpp

#include "pad.h"

void gp::PadConfig::set_default(void)
{
	left = 1; right = 2; up = 3; down = 0; shot = 5; bom = 4; slow = 11; start = 13; change = 6;
}
gp::PadConfig::PadConfig(void){ set_default(); }
gp::PadConfig::PadConfig(int const& _left, int const& _right, int const& _up, int const& _down, int const& _shot, int const& _bom, int const& _slow, int const& _start, int const& _change) :left(_left), right(_right), up(_up), down(_down), shot(_shot), bom(_bom), slow(_slow), start(_start), change(_change){}
void gp::PadConfig::set_config(int const& _left, int const& _right, int const& _up, int const& _down, int const& _shot, int const& _bom, int const& _slow, int const& _start, int const& _change)
{
	left = _left; right = _right; up = _up; down = _down; shot = _shot; bom = _bom; slow = _slow; start = _start; change = _change;
}
const int gp::PadConfig::get_left(void) const { return left; }
const int gp::PadConfig::get_right(void) const { return right; }
const int gp::PadConfig::get_up(void) const { return up; }
const int gp::PadConfig::get_down(void) const { return down; }
const int gp::PadConfig::get_shot(void) const { return shot; }
const int gp::PadConfig::get_bom(void) const { return bom; }
const int gp::PadConfig::get_slow(void) const { return slow; }
const int gp::PadConfig::get_start(void) const { return start; }
const int gp::PadConfig::get_change(void) const { return change; }
gp::PadConfig& gp::PadConfig::operator=(PadConfig const& _pad_config)
{
	this->left = _pad_config.get_left();
	this->right = _pad_config.get_right();
	this->up = _pad_config.get_up();
	this->down = _pad_config.get_down();
	this->shot = _pad_config.get_shot();
	this->bom = _pad_config.get_bom();
	this->slow = _pad_config.get_slow();
	this->start = _pad_config.get_start();
	this->change = _pad_config.get_change();
	return *this;
}
gp::PadConfig::PadConfig(PadConfig const& _pad_config)
{
	this->left = _pad_config.get_left();
	this->right = _pad_config.get_right();
	this->up = _pad_config.get_up();
	this->down = _pad_config.get_down();
	this->shot = _pad_config.get_shot();
	this->bom = _pad_config.get_bom();
	this->slow = _pad_config.get_slow();
	this->start = _pad_config.get_start();
	this->change = _pad_config.get_change();
}

void gp::tagPadTime::update(void)
{
	mul = 1;
	pad_input_flags = GetJoypadInputState(DX_INPUT_PAD1);
	for (int i = 0; i < PAD_BUTTON_NUM; ++i)
	{
		if (pad_input_flags & mul) { ++pad_time[i]; }
		else { pad_time[i] = 0; }
	}
	mul <<= 1;
}
const int gp::tagPadTime::operator[](int const& pad_macro){ return pad_time[pad_macro]; }
const int gp::tagPadTime::get(int const& pad_macro){return pad_time[pad_macro];}

namespace gp
{
	PadConfig default_config_pad;
	tagPadTime pad_time;
}
