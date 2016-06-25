//black_wings.cpp

#include "black_wings.h"
#include "draw_order.h"
#include "dxlib_wrapper.h"
#include "color.h"
#include "environmental_constants.h"
#include "bullet.h"
#include "mathematics.h"
#include <boost/make_unique.hpp>
#include "black_wings_imple.h"
#include "shared_object.h"

namespace
{
	using namespace boost::math::constants;
}

MyGameProject::BlackWings::BlackWings
(
BulletPtrContainer& _bullets,
ItemPtrContainer& _items,
std::function<void(const std::string&)> _se_flag_setter,
std::function<void(float)> _play_speed_changer,
const Player& _player
)
:Boss(_bullets,_items, _se_flag_setter, Point2D(WW<Real>() / 2, WH<Real>() + 30), 0, _player, ShapeElement::Circle(15)),
health(0),
change_play_speed(_play_speed_changer),
immortal_flag(true),
imple(boost::make_unique<BlackWingsImple>(*this))
{}

void MyGameProject::BlackWings::hit(int _shot_power)
{
	if (!immortal_flag)
	{
		health -= _shot_power;
	}
}

int MyGameProject::BlackWings::get_health(void) const { return health; }

void MyGameProject::BlackWings::custom_updater(void)
{
	imple->action();
}

void MyGameProject::BlackWings::draw_charactor(void) const
{
	imple->draw_charactor();
}

void MyGameProject::BlackWings::draw_health_bar(void) const
{

}

MyGameProject::BlackWings::~BlackWings(void){}

void MyGameProject::BlackWings::preperation(void)
{
}