//boss.cpp

#include "boss.h"
#include "player.h"
#include "concrete_enemy.h"

namespace MyGameProject
{
	Boss::Boss
		(
			BulletPtrContainer& _bullets,
			ItemPtrContainer& _items,
			std::function<void(const std::string&)> _se_flag_setter,
			Point2D _initial_pos,
			Real _initial_angle,
			const Player& _player,
			const Shape& _hitbox_shape
		)
		:bullets(_bullets),
		items(_items),
		erect_se_flag_of(_se_flag_setter),
		Enemy(_initial_pos, _initial_angle, Class::Boss, _player, _hitbox_shape)
	{}

	Boss::BulletPtrContainer& Boss::get_bullets_ref(void)
	{
		return bullets;
	}

	Boss::ItemPtrContainer& Boss::get_items_ref(void)
	{
		return items;
	}

	void Boss::draw(void) const
	{
		draw_health_bar();
		draw_charactor();
	}

	Boss::~Boss(void) {}

	void Boss::preperation(void)
	{
		BlackWings::preperation();
		MegaCrow::preperation();
		Aya::preperation();
		Brades::preperation();
	}

	bool Boss::is_just_killed_now(void) const noexcept { return false; }

	void Boss::kill(void) noexcept
	{
		state(State::KILLED_BUT_AVAILABLE);
	}

	BackGroundControl::OptionalSignal Boss::notify(void) const noexcept
	{
		return boost::none;
	}
}