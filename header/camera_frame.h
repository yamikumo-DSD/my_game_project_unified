//camera_frame.h

#ifndef __CAMERA_FRAME_H
#define __CAMERA_FRAME_H

#include "bullet.h"

namespace MyGameProject
{
	class CameraFrame : public Bullet
	{
	private:
		virtual void custom_updater(void) override final;
		static ShapeElement::Polygon& rect(void){ static ShapeElement::Polygon shape; return shape; }
		static constexpr int ACTIVE_TIME = 150;
		Real mag;
	public:
		CameraFrame
			(
			const Enemy& _master,
			const Player& _player,
			const Point2D& _initial_pos,
			Real _initial_angle,
			Behavior _behavior
			);
		virtual bool is_resistant(void) const override final;
		virtual void draw(void) const;
		virtual ~CameraFrame(void);
		static void preperation(void);
	};
}

#endif
