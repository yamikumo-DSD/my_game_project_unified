//invisible.h

#pragma once

#include "bullet.h"
#include <memory>
#include "bullet_behavior.h"

namespace MyGameProject
{
	class InvisibleRoundBullet : public Bullet
	{
	private:
		virtual void custom_updater(void) override final{}
	public:
		InvisibleRoundBullet
		(
			const Enemy& _master,
			const Player& _player,
			const Point2D& _initial_pos,
			Real _initial_angle,
			Real _radius
		) :Bullet(_master, _player, _initial_pos, _initial_angle, ShapeElement::Circle(_radius), dummy_course)
		{}
		virtual void draw(void) const {}
		virtual bool is_resistant(void) const { return true; }
		virtual void hit(void) {}
		virtual ~InvisibleRoundBullet(void) {}
	};

	//Alias.
	using IRB = InvisibleRoundBullet;
}
