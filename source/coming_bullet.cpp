//coming_bullet

#include "aya_imple.h"
#include "environmental_constants.h"
#include "draw_order.h"
#include "image_pool.h"
#include "mathematics.h"
#include "color.h"

namespace MyGameProject
{
	ComingBullet::ComingBullet
	(
		const Enemy& _master,
		const Player& _player,
		const Point2D& _initial_pos,
		Real _initial_angle,
		Behavior _behavior
	) noexcept
		:Bullet(_master, _player, _initial_pos, _initial_angle, ShapeElement::Null(), _behavior),
		z(500), camera_pos({WW_2<Real>(), WH_2<Real>(), -200})
	{}

	ComingBullet::~ComingBullet(void) noexcept = default;

	void ComingBullet::draw(void) const
	{
		using namespace std;
		const auto count{ get_count() };

		const auto x{ pos().x() }, y{ pos().y() };

		const auto k{camera_pos.z / (camera_pos.z - z)};

		const auto x_projected{k * x + (1 - k) * camera_pos.x};
		const auto y_projected{k * y + (1 - k) * camera_pos.y};
		//const auto z_projected{k * z + (1 - k) * camera_pos.z}; //z_projected is 0 anytime because of its definition.

		const auto scale{abs(camera_pos.z) / abs(camera_pos.z - z)};

		if (z > 0)
		{
			gp::DrawRotaGraphF(gp::level(11), x_projected, y_projected, scale, count % 8 > 4? 0 : pi<Real>(), img(), true);
		}
	}

	void ComingBullet::custom_updater(void)
	{
		z -= 10;
		if (z < 0)
		{
			set_flag_off();
		}
		if (z < 2) { area().get_shape() = ShapeElement::Circle(15); }
	}

	void ComingBullet::preparation(void) noexcept
	{
		ImagePool::add("../../data/img/smog.png");
		img() = ImagePool::get("../../data/img/smog.png");
	}

	RedEdges::RedEdges
	(
		const Enemy& _master,
		const Player& _player,
		const Point2D& _initial_pos,
		Real _initial_angle,
		Behavior _behavior
	) noexcept
		:Bullet(_master, _player, _initial_pos, _initial_angle, ShapeElement::Null(), _behavior),
		z(500), camera_pos({WW_2<Real>(), WH_2<Real>(), -200})
	{}

	RedEdges::~RedEdges(void) noexcept = default;

	void RedEdges::draw(void) const
	{
		using namespace std;
		const auto count{ get_count() };

		const auto x{ pos().x() }, y{ pos().y() };

		const auto k{camera_pos.z / (camera_pos.z - z)};

		const auto x_projected{k * x + (1 - k) * camera_pos.x};
		const auto y_projected{k * y + (1 - k) * camera_pos.y};
		//const auto z_projected{k * z + (1 - k) * camera_pos.z}; //z_projected is 0 anytime because of its definition.

		const auto scale{abs(camera_pos.z) / abs(camera_pos.z - z)};

		if (z > 0)
		{
			gp::DrawRotaGraphF(gp::level(12), x_projected, y_projected, scale, count * two_pi<Real>() / 16, img(), true);
		}
	}

	void RedEdges::custom_updater(void)
	{
		z -= 10;
		if (z < 0)
		{
			set_flag_off();
		}
		if (z < 2) { area().get_shape() = ShapeElement::Circle(15); }
	}

	void RedEdges::preparation(void) noexcept
	{
		ImagePool::add("../../data/img/red_edges.png");
		img() = ImagePool::get("../../data/img/red_edges.png");
	}

} //namespace MyGameProject