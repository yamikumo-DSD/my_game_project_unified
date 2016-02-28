//wind_smasher.cpp

#include "wind_smasher.h"
#include "draw_order.h"
#include "dxlib_wrapper.h"
#include "image_pool.h"
#include <boost/math/constants/constants.hpp>
#include "environmental_constants.h"

namespace
{
	using namespace boost::math::constants;
}

void MyGameProject::WindSmasher::custom_updater(void)
{
	if (pos().x() < -100 || pos().x() > WW<Real>() + 100 || pos().y() < -100 || pos().y() > WH<Real>() + 100)
	{
		set_flag_off();
	}
}

MyGameProject::WindSmasher::WindSmasher
(
const Enemy& _master,
const Player& _player,
const Point2D& _initial_pos,
Real _initial_angle,
Behavior _behavior
)
:Bullet(_master, _player, _initial_pos, _initial_angle, ShapeElement::Circle(static_cast<Real>(4.9)), _behavior),
tex(ImagePool::get("../../data/img/wind_smasher.png"))
{}

void MyGameProject::WindSmasher::draw(void) const
{
	{
		if (get_count() % 2 == 0)
		{
			gp::SetDrawBlendModeOf
			(
				gp::DrawRotaGraph(pos().x() - 2 * v().x(), pos().y() - 2 * v().y(), 0.7, angle_of(v()) - pi<Real>() / 2, ImagePool::get("../../data/img/wind_smasher.png"), TRUE),
				DX_BLENDMODE_ALPHA, 100
			);
		}
	}
	gp::DrawRotaGraph(pos().x(), pos().y(),0.7, angle_of(v()) - pi<Real>() / 2,tex, TRUE);
}

MyGameProject::WindSmasher::~WindSmasher(void)
{}

void MyGameProject::WindSmasher::preperation(void)
{
	ImagePool::add("../../data/img/wind_smasher.png");
}

