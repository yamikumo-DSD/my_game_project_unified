//player_update_mode.cpp

#include "player.h"
#include "player_vars.h"
#include "device.h"
#include "player_constants.h"

namespace MyGameProject
{
	void Player::update_mode(void)
	{
		using namespace boost::math::constants;
		vars->is_short_range_weapon_released = device.is_just_released("X") ? true : false;
		if (current_state != State::WARP)
		{
			if (device.get_time("Z") > 0) { shot_flag = true; }
			else { shot_flag = false; }
			if (device.get_time("Shift") == 1)
			{
				locked_flag = !locked_flag;
				lock_icon_brt = 255; lock_icon_rate = 0;
				erect_se_flag_of("../../data/sound/lock_option.wav");
			}
		}
		if (device.get_time("C") == 1/* && warp_charge >= UNIT_WARP_COST && current_state != State::WARP && current_state != State::WARP_MOTION && current_state != State::ACTION_SELECT*/)
		{
			if (is_hyper_mode())
			{
				if (warp_charge >= UNIT_WARP_COST && current_state != State::WARP && current_state != State::WARP_MOTION && current_state != State::ACTION_SELECT)
				{
					shot_flag = false;
					warp_count = WARP_WAIT_TIME;
					warp_charge -= UNIT_WARP_COST;
					current_state = State::WARP;
				}
			}
			else
			{
				if (warp_charge >= UNIT_WARP_COST && warp_charge < UNIT_HYPER_COST)
				{
					if (current_state != State::WARP && current_state != State::WARP_MOTION && current_state != State::ACTION_SELECT)
					{
						shot_flag = false;
						warp_count = WARP_WAIT_TIME;
						warp_charge -= UNIT_WARP_COST;
						current_state = State::WARP;
					}
				}
				else if (warp_charge >= UNIT_HYPER_COST)
				{
					if (current_state != State::WARP && current_state != State::WARP_MOTION && current_state != State::ACTION_SELECT)
					{
						current_state = State::ACTION_SELECT;
						discharge_button_pressed_count = count;
					}
				}
			}
			//current_state = State::ACTION_SELECT;
			//discharge_button_pressed_count = count;
		}
		if (current_state == State::ACTION_SELECT && count == discharge_button_pressed_count + ACTION_SELECT_WAIT_PERIOD)
		{
			if (device.get_time("C") >= ACTION_SELECT_WAIT_PERIOD)
			{
				if (warp_charge >= UNIT_WARP_COST)
				{
					shot_flag = false;
					warp_count = WARP_WAIT_TIME;
					warp_charge -= UNIT_WARP_COST;
					current_state = State::WARP;
				}
			}
			else
			{
				if (warp_charge >= UNIT_HYPER_COST)
				{
					if (!hyper_mode)
					{
						hyper_mode = true;
						warp_charge -= UNIT_HYPER_COST;
						hyper_mode_count = HYPER_MODE_TIME;
						vars->hyper_start_effect.start_playing();
					}
					current_state = State::NORMAL;
				}
			}
		} //if (current_state == State::ACTION_SELECT && count == discharge_button_pressed_count + ACTION_SELECT_WAIT_PERIOD)
		if (current_state == State::WARP && (device.is_just_released("C") || warp_count < 0 || warp_charge <= 0))
		{
			const auto r = static_cast<Real>(boost::geometry::distance(pos(), warp_destination));
			warp_angle = (warp_destination == pos() ? -pi<Real>() / 2 : angle_of(warp_destination - pos()));
			pos() = pos() + Point2D((r - WARP_GAP) * static_cast<Real>(cos(warp_angle)), (r - WARP_GAP) * static_cast<Real>(sin(warp_angle)));
			v_warp = Point2D((2 * WARP_GAP / WARP_MOTION_TIME) * cos(warp_angle), (2 * WARP_GAP / WARP_MOTION_TIME) * sin(warp_angle));
			current_state = State::WARP_MOTION;
			erect_se_flag_of("../../data/sound/magic_wave3.mp3");
		}
	} //Player::update_mode
} //namespace MyGameProject
