//lazer.cpp

#include "lazer.h"
#include "draw_order.h"
#include "enemy.h"
#include "mathematics.h"
#include "image_pool.h"
#include <cmath>
#include <boost/math/constants/constants.hpp>
#include <dxlib.h>
#include "debug_value.h"

namespace MyGameProject
{
	using namespace boost::math::constants;

	struct LazerImple
	{
		Real width;
		Real length;
		Real angle;
		Real duration;
		Real glow_speed;
		std::function<void(const Bullet&, Real&)> angle_changer;
		static int& tex(void) { static int img = 0; return img; }
		static int& tex0(void) { static int img = 0; return img; }
		static int& force_tex(void) { static int img = 0; return img; }
		static int& head_tex(void) { static int img = 0; return img; }
		int tex_scroll_count = 0;
		int tex0_scroll_count = 0;
		Lazer& body;
		LazerImple(Lazer& _body) :body(_body) {}
	};

	Lazer::Lazer
	(
		const Enemy& _master,
		const Player& _player,
		const Point2D& _initial_pos,
		Real _initial_angle,
		Behavior _behavior,
		Real _max_width,
		int _duration,
		Real _grow_speed
	)
		:Bullet(_master,_player, _initial_pos, _initial_angle, ShapeElement::Null(), _behavior),
		vars(std::make_unique<LazerImple>(*this))
	{
		vars->width = _max_width;
		vars->length = 0;
		vars->angle = _initial_angle;
		vars->duration = _duration;
		vars->glow_speed = _grow_speed;
	}

	Lazer::Lazer
	(
		const Enemy& _master,
		const Player& _player,
		const Point2D& _initial_pos,
		Real _initial_angle,
		Behavior _behavior,
		Real _max_width,
		int _duration,
		std::function<void(const Bullet&, Real&)> _f,
		Real _grow_speed
	)
		:Bullet(_master,_player, _initial_pos, _initial_angle, ShapeElement::Null(), _behavior),
		vars(std::make_unique<LazerImple>(*this))
	{
		vars->width = _max_width;
		vars->length = 0;
		vars->angle = _initial_angle;
		vars->duration = _duration;
		vars->angle_changer = _f;
		vars->glow_speed = _grow_speed;
	}

	void Lazer::custom_updater(void)
	{
		const auto count = get_count();
		auto& width  = vars->width;
		auto half_width = width / 2;
		auto& length = vars->length;
		auto& angle  = vars->angle;

		if (vars->angle_changer) { vars->angle_changer(*this, angle); }

		if (count == 0) {}
		else
		{
			if (length <= 1000) { length += vars->glow_speed; }
			area().get_physical_state().angle = half_pi<Real>() - angle;
			area().get_physical_state().p = pos();

			ShapeElement::Polygon rect;
			rect.outer().emplace_back(+half_width, 0);
			rect.outer().emplace_back(-half_width, 0);
			rect.outer().emplace_back(-half_width, length);
			rect.outer().emplace_back(+half_width, length);
			rect.outer().emplace_back(+half_width, 0);

			area().get_shape() = std::move(rect);

			if (count > vars->duration)
			{
				vars->width -= 4;
				if (vars->width <= 0) { set_flag_off(); }
			}

			vars->tex_scroll_count += 15;
			vars->tex0_scroll_count += 30;
		}
	}

	void Lazer::hit(void)
	{
	}

	void Lazer::draw(void) const
	{
		using namespace std;
		const auto tex = LazerImple::tex();
		const auto tex0 = LazerImple::tex0();
		const auto width = vars->width;
		const auto half_width = width / 2;
		const auto angle = vars->angle;
		const auto length = vars->length;
		const auto count = get_count();
		const auto head_pos = pos() + length * Point2D(cos(angle), sin(angle));

		const Point2D left_top     = pos()    + half_width * Point2D(cos(angle + half_pi<Real>()), sin(angle + half_pi<Real>()));
		const Point2D right_top    = pos()    + half_width * Point2D(cos(angle - half_pi<Real>()), sin(angle - half_pi<Real>()));
		const Point2D right_bottom = head_pos + half_width * Point2D(cos(angle - half_pi<Real>()), sin(angle - half_pi<Real>()));
		const Point2D left_bottom  = head_pos + half_width * Point2D(cos(angle + half_pi<Real>()), sin(angle + half_pi<Real>()));

		static constexpr int image_hight = 500;

		const auto gr = gp::Imple::DrawOrder::MakeScreen(100, length + 1, true);
		using namespace gp;
		gp::SetDrawScreenOf
		(
			gp::DrawGraph(0, (vars->tex_scroll_count  % image_hight) + image_hight, tex, true) >>
			gp::DrawGraph(0,  vars->tex_scroll_count  % image_hight,                tex , true) >>
			gp::DrawGraph(0, (vars->tex_scroll_count  % image_hight) - image_hight, tex , true) >>
			gp::DrawGraph(0, (vars->tex_scroll_count  % image_hight) - 2 * image_hight, tex , true) >>
			gp::DrawGraph(0, (vars->tex0_scroll_count % image_hight) + image_hight, tex0, true) >>
			gp::DrawGraph(0,  vars->tex0_scroll_count % image_hight,                tex0, true) >>
			gp::DrawGraph(0, (vars->tex0_scroll_count % image_hight) - image_hight, tex0, true) >>
			gp::DrawGraph(0, (vars->tex0_scroll_count % image_hight) - 2 * image_hight, tex0, true),
			gr
		);

		gp::DrawModiGraph
		(
			gp::level(12),
			left_top.x(), left_top.y(),
			right_top.x(), right_top.y(),
			right_bottom.x(), right_bottom.y(),
			left_bottom.x(), left_bottom.y(),
			gr,
			true
		);
		for (int i = 0; i != 2; ++i)
		{
			gp::SetDrawBlendModeOf
			(
				gp::DrawRotaGraph
				(
					gp::level(12),
					pos().x(), pos().y(),
					0.5, 0.2 * count * count, LazerImple::force_tex(), true
				),
				DX_BLENDMODE_ADD,
				255
			);
		}

		gp::DrawRotaGraph
		(
			gp::level(12),
			head_pos.x(), head_pos.y(),
			0.5, 
			angle - half_pi<Real>(),
			LazerImple::head_tex(), 
			true
		);
	}

	Lazer::~Lazer(void) {}

	void Lazer::preperation(void)
	{
		ImagePool::add("../../data/img/laser_test_0.png");
		LazerImple::tex() = ImagePool::get("../../data/img/laser_test_0.png");
		ImagePool::add("../../data/img/laser_test_1.png");
		LazerImple::tex0() = ImagePool::get("../../data/img/laser_test_1.png");
		ImagePool::add("../../data/img/laser_force.png");
		LazerImple::force_tex() = ImagePool::get("../../data/img/laser_force.png");
		ImagePool::add("../../data/img/laser_test_head.png");
		LazerImple::head_tex() = ImagePool::get("../../data/img/laser_test_head.png");
	}
}