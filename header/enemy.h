//enemy.h

#ifndef __ENEMY_H
#define __ENEMY_H

#include "object.h"
#include <functional>
#include "mathematics.h"
#include <string>
#include "smart_ptr.h"
#include <boost/optional.hpp>

namespace MyGameProject
{
	class Player;
	struct EnemyOrder;

	class Enemy : public MovingObject, public DynamicObject, public HitableObject
	{
	private:
		static int& id_(void) { static int id = 0; return id; }
		const int id;
	public:
		int get_id(void) const;
	public:
		enum class Class
		{
			Boss,
			LARGE,
			MIDDLE,
			SMALL
		};
	private:
		const Class enemy_class;
		const Player& player;
		int count;
		Real ang;
		virtual void custom_updater(void) = 0;
	protected:
		boost::optional<std::vector<gp::smart_ptr<Enemy>>&> enemies;
	public:
		Enemy
			(
			const Point2D& _initial_pos,
			Real _initial_angle,
			Class _enemy_class,
			const Player& _player,
			const Shape& _hitbox_shape
			);
		Enemy
			(
			const Point2D& _initial_pos,
			Real _initial_angle,
			Class _enemy_class,
			const Player& _player,
			const Shape& _hitbox_shape,
			std::vector<gp::smart_ptr<Enemy>>& _enemies
			);
		virtual void draw(void) const = 0;
		Real angle(void) const;
		void angle(Real _ang);
		Class get_class(void) const;
		virtual void hit(int _shot_power) = 0;
		virtual int get_health(void) const = 0;
		int get_count(void) const;
		virtual void update(void) override final;
		const Player& player_ref(void) const;
		virtual ~Enemy(void);
		static void preperation(void);
	};
}

#endif
