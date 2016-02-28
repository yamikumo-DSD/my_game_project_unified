//device.cpp

#include "device.h"

int& MyGameProject::Device::get_ref(const std::string& _button_name)
{
	return button.at(_button_name);
}

void MyGameProject::Device::add_button(const std::string& _button_name)
{
	button_one_frame_ago.insert({ _button_name, 0 });
	button.insert({_button_name,0});
}

void MyGameProject::Device::update(void)
{
	button_one_frame_ago = button;
	update_buttons();
}

bool MyGameProject::Device::is_just_released(const std::string& _button_name) const
{
	return (button.at(_button_name) - button_one_frame_ago.at(_button_name)) < 0;
}

int MyGameProject::Device::get_time(const std::string& _button_name) const
{
	return button.at(_button_name);
}

MyGameProject::Device::~Device(void){}