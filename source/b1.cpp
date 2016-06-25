//b1.cpp

#include "b1.h"
#include "draw_order.h"
#include "environmental_constants.h"
#include "image_pool.h"
#include <boost/math/constants/constants.hpp>

namespace MyGameProject
{
	using namespace boost::math::constants;

	struct B1Imple
	{
		static int& main_img(void) { static int i = 0; return i; }
	};

	void B1::custom_updater(void)
	{
		if (pos().x() < -100 || pos().x() > WW<Real>() + 100 || pos().y() < -100 || pos().y() > WH<Real>() + 100)
		{
			set_flag_off();
		}
	}

	B1::B1
	(
		const Enemy& _master,
		const Player& _player,
		const Point2D& _initial_pos,
		Real _initial_angle,
		Behavior _behavior
	)
		:Bullet(_master, _player, _initial_pos, _initial_angle, ShapeElement::Circle(static_cast<Real>(13.3)), _behavior),
		vars(std::make_unique<B1Imple>())
	{}

	void B1::draw(void) const
	{
		const auto main_img = B1Imple::main_img();
		const auto count = get_count();
		gp::DrawRotaGraphF(gp::level(12), pos().x(), pos().y(), 1.0, count * two_pi<Real>() / 10, main_img, true);
	}

	B1::~B1(void)
	{}

	void B1::preperation(void)
	{
		ImagePool::add("../../data/img/b1.png");
		B1Imple::main_img() = ImagePool::get("../../data/img/b1.png");
	}
}
