//crow.h

#ifndef __CROW_H
#define __CROW_H

#include "mob_enemy.h"

namespace MyGameProject
{
	class Crow : public MobEnemy
	{
	private:
		virtual void accessory_custom_updater(void) override final;
	public:
		Crow
			(
			BulletPtrContainer& _bullets,
			SEManager& _se_manager,
			gp::smart_ptr<EnemyOrder> _order,
			const Player& _player
			);
		virtual void draw(void) const override final;
		static void preperation(void);
		virtual ~Crow(void);
	};
}

#endif