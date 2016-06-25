//b_234.h

#pragma once

#include "bullet.h"
#include "mathematics.h"
#include "draw_order.h"
#include "environmental_constants.h"

namespace MyGameProject
{
	template<int N>
	class B234 : public Bullet
	{
	private:
		virtual void custom_updater(void) override final
		{
			if 
			(
				get_count() > 200 && 
				(
					pos().x() < 0 ||
					pos().x() > WW<Real>() || 
					pos().y() < 0 ||
					pos().y() > WH<Real>()
				)
			)
			{
				set_flag_off();
			}
		}
		static std::array<int, 3>& img_handle(void);
	public:
		B234
		(
			const Enemy& _master,
			const Player& _player,
			const Point2D& _initial_pos,
			Real _initial_angle,
			Behavior _behavior
		) noexcept
		:Bullet(_master, _player, _initial_pos, _initial_angle, ShapeElement::Circle(8), _behavior)
		{}
		virtual ~B234(void) noexcept = default;

		virtual void draw(void) const override
		{
			gp::DrawRotaGraphF
			(
				gp::level(12),
				pos().x(), pos().y(), 1.5, angle_of(v()) + boost::math::constants::half_pi<Real>(),
				img_handle()[(get_count() / 4) % 3], true
			);
		}
		static void preparation(void) noexcept;
	};

}
