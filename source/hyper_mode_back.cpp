//hyper_mode_back.cpp

#include "hyper_mode_back.h"
#include "draw_order.h"
#include "dxlib_wrapper.h"
#include "image_pool.h"
#include <boost/math/constants/constants.hpp>
#include "environmental_constants.h"
#include "color.h"
#include "ranged_uniform_random.h"
#include "find_vacant_object.h"
#include <boost/make_unique.hpp>
#include "diffuse_point.h"
#include "player.h"
#include <cmath>

namespace
{
	using namespace boost::math::constants;
	using namespace std;
}

MyGameProject::Spirit::Spirit(const Point2D& _initial_pos) 
	:MovingObject(_initial_pos),
	mag(gp::safe_rand<Real>(0,2)),
	brt_theta(gp::safe_rand<Real>(-pi<Real>(),pi<Real>())),
	brt_A(static_cast<int>(gp::safe_rand<Real>(100, 255))),
	brt(0),
	speed(gp::safe_rand<Real>(2,8))
{}

void MyGameProject::Spirit::update(void)
{
	pos().y(pos().y() - speed);
	brt = brt_A * sin(brt_theta);
	brt_theta += 2 * pi<Real>() / 100;
	if (pos().x() < -100 || pos().x() > WW<Real>() + 100 || pos().y() < -100 || pos().y() > WH<Real>() + 100)
	{
		set_flag_off();
	}
}

void MyGameProject::Spirit::draw(void) const
{
	ScopedDrawBlendMode blend(DX_BLENDMODE_ADD,brt);
	gp::DrawRotaGraph(pos().x(), pos().y(), mag, 0, ImagePool::get("../../data/img/spirit.png"), true);
}

void MyGameProject::Spirit::preperation(void)
{
	ImagePool::add("../../data/img/spirit.png");
}

MyGameProject::Spirit::~Spirit(void){}

MyGameProject::HyperModeBack::HyperModeBack(const Player& _player):player(_player),rot(0),spirits(100),mahoujin_rot(0){}

void MyGameProject::HyperModeBack::custom_updater(void)
{
	rot += 2 * pi<Real>() / 1200;
	mahoujin_rot += 2 * pi<Real>() / 300;
	if (get_count() % 3 == 0)
	{
		*find_vacant_object(spirits) = boost::make_unique<Spirit>(diffuse_point_rect(Point2D(0,0),Point2D(WW<Real>(),WH<Real>())));
	}
	for (auto& spirit : spirits)
	{
		if (spirit)
		{
			if (spirit->get_flag()){ spirit->update(); }
			else{ spirit.reset(); }
		}
	}
}

void MyGameProject::HyperModeBack::draw_rear(void) const
{
	DrawGraph(0, 0, ImagePool::get("../../data/img/jinja.png"), false);

	{
		ScopedDrawBlendMode blend(DX_BLENDMODE_ADD, 200);
		gp::DrawRotaGraph(WW_2<Real>(), WH_2<Real>(), 0.3, rot, ImagePool::get("../../data/img/onmyodama_large.png"), true);
	}

	{
		ScopedDrawBlendMode blend(DX_BLENDMODE_ADD, 200);
		gp::DrawRotaGraph(player.pos().x(), player.pos().y(),0.7 * static_cast<Real>(player.rest_hyper_mode_time()) / Player::HYPER_MODE_TIME, mahoujin_rot, ImagePool::get("../../data/img/mahoujin.png"), true);
	}

	for (const auto& spirit : spirits)
	{
		if (spirit)
		{
			if (spirit->get_flag()){ spirit->draw(); }
		}
	}
}

void MyGameProject::HyperModeBack::preperation(void)
{
	ImagePool::add("../../data/img/onmyodama_large.png");
	ImagePool::add("../../data/img/jinja.png");
	ImagePool::add("../../data/img/mahoujin.png");
	Spirit::preperation();
}

MyGameProject::HyperModeBack::~HyperModeBack(void){}