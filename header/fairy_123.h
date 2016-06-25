//fairy123.h

#pragma once

#include "mob_enemy.h"

namespace MyGameProject
{
#ifndef DECL_FAIRY_CLASS
#define DECL_FAIRY_CLASS(number)\
	class Fairy_##number : public MobEnemy\
	{\
	private:\
		struct Impl; std::unique_ptr<Impl> pimpl;\
		virtual void accessory_custom_updater(void) override final;\
		virtual int released_p_item_num(void) const override final;\
	public:\
		Fairy_##number\
		(\
			BulletPtrContainer& _bullets,\
			SEManager& _se_manager,\
			gp::smart_ptr<EnemyOrder> _order,\
			const Player& _player\
		) noexcept;\
		Fairy_##number(const Fairy_##number& _fairy) noexcept;\
		Fairy_##number(Fairy_##number&& _fairy) noexcept;\
		virtual ~Fairy_##number(void) noexcept;\
\
		virtual void draw(void) const override final;\
		static void preparation(void);\
	};

	DECL_FAIRY_CLASS(1) //class Fairy_1;
	DECL_FAIRY_CLASS(2) //class Fairy_2;
	DECL_FAIRY_CLASS(3) //class Fairy_3;
#undef DECL_FAIRY_CLASS
#endif
}
