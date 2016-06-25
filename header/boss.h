//boss.h

#ifndef __BOSS_H
#define __BOSS_H

#include "enemy.h"
#include <functional>
#include "back_ground_control.h"

namespace MyGameProject
{
	class Bullet;
	class Item;
	class Player;

	class Boss : public Enemy, public BackGroundControl::SignalSender
	{
	public:
		using BulletPtrContainer = std::vector <gp::smart_ptr<Bullet>>;
		using ItemPtrContainer   = std::vector <gp::smart_ptr<Item>>;

		static gp::smart_ptr<Boss> create
			(
			const std::string& _boss_name,
			BulletPtrContainer& _bullets,
			ItemPtrContainer& _items,
			std::function<void(const std::string&)> _se_flag_setter,
			std::function<void(float)> _play_speed_changer,
			const Player& _player
			);
	protected:
		std::function<void(const std::string&)> erect_se_flag_of;
	private:
		BulletPtrContainer& bullets;
		ItemPtrContainer& items;
		virtual void custom_updater(void) = 0;
		virtual void draw_charactor(void) const = 0;
		virtual void draw_health_bar(void) const = 0;
	public:
		Boss
			(
			BulletPtrContainer& _bullets,
			ItemPtrContainer& _items,
			std::function<void(const std::string&)> _se_flag_setter,
			Point2D _initial_pos,
			Real _initial_angle,
			const Player& _player,
			const Shape& _hitbox_shape
			);
		BulletPtrContainer& get_bullets_ref(void);
		ItemPtrContainer& get_items_ref(void);
		virtual void hit(int _shot_power) = 0;
		virtual void draw(void) const override final;
		virtual BackGroundControl::OptionalSignal notify(void) const noexcept;
		virtual ~Boss(void);
		static void preperation(void);
		virtual bool is_just_killed_now(void) const noexcept override;
		virtual void kill(void) noexcept override final;
	};
}

#endif