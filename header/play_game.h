//play_game.h

#ifndef __PLAY_GAME_H
#define __PLAY_GAME_H

#include "scene.h"
#include "smart_ptr.h"
#include <vector>
#include "vm/vm.h"
#include "player_health_gage.h"
#include "base_rate_drawer.h"
#include "score_drawer.h"
#include <boost/optional.hpp>

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

	class PlayGame : 
		public MyGameProject::Scene,
		public boost::static_visitor<void>,
		public MyVM::RepeatableVM
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

		virtual void user_extention_0(void) override final;
		virtual void user_extention_1(void) override final;
		virtual void user_extention_2(void) override final;
		virtual void user_extention_3(void) override final;
		virtual void user_extention_4(void) override final;
		virtual void user_extention_5(void) override final;
		virtual void user_extention_6(void) override final;
		virtual void user_extention_7(void) override final;
		virtual void user_extention_8(void) override final;
		virtual void user_extention_9(void) override final;
		virtual void user_extention_10(void) override final;
		virtual void user_extention_11(void) override final;
		virtual void user_extention_12(void) override final;
		virtual void user_extention_13(void) override final;
		virtual void user_extention_14(void) override final;
		virtual void user_extention_15(void) override final;
		virtual void user_extention_16(void) override final;
	public:
		PlayGame(void);
		void change_play_speed(float _speed);
		virtual void play_se(int _loudness) const override;
		virtual void draw(void) const override;
		virtual void update(void) override;
		virtual void load(void) override;
		virtual void unload(void) override;

		virtual ~PlayGame(void);
	};
}

#endif