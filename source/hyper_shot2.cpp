//hyper_shot2.cpp

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

	HyperShot2::HyperShot2(Real _angle,const Point2D& _p)
		:Shot(ShapeElement::Circle(15),7,_angle,_p),
		hit_flag(false),
		hit_moment(0),
		count(0)
	{}

	void HyperShot2::draw(void) const
	{
		if (!hit_flag)
		{
			gp::SetDrawBlendModeOf
			(
				gp::DrawRotaGraph(gp::level(13), pos().x(), pos().y(), 1.7, angle() + pi<Real>() / 2, ImagePool::get("../../data/img/hyper_shot.png"), TRUE),
				DX_BLENDMODE_MULA,
				100
			);
			gp::SetDrawBlendModeOf
			(
				gp::DrawRotaGraph(gp::level(13), pos().x(), pos().y(), 1.7, angle() + pi<Real>() / 2, ImagePool::get("../../data/img/hyper_shot.png"), TRUE),
				DX_BLENDMODE_ADD,
				100
			);
		}
		else{ gp::DrawRotaGraph(gp::level(14), pos().x(), pos().y(), 1.7, angle() + pi<Real>() / 2, ImagePool::get("../../data/img/shot_active.png"), TRUE); }
	}

	void HyperShot2::hit(void)
	{
		hit_flag = true;
		if (!hit_flag){ hit_moment = count; }
	}

	bool HyperShot2::is_active(void) const
	{
		return !hit_flag;
	}

	void HyperShot2::custom_updater(void) 
	{
		static constexpr Real SHOT_VELOCITY = 15.0;

		if (!hit_flag)
		{
			const auto x = pos().x(), y = pos().y();
			pos().x(x + SHOT_VELOCITY * std::cos(angle()));
			pos().y(y + SHOT_VELOCITY * std::sin(angle()));
			if (x < -100 || x > 640 + 100 || y < -100 || y > 480 + 100)
			{
				set_flag_off();
			}
		}
		else
		{
			if (count > hit_moment + 10){ set_flag_off(); }
		}

		++count;
	}

	HyperShot2::~HyperShot2(void){}

	void HyperShot2::preperation(void)
	{
		//Load images.
		ImagePool::add("../../data/img/hyper_shot.png");
		ImagePool::add("../../data/img/shot_active.png");
	}
}

