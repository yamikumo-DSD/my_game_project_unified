//lazer.h

#pragma once

#include "bullet.h"
#include <memory>

namespace MyGameProject
{
	struct LazerImple;

	class Lazer : public Bullet
	{
	private:
		friend struct LazerImple;
		virtual void custom_updater(void) override final;
		std::unique_ptr<LazerImple> vars;
	public:
		Lazer
		(
			const Enemy& _master,
			const Player& _player,
			const Point2D& _initial_pos,
			Real _initial_angle,
			Behavior _behavior,
			Real _max_width = 50.f,
			int _duration = 300,
			Real _grow_speed = 10.f
		);
		Lazer
		(
			const Enemy& _master,
			const Player& _player,
			const Point2D& _initial_pos,
			Real _initial_angle,
			Behavior _behavior,
			Real _max_width,
			int _duration,
			std::function<void(const Bullet&, Real&)> _f, //function to change angle. first arg: laser itself, second arg: laser's angle
			Real _grow_speed = 10.f
		);
		virtual void draw(void) const;
		virtual void hit(void);
		virtual bool is_resistant(void) const override final { return true; }
		virtual ~Lazer(void);
		static void preperation(void);
	};
}
