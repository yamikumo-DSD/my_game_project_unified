//forced_crow.h

#pragma once

#include "mob_enemy.h"
#include <memory>

namespace MyGameProject
{
	class ForcedCrow : public MobEnemy
	{
	private:
		struct Impl; std::unique_ptr<Impl> pimpl;
		virtual void accessory_custom_updater(void) override final;
	public:
		ForcedCrow
		(
			BulletPtrContainer& _bullets,
			SEManager& _se_manager,
			gp::smart_ptr<EnemyOrder> _order,
			const Player& _player
		) noexcept;
		virtual ~ForcedCrow(void) noexcept;

		virtual void draw(void) const override final;
		static void preparation(void);
	};
}
