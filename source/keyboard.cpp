//keyboard.cpp

#include "keyboard.h"

void gp::KeyboardConfig::set_default(void)
{
	right = KEY_INPUT_RIGHT;
	left = KEY_INPUT_LEFT;
	up = KEY_INPUT_UP;
	down = KEY_INPUT_DOWN;
	shot = KEY_INPUT_Z;
	bom = KEY_INPUT_X;
	slow = KEY_INPUT_LSHIFT;
	start = KEY_INPUT_ESCAPE;
	change = KEY_INPUT_LCONTROL;
}
gp::KeyboardConfig::KeyboardConfig(void){set_default();}
gp::KeyboardConfig::KeyboardConfig(int const& _left, int const& _right, int const& _up, int const& _down, int const& _shot, int const& _bom, int const& _slow, int const& _start, int const& _change) :left(_left), right(_right), up(_up), down(_down), shot(_shot), bom(_bom), slow(_slow), start(_start), change(_change){}
void gp::KeyboardConfig::set_config(int const& _left, int const& _right, int const& _up, int const& _down, int const& _shot, int const& _bom, int const& _slow, int const& _start, int const& _change)
{
	left = _left; right = _right; up = _up; down = _down; shot = _shot; bom = _bom; slow = _slow; start = _start; change = _change;
}
const int gp::KeyboardConfig::get_left(void) const{ return left; }
const int gp::KeyboardConfig::get_right(void) const{ return right; }
const int gp::KeyboardConfig::get_up(void) const{ return up; }
const int gp::KeyboardConfig::get_down(void) const{ return down; }
const int gp::KeyboardConfig::get_shot(void) const{ return shot; }
const int gp::KeyboardConfig::get_bom(void) const{ return bom; }
const int gp::KeyboardConfig::get_slow(void) const{ return slow; }
const int gp::KeyboardConfig::get_start(void) const{ return start; }
const int gp::KeyboardConfig::get_change(void) const{ return change; }
gp::KeyboardConfig& gp::KeyboardConfig::operator=(KeyboardConfig const& _keyboard_config)
{
	this->left = _keyboard_config.get_left();
	this->right = _keyboard_config.get_right();
	this->up = _keyboard_config.get_up();
	this->down = _keyboard_config.get_down();
	this->shot = _keyboard_config.get_shot();
	this->bom = _keyboard_config.get_bom();
	this->slow = _keyboard_config.get_slow();
	this->start = _keyboard_config.get_start();
	this->change = _keyboard_config.get_change();
	return *this;
}
gp::KeyboardConfig::KeyboardConfig(KeyboardConfig const& _keyboard_config)
{
	this->left = _keyboard_config.get_left();
	this->right = _keyboard_config.get_right();
	this->up = _keyboard_config.get_up();
	this->down = _keyboard_config.get_down();
	this->shot = _keyboard_config.get_shot();
	this->bom = _keyboard_config.get_bom();
	this->slow = _keyboard_config.get_slow();
	this->start = _keyboard_config.get_start();
	this->change = _keyboard_config.get_change();
}

int gp::tagKeyTime::update(void)
{
	GetHitKeyStateAll(key_state);

	for(int i = 0;i < KEY_NUM;++i)
	{
		if(key_state[i]){++key_time[i];}
		else {key_time[i] = 0;}
	}

	return 0;
}

const int gp::tagKeyTime::operator[](int key_macro) const {return key_time[key_macro];}
const int gp::tagKeyTime::get(int key_macro) const {return key_time[key_macro];}

namespace gp
{
	KeyboardConfig default_config_keyboard;
	tagKeyTime key_time;
}
