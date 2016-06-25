//feather.cpp

#include "feather.h"
#include "draw_order.h"
#include "dxlib_wrapper.h"
#include "color.h"
#include "image_pool.h"
#include <boost/math/constants/constants.hpp>
#include "environmental_constants.h"
#include "ranged_uniform_random.h"

namespace MyGameProject
{
	using namespace boost::math::constants;

	void Feather::custom_updater(void)
	{
		if (get_count() > 200 && (pos().x() < -100 || pos().x() > WW<Real>() + 100 || pos().y() < -100 || pos().y() > WH<Real>() + 100))
		{
			set_flag_off();
		}
	}

	Feather::Feather
	(
	const Enemy& _master,
	const Player& _player,
	const Point2D& _initial_pos,
	Real _initial_angle,
	Behavior _behavior
	)
	:Bullet(_master, _player, _initial_pos, _initial_angle, ShapeElement::Circle(static_cast<Real>(8.4)), _behavior),
	rot(gp::safe_rand(-pi<Real>(),pi<Real>()))
	{}

	void Feather::draw(void) const
	{
		gp::DrawRotaGraph(gp::level(12), pos().x(), pos().y(), 0.7,rot + pi<Real>() / 2, ImagePool::get("../../data/img/feather.png"), TRUE);
	}

	Feather::~Feather(void)
	{}

	void Feather::preperation(void)
	{
		ImagePool::add("../../data/img/feather.png");
	}

	void Feather::hit(void)
	{}
}


