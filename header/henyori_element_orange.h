//henyori_element_orange.h

#ifndef __HENYORI_ELEMENT_ORANGE_H
#define __HENYORI_ELEMENT_ORANGE_H

#include "bullet.h"

namespace MyGameProject
{
	class HenyoriElementOrange : public Bullet
	{
	private:
		virtual void custom_updater(void) override final;
		Point2D pre_pos;
		const int tex;
	public:
		HenyoriElementOrange
			(
			const Enemy& _master,
			const Player& _player,
			const Point2D& _initial_pos,
			Real _initial_angle,
			Behavior _behavior
			);
		virtual void draw(void) const;
		virtual ~HenyoriElementOrange(void);
		static void preperation(void);
	};
}

#endif
