//small_battle_ship.cpp

#include "small_battle_ship.h"
#include "header_set_for_mob_enemy.h"
#include "debug.h"
#include "add_get.h"
#include "color.h"
#include "border_check.h"

namespace MyGameProject
{
	static constexpr int INITIAL_HEALTH{100};
	using P = Point2D;

	namespace ImageHandle
	{
		static std::array<int, 3> flag;
		static int sasabune{0};
	}

	struct SmallBattleShip::Impl 
	{
	};

	SmallBattleShip::SmallBattleShip
	(
		BulletPtrContainer& _bullets,
		SEManager& _se_manager,
		gp::smart_ptr<EnemyOrder> _order,
		const Player& _player,
		std::vector<gp::smart_ptr<Enemy>>& _enemies
	) noexcept
		:MobEnemy(_bullets, _se_manager, _order, INITIAL_HEALTH, Class::MIDDLE, _player, ShapeElement::Null(), _enemies),
		pimpl(std::make_unique<Impl>())
	{
	}

	SmallBattleShip::SmallBattleShip(const SmallBattleShip& _small_battle_ship) noexcept
		:MobEnemy(_small_battle_ship),
		pimpl(std::make_unique<Impl>(*_small_battle_ship.pimpl))
	{
	}

	SmallBattleShip::SmallBattleShip(SmallBattleShip&& _small_battle_ship) noexcept = default;

	SmallBattleShip::~SmallBattleShip(void) noexcept = default;
	
	void SmallBattleShip::accessory_custom_updater(void) 
	{
		auto&& count{get_count()};
	}

	void SmallBattleShip::draw(void) const
	{
		auto&& count{get_count()};
		if (count > 0)
		{
			gp::DrawRotaGraphF(pos().x(), pos().y(), 1.0, 0.0, ImageHandle::sasabune, true);
			auto index{0};
			auto i{count % 20};
			if (i < 5) { index = 0; }
			else if (i >= 5 && i < 10) { index = 1; }
			else if (i >= 10 && i < 15) { index = 2; }
			else { index = 1; }

			gp::DrawRotaGraphF ( pos().x() + 9, pos().y() - 105, 2.0, 0.0, ImageHandle::flag[index], true );
		}
	}

	void SmallBattleShip::preparation(void)
	{
		ImageHandle::flag[0]  = add_get("../../data/img/sasabune/flag0.png"   );
		ImageHandle::flag[1]  = add_get("../../data/img/sasabune/flag1.png"   );
		ImageHandle::flag[2]  = add_get("../../data/img/sasabune/flag2.png"   );
		ImageHandle::sasabune = add_get("../../data/img/sasabune/sasabune.png");
	}
}
