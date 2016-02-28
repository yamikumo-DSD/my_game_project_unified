//pseud3d.h

//back_ground1.h

#ifndef __PSEUD3D_H
#define __PSEUD3D_H

#include "back_ground.h"
#include "object.h"
#include <array>
#include "smart_ptr.h"

namespace MyGameProject
{
	class Pseud3DTile : public DynamicObject
	{
	private:
		Real angle;
		Real distance;
		std::array<Point2D, 4> p;
	public:
		explicit Pseud3DTile(Real _angle);
		virtual void update(void) override final;
		virtual void draw(void) const override final;
		Real get_angle(void) const;
		static void preperation(void);
		virtual ~Pseud3DTile(void);
	};

	class Pseud3D : public BackGround
	{
	private:
		virtual void custom_updater(void) override final;
		std::vector<gp::smart_ptr<Pseud3DTile>> tiles;
	public:
		Pseud3D(void);
		virtual void draw_front(void) const override final;
		virtual void draw_rear(void) const override final;
		static void preperation(void);
		virtual ~Pseud3D(void);
	};

	class ThroughCloud : public BackGround
	{
	private:
		virtual void custom_updater(void) override final;
		Pseud3D pseud3d;
		int y_1_1, y_1_2, y_2_1, y_2_2;
		int img_1, img_2;
	public:
		ThroughCloud(void);
		virtual void draw_front(void) const override final;
		virtual void draw_rear(void) const override final;
		static void preperation(void);
		virtual ~ThroughCloud(void);
	};
}

#endif
