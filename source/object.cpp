//object.cpp

#include "object.h"
#include "hitbox.h"

namespace MyGameProject
{

	Object::Object(void) {}

	Object::~Object(void) {}

	void StaticObject::update(void) {}

	StaticObject::~StaticObject(void) {}

	HitableObject::HitableObject(const HitBox& _hitbox)
		:hitbox(_hitbox)
	{}

	const HitBox& HitableObject::area(void) const
	{
		return hitbox;
	}

	HitBox& HitableObject::area(void)
	{
		return hitbox;
	}

	HitableObject::~HitableObject(void) {}

	bool is_hit(const HitableObject& _obj1, const HitableObject& _obj2)
	{
		return intersect(_obj1.area(), _obj2.area());
	}

	DynamicObject::DynamicObject(void) :flag(true) {}

	bool DynamicObject::get_flag(void) const { return flag; }

	void DynamicObject::set_flag_off(void) { flag = false; }

	DynamicObject::~DynamicObject(void) {}

	MovingObject::MovingObject(const Point2D& _p)
		:p(_p)
	{}

	MovingObject::MovingObject(void)
		: p(Point2D(0, 0))
	{}

	const Point2D& MovingObject::pos(void) const
	{
		return p;
	}

	Point2D& MovingObject::pos(void)
	{
		return p;
	}

	MovingObject::~MovingObject(void) {}

}//namespace MyGameProject