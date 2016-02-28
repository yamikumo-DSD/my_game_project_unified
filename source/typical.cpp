//typical.cpp

#include "typical.h"
#include "draw_order.h"
#include "dxlib_wrapper.h"
#include "color.h"
#include "environmental_constants.h"

void MyGameProject::Typical::custom_updater(void)
{
	if (pos().x() < -100 || pos().x() > WW<Real>() + 100 || pos().y() < -100 || pos().y() > WH<Real>() + 100)
	{
		set_flag_off();
	}
}

MyGameProject::Typical::Typical
	(
	const Enemy& _master,
	const Player& _player,
	const Point2D& _initial_pos,
	Real _initial_angle,
	Behavior _behavior
	)
	:Bullet(_master,_player,_initial_pos,_initial_angle,ShapeElement::Circle(static_cast<Real>(13.3)),_behavior)
{}

void MyGameProject::Typical::draw(void) const
{
	gp::DrawCircle(pos().x(),pos().y(),14,gp::blue.get());
}

MyGameProject::Typical::~Typical(void)
{}

void MyGameProject::Typical::preperation(void)
{}

