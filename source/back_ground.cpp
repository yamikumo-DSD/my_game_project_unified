//back_ground.cpp

#include "back_ground.h"

MyGameProject::BackGround::BackGround(void):count(0){}

void MyGameProject::BackGround::update(void)
{
	custom_updater();
	++count;
}

int MyGameProject::BackGround::get_count(void) const
{
	return count;
}

MyGameProject::BackGround::~BackGround(void){}