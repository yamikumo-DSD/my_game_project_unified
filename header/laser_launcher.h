//laser_launcher.h

#pragma once

#include "mob_enemy.h"
#include "pimpl.h"

namespace MyGameProject
{
	struct LaserLauncherImpl;

	class LaserLauncher : public MobEnemy, public gp::StaticPimplOwner<LaserLauncherImpl>
	{
	private:
		virtual void accessory_custom_updater(void) override final;
	public:
		LaserLauncher
		(
			BulletPtrContainer& _bullets,
			SEManager& _se_manager,
			gp::smart_ptr<EnemyOrder> _order,
			const Player& _player
		) noexcept;
		LaserLauncher(const LaserLauncher&) noexcept;
		virtual ~LaserLauncher(void) noexcept;
		virtual void hit(int _shot_power) override final;

		virtual void draw(void) const override final;
		static void preparation(void);
	};
}