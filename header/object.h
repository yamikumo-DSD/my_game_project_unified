//object.h

#ifndef __OBJECT_H
#define __OBJECT_H

#include "hitbox.h"
#include "mathematics.h"

namespace MyGameProject
{
	class Object
	{
	public:
		Object(void);
		virtual void draw(void) const = 0;
		virtual void update(void) = 0;
		virtual ~Object(void);
	};

	class StaticObject : virtual public Object
	{
	public:
		using Object::Object;
		virtual void draw(void) const = 0;
		virtual void update(void) final;
		virtual ~StaticObject(void);
	};

	class HitableObject : virtual public Object
	{
	private:
		HitBox hitbox;
	public:
		explicit HitableObject(const HitBox& _hitbox);
		HitBox& area(void);
		const HitBox& area(void) const;
		virtual ~HitableObject(void);
	};

	bool is_hit(const HitableObject& _obj1,const HitableObject& _obj2);

	class DynamicObject : virtual public Object
	{
	private:
		bool flag;
	public:
		DynamicObject(void);
		bool get_flag(void) const;
		void set_flag_off(void);
		virtual ~DynamicObject(void);
	};

	class MovingObject : virtual public Object
	{
	private:
		Point2D p;
	public:
		explicit MovingObject(const Point2D& _p);
		MovingObject(void);
		const Point2D& pos(void) const;
		Point2D& pos(void);
		virtual ~MovingObject(void);
	};
}

#endif