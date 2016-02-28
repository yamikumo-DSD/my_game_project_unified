//emerge_from_cloud.h

#pragma once

#include "mob_enemy.h"

namespace MyGameProject
{
	class EmergeFromCloud : public MobEnemy
	{
	private:
		class Tail : public DynamicObject, public MovingObject
		{
		private:
			int count;
			const int termination_count;
			int img;
		public:
			Tail(const Point2D& _initial_pos, int _termination_count);
			virtual void update(void) override final;
			virtual void draw(void) const override final;
			static void preperation(void);
			virtual ~Tail(void);
		};

	private:
		virtual void accessory_custom_updater(void) override final;
		std::array<gp::smart_ptr<Tail>, 35> followings;

	public:
		EmergeFromCloud
			(
			BulletPtrContainer& _bullets,
			SEManager& _se_manager,
			gp::smart_ptr<EnemyOrder> _order,
			const Player& _player
			);
		virtual void draw(void) const override final;
		static void preperation(void);
		virtual ~EmergeFromCloud(void);
	};
}
