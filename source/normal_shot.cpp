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

	struct ShotHit : public MovingObject
	{
		static int& shot_hit_img(void) { static int img = 0; return img; }
		static constexpr Real speed = 6.f;
		Real angle;
		int count;
		ShotHit(void):MovingObject(), angle(0.f), count(0){}
		explicit ShotHit(const Point2D& _initial_pos, Real _angle) :MovingObject(_initial_pos), angle(_angle), count(0)
		{
		}
		virtual void draw(void) const override final
		{
			gp::SetDrawBlendModeOf
			(
				gp::DrawRotaGraph(gp::level(14), pos().x(), pos().y(), 0.5, angle + half_pi<Real>(), shot_hit_img(), true),
				DX_BLENDMODE_ALPHA,
				200 - 20 * count
			);
		}
		virtual void update(void)override final
		{
			using namespace std;
			pos() += speed * Point2D(cos(angle), sin(angle));
			++count;
		}
		static void preperation(void)
		{
			shot_hit_img() = ImagePool::get("../../data/img/shot_hit.png");
		}
	};

	struct NormalShotImple
	{
		static int& ofuda_img(void) { static int img = 0; return img; }
		std::array<ShotHit, 3> shot_hits;
		int count_after_hit;
	};
}

MyGameProject::NormalShot::NormalShot(Real _angle,const Point2D& _p)
	:Shot(ShapeElement::Circle(25),5,_angle,_p),
	hit_flag(false),
	hit_moment(0),
	count(0),
	vars(std::make_unique<NormalShotImple>())
{
	vars->count_after_hit = 0;
}

void MyGameProject::NormalShot::draw(void) const
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
		for (const auto& shot_hit : vars->shot_hits)
		{
			shot_hit.draw();
		}
		//gp::DrawRotaGraph(pos().x(), pos().y(), 2.0, angle() + pi<Real>() / 2, ImagePool::get("../../data/img/shot_active.png"), TRUE); 
	}
}

void MyGameProject::NormalShot::hit(void)
{
	hit_flag = true;
	if (!hit_flag){ hit_moment = count; }
}

bool MyGameProject::NormalShot::is_active(void) const
{
	return !hit_flag;
}

void MyGameProject::NormalShot::custom_updater(void) 
{
	static constexpr MyGameProject::Real SHOT_VELOCITY = 25.0;

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
		if (vars->count_after_hit == 0)
		{
			auto theta0 = gp::safe_rand(0.f, two_pi<Real>());
			for (int i = 0; i != vars->shot_hits.size(); ++i)
			{
				vars->shot_hits[i] = ShotHit(pos(), theta0 + i * two_pi<Real>() / vars->shot_hits.size());
			}
		}
		for (auto& shot_hit : vars->shot_hits)
		{
			shot_hit.update();
		}
		++vars->count_after_hit;
	}
	++count;
}

MyGameProject::NormalShot::~NormalShot(void){}

void MyGameProject::NormalShot::preperation(void)
{
	//Load images.
	ImagePool::add("../../data/img/ofuda.png");
	NormalShotImple::ofuda_img() = ImagePool::get("../../data/img/ofuda.png");
	ImagePool::add("../../data/img/aura.png");
	ImagePool::add("../../data/img/shot_active.png");
	ImagePool::add("../../data/img/shot_hit.png");
	ShotHit::preperation();
}
