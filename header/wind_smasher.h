//wind_smasher.h

#ifndef __WIND_SMASHER
#define __WIND_SMASHER

#include "bullet.h"

namespace MyGameProject
{
	class WindSmasher : public Bullet
	{
	private:
		virtual void custom_updater(void) override final;
		const int tex;
	public:
		WindSmasher
			(
			const Enemy& _master,
			const Player& _player,
			const Point2D& _initial_pos,
			Real _initial_angle,
			Behavior _behavior
			);
		virtual void draw(void) const;
		virtual ~WindSmasher(void);
		static void preperation(void);
	};
}

#endif
