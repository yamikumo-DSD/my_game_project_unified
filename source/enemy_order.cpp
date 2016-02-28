//enemy_order.cpp

#include "enemy_order.h"
#include "enemy_act_pattern.h"

gp::smart_ptr<MyGameProject::EnemyOrder> MyGameProject::EnemyOrder::make_order
(
std::string _name,
int _register_time,
Point2D _initial_pos,
Real _initial_angle,
MobEnemy::ActPattern _act_pattern
)
{
	auto order = gp::make_smart<EnemyOrder>();
	order->name = _name;
	order->register_time = _register_time;
	order->initial_pos = _initial_pos;
	order->initial_angle = _initial_angle;
	order->act_pattern = _act_pattern;
	return order;
}

