//cutter.cpp

#include "cutter.h"
#include "draw_order.h"
#include "dxlib_wrapper.h"
#include "color.h"
#include "image_pool.h"
#include <boost/math/constants/constants.hpp>
#include "environmental_constants.h"

namespace
{
	using namespace boost::math::constants;
}

void MyGameProject::Cutter::custom_updater(void)
{
	if (pos().x() < -100 || pos().x() > WW<Real>() + 100 || pos().y() < -100 || pos().y() > WH<Real>() + 100)
	{
		set_flag_off();
	}
}

MyGameProject::Cutter::Cutter
(
const Enemy& _master,
const Player& _player,
const Point2D& _initial_pos,
Real _initial_angle,
Behavior _behavior
)
:Bullet(_master, _player, _initial_pos, _initial_angle, ShapeElement::Circle(7), _behavior)
{}

void MyGameProject::Cutter::draw(void) const
{
	gp::DrawRotaGraph(pos().x(), pos().y(), 0.07, angle() - pi<Real>() / 2, ImagePool::get("../../data/img/cutter.png"), TRUE);
}

MyGameProject::Cutter::~Cutter(void)
{}

void MyGameProject::Cutter::preperation(void)
{
	ImagePool::add("../../data/img/cutter.png");
}

