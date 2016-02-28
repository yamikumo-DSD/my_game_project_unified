//keyboard_device.h

#ifndef __KEYBOARD_DEVICE_H
#define __KEYBOARD_DEVICE_H

#include "device.h"
#include "keyboard.h"

namespace MyGameProject
{
	class Keyboard : public MyGameProject::Device
	{
	private:
		gp::tagKeyTime key_time;
		virtual void update_buttons(void) override;
	public:
		Keyboard(void);
	};
}

#endif
