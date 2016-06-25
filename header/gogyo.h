//gogyo.h

#pragma once

#include "mob_enemy.h"
#include <memory>

namespace MyGameProject
{
	class Gogyo final : public MobEnemy
	{
	private:
		struct Impl; std::unique_ptr<Impl> pimpl;
		virtual void accessory_custom_updater(void) override final;
	public:
		Gogyo 
		(
			BulletPtrContainer& _bullets,
			SEManager& _se_manager,
			gp::smart_ptr<EnemyOrder> _order,
			const Player& _player
		) noexcept;

		virtual ~Gogyo(void) noexcept;

		virtual void draw(void) const override final;
		static void preperation(void);
	};
}

