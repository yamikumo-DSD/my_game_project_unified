//resitant_bullet.cpp

#include "resistant_bullet.h"
#include "draw_order.h"
#include "add_get.h"
#include "border_check.h"

namespace MyGameProject
{
	inline int c2i(ResistantBullet::Color _color) { return static_cast<int>(_color); }

	struct ResistantBullet::Impl
	{
		//Image handles.
		static std::array<int, COLOR_VARIATION> img;

		Color color;
	};

	std::array<int, ResistantBullet::COLOR_VARIATION> ResistantBullet::Impl::img;

	void ResistantBullet::custom_updater(void)
	{
		const auto count{get_count()};
		if (count > 300 && is_within_window(pos(), 100)) { set_flag_off(); }
	}

	ResistantBullet::ResistantBullet
	(
		const Enemy& _master,
		const Player& _player,
		const Point2D& _initial_pos,
		Real _initial_angle,
		Behavior _behavior,
		Color _color
	)
		:Bullet(_master, _player, _initial_pos, _initial_angle, ShapeElement::Circle(static_cast<Real>(15)), _behavior),
		pimpl(std::make_unique<Impl>())
	{
		pimpl->color = _color;
	}

	void ResistantBullet::draw(void) const
	{
		gp::DrawRotaGraphF(gp::level(12), pos().x(), pos().y(), 0.5, 0, Impl::img[c2i(pimpl->color)], true);
	}

	ResistantBullet::~ResistantBullet(void) {}

	void ResistantBullet::preperation(void)
	{
		Impl::img[c2i(Color::RED )] = add_get("../../data/img/resistance/red.png");
		Impl::img[c2i(Color::BLUE)] = add_get("../../data/img/resistance/blue.png");
		Impl::img[c2i(Color::DARK)] = add_get("../../data/img/resistance/dark.png");
	}
}
