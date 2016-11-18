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
		static constexpr auto IMMORTAL = INT_MAX;
	private:
		virtual void custom_updater(void) override final;
		virtual void accessory_custom_updater(void) = 0;
		virtual int released_p_item_num(void) const;

		//Default procedure after Health become 0.
		struct Impl;
		std::unique_ptr<Impl> pimpl;
		virtual void update_after_killed(void);
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
		MobEnemy(const MobEnemy& _mob_enemy) noexcept;
		MobEnemy(MobEnemy&& _mob_enemy) noexcept;
		MobEnemy& operator=(const MobEnemy& _mob_enemy) = delete;
		MobEnemy& operator=(MobEnemy&& _mob_enemy) = delete;
		BulletPtrContainer& get_bullets_ref(void);
		SEManager& get_se_manager_ref(void);
		const Point2D& velocity(void) const;
		Point2D& velocity(void);
		Point2D calc_velocity(void) const;
		virtual void hit(int _shot_power) override;
		void damage(int _shot_power);
		virtual void draw(void) const = 0;
		virtual void draw_killed_effect(void) const;
		virtual ~MobEnemy(void);
		static void preperation(void);
		virtual int get_health(void) const override final;
		virtual bool is_just_killed_now(void) const noexcept override final;
		virtual void kill(void) noexcept override final;
	};
}

#endif
