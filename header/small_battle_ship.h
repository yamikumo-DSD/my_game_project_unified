//small_battle_ship.h

#pragma once

#include "mob_enemy.h"

namespace MyGameProject
{
	class SmallBattleShip : public MobEnemy
	{
	private:
		struct Impl; std::unique_ptr<Impl> pimpl;
		virtual void accessory_custom_updater(void) override final;
	public:
		SmallBattleShip
		(
			BulletPtrContainer& _bullets,
			SEManager& _se_manager,
			gp::smart_ptr<EnemyOrder> _order,
			const Player& _player,
			std::vector<gp::smart_ptr<Enemy>>& _enemies
		) noexcept;
		SmallBattleShip(const SmallBattleShip& _small_battle_ship) noexcept;
		SmallBattleShip(SmallBattleShip&& _small_battle_ship) noexcept;
		virtual ~SmallBattleShip(void) noexcept;

		virtual void draw(void) const override final;
		static void preparation(void);
	};
}
