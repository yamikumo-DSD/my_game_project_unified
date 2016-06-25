//boss.cpp

#include "boss.h"
#include "player.h"
#include "concrete_enemy.h"

namespace MyGameProject
{
	gp::smart_ptr<Boss> Boss::create
		(
			const std::string& _boss_name,
			BulletPtrContainer& _bullets,
			ItemPtrContainer& _items,
			std::function<void(const std::string&)> _se_flag_setter,
			std::function<void(float)> _play_speed_changer,
			const Player& _player
			)
	{
		if (_boss_name == "BlackWings") { return gp::make_smart<BlackWings>(_bullets, _items, _se_flag_setter, _play_speed_changer, _player); }
		if (_boss_name == "MegaCrow") { return gp::make_smart<MegaCrow>(_bullets, _items, _se_flag_setter, _play_speed_changer, _player); }
		if (_boss_name == "Aya") { return gp::make_smart<Aya>(_bullets, _items, _se_flag_setter, _play_speed_changer, _player); }
		else { throw std::runtime_error("Undefined type of boss."); }
	}

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