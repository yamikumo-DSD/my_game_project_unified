//player_vars.h

#pragma once

#include "player.h"
#include "onmyo_dama.h"
#include "hyper_start_effect.h"
#include "decl_static_image_handler.h"
#include <fstream>

namespace MyGameProject
{
	class LockedMarker;

	struct PlayerVars
	{
		std::ofstream log_direction_of_options;
		Real radius;
		static constexpr Real MAX_RADIUS{200};
		static constexpr std::size_t MARKING_LIMIT = 30;
		std::array<std::weak_ptr<Enemy>, MARKING_LIMIT> marked_enemies;
		std::array<gp::smart_ptr<LockedMarker>, MARKING_LIMIT> locked_marker;
		unsigned int mark_index;
		bool is_short_range_weapon_released;
		int short_range_weapon_count;
		PlayerVars(const Player& _player) :radius(0.f),marked_enemies(), is_short_range_weapon_released(false),mark_index(0) ,short_range_weapon_count(0), hyper_start_effect(_player), log_direction_of_options("direction_of_options.txt"){}
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
		HyperStartEffect hyper_start_effect;
		STATIC_IMAGE_HANDLER_LIST(lock_range_img_level, 3)
		void draw_lock_gage(void) const noexcept;
		static void load_lock_gage_images(void) noexcept;
		int count{0};
		enum LockGageColor
		{
			BLUE = 0, GREEN, YELLOW,
		};

		STATIC_IMAGE_HANDLER_LIST(lock_gage_img, 3)
		STATIC_IMAGE_HANDLER(lock_gage_frame_img)
		STATIC_IMAGE_HANDLER(lock_gage_unavailable_img)
		bool is_hyper_mode{false};
	};

}

