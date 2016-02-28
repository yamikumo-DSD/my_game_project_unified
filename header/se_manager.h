//se_manager.h

#ifndef __SE_MANAGER_H
#define __SE_MANAGER_H

#include <utility>
#include <string>
#include <unordered_map>

namespace MyGameProject
{
	class SEManager
	{
	private:
		//string:name, int:handle, bool:flag
		std::unordered_map<std::string, std::pair<int, bool>> data;
	public:
		void clear(void);
		void remove(const std::string& _name);
		void add(const std::string& _name);
		void play(int _loudness);
		void erect_play_flag_of(const std::string& _name);
		void break_play_flag_of(const std::string& _name);
	};
}

#endif
