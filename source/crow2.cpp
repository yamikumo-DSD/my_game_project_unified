//crow2.cpp

#include "crow2.h"
#include "draw_order.h"
#include "dxlib_wrapper.h"
#include "color.h"
#include "dxsstream.h"
#include "image_pool.h"

namespace
{
	constexpr int MAX_HP = 70;
}

MyGameProject::Crow2::Crow2
(
BulletPtrContainer& _bullets,
SEManager& _se_manager,
gp::smart_ptr<EnemyOrder> _order,
const Player& _player
)
	:MobEnemy(_bullets,_se_manager,_order,MAX_HP,Class::SMALL,_player,ShapeElement::Circle(20))
{}

void MyGameProject::Crow2::draw(void) const
{
	const auto vx = velocity().x(), vy = velocity().y();
	const auto direction = vy > 0 ? boost::math::constants::pi<Real>() : 0;
	if (vx > 0)
	{
		gp::DrawRotaGraph(pos().x(), pos().y(), 0.15, direction, ImagePool::get("../../data/img/crow.png@2"), TRUE);
	}
	else if (vx == 0)
	{ 
		gp::DrawRotaGraph(pos().x(), pos().y(), 0.15, direction, ImagePool::get("../../data/img/crow.png@1"), TRUE); 
	}
	else
	{
		gp::DrawRotaGraph(pos().x(), pos().y(), 0.15, direction, ImagePool::get("../../data/img/crow.png@0"), TRUE); 
	}
}

void MyGameProject::Crow2::accessory_custom_updater(void)
{
	const auto x = pos().x(), y = pos().y();
	if (x < -100 || x > 640 + 100 || y < -100 || y > 480 + 100)
	{
		set_flag_off();
	}
}

void MyGameProject::Crow2::preperation(void)
{
	ImagePool::add_multi("../../data/img/crow.png",3,3,1,400,400);
}

MyGameProject::Crow2::~Crow2(void){}