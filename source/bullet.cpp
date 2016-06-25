//bullet.cpp

#include "bullet.h"
#include <dxlib.h>
#include "player.h"
#include "enemy.h"
#include "draw_order.h"
#include "dxlib_wrapper.h"
#include <exception>
#include "concrete_bullet.h"

gp::smart_ptr<MyGameProject::Bullet> MyGameProject::Bullet::create
(
	const std::string& _name,
	const Enemy& _master,
	const Player& _player,
	const Point2D& _initial_pos,
	Real _initial_angle,
	Behavior _behavior
)
{
	if (_name == "Typical"){ return gp::make_smart<Typical>(_master,_player,_initial_pos,_initial_angle,_behavior); }
	if (_name == "Cutter"){ return gp::make_smart<Cutter>(_master, _player, _initial_pos, _initial_angle, _behavior); }
	if (_name == "Feather"){ return gp::make_smart<Feather>(_master, _player, _initial_pos, _initial_angle, _behavior); }
	if (_name == "WindSmasher"){ return gp::make_smart<WindSmasher>(_master, _player, _initial_pos, _initial_angle, _behavior); }
	if (_name == "HenyoriElementOrange"){ return gp::make_smart<HenyoriElementOrange>(_master, _player, _initial_pos, _initial_angle, _behavior); }
	if (_name == "CameraFrame"){ return gp::make_smart<CameraFrame>(_master, _player, _initial_pos, _initial_angle, _behavior); }
	if (_name == "B0"){ return gp::make_smart<B0>(_master, _player, _initial_pos, _initial_angle, _behavior); }
	if (_name == "B1"){ return gp::make_smart<B1>(_master, _player, _initial_pos, _initial_angle, _behavior); }
	if (_name == "B2"){ return gp::make_smart<B234<2>>(_master, _player, _initial_pos, _initial_angle, _behavior); }
	if (_name == "B3"){ return gp::make_smart<B234<3>>(_master, _player, _initial_pos, _initial_angle, _behavior); }
	if (_name == "B4"){ return gp::make_smart<B234<4>>(_master, _player, _initial_pos, _initial_angle, _behavior); }
	else{ throw std::runtime_error("Undefined type of bullet."); }
}

MyGameProject::Bullet::Bullet
(
	const Enemy& _master,
	const Player& _player,
	const Point2D& _initial_pos,
	Real _initial_angle,
	const Shape& _hitbox_shape,
	Behavior _behavior
)
: HitableObject(HitBox(_hitbox_shape, PhysicalState(_initial_angle, _initial_pos))),
MovingObject(_initial_pos),
player(_player),
master(_master),
count(0),
behavior(_behavior),
ang(_initial_angle)
{}

void MyGameProject::Bullet::update(void)
{
	pre_pos = pos();
	behavior(*this);
	custom_updater();
	area().get_physical_state().p = pos();
	++count;
}

bool MyGameProject::Bullet::is_resistant(void) const{ return false; }

int MyGameProject::Bullet::get_count(void) const
{
	return count;
}

MyGameProject::Point2D MyGameProject::Bullet::v(void) const
{
	return pos() - pre_pos;
}

MyGameProject::Real MyGameProject::Bullet::angle(void) const
{
	return ang;
}

void MyGameProject::Bullet::angle(Real _ang)
{
	ang = _ang;
}

const MyGameProject::Enemy& MyGameProject::Bullet::get_master_ref(void) const
{
	return master;
}

const MyGameProject::Player& MyGameProject::Bullet::get_player_ref(void) const
{
	return player;
}

void MyGameProject::Bullet::hit(void)
{
	set_flag_off();
}

MyGameProject::Bullet::~Bullet(void){}

void MyGameProject::Bullet::preperation(void)
{
	Typical::preperation();
	Cutter::preperation();
	WindSmasher::preperation();
	Feather::preperation();
	HenyoriElementOrange::preperation();
	CameraFrame::preperation();
	Lazer::preperation();
	B0::preperation();
	B1::preperation();
	B234<2>::preparation();
	B234<3>::preparation();
	B234<4>::preparation();
}