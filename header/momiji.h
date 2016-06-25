//momiji.h

#pragma once

#include "object.h"
#include <type_traits>
#include "draw_order.h"
#include "decl_static_image_handler.h"
#include "image_pool.h"
#include "ranged_uniform_random.h"
#include <dxlib.h>

namespace MyGameProject
{
	class Momiji final : public DynamicObject, public MovingObject
	{
	private:
		STATIC_IMAGE_HANDLER(img)
		Real rot;
		const Real amp_x;
		Real theta{0};
		const Real dtheta;
		const Real falling_speed;
		Real rot_speed; 
		const Real x_base_line;
		const Real scale;
	public:
		template<typename Point, std::enable_if_t<!std::is_base_of<Momiji, std::decay_t<Point>>::value>* = nullptr>
		explicit Momiji(Point&& _initial_pos)
			:MovingObject(std::forward<Point>(_initial_pos)),
			rot(gp::safe_rand<Real>(0, boost::math::constants::two_pi<Real>())),
			falling_speed(gp::safe_rand(3, 7)),
			amp_x(gp::safe_rand(30, 100)),
			dtheta(gp::safe_rand(0.01f, 0.02f)),
			rot_speed(gp::safe_rand(0.05f, 0.15f)),
			x_base_line(_initial_pos.x()),
			scale(gp::safe_rand<Real>(0.5, 1.3))
		{}
		virtual ~Momiji(void) noexcept = default;

		virtual void update(void) override final
		{
			using namespace std;
			pos().x(x_base_line + amp_x * sin(theta += dtheta));
			pos().y(pos().y() + falling_speed);
			rot += rot_speed;
			if (pos().y() > 500) { set_flag_off(); }
		}
		virtual void draw(void) const override final
		{
			gp::SetDrawBlendModeOf
			(
				gp::DrawRotaGraphF(gp::level(1), pos().x(), pos().y(), scale, rot, img(), true),
				DX_BLENDMODE_ALPHA,
				100
			);
		}
		static void preparation(void) noexcept
		{
			ImagePool::add("../../data/img/momiji.png");
			img() = ImagePool::get("../../data/img/momiji.png");
		}
	};
}
