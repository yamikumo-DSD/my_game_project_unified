//option.cpp

#if 0

#include "option.h"
#include "player.h"
#include "draw_order.h"
#include "dxlib_wrapper.h"
#include "image_pool.h"
#include <boost/math/constants/constants.hpp>
#include <cmath>

namespace
{
	using namespace boost::math::constants;
	using namespace std;
	static const auto omega = pi<MyGameProject::Real>() / 50;
	static const auto cpi = pi<MyGameProject::Real>();
}

MyGameProject::Option::Option(const Player& _player, const Point2D& _rational_p)
	:player(_player), rational_p(_rational_p), count(0), angle(0), direction(0)
{
	pos() = player.pos() + rational_p;	
}

void MyGameProject::Option::draw(void) const
{
	gp::DrawRotaGraph(pos().x(),pos().y(),0.7,angle,ImagePool::get("../../data/img/option.png"),TRUE);
}

void MyGameProject::Option::update(void) 
{
	const auto theta = angle_of(player.v());
	const auto px = player.pos().x(), py = player.pos().y();
	const auto rx = rational_p.x(), ry = rational_p.y();

	if (!player.is_locked())
	{
		if (norm(player.v()) > 0)
		{
			if (abs(direction - theta) >= omega)
			{
				if (cos(theta - direction) > 0)
				{
					direction -= omega;
				}
				if (cos(theta - direction) < 0)
				{
					direction += omega;
				}
			}
			else
			{
				direction = theta;
			}
		}
	}
	
	pos().x(px + rx * cos(direction) - ry * sin(direction));
	pos().y(py + rx * sin(direction) + ry * cos(direction));

	angle = count * static_cast<Real>(0.05) * cpi;
	++count;
}

MyGameProject::Real MyGameProject::Option::dir(void) const
{
	return direction;
}

void MyGameProject::Option::preperation(void)
{
	ImagePool::add("../../data/img/option.png");
}

MyGameProject::Option::~Option(void){}

#endif
