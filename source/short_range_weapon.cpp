//short_range_weapon.cpp

#include "short_range_weapon.h"
#include <boost/math/constants/constants.hpp>
#include "image_pool.h"
#include "draw_order.h"
#include "dxlib_wrapper.h"
#include "player.h"
#include "drawblendmode.h"

namespace
{
	using namespace boost::math::constants;
}

MyGameProject::ShortRangeWeapon::ShortRangeWeapon(const Player& _player)
	:angle(0),
	MovingObject(_player.pos()),
	HitableObject(HitBox(ShapeElement::Circle(150), PhysicalState(0,_player.pos()))),
	player(_player),
	magnification(0),
	pow(3)
{}

void MyGameProject::ShortRangeWeapon::draw(void) const
{
	gp::SetDrawBlendModeOf
	(
		gp::DrawRotaGraph(pos().x(), pos().y(), magnification, angle, ImagePool::get("../../data/img/magical_circle.png"), TRUE),
		DX_BLENDMODE_ALPHA,
		100
	);
}

void MyGameProject::ShortRangeWeapon::update(void)
{
	angle += static_cast<Real>(0.02) * pi<Real>();
	if (magnification < 0.7)
	{
		magnification += static_cast<Real>(0.05);
	}
	pos() = player.pos();
	++count;
}

int MyGameProject::ShortRangeWeapon::power(void) const
{
	return pow;
}

MyGameProject::ShortRangeWeapon::~ShortRangeWeapon(void){}

void MyGameProject::ShortRangeWeapon::preperation(void)
{
	//Load images.
	ImagePool::add("../../data/img/magical_circle.png");
}
