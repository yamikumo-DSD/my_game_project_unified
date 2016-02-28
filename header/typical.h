//typical.h

#ifndef __TYPICAL_H
#define __TYPICAL_H

#include "bullet.h"

namespace MyGameProject
{
	class Typical : public Bullet
	{
	private:
		virtual void custom_updater(void) override final;
	public:
		Typical
			(
			const Enemy& _master,
			const Player& _player,
			const Point2D& _initial_pos,
			Real _initial_angle,
			Behavior _behavior
			);
		virtual void draw(void) const;
		virtual ~Typical(void);
		static void preperation(void);
	};
}

#endif