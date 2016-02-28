//camera_frame.cpp

#include "camera_frame.h"

#include "draw_order.h"
#include "dxlib_wrapper.h"
#include "image_pool.h"
#include <boost/math/constants/constants.hpp>
#include "boss.h"
#include "environmental_constants.h"
#include "color.h"

namespace
{
	using namespace boost::math::constants;
}

void MyGameProject::CameraFrame::custom_updater(void)
{
	if (get_count() < ACTIVE_TIME){ mag -= static_cast<Real>(0.006); }
	if (get_count() == ACTIVE_TIME){ area().get_shape() = rect(); }
	if (get_count() == ACTIVE_TIME + 8){ set_flag_off(); }
}

MyGameProject::CameraFrame::CameraFrame
(
const Enemy& _master,
const Player& _player,
const Point2D& _initial_pos,
Real _initial_angle,
Behavior _behavior
)
:Bullet(_master, _player, _initial_pos, _initial_angle, ShapeElement::Null(), _behavior),
mag(1)
{}

bool MyGameProject::CameraFrame::is_resistant(void) const { return true; }

void MyGameProject::CameraFrame::draw(void) const
{
	if (get_count() < ACTIVE_TIME){ gp::DrawRotaGraph(pos().x(), pos().y(), mag, pi<Real>() / 2 + angle_of(pos() - get_master_ref().pos()), ImagePool::get("../../data/img/camera_frame.png"), TRUE); }
	else{ gp::DrawRotaGraph(pos().x(), pos().y(), mag, pi<Real>() / 2 + angle_of(pos() - get_master_ref().pos()), ImagePool::get("../../data/img/camera_frame_active.png"), TRUE); }
	gp::DrawLine(get_master_ref().pos().x(),get_master_ref().pos().y(),pos().x(),pos().y(),gp::white.get());
}

MyGameProject::CameraFrame::~CameraFrame(void)
{}

void MyGameProject::CameraFrame::preperation(void)
{
	rect().outer().push_back(Point2D(-20,-15));
	rect().outer().push_back(Point2D(-20,15));
	rect().outer().push_back(Point2D(20, 15));
	rect().outer().push_back(Point2D(20, -15));
	rect().outer().push_back(Point2D(-20, -15));
	ImagePool::add("../../data/img/camera_frame.png");
	ImagePool::add("../../data/img/camera_frame_active.png");
}

