//sound_pool.h

#ifndef __SOUND_POOL_H
#define __SOUND_POOL_H

#include <unordered_map>
#include <string>

namespace MyGameProject
{
	class SoundPool
	{
	private:
		static std::unordered_map<std::string, int>& data(void)
		{
			static std::unordered_map<std::string, int> d;
			return d;
		}
	public:
		static void add(const std::string& _name);
		static int get(const std::string& _name);
		static void remove(const std::string& _name);
		static void clear(void);
	};
}

#endif
