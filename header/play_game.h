//play_game.h

#ifndef __PLAY_GAME_H
#define __PLAY_GAME_H

#include "scene.h"
#include "smart_ptr.h"
#include <vector>
#include "player_health_gage.h"
#include "base_rate_drawer.h"
#include "score_drawer.h"
#include "multitask.h"
#include <lua.h>

#define DEBUG_MODE 1

namespace MyGameProject
{
	class Device;
	class Player;
	class Bullet;
	class Shot;
	class SEManager;
	class MobEnemy;
	class Enemy;
	class Boss;
	class Item;
	class BackGroundControl;
	class DestroyEnemyRate;

	class PlayGame : public MyGameProject::Scene
	{
#if DEBUG_MODE
	private:
		mutable int draw_cost, 
					update_cost, 
					draw_back_ground_cost,
					draw_shot_cost,
					draw_bullet_cost,
					update_back_ground_cost,
					update_shot_cost,
					update_bullet_cost;
#endif
	private:
		gp::smart_ptr<Device>                  device;
		gp::smart_ptr<Player>                  player;
		std::vector<gp::smart_ptr<Bullet>>     bullets;
		std::vector<gp::smart_ptr<Shot>>       shots;
		std::vector<gp::smart_ptr<Enemy>>      enemies; //enemy = boss + mob_enemies
		std::vector<std::weak_ptr<MobEnemy>>   mob_enemies;
		std::weak_ptr<Boss>                    boss;
		std::vector<gp::smart_ptr<Item>>       items;
		gp::smart_ptr<BackGroundControl>       back_ground_control;
		gp::smart_ptr<SEManager>               se_manager;
		void hit_and_damage_dealer(void);
		void item_and_score_dealer(void);
		void convert_bullets(void);
		void load_stage_enemy_list(int _stage_num);
		int static_count;
		int count;
		float base_rate{1.f};
		int score{0};
		unsigned int slow;
		PlayerHealthGage player_health_gage;
		BaseRateDrawer base_rate_drawer;
		ScoreDrawer score_drawer;
		std::vector<gp::smart_ptr<DestroyEnemyRate>> lock_rate_indicator;
		enum class StageProceedingSignal
		{
			STAY, START_INITIAL_STAGE, GOTO_NEXT_STAGE, GOTO_STAGE0, GOTO_STAGE3,
		}stage_proceeding_signal;

		//BUILTIN LUA
		std::unique_ptr<lua_State, decltype(&lua_close)> L;
		//Main thread
		lua_State* this_thread;
		LuaUtilities::ThreadStatus thread_status;
		//Child threads.
		LuaUtilities::TaskSystem tasks;
		//Adapt C++ class to lua environments.
		void adapt_class(lua_State* _state);
	public:
		PlayGame(void);
		void change_play_speed(float _speed);
		virtual void play_se(int _loudness) const override;
		virtual void draw(void) const override;
		virtual void update(void) override;
		virtual void load(void) override;
		virtual void unload(void) override;

		auto player_handle(void) const { return player; }
		auto se_manager_handle(void) const { return se_manager; }

		virtual ~PlayGame(void);
	};
}

#endif