//enemy_order.h

#ifndef __ENEMY_ORDER_H
#define __ENEMY_ORDER_H

#include "mathematics.h"
#include "mob_enemy.h"
#include "smart_ptr.h"
#include <string>
#include <memory>

namespace MyGameProject
{
	struct EnemyOrder
	{
		static gp::smart_ptr<EnemyOrder> make_order
			(
			std::string _name,
			int _register_time,
			Point2D _initial_pos,
			Real _initial_angle,
			MobEnemy::ActPattern _act_pattern
			);

		std::string name;
		int register_time;
		Point2D initial_pos;
		Real initial_angle;
		MobEnemy::ActPattern act_pattern;
	};
}

#endif