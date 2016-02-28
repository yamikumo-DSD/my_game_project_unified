//item.cpp

#include "item.h"
#include "player.h"
#include <exception>
#include "draw_order.h"
#include "dxlib_wrapper.h"
#include <cmath>
#include "color.h"
#include "image_pool.h"

namespace
{
	using namespace std;
}

gp::smart_ptr<MyGameProject::Item> MyGameProject::Item::create
(
const std::string& _name,
const Player& _player,
const Point2D& _initial_pos
)
{
	if (_name == "ConvertedBullet"){ return gp::make_smart<ConvertedBullet>(_player, _initial_pos); }
	else{ throw std::runtime_error("Undefined type of enemy."); }
}

MyGameProject::Item::Item
(
const Player& _player,
const Point2D& _initial_pos
)
:player(_player),
MovingObject(_initial_pos),
HitableObject(HitBox(ShapeElement::Circle(25),PhysicalState(0,_initial_pos))),
count(0)
{}

int MyGameProject::Item::get_count(void) const
{
	return count;
}

const MyGameProject::Player& MyGameProject::Item::get_player_ref(void) const
{
	return player;
}

void MyGameProject::Item::update(void)
{
	custom_updater();
	area().get_physical_state().p = pos();
	++count;
}

MyGameProject::Item::~Item(void){}

void MyGameProject::Item::preperation(void)
{
	ConvertedBullet::preperation();
}

MyGameProject::ConvertedBullet::ConvertedBullet(const Player& _player, const Point2D& _initial_pos)
	:Item(_player,_initial_pos),
	angle(0)
{}

void MyGameProject::ConvertedBullet::custom_updater(void)
{
	static constexpr Real VELOCITY = 13;
	angle = angle_of(get_player_ref().pos() - pos());
	pos().x(pos().x() + VELOCITY * cos(angle));
	pos().y(pos().y() + VELOCITY * sin(angle));
	if (get_count() > 300){ set_flag_off(); }
}

void MyGameProject::ConvertedBullet::draw(void) const
{
	ScopedDrawBlendMode blend(DX_BLENDMODE_ALPHA,150);
	std::string img;
	switch (get_count() % 16)
	{
	case 0:
		img = "../../data/img/converted_bullet.png@0";break;
	case 1:
		img = "../../data/img/converted_bullet.png@1";break;
	case 2:
		img = "../../data/img/converted_bullet.png@2";break;
	case 3:
		img = "../../data/img/converted_bullet.png@3";break;
	case 4:
		img = "../../data/img/converted_bullet.png@4";break;
	case 5:
		img = "../../data/img/converted_bullet.png@5";break;
	case 6:
		img = "../../data/img/converted_bullet.png@6";break;
	case 7:
		img = "../../data/img/converted_bullet.png@7";break;
	case 8:
		img = "../../data/img/converted_bullet.png@8";break;
	case 9:
		img = "../../data/img/converted_bullet.png@7";break;
	case 10:
		img = "../../data/img/converted_bullet.png@6";break;
	case 11:
		img = "../../data/img/converted_bullet.png@5"; break;
	case 12:
		img = "../../data/img/converted_bullet.png@4"; break;
	case 13:
		img = "../../data/img/converted_bullet.png@3"; break;
	case 14:
		img = "../../data/img/converted_bullet.png@2"; break;
	case 15:
		img = "../../data/img/converted_bullet.png@1"; break;
	default:
		break;
	}

	if (get_count() < 200){ gp::DrawRotaGraph(pos().x(),pos().y(),1.0,0.0,ImagePool::get(img),true); }
	else
	{
		if (get_count() % 3 == 0){ gp::DrawRotaGraph(pos().x(), pos().y(), 1.0, 0.0, ImagePool::get(img), true); }
	}
}

void MyGameProject::ConvertedBullet::preperation(void)
{
	ImagePool::add_multi("../../data/img/converted_bullet.png",9,9,1,30,30);
}

MyGameProject::ConvertedBullet::~ConvertedBullet(void){}
