//feather.h

#ifndef __FEATHER_H
#define __FEATHER_H

#include "bullet.h"

namespace MyGameProject
{
	class Feather : public Bullet
	{
	private:
		virtual void custom_updater(void) override final;
		Real rot;
	public:
		Feather
			(
			const Enemy& _master,
			const Player& _player,
			const Point2D& _initial_pos,
			Real _initial_angle,
			Behavior _behavior
			);
		virtual void draw(void) const;
		virtual ~Feather(void);
		static void preperation(void);
	};
}

#endif
