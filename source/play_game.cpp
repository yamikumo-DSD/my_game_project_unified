//play_game.cpp

#include "play_game.h"
#include "image_pool.h"
#include "dxsstream.h"
#include "keyboard_device.h"
#include "player.h"
#include "bullet.h"
#include "option.h"
#include "shot.h"
#include "find_vacant_object.h"
#include "title.h"
#include "se_manager.h"
#include "short_range_weapon.h"
#include "mob_enemy.h"
#include "boss.h"
#include "enemy_order.h"
#include "item.h"
#include "concrete_back_ground.h"
#include "draw_order.h"
#include "dxlib_wrapper.h"
#include "environmental_constants.h"
#include <chrono>
#include "color.h"
#include "msgbox.h"
#include "draw_order.h"
#include "dxlib_wrapper.h"

namespace
{
	constexpr size_t MAX_BULLET_NUM = 512;
	constexpr size_t MAX_OPTION_NUM = 4;
	constexpr size_t MAX_SHOT_NUM = 150;
	constexpr size_t MAX_MOB_ENEMY_NUM = 100;
	constexpr size_t MAX_BOSS_NUM = 2;
	constexpr size_t MAX_ITEM_NUM = 512;

	template<class DynamicObjectPtrContainer>
	inline void dynamic_object_updater(DynamicObjectPtrContainer& _objects)
	{
		for (auto& object : _objects)
		{
			if (object && object->get_flag()) { object->update(); }
			else { object.reset(); }
		}
	}
}

MyGameProject::PlayGame::PlayGame(void)
	:device(gp::make_smart<Keyboard>()),
	back_ground(gp::make_smart<ThroughCloud>()),
	se_manager(gp::make_smart<SEManager>()),
	bullets(MAX_BULLET_NUM),
	options(MAX_OPTION_NUM),
	shots(MAX_SHOT_NUM),
	enemies(MAX_MOB_ENEMY_NUM + MAX_BOSS_NUM),
	enemy_orders(),
	mob_enemies(MAX_MOB_ENEMY_NUM),
	bosses(MAX_BOSS_NUM),
	items(MAX_ITEM_NUM),
	short_range_weapon(),
	count(0),
	slow(1),
	static_count(0),
	RepeatableVM()
{
	player = gp::make_smart<Player>
	(
		*device,
		std::bind(std::mem_fn(&SEManager::erect_play_flag_of), se_manager.get(), std::placeholders::_1),
		shots,
		enemies
	);
	//hyper_mode_back = gp::make_smart<HyperModeBack>(*player);

	//options[0] = gp::make_smart<Option>(*player, Point2D(-35,0));
	//options[1] = gp::make_smart<Option>(*player, Point2D(-15, -35));
	//options[2] = gp::make_smart<Option>(*player, Point2D(15, -35));
	//options[3] = gp::make_smart<Option>(*player, Point2D(35, 0));
	load_stage_enemy_list(0);
}

void MyGameProject::PlayGame::change_play_speed(float _speed)
{
	slow = static_cast<unsigned int>(1.f / _speed);
}

void MyGameProject::PlayGame::play_se(int _loudness) const
{
	if (se_manager){ se_manager->play(_loudness); }
}

void MyGameProject::PlayGame::draw(void) const
{
	using namespace std::chrono;

	auto start = system_clock::now();
	if (player->state() == Player::State::WARP)
	{
		auto start_t = system_clock::now();
		if (back_ground)
		{ 
			back_ground->draw_rear();
		}
		draw_back_ground_cost = static_cast<int>(duration_cast<microseconds>(system_clock::now() - start_t).count());

		//if (player->is_hyper_mode())
		//{
		//	hyper_mode_back->draw_rear();
		//}

		for (const auto& enemy : enemies){ if (enemy){ enemy->draw(); } }

		if (short_range_weapon){ short_range_weapon->draw(); }

		start_t = system_clock::now();
		for (const auto& shot : shots)
		{
			if (shot){ shot->draw(); }
		}
		draw_shot_cost = static_cast<int>(duration_cast<microseconds>(system_clock::now() - start_t).count());

		for (const auto& item : items){ if (item){ item->draw(); } }

		start_t = system_clock::now();
		for (const auto& bullet : bullets)
		{
			if (bullet){ bullet->draw(); }
		}
		draw_bullet_cost = static_cast<int>(duration_cast<microseconds>(system_clock::now() - start_t).count());

		{
			ScopedDrawBlendMode blend(DX_BLENDMODE_ALPHA, 100);
			DrawBox(0, 0, WW<int>(), WH<int>(), gp::black.get(), TRUE);
		}
		player->draw();
		//for (const auto& option : options){ if (option){ option->draw(); } }
		//if(!player->is_hyper_mode())
		//{
			if (back_ground)
			{ 
				back_ground->draw_front();
			}
		//}
	}
	else
	{
		auto start_t = system_clock::now();
		if (back_ground)
		{
			back_ground->draw_rear();
		}
		draw_back_ground_cost = static_cast<int>(duration_cast<microseconds>(system_clock::now() - start_t).count());

		//if (player->is_hyper_mode())
		//{
		//	hyper_mode_back->draw_rear();
		//}
		for (const auto& enemy : enemies){ if (enemy){ enemy->draw(); } }
		if (short_range_weapon){ short_range_weapon->draw(); }

		start_t = system_clock::now();
		for (const auto& shot : shots)
		{
			if (shot){ shot->draw(); }
		}
		draw_shot_cost = static_cast<int>(duration_cast<microseconds>(system_clock::now() - start_t).count());

		if (player){ player->draw(); }
		//for (const auto& option : options){ if (option){ option->draw(); } }
		for (const auto& item : items){ if (item){ item->draw(); } }

		start_t = system_clock::now();
		for (const auto& bullet : bullets)
		{
			if (bullet){ bullet->draw(); }
		}
		//if(!player->is_hyper_mode())
		//{
			if (back_ground)
			{ 
				back_ground->draw_front();
			}
		//}
		draw_bullet_cost = static_cast<int>(duration_cast<microseconds>(system_clock::now() - start_t).count());
	}

	gp::ApplyDrawer();
	gp::ClearDrawer();

	gp::DX_SStream(4 * 50, 7 * 50) << "Press Q to go back to title." << gp::end;
	draw_cost = static_cast<int>(duration_cast<milliseconds>(system_clock::now() - start).count());
	gp::Color c1 = gp::white,c2 = gp::white;
	if (draw_cost >= 10){ c1 = gp::red; }if (update_cost >= 10){ c2 = gp::red; }
	gp::DX_SStream(100, 100) << c1 << "Drawing time: " << draw_cost << "ms" << gp::end;
	gp::DX_SStream(100, 115) << c1 << "BG: " << draw_back_ground_cost << "ƒÊs" << gp::end;
	gp::DX_SStream(100, 130) << c1 << "shot: " << draw_shot_cost << "ƒÊs" << gp::end;
	gp::DX_SStream(100, 145) << c1 << "bullet: " << draw_bullet_cost << "ƒÊs" << gp::end;
	gp::DX_SStream(100, 160) << c2 << "Updating time: " << update_cost << "ms" << gp::end;
	gp::DX_SStream(100, 175) << c2 << "BG: " << update_back_ground_cost << "ƒÊs" << gp::end;
	gp::DX_SStream(100, 190) << c2 << "shot: " << update_shot_cost << "ƒÊs" << gp::end;
	gp::DX_SStream(100, 205) << c2 << "bullet: " << update_bullet_cost << "ƒÊs" << gp::end;
	gp::dxout(100, 220) << gp::black << "total shot num: " << std::count_if(shots.begin(), shots.end(), [](auto _b) {if (_b && _b->get_flag()) { return true; } else { return false; }}) << gp::end;

}

void MyGameProject::PlayGame::update(void)
{
	auto start = std::chrono::system_clock::now();
	if (device)
	{
		device->update();
		if (device->get_time("Q") > 0)
		{
			set_new_scene(gp::make_smart<Title>());
		}
	}

	if (player){ player->update_mode(); }

	if (static_count % slow == 0)
	{
		using namespace std::chrono;
		auto start_t = system_clock::now();
		if (back_ground){ back_ground->update(); }
		update_back_ground_cost = static_cast<int>(duration_cast<microseconds>(system_clock::now() - start_t).count());
		//if (hyper_mode_back && player && player->is_hyper_mode()){ hyper_mode_back->update(); }
		if (player){ player->update(); }
		dynamic_object_updater(enemies);

		shot_register();

		//exec();
		execute();

		//for (auto& option : options)
		//{
		//	if (option){ option->update(); }
		//}
		start_t = system_clock::now();
		dynamic_object_updater(bullets);
		update_bullet_cost = static_cast<int>(duration_cast<microseconds>(system_clock::now() - start_t).count());
		if (short_range_weapon){ short_range_weapon->update(); }
		start_t = system_clock::now();
		dynamic_object_updater(shots);
		update_shot_cost = static_cast<int>(duration_cast<microseconds>(system_clock::now() - start_t).count());
		dynamic_object_updater(items);

		hit_and_damage_dealer();
		item_and_score_dealer();

		if (player->rest_hyper_mode_time() == Player::HYPER_MODE_TIME - 1)
		{
			if (se_manager){ se_manager->erect_play_flag_of("../../data/sound/attack2.mp3"); }
			convert_bullets();
		}
		if (player->rest_hyper_mode_time() == 1)
		{
			convert_bullets();
		}

		++count;
	}
	++static_count;
	update_cost = static_cast<int>(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - start).count());
}

void MyGameProject::PlayGame::load(void)
{
	Player::preperation();
	Option::preperation();
	Shot::preperation();
	ShortRangeWeapon::preperation();
	//BackGround1::preperation();
	Pseud3D::preperation();
	ThroughCloud::preperation();
	HyperModeBack::preperation();
	Enemy::preperation();
	Bullet::preperation();
	Item::preperation();
	if (se_manager)
	{
		se_manager->add("../../data/sound/key-in1.mp3");
		se_manager->add("../../data/sound/meka_ge_kaseto_deki_botan03.mp3");
		se_manager->add("../../data/sound/shot.mp3"); 
		se_manager->add("../../data/sound/enemy_shot.wav");
		se_manager->add("../../data/sound/magic_wave3.mp3");
		se_manager->add("../../data/sound/ata_a27.mp3");
		se_manager->add("../../data/sound/se-033a.mp3");
		se_manager->add("../../data/sound/Clock-Second_Hand01-1L.mp3");
		se_manager->add("../../data/sound/sei_ge_garasu_hibi02.mp3");
		se_manager->add("../../data/sound/button04b.mp3");
		se_manager->add("../../data/sound/button04a.mp3");
		se_manager->add("../../data/sound/click.mp3");
		se_manager->add("../../data/sound/glass-break3.mp3");
		se_manager->add("../../data/sound/attack2.mp3");
	}
}

void MyGameProject::PlayGame::unload(void)
{
	ImagePool::clear();
	if (se_manager){ se_manager->clear(); }
}

MyGameProject::PlayGame::~PlayGame(void){}
