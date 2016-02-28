//back_ground1.h

#ifndef __BACK_GROUND_1_H
#define __BACK_GROUND_1_H

#include "back_ground.h"
#include "object.h"
#include <vector>
#include "smart_ptr.h"

namespace MyGameProject
{
	class Cloud : public MovingObject, public DynamicObject
	{
		const int img_type;
	public:
		Cloud(int _img_type,const Point2D& _initial_pos);
		virtual void update(void) override final;
		virtual void draw(void) const override final;
		static void preperation(void);
		virtual ~Cloud(void);
	};

	class BackGround1 : public BackGround
	{
	private:
		std::vector<gp::smart_ptr<Cloud>> clouds;
		virtual void custom_updater(void) override final;
	public:
		BackGround1(void);
		//virtual void draw(void) const override final;
		virtual void draw_rear(void) const override final;
		static void preperation(void);
		virtual ~BackGround1(void);
	};
}

#endif
