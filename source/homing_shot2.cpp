//homing_shot2.cpp

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
#include "border_check.h"

namespace MyGameProject
{
	struct HomingShot2Imple
	{
		static int& img(void) { static int img = 0; return img; }
	};

	using namespace boost::math::constants;

	HomingShot2::HomingShot2(decltype(target) _target, Real _angle, const Point2D& _p, int _lock_num)
		:Shot(ShapeElement::Null(), 90, _angle, _p, lock_weight_to_rate(_lock_num)),
		target(_target),
		hit_flag(false),
		mag(static_cast<Real>(0.7)),
		brt(255),
		rot(0),
		count(0),
		count_of_hit(0),
		vars(std::make_unique<HomingShot2Imple>())
	{}

	void HomingShot2::draw(void) const
	{
		int i = 0;

		for (int j = 0; j != trail.size(); ++j)
		{
			gp::SetDrawBlendModeOf
			(
				gp::DrawRotaGraph(gp::level(13), trail[j].x(), trail[j].y(), 0.7 - 0.04 * i, rot, HomingShot2Imple::img(), TRUE),
				DX_BLENDMODE_ADD,
				255 - 20 * i++
			);

			//Fill median position.
			if (j < static_cast<int>(trail.size() - 1))
			{
				gp::SetDrawBlendModeOf
				(
					gp::DrawRotaGraph
					(
						gp::level(13), 
						(trail[j].x() + trail[j + 1].x()) / 2,
						(trail[j].y() + trail[j + 1].y()) / 2,
						(0.7 - 0.04 * i + 0.7 - 0.04 * (i + 1)) / 2, 
						rot, HomingShot2Imple::img(), TRUE
					),
					DX_BLENDMODE_ADD,
					(255 - 20 * i + 255 - 20 * (i + 1)) / 2
				);
			}
		}
	}

	void HomingShot2::hit(void)
	{
		hit_flag = true;
		count_of_hit = count;
	}

	bool HomingShot2::is_active(void) const
	{
		return !hit_flag;
	}

	void HomingShot2::custom_updater(void)
	{
		if (count == 0) { for (auto& p : trail) { p = pos(); } }
		else
		{
			for (int i = trail.size() - 1; i != 0; --i)
			{
				trail[i] = trail[i - 1];
			}
		}

		Real speed = 30.0;

		if (!hit_flag)
		{
			const auto x = pos().x(), y = pos().y();

			if (target && count >= 5)
			{
				if (count == 5) { area().get_shape() = ShapeElement::Circle(20); }
				auto to_target{target->pos() - pos()};
				to_target = to_target * (1 / norm(to_target)); //normalize
				const auto cross{cos(angle()) * to_target.y() - sin(angle()) * to_target.x()};
				angle(angle() + 0.6 * cross);
			}
			pos().x(x + speed * std::cos(angle()));
			pos().y(y + speed * std::sin(angle()));

			trail[0] = pos();

			if (is_within_window(pos(), 200) && (count > 200 || !target))
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
	} //HomingShot2::custom_updater

	HomingShot2::~HomingShot2(void) {}

	void HomingShot2::preperation(void)
	{
		ImagePool::add("../../data/img/white_ball_red.png");
		HomingShot2Imple::img() = ImagePool::get("../../data/img/white_ball_red.png");
	}
} //namespace MyGameProject