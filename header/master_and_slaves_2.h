//master_and_slaves_2.h

#pragma once

#include "mob_enemy.h"
#include "decl_static_image_handler.h"

namespace MyGameProject
{
	struct Slave2MemberVars;

	class Master2 : public MobEnemy, public std::enable_shared_from_this<Master2>
	{
	private:
		class Slave2 : public MobEnemy
		{
		private:
			gp::smart_ptr<Master2> master;
			gp::smart_ptr<Slave2MemberVars> vars;
			virtual void accessory_custom_updater(void) override final;
		public:
			Slave2
			(
				BulletPtrContainer& _bullets,
				SEManager& _se_manager,
				const Player& _player,
				gp::smart_ptr<Master2> _master,
				decltype(vars) _vars,
				Real _max_distance = 120.f
			);
			virtual void draw(void) const override final;
			static void preperation(void);
			virtual ~Slave2(void);
		};

	private:
		virtual void accessory_custom_updater(void) override final;

	public:
		Master2
		(
			BulletPtrContainer& _bullets,
			SEManager& _se_manager,
			gp::smart_ptr<EnemyOrder> _order,
			const Player& _player,
			std::vector<gp::smart_ptr<Enemy>>& _enemies
		);
		virtual void draw(void) const override final;
		STATIC_IMAGE_HANDLER_LIST(img, 4)
		static void preperation(void);
		virtual ~Master2(void);
	};

	class GreenMaster : public MobEnemy
	{
	private:
		struct Impl;
		std::unique_ptr<Impl> pimpl;
		virtual void accessory_custom_updater(void) override final;
	public:
		GreenMaster
		(
			BulletPtrContainer& _bullets,
			SEManager& _se_manager,
			gp::smart_ptr<EnemyOrder> _order,
			const Player& _player
		);
		virtual void draw(void) const override final;
		static void preperation(void);
		virtual ~GreenMaster(void);
	};

}
