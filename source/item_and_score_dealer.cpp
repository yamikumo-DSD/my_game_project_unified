//item_and_score_dealer.cpp

#include "play_game.h"
#include "item.h"
#include "player.h"
#include <typeinfo>
#include "se_manager.h"

void MyGameProject::PlayGame::item_and_score_dealer(void)
{
	if (player)
	{
		for (auto& item : items)
		{
			if (item)
			{
				if (is_hit(*item, *player))
				{
					se_manager->erect_play_flag_of("../../data/sound/click.mp3");
					if (typeid(*item) == typeid(ConvertedBullet))
					{
						player->charge_warp_point(3);
					}
					item.reset();
				}
			}
		}
	}
}