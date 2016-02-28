//device.h

#ifndef __DEVICE_H
#define __DEVICE_H

#include <unordered_map>
#include <string>

namespace MyGameProject
{
	class Device
	{
	private:
		std::unordered_map<std::string, int> button;
		std::unordered_map<std::string, int> button_one_frame_ago;
		virtual void update_buttons(void) = 0;
	protected:
		int& get_ref(const std::string& _button_name);
	public:
		void update(void);
		void add_button(const std::string& _button_name);
		int get_time(const std::string& _button_name) const;
		bool is_just_released(const std::string& _button_name) const;
		virtual ~Device(void);
	};
}

#endif
