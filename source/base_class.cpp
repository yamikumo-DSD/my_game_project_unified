//base_class.cpp

#include "base_class.h"
#include "smart_ptr.h"
#include "title.h"
#include "is_debug_mode.h"
#include "image_pool.h"
#include "msgbox.h"
#include "scene_manager.h"

MyGameProject::GameMain::GameMain(int _window_mode)
	:MakeDxLib(_window_mode),
	state(State::INITIATION),
	scene(gp::make_smart<SceneManager>(gp::make_smart<Title>()))
{}

void MyGameProject::GameMain::one_loop(void)
{
	switch (state)
	{
	case State::INITIATION:
		state = State::NORMAL;
		break;
	case State::NORMAL:
		scene->update();
		scene->play_se(100);
		draw();
		if (scene->no_scene()){ state = State::FINALIZATION; }
		break;
	case State::FINALIZATION:
		MyGameProject::ImagePool::clear();
		break;
	}
}

bool MyGameProject::GameMain::fin_loop_if(void)
{
	return state == State::FINALIZATION;
}

MyGameProject::GameMain::~GameMain(void){}