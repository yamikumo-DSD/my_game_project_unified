//mob_enemy.h

#ifndef __MOB_ENEMY_H
#define __MOB_ENEMY_H

#include "enemy.h"
#include <climits>
#include "smart_ptr.h"

namespace MyGameProject
{
	struct EnemyOrder;
	class Bullet;
	class SEManager;

	class MobEnemy : public Enemy
	{
	public:
		using BulletPtrContainer = std::vector < gp::smart_ptr<Bullet> >;
		using ActPattern = std::function<void(MobEnemy&)>;

		static gp::smart_ptr<MobEnemy> create
		(
			const std::string& _name,
			BulletPtrContainer& _bullets,
			SEManager& _se_manager,
			gp::smart_ptr<EnemyOrder> _order, 
			const Player& _player
		);
		static gp::smart_ptr<MobEnemy> create
		(
			const std::string& _name,
			BulletPtrContainer& _bullets,
			SEManager& _se_manager,
			gp::smart_ptr<EnemyOrder> _order, 
			const Player& _player,
			std::vector<gp::smart_ptr<Enemy>>& _enemies
		);
		static constexpr auto IMMORTAL = INT_MAX;
	private:
		int hp;
		ActPattern act_pattern;
		BulletPtrContainer& bullets;
		SEManager& se_manager;
		gp::smart_ptr<EnemyOrder> order;
		Point2D v; //Velocity
		virtual void custom_updater(void) override final;
		virtual void accessory_custom_updater(void) = 0;
	public:
		MobEnemy
		(
			BulletPtrContainer& _bullets,
			SEManager& _se_manager,
			gp::smart_ptr<EnemyOrder> _order,
			int _initial_health,
			Class _enemy_class,
			const Player& _player,
			const Shape& _hitbox_shape
		);
		MobEnemy
		(
			BulletPtrContainer& _bullets,
			SEManager& _se_manager,
			gp::smart_ptr<EnemyOrder> _order,
			int _initial_health,
			Class _enemy_class,
			const Player& _player,
			const Shape& _hitbox_shape,
			std::vector<gp::smart_ptr<Enemy>>& _enemies
		);
		BulletPtrContainer& get_bullets_ref(void);
		SEManager& get_se_manager_ref(void);
		const Point2D& velocity(void) const;
		Point2D& velocity(void);
		virtual void hit(int _shot_power) override final;
		virtual void draw(void) const = 0;
		virtual ~MobEnemy(void);
		static void preperation(void);
		virtual int get_health(void) const override final;
	};
}

#endif
