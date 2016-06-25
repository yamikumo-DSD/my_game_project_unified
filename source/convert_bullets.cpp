//convert_bullet.cpp

#include "play_game.h"
#include "bullet.h"
#include "item.h"
#include "find_vacant_object.h"
#include "shared_object.h"
#include "border_check.h"

namespace MyGameProject
{
	void PlayGame::convert_bullets(void)
	{
		for (auto& bullet : SharedObject::bullets())
		{
			if (bullet && !bullet->is_resistant() && is_within_window(bullet->pos(), 10))
			{
				*find_vacant_object(SharedObject::items()) = Item::create("ConvertedBullet", *player, bullet->pos());
				bullet.reset();
			}
		}
	}
}