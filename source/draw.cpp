//draw.cpp

#include "base_class.h"
#include "fps.h"
#include <iomanip>
#include "dxsstream.h"
#include <dxlib.h>
#include "color.h"
#include "is_debug_mode.h"
#include "scene_manager.h"

void MyGameProject::GameMain::draw(void) const
{
	scene->draw();
#if DEBUG_MODE
	draw_glid(50);
	draw_fps();
#endif
}

void MyGameProject::GameMain::draw_fps(void) const
{
	gp::DX_SStream(5, 460) << std::setprecision(3) << gp::blue << gp::get_fps() << "fps" << gp::end;
}

void MyGameProject::GameMain::draw_glid(int _interval) const
{
	for (int i = 0; i < 640;i += _interval)
	{
		DrawLine(i,0,i,480,gp::white.get());
	}
	for (int i = 0; i < 480; i += _interval)
	{
		DrawLine(0,i,640,i, gp::white.get());
	}
}