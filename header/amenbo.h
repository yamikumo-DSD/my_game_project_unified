//amenbo.h

#pragma once

#include "mob_enemy.h"

namespace MyGameProject
{
	class Amenbo : public MobEnemy
	{
	private:
		struct Impl; std::unique_ptr<Impl> pimpl;
		virtual void accessory_custom_updater(void) override final;
	public:
		Amenbo
		(
			BulletPtrContainer& _bullets,
			SEManager& _se_manager,
			gp::smart_ptr<EnemyOrder> _order,
			const Player& _player
		) noexcept;
		Amenbo(const Amenbo& _amenbo) noexcept;
		Amenbo(Amenbo&& _amenbo) noexcept;
		virtual ~Amenbo(void) noexcept;

		virtual void draw(void) const override final;
		static void preparation(void);
	};
}
