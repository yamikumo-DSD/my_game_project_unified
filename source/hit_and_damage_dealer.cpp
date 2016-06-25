//hit_and_damage_dealer.cpp

#include "play_game.h"
#include "short_range_weapon.h"
#include "mob_enemy.h"
#include "shot.h"
#include "bullet.h"
#include <boost/geometry.hpp>
#include "player.h"
#include "find_vacant_object.h"
#include "item.h"
#include "shared_object.h"
#include "destroy_enemy_rate.h"
#include "debug.h"

namespace MyGameProject
{
	using namespace boost::geometry;
	void PlayGame::hit_and_damage_dealer(void)
	{
		//Player v.s. bullets of enemies
		if (player)
		{
			for (auto& bullet : SharedObject::bullets())
			{
				if (bullet)
				{
					if (is_hit(*player, *bullet))
					{
						bullet->hit();
						if (player->hit())
						{
							base_rate = base_rate >= 300 ? base_rate - 300 : 0;
						}
					}
				}
			}
		}

		//Shot of player v.s. enemies
		for (auto& shot : shots)
		{
			if (shot)
			{
				//for (auto& enemy : enemies)
				for (auto& enemy : SharedObject::enemies())
				{
					if (enemy)
					{
						if (shot->is_active() && is_hit(*shot, *enemy))
						{
							shot->hit();
							enemy->hit(shot->power());
							base_rate += 0.2f;
							score += 10;
						}
					}
					if (enemy && enemy->get_flag() && enemy->get_health() <= 0 && enemy->state() == Enemy::State::ALIVE)
					{
						enemy->kill();
						if (shot->rate() > 1.01f)
						{
							*find_vacant_object(lock_rate_indicator) =
								std::make_shared<DestroyEnemyRate>(enemy->pos().x(), enemy->pos().y(), shot->rate());
						}
						base_rate += (3.f * shot->rate());
						score += static_cast<decltype(score)>(500.f * base_rate * shot->rate());
					}
				}
			}
		}


		if (player->state() == Player::State::WARP_MOTION)
		{
			for (auto& bullet : SharedObject::bullets())
			{
				if (bullet)
				{
					if (boost::geometry::distance(player->pos(), bullet->pos()) < 200 && !bullet->is_resistant())
					{
						*find_vacant_object(SharedObject::items()) = Item::create("ConvertedBullet", *player, bullet->pos());
						bullet.reset();
					}
				}
			}
		}
	} //PlayGame::hit_and_damage_dealer
} //namespace MyGameProject