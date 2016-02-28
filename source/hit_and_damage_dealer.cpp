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

namespace
{
	using namespace boost::geometry;
}

void MyGameProject::PlayGame::hit_and_damage_dealer(void)
{
	//Player v.s. bullets of enemies
	if (player)
	{
		for (auto& bullet : bullets)
		{
			if (bullet)
			{
				if (is_hit(*player,*bullet))
				{
					bullet->hit();
					player->hit();
				}
			}
		}
	}

	//Shot of player v.s. enemies
	for (auto& shot : shots)
	{
		if (shot)
		{
			for (auto& enemy : enemies)
			{
				if (enemy)
				{
					if (shot->is_active() && is_hit(*shot, *enemy))
					{
						shot->hit();
						enemy->hit(shot->power());
					}
				}
			}
		}
	}

	//Short range weapon of player v.s. enemies
	if (short_range_weapon)
	{
		for (auto& enemy : enemies)
		{
			if (enemy)
			{
				if (is_hit(*short_range_weapon, *enemy))
				{
					enemy->hit(short_range_weapon->power());
				}
			}
		}
	}

	if (player->state() == Player::State::WARP_MOTION)
	{
		for (auto& bullet : bullets)
		{
			if (bullet)
			{
				if (boost::geometry::distance(player->pos(), bullet->pos()) < 200 && !bullet->is_resistant())
				{
					*find_vacant_object(items) = Item::create("ConvertedBullet",*player, bullet->pos());
					bullet.reset();
				}
			}
		}
	}
}