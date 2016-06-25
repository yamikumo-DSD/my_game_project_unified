//bullet.h

#ifndef __ITEM_H
#define __ITEM_H

#include "object.h"
#include "smart_ptr.h"
#include <string>
#include <memory>

namespace MyGameProject
{
	class Player;

	class Item : public HitableObject, public MovingObject, public DynamicObject
	{
	public:
		static gp::smart_ptr<Item> create
			(
			const std::string& _name,
			const Player& _player,
			const Point2D& _initial_pos
			);
	private:
		const Player& player;
		int count;
		virtual void custom_updater(void) = 0;
	public:
		Item
			(
			const Player& _player,
			const Point2D& _initial
			);
		int get_count(void) const;
		const Player& get_player_ref(void) const;
		virtual void draw(void) const override = 0;
		virtual void update(void) override final;
		virtual ~Item(void);
		static void preperation(void);
	};

	class ConvertedBullet : public Item
	{
	private:
		Real angle;
	public:
		ConvertedBullet(const Player& _player,const Point2D& _initial_pos);
		virtual void draw(void) const override final;
		virtual void custom_updater(void) override final;
		virtual ~ConvertedBullet(void);
		static void preperation(void);
	};

	class Point : public Item
	{
	private:
		struct Impl;
		std::unique_ptr<Impl> pimpl;
	public:
		//Ctors
		Point(const Player& _player, const Point2D& _initial_pos, Real _initial_angle) noexcept;
		Point(const Point& _point) noexcept; 
		Point(Point&& _point) noexcept;
		//Assign operators
		Point& operator=(const Point& _point) noexcept; 
		Point& operator=(Point&& _point) noexcept;
		virtual void draw(void) const override final;
		virtual void custom_updater(void) override final;
		virtual ~Point(void);
		static void preperation(void);
	};
}

#endif
