//convert_bullet.cpp

#include "play_game.h"
#include "bullet.h"
#include "item.h"
#include "find_vacant_object.h"

void MyGameProject::PlayGame::convert_bullets(void)
{
	for (auto& bullet : bullets)
	{
		if (bullet && !bullet->is_resistant())
		{
			*find_vacant_object(items) = Item::create("ConvertedBullet", *player, bullet->pos());
			bullet.reset();
		}
	}
}