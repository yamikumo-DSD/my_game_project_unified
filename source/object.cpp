//object.cpp

#include "object.h"
#include "hitbox.h"

MyGameProject::Object::Object(void){}

MyGameProject::Object::~Object(void){}

void MyGameProject::StaticObject::update(void){}

MyGameProject::StaticObject::~StaticObject(void){}

MyGameProject::HitableObject::HitableObject(const HitBox& _hitbox)
	:hitbox(_hitbox)
{}

const MyGameProject::HitBox& MyGameProject::HitableObject::area(void) const
{
	return hitbox;
}

MyGameProject::HitBox& MyGameProject::HitableObject::area(void)
{
	return hitbox;
}

MyGameProject::HitableObject::~HitableObject(void){}

bool MyGameProject::is_hit(const HitableObject& _obj1, const HitableObject& _obj2)
{
	return intersect(_obj1.area(),_obj2.area());
}

MyGameProject::DynamicObject::DynamicObject(void) :flag(true){}

bool MyGameProject::DynamicObject::get_flag(void) const{ return flag; }

void MyGameProject::DynamicObject::set_flag_off(void){ flag = false; }

MyGameProject::DynamicObject::~DynamicObject(void){}

MyGameProject::MovingObject::MovingObject(const Point2D& _p)
	:p(_p)
{}

MyGameProject::MovingObject::MovingObject(void)
	: p(Point2D(0, 0))
{}

const MyGameProject::Point2D& MyGameProject::MovingObject::pos(void) const
{
	return p;
}

MyGameProject::Point2D& MyGameProject::MovingObject::pos(void)
{
	return p;
}

MyGameProject::MovingObject::~MovingObject(void){}