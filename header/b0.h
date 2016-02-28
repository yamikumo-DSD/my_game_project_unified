//b0.h

#pragma once

#include "bullet.h"
#include <memory>

namespace MyGameProject
{
	struct B0Imple;

	class B0 : public Bullet
	{
	private:
		virtual void custom_updater(void) override final;
		std::unique_ptr<B0Imple> vars;
	public:
		friend struct B0Imple;
		B0
		(
			const Enemy& _master,
			const Player& _player,
			const Point2D& _initial_pos,
			Real _initial_angle,
			Behavior _behavior
		);
		virtual void draw(void) const;
		virtual ~B0(void);
		static void preperation(void);
	};
}
