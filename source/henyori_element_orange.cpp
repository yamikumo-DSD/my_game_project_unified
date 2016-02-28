//henyori_element_orange.cpp

#include "henyori_element_orange.h"
#include "draw_order.h"
#include "dxlib_wrapper.h"
#include "image_pool.h"
#include <boost/math/constants/constants.hpp>
#include "environmental_constants.h"
#include "ranged_uniform_random.h"

namespace
{
	using namespace boost::math::constants;
}

void MyGameProject::HenyoriElementOrange::custom_updater(void)
{
	if (pos().x() < -100 || pos().x() > WW<Real>() + 100 || pos().y() < -100 || pos().y() > WH<Real>() + 100)
	{
		set_flag_off();
	}
}

MyGameProject::HenyoriElementOrange::HenyoriElementOrange
(
const Enemy& _master,
const Player& _player,
const Point2D& _initial_pos,
Real _initial_angle,
Behavior _behavior
)
:Bullet(_master, _player, _initial_pos, _initial_angle, ShapeElement::Circle(5), _behavior),
tex(ImagePool::get("../../data/img/henyori_element_orange.png"))
{}

void MyGameProject::HenyoriElementOrange::draw(void) const
{
	//ScopedDrawBlendMode blend(DX_BLENDMODE_ADD,255);
	gp::DrawRotaGraph(pos().x(), pos().y(), 1.0,pi<Real>() / 2 + angle_of(v()) , tex, TRUE);
}

MyGameProject::HenyoriElementOrange::~HenyoriElementOrange(void)
{}

void MyGameProject::HenyoriElementOrange::preperation(void)
{
	ImagePool::add("../../data/img/henyori_element_orange.png");
}

