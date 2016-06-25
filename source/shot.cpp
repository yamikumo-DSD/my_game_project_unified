//shot.cpp

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

namespace MyGameProject
{
	using namespace boost::math::constants;

	Shot::Shot(const Shape& _shape, int _pow, Real _angle, const Point2D& _p)
		:ang(_angle - pi<Real>() / 2),
		MovingObject(_p),
		HitableObject(HitBox(_shape, PhysicalState(0, _p))),
		pow(_pow), rate_(1.f)
	{}

	Shot::Shot(const Shape& _shape, int _pow, Real _angle, const Point2D& _p, Real _rate)
		:ang(_angle - pi<Real>() / 2),
		MovingObject(_p),
		HitableObject(HitBox(_shape, PhysicalState(0, _p))),
		pow(_pow), rate_(_rate)
	{}

	Real Shot::rate(void) const noexcept
	{
		return rate_;
	}

	void Shot::update(void)
	{
		custom_updater();
		area().get_physical_state().p = pos();
	}

	int Shot::power(void) const { return pow; }

	Real Shot::angle(void) const { return ang; }

	void Shot::angle(Real _angle) { ang = _angle; }

	Shot::~Shot(void) {}

	void Shot::preperation(void)
	{
		NormalShot::preperation();
		HyperShot2::preperation();
		HomingShot::preperation();
		HomingLazer::preperation();
		HomingLazer2::preperation();
		HomingShot2::preperation();
	}

	HomingShot::HomingShot(std::function<gp::smart_ptr<Enemy>(HomingShot&)> _find_target, Real _angle, const Point2D& _p)
		:Shot(ShapeElement::Circle(20), 1, _angle, _p),
		target(),
		find_target(_find_target),
		hit_flag(false),
		mag(static_cast<Real>(0.7)),
		brt(255),
		rot(0),
		count(0),
		color_type(0),
		count_of_hit(0)
	{}

	HomingShot::HomingShot(std::function<gp::smart_ptr<Enemy>(HomingShot&)> _find_target, Real _angle, const Point2D& _p, int _color_type)
		:Shot(ShapeElement::Circle(20), 15, _angle, _p),
		target(),
		find_target(_find_target),
		hit_flag(false),
		mag(static_cast<Real>(0.7)),
		brt(255),
		rot(0),
		count(0),
		color_type(_color_type),
		count_of_hit(0)
	{}

	void HomingShot::draw(void) const
	{
		int i = 0;

		for (const auto& p : trail)
		{
			ScopedDrawBlendMode blend(DX_BLENDMODE_ADD, 255 - 20 * i++);
			switch (color_type)
			{
			case 0:
				gp::DrawRotaGraph(p.x(), p.y(), 0.7 - 0.04 * i, rot, ImagePool::get("../../data/img/white_ball_green.png"), TRUE); break;
			case 1:
				gp::DrawRotaGraph(p.x(), p.y(), 0.7 - 0.04 * i, rot, ImagePool::get("../../data/img/white_ball_yellow.png"), TRUE); break;
			case 2:
				gp::DrawRotaGraph(p.x(), p.y(), 0.7 - 0.04 * i, rot, ImagePool::get("../../data/img/white_ball_red.png"), TRUE); break;
			case 3:
				gp::DrawRotaGraph(p.x(), p.y(), 0.7 - 0.04 * i, rot, ImagePool::get("../../data/img/white_ball_black.png"), TRUE); break;
			case 4:
				gp::DrawRotaGraph(p.x(), p.y(), 0.7 - 0.04 * i, rot, ImagePool::get("../../data/img/white_ball_white.png"), TRUE); break;
			default:
				gp::DrawRotaGraph(p.x(), p.y(), 0.7 - 0.04 * i, rot, ImagePool::get("../../data/img/white_ball_black.png"), TRUE); break;
			}
		}
	} //HomingShot::draw

	void HomingShot::hit(void)
	{
		hit_flag = true;
		count_of_hit = count;
	}

	bool HomingShot::is_active(void) const
	{
		return !hit_flag;
	}

	void HomingShot::custom_updater(void)
	{
		if (count == 0) { for (auto& p : trail) { p = pos(); } }
		else
		{
			for (int i = trail.size() - 1; i != 0; --i)
			{
				trail[i] = trail[i - 1];
			}
		}

		Real speed = 15.0;

		if (!hit_flag)
		{
			const auto x = pos().x(), y = pos().y();

			target = find_target(*this);

			if (target)
			{
				if (count > 20) { speed = 30; }
				Real omega = static_cast<Real>(0.2); if (count > 30 && count < 100) { omega += (count - 30) * static_cast<Real>(0.002); }

				Real theta = angle_of(target->pos() - pos());

				if (count < 100)
				{
					if (abs(angle() - theta) >= omega)
					{
						if (sin(angle() - theta) <= 0) { angle(angle() + omega); }
						else { angle(angle() - omega); }
					}
					if (abs(abs(angle() - theta) - two_pi<Real>()) < omega) { angle(theta); }
				}
			}
			pos().x(x + speed * std::cos(angle()));
			pos().y(y + speed * std::sin(angle()));

			trail[0] = pos();

			if (x < -100 || x > 640 + 100 || y < -100 || y > 480 + 100)
			{
				set_flag_off();
			}
		}
		else
		{
			if (count > count_of_hit + 20) { set_flag_off(); }
			else { brt -= 50; }
		}

		++count;
	} //HomingShot::custom_updater

	HomingShot::~HomingShot(void) {}

	void HomingShot::preperation(void)
	{
		ImagePool::add("../../data/img/white_ball_green.png");
		ImagePool::add("../../data/img/white_ball_red.png");
		ImagePool::add("../../data/img/white_ball_black.png");
		ImagePool::add("../../data/img/white_ball_white.png");
		ImagePool::add("../../data/img/white_ball_yellow.png");
	}

	HomingLazer::HomingLazer(std::function<gp::smart_ptr<Enemy>(HomingLazer&)> _find_target, Real _angle, const Point2D& _p, int _color_type)
		:Shot(ShapeElement::Circle(20), 10, _angle, _p),
		target(),
		find_target(_find_target),
		hit_flag(false),
		brt(255),
		count(0),
		color_type(_color_type),
		count_of_hit(0),
		tail_tex(ImagePool::get("../../data/img/tail_green.png"))
	{}

	void HomingLazer::draw(void) const
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
					m0_x + mx_i_0_x, m0_y + mx_i_0_y,
					m0_x + mx_i_1_x, m0_y + mx_i_1_y,
					m1_x + mx_i__1_x, m1_y + mx_i__1_y,
					m1_x + mx_i__0_x, m1_y + mx_i__0_y,
					hl_tex()[i],
					true
					);

			gp::DrawModiGraph
				(
					m0_x + mx_i_0_x, m0_y + mx_i_0_y,
					m0_x + mx_i_1_x, m0_y + mx_i_1_y,
					m1_x + mx_i__1_x, m1_y + mx_i__1_y,
					m1_x + mx_i__0_x, m1_y + mx_i__0_y,
					texture()[i],
					true
					);
		}

	} //namespace HomingLazer::draw

	void HomingLazer::hit(void) { hit_flag = true; }

	bool HomingLazer::is_active(void) const { return !hit_flag; }

	void HomingLazer::custom_updater(void)
	{
		static constexpr Real W = 30.0;

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
				trail[i] = post_trail[i - 1];
			}
		}

		//Update position.
		Real speed = 30.0;

		if (!hit_flag)
		{
			const auto x = pos().x(), y = pos().y();

			target = find_target(*this);

			if (target)
			{
				if (count > 20) { speed = 40; }
				Real omega = static_cast<Real>(0.25); if (count > 30 && count < 70) { omega += (count - 30) * static_cast<Real>(0.002); }

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
			if (count > count_of_hit + 100) { set_flag_off(); }
		}

		++count;
	} //HomingLazer::custom_updater

	HomingLazer::~HomingLazer(void) {}

	void HomingLazer::preperation(void)
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

