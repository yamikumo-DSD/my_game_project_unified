//cutter.h

#ifndef __CUTTER_H
#define __CUTTER_H

#include "bullet.h"

namespace MyGameProject
{
	class Cutter : public Bullet
	{
	private:
		virtual void custom_updater(void) override final;
	public:
		Cutter
			(
			const Enemy& _master,
			const Player& _player,
			const Point2D& _initial_pos,
			Real _initial_angle,
			Behavior _behavior
			);
		virtual void draw(void) const;
		virtual ~Cutter(void);
		static void preperation(void);
	};
}

#endif