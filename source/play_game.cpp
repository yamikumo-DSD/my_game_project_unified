//play_game.cpp

#include "play_game.h"
#include "image_pool.h"
#include "dxsstream.h"
#include "keyboard_device.h"
#include "player.h"
#include "bullet.h"
#include "shot.h"
#include "title.h"
#include "se_manager.h"
#include "mob_enemy.h"
#include "item.h"
#include "concrete_back_ground.h"
#include "environmental_constants.h"
#include <chrono>
#include "color.h"
#include "draw_order.h"
#include "stage0.h"
#include "stage3.h"
#include "boss.h"
#include "shared_object.h"
#include <type_traits>
#include "destroy_enemy_rate.h"
#include "find_vacant_object.h"

namespace MyGameProject
{

	namespace Tools
	{
		using namespace std;

		template<class DynamicObjectPtrContainer>
		inline void dynamic_object_updater(DynamicObjectPtrContainer& _objects)
		{
			for (auto& object : _objects)
			{
				if (object && object->get_flag()) { object->update(); }
				else { object.reset(); }
			}
		}

		template<typename T>
		class is_pointer
		{
		private:
			template<typename U>
			static constexpr decltype(*declval<U>(), true_type()) impl(int*);
			template<typename U>
			static constexpr false_type impl(...);
		public:
			static constexpr auto value{ decltype(impl<T>(nullptr))::value };
		};
		template<typename T>
		struct is_weak : false_type{};

		template<typename T>
		struct is_weak<weak_ptr<T>> : true_type {};

		template<typename T>
		struct is_shared : false_type{};

		template<typename T>
		struct is_shared<shared_ptr<T>> : true_type {};

		template<int L>
		struct Priority :Priority<L + 1>{};
		template<>
		struct Priority<10> {};

		template<typename Obj>
		inline auto draw_impl(Obj&& _obj, Priority<2>)
		{
			_obj.draw();
		}
		template<typename Obj, std::enable_if_t<Tools::is_shared<std::decay_t<Obj>>::value>* = nullptr>
		inline auto draw_impl(Obj&& _obj, Priority<1>)
		{
			if (_obj) { _obj->draw(); }
		}
		template<typename Obj, std::enable_if_t<Tools::is_weak<std::decay_t<Obj>>::value>* = nullptr>
		inline auto draw_impl(Obj&& _obj, Priority<0>)
		{
			if (!_obj.expired())
			{
				auto shared = _obj.lock();
				if (shared->get_flag()) { shared->draw(); }
			}
		}
		template<typename Obj>
		inline auto draw(Obj&& _obj) { draw_impl(std::forward<Obj>(_obj), Priority<0>()); }
	}

	constexpr size_t MAX_BULLET_NUM = 512;
	constexpr size_t MAX_SHOT_NUM = 150;
	constexpr size_t MAX_MOB_ENEMY_NUM = 100;
	constexpr size_t MAX_ITEM_NUM = 512;
	constexpr size_t MAX_LOCK_RATE_INDICATOR_UNIT_DISPLAYED = 100;

	PlayGame::PlayGame(void)
		:device(gp::make_smart<Keyboard>()),
		se_manager(gp::make_smart<SEManager>()),
		bullets(MAX_BULLET_NUM),
		shots(MAX_SHOT_NUM),
		enemies(MAX_MOB_ENEMY_NUM + 1), //Mob + Boss
		mob_enemies(MAX_MOB_ENEMY_NUM),
		items(MAX_ITEM_NUM),
		count(0),
		slow(1),
		static_count(0),
		RepeatableVM(),
		player_health_gage(),
		lock_rate_indicator(MAX_LOCK_RATE_INDICATOR_UNIT_DISPLAYED),
		stage_proceeding_signal(StageProceedingSignal::START_INITIAL_STAGE)
	{
		player = gp::make_smart<Player>
		(
			*device,
			std::bind(std::mem_fn(&SEManager::erect_play_flag_of), se_manager.get(), std::placeholders::_1),
			shots,
			enemies
		);
	}

	void PlayGame::change_play_speed(float _speed)
	{
		slow = static_cast<unsigned int>(1.f / _speed);
	}

	void PlayGame::play_se(int _loudness) const
	{
		if (se_manager) { se_manager->play(_loudness); }
	}

	void PlayGame::draw(void) const
	{
		using namespace std::chrono;

		auto start = system_clock::now();
		auto start_t = start;


		//back ground
		Tools::draw(back_ground_control);
		draw_back_ground_cost = static_cast<int>(duration_cast<microseconds>(system_clock::now() - std::move(start_t)).count());

		//enemies
		//Tools::draw(boss);
		//for (const auto& mob : mob_enemies) 
		//{
		//	Tools::draw(mob); 
		//	if (!mob.expired()) { mob.lock()->draw_killed_effect(); }
		//}
		Tools::draw(SharedObject::boss());
		for (const auto& mob : SharedObject::mob_enemies()) 
		{
			Tools::draw(mob); 
			if (!mob.expired()) { mob.lock()->draw_killed_effect(); }
		}

		//shots
		start_t = system_clock::now();
		for (const auto& shot : shots) { Tools::draw(shot); }
		draw_shot_cost = static_cast<int>(duration_cast<microseconds>(system_clock::now() - std::move(start_t)).count());

		//player
		Tools::draw(player);

		//items
		for (const auto& item : SharedObject::items()) { Tools::draw(item); }

		//bullets
		start_t = system_clock::now();
		for (const auto& bullet : SharedObject::bullets()) { Tools::draw(bullet); }
		draw_bullet_cost = static_cast<int>(duration_cast<microseconds>(system_clock::now() - std::move(start_t)).count());

		//health gage
		player_health_gage.draw();

		//draw base rate
		base_rate_drawer.draw();

		//draw score
		score_drawer.draw();

		//lock rate indicator
		for (const auto& rate : lock_rate_indicator)
		{
			if (rate && rate->get_flag()) { rate->draw(); } 
		}

		gp::ApplyDrawer();
		gp::ClearDrawer();

		gp::DX_SStream(4 * 50, 7 * 50) << "Press Q to go back to title." << gp::end;
		draw_cost = static_cast<int>(duration_cast<milliseconds>(system_clock::now() - start).count());
		gp::Color c1 = gp::white, c2 = gp::white;
		if (draw_cost >= 10) { c1 = gp::red; }if (update_cost >= 10) { c2 = gp::red; }
		gp::DX_SStream(100, 100) << c1 << "Drawing time: " << draw_cost << "ms" << gp::end;
		gp::DX_SStream(100, 115) << c1 << "BG: " << draw_back_ground_cost << "ƒÊs" << gp::end;
		gp::DX_SStream(100, 130) << c1 << "shot: " << draw_shot_cost << "ƒÊs" << gp::end;
		gp::DX_SStream(100, 145) << c1 << "bullet: " << draw_bullet_cost << "ƒÊs" << gp::end;
		gp::DX_SStream(100, 160) << c2 << "Updating time: " << update_cost << "ms" << gp::end;
		gp::DX_SStream(100, 175) << c2 << "BG: " << update_back_ground_cost << "ƒÊs" << gp::end;
		gp::DX_SStream(100, 190) << c2 << "shot: " << update_shot_cost << "ƒÊs" << gp::end;
		gp::DX_SStream(100, 205) << c2 << "bullet: " << update_bullet_cost << "ƒÊs" << gp::end;
		gp::dxout(100, 220) << gp::black << "total shot num: " << std::count_if(shots.begin(), shots.end(), [](const auto& _b) {if (_b && _b->get_flag()) { return true; } else { return false; }}) << gp::end;
	}

	void PlayGame::update(void)
	{
		switch (stage_proceeding_signal) //Control stage procedure.
		{
			case StageProceedingSignal::START_INITIAL_STAGE:
			{
				load_stage_enemy_list(0);
				SharedObject::init_all();
				back_ground_control = gp::make_smart<Stage0BG>(*player);
				stage_proceeding_signal = StageProceedingSignal::STAY;
			}break;
			case StageProceedingSignal::GOTO_STAGE0:
			{
				load_stage_enemy_list(0);
				SharedObject::init_all();
				back_ground_control = gp::make_smart<Stage0BG>(*player);
				stage_proceeding_signal = StageProceedingSignal::STAY;
			}break;
			case StageProceedingSignal::GOTO_STAGE3:
			{
				load_stage_enemy_list(3);
				SharedObject::init_all();
				back_ground_control = gp::make_smart<Stage3BG>(*player);
				stage_proceeding_signal = StageProceedingSignal::STAY;
			}break;
			case StageProceedingSignal::GOTO_NEXT_STAGE:
			{
				throw std::runtime_error("StageProceedingSignal GOTO_NEXT_STAGE has not been implemented yet.");
			}break;
			case StageProceedingSignal::STAY:
			{
				//Do nothing.
			}break;
			default:
			{
				throw std::runtime_error("Undefined StageProceedingSignal is set.");
			}break;
		}

		auto start = std::chrono::system_clock::now();
		if (device)
		{
			device->update();
			if (device->get_time("Q") > 0)
			{
				set_new_scene(gp::make_smart<Title>());
			}
		}

		if (player) { player->update_mode(); }

		if (static_count % slow == 0)
		{
			using namespace std::chrono;
			player->update();
			//Tools::dynamic_object_updater(enemies);
			Tools::dynamic_object_updater(SharedObject::enemies());

			execute();
			if (!boss.expired() && boss.lock()->get_flag())
			{
				back_ground_control->wait_signal(boss.lock()->notify());
			}

			auto start_t = system_clock::now();
			back_ground_control->apply();
			update_back_ground_cost = static_cast<int>(duration_cast<microseconds>(system_clock::now() - start_t).count());

			start_t = system_clock::now();
			Tools::dynamic_object_updater(SharedObject::bullets());
			update_bullet_cost = static_cast<int>(duration_cast<microseconds>(system_clock::now() - start_t).count());
			start_t = system_clock::now();
			Tools::dynamic_object_updater(shots);
			update_shot_cost = static_cast<int>(duration_cast<microseconds>(system_clock::now() - start_t).count());
			Tools::dynamic_object_updater(SharedObject::items());
			player_health_gage.update(player->health(), player->pos().y());
			Tools::dynamic_object_updater(lock_rate_indicator);
			base_rate_drawer.update(base_rate);
			score_drawer.update(score);

			hit_and_damage_dealer();
			item_and_score_dealer();

			if (base_rate >= 0) { base_rate -= 0.05f; }
			else { base_rate = 0.f; }

			if (player->rest_hyper_mode_time() == Player::HYPER_MODE_TIME - 1)
			{
				if (se_manager) { se_manager->erect_play_flag_of("../../data/sound/attack2.mp3"); }
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

	void PlayGame::unload(void)
	{
		ImagePool::clear();
		if (se_manager) { se_manager->clear(); }
	}

	PlayGame::~PlayGame(void) {}
}
