//se_manager.cpp

#include "se_manager.h"
#include "sound_pool.h"
#include <dxlib.h>

void MyGameProject::SEManager::clear(void)
{
	for (auto& sound : data)
	{
		SoundPool::remove(sound.first);
	}
	data.clear();
}

void MyGameProject::SEManager::remove(const std::string& _name)
{
	SoundPool::remove(_name);
	data.erase(_name);
}

void MyGameProject::SEManager::add(const std::string& _name)
{
	SoundPool::add(_name);
	data.insert({_name,std::make_pair<int,bool>(SoundPool::get(_name),false)});
}

void MyGameProject::SEManager::erect_play_flag_of(const std::string& _name)
{
	data[_name].second = true;
}

void MyGameProject::SEManager::break_play_flag_of(const std::string& _name)
{
	data[_name].second = false;
}

void MyGameProject::SEManager::play(int _loudness)
{
	for (auto& sound : data)
	{
		ChangeVolumeSoundMem(_loudness,sound.second.first);
		if (sound.second.second)
		{
			PlaySoundMem(sound.second.first, DX_PLAYTYPE_BACK);
			break_play_flag_of(sound.first);
		}
	}
}
