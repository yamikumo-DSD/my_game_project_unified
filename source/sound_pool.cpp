//sound_pool.cpp

#include "sound_pool.h"
#include <dxlib.h>

void MyGameProject::SoundPool::add(const std::string& _name)
{
	data().insert({ _name, LoadSoundMem(_name.c_str()) });
}

int MyGameProject::SoundPool::get(const std::string& _name)
{
	return data()[_name];
}

void MyGameProject::SoundPool::remove(const std::string& _name)
{
	DeleteSoundMem(data()[_name]);
	data().erase(_name);
}

void MyGameProject::SoundPool::clear(void)
{
	data().clear();
	InitSoundMem();
}
