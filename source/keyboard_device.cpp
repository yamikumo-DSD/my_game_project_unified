//keyboard_device.cpp

#include "keyboard_device.h"

MyGameProject::Keyboard::Keyboard(void)
{
	add_button("Escape");
	add_button("Right");
	add_button("Left");
	add_button("Up");
	add_button("Down");
	add_button("Z");
	add_button("Q");
	add_button("C");
	add_button("Shift");
	add_button("X");
	add_button("A");
}

void MyGameProject::Keyboard::update_buttons(void)
{
	

	key_time.update();
	get_ref("Escape") = key_time.get(KEY_INPUT_ESCAPE);
	get_ref("Right") = key_time.get(KEY_INPUT_RIGHT);
	get_ref("Left") = key_time.get(KEY_INPUT_LEFT);
	get_ref("Up") = key_time.get(KEY_INPUT_UP);
	get_ref("Down") = key_time.get(KEY_INPUT_DOWN);
	get_ref("Z") = key_time.get(KEY_INPUT_Z);
	get_ref("Q") = key_time.get(KEY_INPUT_Q);
	get_ref("X") = key_time.get(KEY_INPUT_X);
	get_ref("A") = key_time.get(KEY_INPUT_A);
	get_ref("C") = key_time.get(KEY_INPUT_C);
	get_ref("Shift") = key_time.get(KEY_INPUT_LSHIFT);
}
