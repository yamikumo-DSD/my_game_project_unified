//item_and_score_dealer.cpp

#include "play_game.h"
#include "item.h"
#include "player.h"
#include <typeinfo>
#include "se_manager.h"
#include "shared_object.h"

namespace MyGameProject
{
	void PlayGame::item_and_score_dealer(void)
	{
		for (auto& item : SharedObject::items())
		{
			if (item)
			{
				if (is_hit(*item, *player))
				{
					if (typeid(*item) == typeid(ConvertedBullet))
					{
						se_manager->erect_play_flag_of("../../data/sound/click.mp3");
						player->charge_warp_point(12);
						score += 10;
					}
					else if (typeid(*item) == typeid(Point))
					{
						/*Do something*/
						se_manager->erect_play_flag_of("../../data/sound/item_get.wav");
						score += static_cast<int>(base_rate * 30);
					}
					item.reset();
				}
			}
		} //for (auto& item : SharedObject::items())
	} //PlayGame::item_and_score_dealer
} //namespace MyGameProject