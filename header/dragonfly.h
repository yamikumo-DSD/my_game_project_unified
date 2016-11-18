//dragonfly.h

#pragma once

#include "mob_enemy.h"

namespace MyGameProject
{
	class Dragonfly : public MobEnemy
	{
	private:
		struct Impl; std::unique_ptr<Impl> pimpl;
		virtual void accessory_custom_updater(void) override final;
	public:
		Dragonfly
		(
			BulletPtrContainer& _bullets,
			SEManager& _se_manager,
			gp::smart_ptr<EnemyOrder> _order,
			const Player& _player
		) noexcept;
		Dragonfly(const Dragonfly& _dragonfly) noexcept;
		Dragonfly(Dragonfly&& _dragonfly) noexcept;
		virtual ~Dragonfly(void) noexcept;

		virtual void draw(void) const override final;
		static void preparation(void);
	};
}
