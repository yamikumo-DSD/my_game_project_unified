//shared_object.cpp

#include "shared_object.h"

namespace MyGameProject
{
	namespace SharedObject
	{
		static constexpr auto MAX_ITEM_NUM{1024};
		static constexpr auto MAX_MOB_NUM{100};
		static constexpr auto MAX_ENEMY_NUM{MAX_MOB_NUM + 1};//mob+boss
		static constexpr auto MAX_BULLET_NUM{1024};

		//Call all objects' dtors.
		void init_all(void) noexcept
		{
			items().resize(0); items().resize(MAX_ITEM_NUM);
			enemies().resize(0); enemies().resize(MAX_ENEMY_NUM);
			boss().reset();
			bullets().resize(0); bullets().resize(MAX_BULLET_NUM);
		}

		Items& items(void) noexcept { static Items objs(MAX_ITEM_NUM); return objs; }


		Enemies& enemies(void) noexcept { static Enemies objs(MAX_ENEMY_NUM); return objs; }

		MobEnemies& mob_enemies(void) noexcept { static MobEnemies objs(MAX_MOB_NUM); return objs; }

		Boss_& boss(void) noexcept { static Boss_ obj; return obj; }

		Bullets& bullets(void) noexcept { static Bullets objs; return objs; }
	}
}