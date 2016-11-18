//bomber.h

#pragma once

#include "mob_enemy.h"
#include "pimpl.h"

namespace MyGameProject
{
	struct BomberImpl;

	class Bomber : public MobEnemy, public gp::StaticPimplOwner<BomberImpl>
	{
	private:
		virtual void accessory_custom_updater(void) override final;
	public:
		Bomber
		(
			BulletPtrContainer& _bullets,
			SEManager& _se_manager,
			gp::smart_ptr<EnemyOrder> _order,
			const Player& _player
		) noexcept;
		Bomber(const Bomber&) noexcept;
		virtual ~Bomber(void) noexcept;
		virtual void hit(int _shot_power) override final;

		void open_shell(int _time_to_open = 70);
		void close_shell(int _time_to_close = 70);

		virtual void draw(void) const override final;
		static void preparation(void);
	};
}
