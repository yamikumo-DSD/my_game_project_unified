//b1.h

#pragma once

#include "bullet.h"
#include <memory>

namespace MyGameProject
{
	struct B1Imple;

	//Red version of B0
	class B1 : public Bullet
	{
	private:
		virtual void custom_updater(void) override final;
		std::unique_ptr<B1Imple> vars;
	public:
		friend struct B1Imple;
		B1
		(
			const Enemy& _master,
			const Player& _player,
			const Point2D& _initial_pos,
			Real _initial_angle,
			Behavior _behavior
		);
		virtual void draw(void) const;
		virtual ~B1(void);
		static void preperation(void);
	};
}
