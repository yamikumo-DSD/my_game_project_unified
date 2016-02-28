//back_ground1.cpp

#include "back_ground1.h"
#include "image_pool.h"
#include "draw_order.h"
#include "dxlib_wrapper.h"
#include "ranged_uniform_random.h"
#include "find_vacant_object.h"

namespace
{
	constexpr size_t MAX_CLOUD_NUM = 64;
}

MyGameProject::Cloud::Cloud(int _img_type,const Point2D& _initial_pos)
	:MovingObject(_initial_pos),
	img_type(_img_type)
{}

void MyGameProject::Cloud::update(void)
{
	pos().y(pos().y() + static_cast<Real>(9));
	if(pos().y() < -200 || pos().y() > 640 + 200){set_flag_off();}
}

void MyGameProject::Cloud::draw(void) const
{
	switch (img_type)
	{
	case 0:
		gp::DrawRotaGraph(pos().x(), pos().y(), 0.5, 0, ImagePool::get("../../data/img/cloud0.png"), TRUE);
		break;
	case 1:
		gp::DrawRotaGraph(pos().x(), pos().y(), 0.5, 0, ImagePool::get("../../data/img/cloud1.png"), TRUE);
		break;
	default:
		break;
	}
}

void MyGameProject::Cloud::preperation(void)
{
	ImagePool::add("../../data/img/cloud0.png");
	ImagePool::add("../../data/img/cloud1.png");
}

MyGameProject::Cloud::~Cloud(void){}

MyGameProject::BackGround1::BackGround1(void)
	:clouds(MAX_CLOUD_NUM)
{}

void MyGameProject::BackGround1::custom_updater(void)
{ 
	for(auto& cloud : clouds)
	{
		if(cloud)
		{
			if(cloud->get_flag()){ cloud->update();}
			else{cloud.reset();}
		}
	}

	if (get_count() % 2 == 0)
	{
		*find_vacant_object(clouds) = gp::make_smart<Cloud>(get_count() % 4 == 0,Point2D(static_cast<Real>(gp::rand(-150,640 + 150)),static_cast<Real>(-150)));
	}
}


void MyGameProject::BackGround1::draw_rear(void) const
{ 
	gp::DrawGraph(0,0,ImagePool::get("../../data/img/soramoyo.png"),FALSE);
	for(const auto& cloud : clouds)
	{
		if(cloud){cloud->draw();}
	}
}

void MyGameProject::BackGround1::preperation(void)
{
	Cloud::preperation();
	ImagePool::add("../../data/img/soramoyo.png");
}

MyGameProject::BackGround1::~BackGround1(void){}
