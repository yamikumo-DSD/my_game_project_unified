//b_234.h

#pragma once

#include "bullet.h"
#include "mathematics.h"
#include "draw_order.h"
#include "border_check.h"

namespace MyGameProject
{
	template<int N>
	class B234 : public Bullet
	{
	private:
		static std::array<int, 3>& img_handle(void);
		Real old_angle{0};
		virtual void custom_updater(void) override final
		{
			if ( get_count() > 200 && !is_within_window(pos(), 10) )
			{
				set_flag_off();
			}
			if (norm(v()) > 0.001f) { old_angle = angle_of(v()); }
		}
	public:
		B234
		(
			const Enemy& _master,
			const Player& _player,
			const Point2D& _initial_pos,
			Real _initial_angle,
			Behavior _behavior
		) noexcept
		:Bullet(_master, _player, _initial_pos, _initial_angle, ShapeElement::Circle(8), _behavior),
			old_angle(_initial_angle)
		{}
		virtual ~B234(void) noexcept = default;

		virtual void draw(void) const override
		{
			const Real angle = norm(v()) > 0.001f ? angle_of(v()) : old_angle;
			gp::DrawRotaGraphF
			(
				gp::level(12),
				pos().x(), pos().y(), 1.5, angle + boost::math::constants::half_pi<Real>(),
				img_handle()[(get_count() / 4) % 3], true
			);
		}
		static void preparation(void) noexcept;
	};

}
