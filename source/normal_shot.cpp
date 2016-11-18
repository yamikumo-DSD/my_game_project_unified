//normal_shot.cpp

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

	struct NormalShotImple
	{
		static int& ofuda_img(void) { static int img = 0; return img; }
		int count_after_hit;
	};

	NormalShot::NormalShot(Real _angle,const Point2D& _p)
		:Shot(ShapeElement::Circle(25),5,_angle,_p),
		hit_flag(false),
		hit_moment(0),
		count(0),
		vars(std::make_unique<NormalShotImple>())
	{
		vars->count_after_hit = 0;
	}

	void NormalShot::draw(void) const
	{
		if (!hit_flag)
		{
			gp::DrawRotaGraph(gp::level(13), pos().x(), pos().y(), 1.0, angle() + pi<Real>() / 2, NormalShotImple::ofuda_img(), TRUE); 
			gp::SetDrawBlendModeOf
			(
				gp::DrawRotaGraph(gp::level(13), pos().x(), pos().y(), 1.0, angle() + pi<Real>() / 2, ImagePool::get("../../data/img/aura.png"), TRUE),
				DX_BLENDMODE_ALPHA,
				255
			);
		}
		else
		{
			gp::SetDrawBlendModeOf
			(
				gp::DrawRotaGraph(gp::level(13), pos().x(), pos().y(), 1.0, angle() + pi<Real>() / 2, NormalShotImple::ofuda_img(), TRUE), 
				DX_BLENDMODE_ADD,
				100
			);
		}
	}

	void NormalShot::hit(void)
	{
		hit_flag = true;
		if (!hit_flag){ hit_moment = count; }
	}

	bool NormalShot::is_active(void) const
	{
		return !hit_flag;
	}

	void NormalShot::custom_updater(void) 
	{
		static constexpr Real SHOT_VELOCITY = 25.0;

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

		if (hit_flag)
		{
			pos().x(pos().x() + (SHOT_VELOCITY / 5) * std::cos(angle()));
			pos().y(pos().y() + (SHOT_VELOCITY / 5) * std::sin(angle()));
			++vars->count_after_hit;
		}
		++count;
	}

	NormalShot::~NormalShot(void){}

	void NormalShot::preperation(void)
	{
		//Load images.
		ImagePool::add("../../data/img/ofuda.png");
		NormalShotImple::ofuda_img() = ImagePool::get("../../data/img/ofuda.png");
		ImagePool::add("../../data/img/aura.png");
		ImagePool::add("../../data/img/shot_active.png");
		ImagePool::add("../../data/img/shot_hit.png");
	}
}

