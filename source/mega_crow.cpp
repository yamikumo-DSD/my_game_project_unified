//mega_crow.cpp

#include "mega_crow.h"
#include "draw_order.h"
#include "dxlib_wrapper.h"
#include "color.h"
#include "bullet.h"
#include "mathematics.h"
#include "mega_crow_imple.h"
#include "math_constants.h"
#include "image_pool.h"

MyGameProject::MegaCrow::MegaCrow
(
BulletPtrContainer& _bullets,
ItemPtrContainer& _items,
std::function<void(const std::string&)> _se_flag_setter,
std::function<void(float)> _play_speed_changer,
const Player& _player
)
:Boss(_bullets,_items, _se_flag_setter, Point2D(0, 0), 0, _player, ShapeElement::Circle(100)),
health(10000),
change_play_speed(_play_speed_changer),
immortal_flag(true),
imple(std::make_unique<MegaCrowImple>(*this))
{}

void MyGameProject::MegaCrow::hit(int _shot_power)
{
	if (!immortal_flag)
	{
		health -= _shot_power;
	}
}

void MyGameProject::MegaCrow::custom_updater(void)
{
	imple->action();
}

void MyGameProject::MegaCrow::draw_charactor(void) const
{
	imple->draw_charactor();
}

void MyGameProject::MegaCrow::draw_health_bar(void) const
{

}

MyGameProject::MegaCrow::~MegaCrow(void){}

void MyGameProject::MegaCrow::preperation(void)
{
	MegaCrowImple::preperation();
}

int MyGameProject::MegaCrow::get_health(void) const { return health; }