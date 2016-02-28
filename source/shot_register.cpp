//shot_register.cpp

#include "play_game.h"
#include "player.h"
#include "short_range_weapon.h"
#include "se_manager.h"
#include "find_vacant_object.h"
#include "option.h"
#include "shot.h"
#include "mob_enemy.h"
#include "find_nearest_object.h"
#include <functional>
#include "ranged_uniform_random.h"
#include <boost/math/constants/constants.hpp>

namespace
{
	using namespace boost::math::constants;
}

void MyGameProject::PlayGame::shot_register(void)
{
	if (player)
	{
		if (player->shot_if())
		{
			Real dir = 0;
			if (options[0]){ dir = options[0]->dir(); } 

			switch (player->weapon())
			{
			case Player::WeaponType::HYPER_LONG_RANGE:
				if (count % 8 == 0)
				{
					for (const auto& option : options)
					{
						if (option){ *find_vacant_object(shots) = gp::make_smart<HyperShot2>(dir, option->pos()); }
					}
				}
				if (count % 6 == 0 && se_manager){ se_manager->erect_play_flag_of("../../data/sound/shot.mp3"); }
				if (short_range_weapon){ short_range_weapon.reset(); }
				break;
			case Player::WeaponType::LONG_RANGE:
				//if (count % 8 == 0)
				//{
				//	for (const auto& option : options)
				//	{
				//		if (option){ *find_vacant_object(shots) = gp::make_smart<NormalShot>(dir, option->pos()); }
				//	}
				//}
				//if (count % 6 == 0 && se_manager){ se_manager->erect_play_flag_of("../../data/sound/shot.mp3"); }
				if (short_range_weapon){ short_range_weapon.reset(); }
				break;
			case Player::WeaponType::SHORT_RANGE:
//				if (count % 5 == 0)
//				{
//#define LAUNCH_LAZER_MACRO(angle) *find_vacant_object(shots) = gp::make_smart<HomingLazer>(std::bind(find_nearest_object<HomingLazer, std::vector<gp::smart_ptr<Enemy>>>, std::placeholders::_1, std::ref(enemies)),(angle), player->pos(), 0);
//					switch (count % 25)
//					{
//					case 0:
//						LAUNCH_LAZER_MACRO(dir + 1 * pi<Real>() / 6); LAUNCH_LAZER_MACRO(dir - 1 * pi<Real>() / 6);
//						break;
//					case 5:
//						LAUNCH_LAZER_MACRO(dir + 2 * pi<Real>() / 6); LAUNCH_LAZER_MACRO(dir - 2 * pi<Real>() / 6);
//						break;
//					case 10:
//						LAUNCH_LAZER_MACRO(dir + 3 * pi<Real>() / 6); LAUNCH_LAZER_MACRO(dir - 3 * pi<Real>() / 6);
//						break;
//					case 15:
//						LAUNCH_LAZER_MACRO(dir + 4 * pi<Real>() / 6); LAUNCH_LAZER_MACRO(dir - 4 * pi<Real>() / 6);
//						break;
//					case 20:
//						LAUNCH_LAZER_MACRO(dir + 5 * pi<Real>() / 6); LAUNCH_LAZER_MACRO(dir - 5 * pi<Real>() / 6);
//						break;
//					}
//				}
//				if (count % 30 == 0 && se_manager){ se_manager->erect_play_flag_of("../../data/sound/ata_a27.mp3"); }
				break;
			case Player::WeaponType::HYPER_SHORT_RANGE:
				//if (count % 30 == 0)
				//{
				//	for (int i = -2; i != 3; ++i)
				//	{
				//		*find_vacant_object(shots) =
				//			gp::make_smart<HomingShot>(std::bind(find_nearest_object<HomingShot, std::vector<gp::smart_ptr<Enemy>>>, std::placeholders::_1, std::ref(enemies)), dir + pi<Real>() + i * pi<Real>() / 8, player->pos(), i);
				//	}
				//}
				//if (count % 30 == 0 && se_manager){ se_manager->erect_play_flag_of("../../data/sound/ata_a27.mp3"); }
				break;
			default:
				break;
			}
		}
		else{ if (short_range_weapon){ short_range_weapon.reset(); } }
	}
}