//enemy.cpp

#include "enemy.h"
#include "player.h"
#include "boss.h"
#include "mob_enemy.h"
#include "debug.h"

namespace MyGameProject
{
	Enemy::Enemy(const Point2D& _initial_pos, Real _initial_angle, Class _enemy_class, const Player& _player, const Shape& _hitbox_shape)
		:MovingObject(_initial_pos),
		ang(_initial_angle),
		HitableObject(HitBox(_hitbox_shape, PhysicalState(_initial_angle, _initial_pos))),
		player(_player),
		count(0),
		enemy_class(_enemy_class),
		enemies(),
		id(id_()++),
		state_(State::ALIVE)
	{}

	Enemy::Enemy(const Point2D& _initial_pos, Real _initial_angle, Class _enemy_class, const Player& _player, const Shape& _hitbox_shape, std::vector<gp::smart_ptr<Enemy>>& _enemies)
		:MovingObject(_initial_pos),
		ang(_initial_angle),
		HitableObject(HitBox(_hitbox_shape, PhysicalState(_initial_angle, _initial_pos))),
		player(_player),
		count(0),
		enemy_class(_enemy_class),
		enemies(_enemies),
		id(id_()++),
		state_(State::ALIVE)
	{}

	Enemy::Enemy(const Enemy& _enemy) noexcept
		:HitableObject(_enemy), id(_enemy.id), enemy_class(_enemy.enemy_class), player(_enemy.player),
		count(_enemy.count), ang(_enemy.ang), enemies(_enemy.enemies), state_(_enemy.state_) {}

	Enemy::Enemy(Enemy&& _enemy) noexcept
		:HitableObject(_enemy), id(_enemy.id), enemy_class(_enemy.enemy_class), player(_enemy.player),
		count(_enemy.count), ang(_enemy.ang), enemies(_enemy.enemies), state_(_enemy.state_) {}

	int Enemy::get_id(void) const { return id; }

	void Enemy::update(void)
	{
		custom_updater();
		area().get_physical_state().p = pos();
		++count;
	}

	Enemy::Class Enemy::get_class(void) const { return enemy_class; }

	int Enemy::get_count(void) const { return count; }

	Real Enemy::angle(void) const { return ang; }

	void Enemy::angle(Real _ang) { ang = _ang; }

	Enemy::State Enemy::state(void) const noexcept { return state_; }

	void Enemy::state(State _new_state) noexcept
	{
		state_ = _new_state;
	}

	const Player& Enemy::player_ref(void) const { return player; }

	Enemy::~Enemy(void) = default;

	void Enemy::preperation(void)
	{
		Boss::preperation();
		MobEnemy::preperation();
	}
}
