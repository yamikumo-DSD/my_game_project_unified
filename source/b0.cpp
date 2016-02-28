//b0.cpp

#include "b0.h"
#include "draw_order.h"
#include "environmental_constants.h"
#include "image_pool.h"
#include <boost/math/constants/constants.hpp>

namespace MyGameProject
{
	using namespace boost::math::constants;

	struct B0Imple
	{
		static int& main_img(void) { static int i = 0; return i; }
	};

	void B0::custom_updater(void)
	{
		if (pos().x() < -100 || pos().x() > WW<Real>() + 100 || pos().y() < -100 || pos().y() > WH<Real>() + 100)
		{
			set_flag_off();
		}
	}

	B0::B0
	(
		const Enemy& _master,
		const Player& _player,
		const Point2D& _initial_pos,
		Real _initial_angle,
		Behavior _behavior
	)
		:Bullet(_master, _player, _initial_pos, _initial_angle, ShapeElement::Circle(static_cast<Real>(13.3)), _behavior),
		vars(std::make_unique<B0Imple>())
	{}

	void B0::draw(void) const
	{
		const auto main_img = B0Imple::main_img();
		const auto count = get_count();
		gp::DrawRotaGraphF(gp::level(12), pos().x(), pos().y(), 1.0, count * two_pi<Real>() / 10, main_img, true);
	}

	B0::~B0(void)
	{}

	void B0::preperation(void)
	{
		ImagePool::add("../../data/img/b0.png");
		B0Imple::main_img() = ImagePool::get("../../data/img/b0.png");
	}
}