//bullet.h

#ifndef __BULLET_H
#define __BULLET_H

#include "object.h"
#include <functional>
#include "smart_ptr.h"
#include <string>

namespace MyGameProject
{
	class Player;
	class Enemy;

	class Bullet : public HitableObject, public MovingObject,public DynamicObject
	{
	public:
		using Behavior = std::function<void(Bullet&)>;
		static gp::smart_ptr<Bullet> create
			(
			const std::string& _name,
			const Enemy& _master,
			const Player& _player,
			const Point2D& _initial_pos,
			Real _initial_angle,
			Behavior _behavior
			);
	private:
		const Player& player;
		const Enemy& master;
		Real ang;
		int count;
		Behavior behavior;
		Point2D pre_pos;
		virtual void custom_updater(void) = 0;
	public:
		Bullet
			(
			const Enemy& _master,
			const Player& _player,
			const Point2D& _initial_pos,
			Real _initial_angle,
			const Shape& _hitbox_shape,
			Behavior _behavior
			);
		Real angle(void) const;
		void angle(Real _ang);
		Point2D v(void) const;
		int get_count(void) const;
		const Enemy& get_master_ref(void) const;
		const Player& get_player_ref(void) const;
		virtual void draw(void) const override = 0;
		virtual void update(void) override final;
		virtual bool is_resistant(void) const;
		virtual void hit(void);
		virtual ~Bullet(void);
		static void preperation(void);
	};
}

#endif
