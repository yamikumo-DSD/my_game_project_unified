//crow2.h

#pragma once
#include "mob_enemy.h"

namespace MyGameProject
{
	class Crow2 : public MobEnemy
	{
	private:
		virtual void accessory_custom_updater(void) override final;
	public:
		Crow2
			(
			BulletPtrContainer& _bullets,
			SEManager& _se_manager,
			gp::smart_ptr<EnemyOrder> _order,
			const Player& _player
			);
		virtual void draw(void) const override final;
		static void preperation(void);
		virtual ~Crow2(void);
	};
}
