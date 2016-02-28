//master_and_slaves.h

#pragma once

#include "mob_enemy.h"

namespace MyGameProject
{
	struct SlaveMemberVars;

	class Master : public MobEnemy, public std::enable_shared_from_this<Master>
	{
	private:
		class Slave : public MobEnemy
		{
		private:
			gp::smart_ptr<Master> master;
			gp::smart_ptr<SlaveMemberVars> vars;
			virtual void accessory_custom_updater(void) override final;
		public:
			Slave
			(
				BulletPtrContainer& _bullets,
				SEManager& _se_manager,
				const Player& _player,
				gp::smart_ptr<Master> _master,
				decltype(vars) _vars
			);
			virtual void draw(void) const override final;
			static void preperation(void);
			virtual ~Slave(void);
		};

	private:
		virtual void accessory_custom_updater(void) override final;

	public:
		Master
		(
			BulletPtrContainer& _bullets,
			SEManager& _se_manager,
			gp::smart_ptr<EnemyOrder> _order,
			const Player& _player,
			std::vector<gp::smart_ptr<Enemy>>& _enemies
		);
		virtual void draw(void) const override final;
		static void preperation(void);
		virtual ~Master(void);
	};
}