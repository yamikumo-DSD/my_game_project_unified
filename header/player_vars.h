//player_vars.h

#pragma once

#include "player.h"
#include "onmyo_dama.h"

namespace MyGameProject
{
	class LockedMarker;

	struct PlayerVars
	{
		Real radius;
		static constexpr std::size_t MARKING_LIMIT = 30;
		std::array<gp::smart_ptr<Enemy>, MARKING_LIMIT> marked_enemies;
		std::array<gp::smart_ptr<LockedMarker>, MARKING_LIMIT> locked_marker;
		unsigned int mark_index;
		bool is_short_range_weapon_released;
		int short_range_weapon_count;
		PlayerVars(void) :radius(0.f),marked_enemies(), is_short_range_weapon_released(false),mark_index(0) ,short_range_weapon_count(0){}
		static int& ring_img(void) { static int img = 0; return img; }
#ifndef RING_IMG_HANDLER
#define RING_IMG_HANDLER PlayerVars::ring_img()
#endif
		struct LockInformation
		{
			int index_to_enemy_list;
			int m; //multiplicity
			LockInformation(void) :index_to_enemy_list(0), m(0) {}
		};
		int number_of_locked_enemies;
		std::vector<LockInformation> locked_enemy_list;
		std::vector<gp::smart_ptr<LockedMarker>> locked_enemy_marker_list;
		std::array<OnmyoDama, 8> onmyo_damas;
	};

}

