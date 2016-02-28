//enemy.cpp

#include "enemy.h"
#include "player.h"
#include "boss.h"
#include "mob_enemy.h"

MyGameProject::Enemy::Enemy(const Point2D& _initial_pos,Real _initial_angle,Class _enemy_class, const Player& _player, const Shape& _hitbox_shape)
	:MovingObject(_initial_pos),
	ang(_initial_angle),
	HitableObject(HitBox(_hitbox_shape,PhysicalState(_initial_angle,_initial_pos))),
	player(_player),
	count(0),
	enemy_class(_enemy_class),
	enemies(),
	id(id_()++)
{}

MyGameProject::Enemy::Enemy(const Point2D& _initial_pos,Real _initial_angle,Class _enemy_class, const Player& _player, const Shape& _hitbox_shape, std::vector<gp::smart_ptr<Enemy>>& _enemies)
	:MovingObject(_initial_pos),
	ang(_initial_angle),
	HitableObject(HitBox(_hitbox_shape,PhysicalState(_initial_angle,_initial_pos))),
	player(_player),
	count(0),
	enemy_class(_enemy_class),
	enemies(_enemies),
	id(id_()++)
{}

int MyGameProject::Enemy::get_id(void) const { return id; }

void MyGameProject::Enemy::update(void)
{
	
	custom_updater();
	area().get_physical_state().p = pos();
	++count;
}

MyGameProject::Enemy::Class MyGameProject::Enemy::get_class(void) const
{
	return enemy_class;
}

int MyGameProject::Enemy::get_count(void) const
{
	return count;
}

MyGameProject::Real MyGameProject::Enemy::angle(void) const
{
	return ang;
}

void MyGameProject::Enemy::angle(Real _ang)
{
	ang = _ang;
}


const MyGameProject::Player& MyGameProject::Enemy::player_ref(void) const
{
	return player;
}

MyGameProject::Enemy::~Enemy(void){}

void MyGameProject::Enemy::preperation(void)
{
	Boss::preperation();
	MobEnemy::preperation();
}