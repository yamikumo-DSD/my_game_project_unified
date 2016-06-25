//forced_crow_bullet.cpp

#include "aya_imple.h"
#include "image_pool.h"
#include "draw_order.h"
#include <dxlib.h>
#include "border_check.h"

#ifndef FCB
#define FCB ForcedCrowBullet::
#endif

namespace MyGameProject
{
	FCB ForcedCrowBullet
		(
			const Enemy& _master,
			const Player& _player,
			const Point2D& _initial_pos,
			Real _initial_angle,
			const Behavior& _behavior
		) noexcept
		:Bullet
		(
			_master,
			_player,
			_initial_pos,
			_initial_angle,
			ShapeElement::Circle(15), 
			_behavior
		)
	{} //ForcedCrowBullet::ForcedCrowBullet

	void FCB draw(void) const
	{
		if (get_count() > 0)
		{
			const auto dir{ angle_of(v()) };
			for (const auto& p : trails)
			{
				gp::SetDrawBlendModeOf
				(
					gp::DrawRotaGraphF(gp::level(10), p.first.x(), p.first.y(), 1.0, boost::math::constants::half_pi<Real>() + p.second, trail_img(), true),
					DX_BLENDMODE_ALPHA,
					100
				);
			}
			gp::DrawRotaGraphF(gp::level(10), pos().x(), pos().y(), 1.0, boost::math::constants::half_pi<Real>() + dir, img(), true);
		}
	}

	void FCB custom_updater(void)
	{
		const auto count{get_count()};
		if (count == 0)
		{
			for (auto& p : trails) { p.first = pos(); p.second = angle_of(v());}
		}
		else
		{
			if (count >= 1000 && !is_within_window(pos(), 100)) { set_flag_off(); }

			for (int i = trails.size() - 1; i != 0; --i) { trails[i] = trails[i - 1]; ; }
			trails[0].first = pos(); trails[0].second = angle_of(v());
		}
	} //ForcedCrowBullet::custom_updater

	void FCB preparation(void) noexcept
	{
		static auto add_and_get
		{
			[](auto&& _path)
			{
				ImagePool::add(_path);
				return ImagePool::get(std::forward<decltype(_path)>(_path));
			}
		};

		img() = add_and_get("../../data/img/crow1.png");
		trail_img() = add_and_get("../../data/img/crow_trail.png");
	} //ForcedCrowbullet::preparation

} //namespace MyGameProject
