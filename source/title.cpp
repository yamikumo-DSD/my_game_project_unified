//title.cpp

#include "title.h"
#include "image_pool.h"
#include <dxlib.h>
#include "keyboard_device.h"
#include "play_game.h"
#include "dxsstream.h"
#include "se_manager.h"
#include "draw_order.h"
#include "dxlib_wrapper.h"
#include "title_bg_drawer.h"

MyGameProject::Title::Title(void)
	:device(gp::make_smart<MyGameProject::Keyboard>()),
	selected_menu(0),
	se_manager(gp::make_smart<SEManager>()),
	bg_drawer(gp::make_smart<TitleBGDrawer>(*this))
{}

void MyGameProject::Title::play_se(int _loudness) const
{
	if (se_manager){ se_manager->play(_loudness); }
}

void MyGameProject::Title::draw(void) const
{
	if (bg_drawer){ bg_drawer->draw(); }
	gp::DrawRotaGraph(125, 55 + selected_menu * 50, 1.2, 0.0, ImagePool::get("../../data/img/title_icon.png"), TRUE);
	gp::ApplyDrawer();
	gp::ClearDrawer();

	gp::DX_SStream(100, 50) << gp::red << "Start game" << gp::end;
	gp::DX_SStream(100, 100) << gp::red << "Player data" << gp::end;
	gp::DX_SStream(100, 150) << gp::red << "Config" << gp::end;
	gp::DX_SStream(100, 200) << gp::red << "Music" << gp::end;
	gp::DX_SStream(100, 250) << gp::red << "End program" << gp::end;

}

void MyGameProject::Title::update(void)
{
	if (device && se_manager)
	{
		device->update();
		if (bg_drawer){ bg_drawer->update(); }

		if ((device->get_time("Up") == 1 || device->get_time("Up") > 40) && device->get_time("Up") % 7 == 1)
		{
			se_manager->erect_play_flag_of("../../data/sound/select.wav"); 
			selected_menu = (selected_menu += (MENU_NUM - 1)) % MENU_NUM;
		}
		else if ((device->get_time("Down") == 1 || device->get_time("Down") > 40) && device->get_time("Down") % 7 == 1)
		{
			se_manager->erect_play_flag_of("../../data/sound/select.wav"); 
			selected_menu = ++selected_menu % MENU_NUM;
		}
		else if (device->get_time("Escape") == 1)
		{
			se_manager->erect_play_flag_of("../../data/sound/select.wav"); 
			selected_menu = 4;
		}

		if (device->get_time("Z") == 1)
		{
			switch (selected_menu)
			{
			case 0:
				set_new_scene(gp::make_smart<PlayGame>()); break;
			case 4:
				set_new_scene(gp::smart_ptr<Scene>()); break;
			default:
				break;
			}
		}
	}
}

void MyGameProject::Title::load(void)
{
	if (bg_drawer){ bg_drawer->preperation(); }
	ImagePool::add("../../data/img/title_icon.png");
	if (se_manager){ se_manager->add("../../data/sound/select.wav"); }
}

void MyGameProject::Title::unload(void)
{
	if (se_manager){ se_manager->clear(); }
	ImagePool::clear();
}

MyGameProject::Title::~Title(void){}