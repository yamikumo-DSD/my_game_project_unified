//resistant_bullet.h

#pragma once

#include "bullet.h"
#include <memory>

namespace MyGameProject
{

	class ResistantBullet : public Bullet
	{
	public:
		enum class Color {RED = 0, BLUE, DARK,};
	private:
		struct Impl;
		std::unique_ptr<Impl> pimpl;
		static constexpr auto COLOR_VARIATION{static_cast<std::size_t>(Color::DARK) + 1};
		virtual void custom_updater(void) override final;
	public:
		ResistantBullet
		(
			const Enemy& _master,
			const Player& _player,
			const Point2D& _initial_pos,
			Real _initial_angle,
			Behavior _behavior,
			Color _color = Color::RED
		);
		virtual bool is_resistant(void) const override final{ return true; }
		virtual void draw(void) const;
		virtual ~ResistantBullet(void);
		static void preperation(void);
	};
}
