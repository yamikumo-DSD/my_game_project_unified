//padkey.cpp

#include "padkey.h"

#undef max

namespace
{
	template<class T> inline T max(T x,T y)
	{
		return (x > y)? x : y;
	}
}

gp::InputTime::InputTime(PadConfig const& _pad_config,KeyboardConfig const& _keyboard_config) :pad_config(_pad_config),keyboard_config(_keyboard_config){}
const int gp::InputTime::right(void) const { return max( pad_time[pad_config.get_right()], key_time[keyboard_config.get_right()] ); }
const int gp::InputTime::left(void) const { return max( pad_time[pad_config.get_left()], key_time[keyboard_config.get_left()]); }
const int gp::InputTime::up(void) const { return max( pad_time[pad_config.get_up()], key_time[keyboard_config.get_up()]); }
const int gp::InputTime::down(void) const { return max( pad_time[pad_config.get_down()], key_time[keyboard_config.get_down()]); }
const int gp::InputTime::shot(void) const { return max( pad_time[pad_config.get_shot()], key_time[keyboard_config.get_shot()]); }
const int gp::InputTime::bom(void) const { return max( pad_time[pad_config.get_bom()], key_time[keyboard_config.get_bom()]); }
const int gp::InputTime::start(void) const { return max( pad_time[pad_config.get_start()], key_time[keyboard_config.get_start()]); }
const int gp::InputTime::slow(void) const { return max( pad_time[pad_config.get_slow()], key_time[keyboard_config.get_slow()]); }
const int gp::InputTime::change(void) const { return max( pad_time[pad_config.get_change()], key_time[keyboard_config.get_change()]); }
int gp::InputTime::update(void)
{
	pad_time.update(); key_time.update();
	return 0;
}
void gp::InputTime::load_config(void)
{
	pad_config.load_pad_config();
	keyboard_config.load_keyboard_config();
}

namespace gp
{
	InputTime input_time_default(default_config_pad,default_config_keyboard);
}
