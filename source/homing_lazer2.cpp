//homing_lazer2.cpp

#include "shot.h"
#include "hitbox.h"
#include <cmath>
#include "image_pool.h"
#include "draw_order.h"
#include "dxlib_wrapper.h"
#include <boost/math/constants/constants.hpp>
#include "enemy.h"
#include "color.h"
#include "drawblendmode.h"
#include "ranged_uniform_random.h"
#include "dxsstream.h"
#include "debug_value.h"
#include "lock_weight_to_rate.h"

namespace MyGameProject
{
	using namespace boost::math::constants;

	HomingLazer2::HomingLazer2(decltype(target) _target, Real _angle, const Point2D& _p, int _lock_num)
		:Shot(ShapeElement::Circle(20), 70, _angle, _p, lock_weight_to_rate(_lock_num)),
		target(_target),
		hit_flag(false),
		brt(255),
		count(0),
		count_of_hit(0),
		tail_tex(ImagePool::get("../../data/img/tail_green.png"))
	{
	}

	void HomingLazer2::draw(void) const
	{
		Point2D OM0, OM1;
		const auto s = trail.size();
		for (int i = 0; i != s; ++i)
		{
			if (i == 0)
			{
				OM1.x((trail[0].x() + trail[1].x()) / 2); OM1.y((trail[0].y() + trail[1].y()) / 2);
				OM0.x(2 * trail[0].x() - OM1.x()); OM0.y(2 * trail[0].y() - OM1.y());
			}
			else if (i == s - 1)
			{
				OM0.x((trail[s - 1].x() + trail[s - 2].x()) / 2); OM0.y((trail[s - 1].y() + trail[s - 2].y()) / 2);
				OM1.x(2 * trail[s - 1].x() - OM0.x()); OM1.y(2 * trail[s - 1].y() - OM0.y());
			}
			else
			{
				OM0.x((trail[i].x() + trail[i - 1].x()) / 2); OM0.y((trail[i].y() + trail[i - 1].y()) / 2);
				OM1.x((trail[i + 1].x() + trail[i].x()) / 2); OM1.y((trail[i + 1].y() + trail[i].y()) / 2);
			}

			const auto m0_x = OM0.x(), m0_y = OM0.y(), m1_x = OM1.x(), m1_y = OM1.y();
			const auto mx_i_0_x = MX[i][0].x(), mx_i_0_y = MX[i][0].y();
			const auto mx_i_1_x = MX[i][1].x(), mx_i_1_y = MX[i][1].y();
			const auto mx_i__0_x = MX[i][0].x(), mx_i__0_y = MX[i][0].y();
			const auto mx_i__1_x = MX[i][1].x(), mx_i__1_y = MX[i][1].y();

			gp::DrawModiGraph
				(
					gp::level(13),
					m0_x + mx_i_0_x, m0_y + mx_i_0_y,
					m0_x + mx_i_1_x, m0_y + mx_i_1_y,
					m1_x + mx_i__1_x, m1_y + mx_i__1_y,
					m1_x + mx_i__0_x, m1_y + mx_i__0_y,
					hl_tex()[i],
					true
					);

			gp::DrawModiGraph
				(
					gp::level(13),
					m0_x + mx_i_0_x, m0_y + mx_i_0_y,
					m0_x + mx_i_1_x, m0_y + mx_i_1_y,
					m1_x + mx_i__1_x, m1_y + mx_i__1_y,
					m1_x + mx_i__0_x, m1_y + mx_i__0_y,
					texture()[i],
					true
					);
		}
	} //HomingLazer2::draw

	void HomingLazer2::hit(void)
	{
		hit_flag = true;
	}

	bool HomingLazer2::is_active(void) const
	{
		return !hit_flag;
	}

	void HomingLazer2::custom_updater(void)
	{
		static constexpr Real W = 50.0;

		if (count == 0) { for (auto& p : trail) { p = pos(); } }
		else
		{
			for (int i = 1; i != trail.size(); ++i)
			{
				const auto v = trail[i] - trail[i - 1];
				const auto r = norm(v);
				auto& mx_i = MX[i];
				if (r != 0)
				{
					const auto x = (W / 2) * (v.y()) / r;
					const auto y = (W / 2) * (v.x()) / r;
					mx_i[0].x(x); mx_i[0].y(-y);
					mx_i[1].x(-x); mx_i[1].y(y);
				}
				else
				{
					mx_i[0].x(0); mx_i[0].y(0);
					mx_i[1].x(0); mx_i[1].y(0);
				}
			}
			MX[0][0] = MX[1][0]; MX[0][1] = MX[1][1];
			MX[trail.size()][0] = MX[trail.size() - 1][0]; MX[trail.size()][1] = MX[trail.size() - 1][1];

			auto post_trail = trail;

			for (int i = 1; i != trail.size(); ++i)
			{
				trail[i] = std::move(post_trail[i - 1]);
			}
		}

		//Update position.
		Real speed = 30.0;

		if (!hit_flag)
		{
			const auto x = pos().x(), y = pos().y();

			if (target)
			{
				if (count > 20) { speed = 40; }
				Real omega = static_cast<Real>(0.30); if (count > 30 && count < 70) { omega += (count - 30) * static_cast<Real>(0.007); }

				Real theta = angle_of(target->pos() - pos());

				if (count < 70)
				{
					if (abs(angle() - theta) > omega)
					{
						if (sin(angle() - theta) < 0) { angle(angle() + omega); }
						else if (sin(angle() - theta) > 0) { angle(angle() - omega); }
					}
					if (abs(abs(angle() - theta) - two_pi<Real>()) < omega) { angle(theta); }
				}
			}
			pos().x(x + speed * cos(angle()));
			pos().y(y + speed * sin(angle()));

			//trail[0] corresponding with current position.
			trail[0] = pos();

			if (x < -100 || x > 640 + 100 || y < -100 || y > 480 + 100)
			{
				set_flag_off();
			}
		}
		else
		{
			if (count > count_of_hit + 30) { set_flag_off(); }
		}

		++count;
	} //HomingLazer2::custom_updater

	HomingLazer2::~HomingLazer2(void) {}

	void HomingLazer2::preperation(void)
	{
		ImagePool::add("../../data/img/laser2.png");
		ImagePool::add("../../data/img/laser3.png");
		ImagePool::add("../../data/img/tail_green.png");
		ImagePool::add("../../data/img/bl_00.png");
		ImagePool::add_multi("../../data/img/red_laser.png", TRAIL_NUM - 1, 1, TRAIL_NUM - 1, 30, 100 / (TRAIL_NUM - 1));
		for (int i = 0; i != texture().size(); ++i)
		{
			texture()[i] = ImagePool::get("../../data/img/red_laser.png@" + boost::lexical_cast<std::string>(i));
		}
		ImagePool::add_multi("../../data/img/red_back.png", TRAIL_NUM - 1, 1, TRAIL_NUM - 1, 30, 100 / (TRAIL_NUM - 1));
		for (int i = 0; i != texture().size(); ++i)
		{
			hl_tex()[i] = ImagePool::get("../../data/img/red_back.png@" + boost::lexical_cast<std::string>(i));
		}
	}
} //namespace MyGameProject
