//helli.h

#pragma once

#include "mob_enemy.h"
#include "pimpl.h"

namespace MyGameProject
{
	struct HelliImpl;

	class Helli : public MobEnemy, public gp::StaticPimplOwner<HelliImpl>
	{
	private:
		virtual void accessory_custom_updater(void) override final;
	public:
		Helli
		(
			BulletPtrContainer& _bullets,
			SEManager& _se_manager,
			gp::smart_ptr<EnemyOrder> _order,
			const Player& _player
		) noexcept;
		Helli(const Helli&) noexcept;
		virtual ~Helli(void) noexcept;

		virtual void hit(int _shot_power) override final;

		virtual void draw(void) const override final;
		static void preparation(void);
	};
}