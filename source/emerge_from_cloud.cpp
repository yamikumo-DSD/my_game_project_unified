//emerge_from_cloud.cpp

#define _AFX_SECURE_NO_WARNINGS
#define D_SCL_SECURE_NO_WARNINGS

#include "emerge_from_cloud.h"
#include "draw_order.h"
#include "dxlib_wrapper.h"
#include "image_pool.h"
#include "find_vacant_object.h"
#include "diffuse_point.h"

namespace
{
	constexpr int MAX_HP = 70;
}

MyGameProject::EmergeFromCloud::EmergeFromCloud
(
BulletPtrContainer& _bullets,
SEManager& _se_manager,
gp::smart_ptr<EnemyOrder> _order,
const Player& _player
)
:MobEnemy(_bullets, _se_manager, _order, MAX_HP, Class::SMALL, _player, ShapeElement::Null())
{}

namespace
{
	constexpr int TURNING_POINT = 180;
}

void MyGameProject::EmergeFromCloud::draw(void) const
{
	if (get_count() > TURNING_POINT)
	{
		const auto vx = velocity().x();
		if (vx > 0){ gp::DrawRotaGraph(gp::level(11), pos().x(), pos().y(), 0.15, angle(), ImagePool::get("../../data/img/crow.png@2"), TRUE); }
		else if (vx == 0){ gp::DrawRotaGraph(gp::level(11),pos().x(), pos().y(), 0.15, angle(), ImagePool::get("../../data/img/crow.png@1"), TRUE); }
		else{ gp::DrawRotaGraph(gp::level(11),pos().x(), pos().y(), 0.15, angle(), ImagePool::get("../../data/img/crow.png@0"), TRUE); }

	}

	for (const auto& following : followings)
	{
		if (following && following->get_flag())
		{
			following->draw();
		}
	}
}

void MyGameProject::EmergeFromCloud::accessory_custom_updater(void)
{
	const auto x = pos().x(), y = pos().y();
	if (x < -100 || x > 640 + 100 || y < -100 || y > 480 + 100){set_flag_off();}

	if (get_count() <= TURNING_POINT)
	{
		for (int i = 0; i != 2; ++i)
		{
			*find_vacant_object(followings) = gp::make_smart<Tail>(diffuse_point_rect(pos() - Point2D(30, 30), pos() + Point2D(30, 30)), 10);
		}
	}

	if (get_count() == TURNING_POINT)
	{
		area().get_shape() = ShapeElement::Circle(20);
	}

	for (auto& following : followings)
	{
		if (following)
		{
			if (following->get_flag()){following->update();}
			else{following.reset();}
		}
	}

}

void MyGameProject::EmergeFromCloud::preperation(void)
{
	ImagePool::add_multi("../../data/img/crow.png", 3, 3, 1, 400, 400);
	Tail::preperation();
}

MyGameProject::EmergeFromCloud::~EmergeFromCloud(void){}

MyGameProject::EmergeFromCloud::Tail::Tail(const Point2D& _init_pos, int _tc)
	:count(0),
	termination_count(_tc)
{
	pos() = _init_pos;
}

void MyGameProject::EmergeFromCloud::Tail::update(void)
{
	if (count == 0){ img = ImagePool::get("../../data/img/particle.png"); }
	if (count == termination_count){set_flag_off();}
	pos().y(pos().y() + 5);
	++count;
}

void MyGameProject::EmergeFromCloud::Tail::draw(void) const
{
	gp::DrawRotaGraph(gp::level(9), pos().x(),pos().y(),2 - 2 * count / termination_count,0,img,true);
}

void MyGameProject::EmergeFromCloud::Tail::preperation(void)
{
	ImagePool::add("../../data/img/particle.png");
}

MyGameProject::EmergeFromCloud::Tail::~Tail(void){}