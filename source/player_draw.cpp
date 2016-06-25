//player_draw.cpp

#include "player.h"
#include "onmyo_dama.h"
#include "draw_order.h"
#include "dxlib_wrapper.h"
#include "player_vars.h"
#include <boost/math/constants/constants.hpp>
#include "player_constants.h"
#include "color.h"
#include "locked_marker.h"
#include "debug.h"
#include "print_digit.h"

namespace MyGameProject
{

	void PlayerVars::draw_lock_gage(void) const noexcept
	{
		for (int i = 1; i <= number_of_locked_enemies; ++i)
		{
			gp::SetDrawBlendModeOf
			(
				gp::DrawRotaGraphF
				(
					gp::level(28),
					25, 30 + 15 * i, 
					0.35, 0.0,
					lock_gage_img()
					[
						i <= 10?
						LockGageColor::BLUE : ( i <= 19?  LockGageColor::GREEN : LockGageColor::YELLOW )
					], true
				),
				DX_BLENDMODE_ALPHA,
				100
			);
			gp::SetDrawBlendModeOf
			(
				gp::DrawRotaGraphF
				(
					gp::level(28),
					25, 30 + 15 * i, 
					0.35, 0.0,
					lock_gage_img()
					[
						i <= 10?
						LockGageColor::BLUE : ( i <= 19?  LockGageColor::GREEN : LockGageColor::YELLOW )
					], true
				),
				DX_BLENDMODE_ADD,
				static_cast<int>(255 * abs(sin(0.05 * i - count * 0.04)))
			);
		}
		for (int i = 1; i <= 25; ++i)
		{
			gp::DrawRotaGraphF
			(
				gp::level(28),
				25, 30 + 15 * i, 
				0.35, 0.0,
				lock_gage_frame_img(), true
			);
			if (!is_hyper_mode && i > 20)
			{
				gp::SetDrawBlendModeOf
				(
					gp::DrawRotaGraphF
					(
						gp::level(28),
						25, 30 + 15 * i, 
						0.35, 0.0,
						lock_gage_unavailable_img(), true
					),
					DX_BLENDMODE_ALPHA,
					150
				);
			}
		}
	}

	void Player::draw(void) const
	{
		using namespace boost::math::constants;
		static constexpr int T = 20;
		const auto p{pos()};
		const auto x{p.x()}, y{p.y()};
		auto draw_lock_range
		{
			[x, y, radius = this->vars->radius, lock_num = this->vars->number_of_locked_enemies](void)
			{
				if (radius > 10)
				{
					gp::DrawRotaGraphF
					(
						gp::level(15), x, y,
						0.9 * radius / PlayerVars::MAX_RADIUS, 0.0,
						PlayerVars::lock_range_img_level()
						[
							lock_num <= 10?
							0 : ( lock_num <= 19?  1 : 2 )
						], 
						true
					);
				}
			}
		}; //lambda, draw_lock_range
		auto draw_char
		{
			[x, y, count = this->count, is_hyper_mode = is_hyper_mode()](void)
			{
				gp::DrawRotaGraphF
				(
					gp::level(15), x, y, 0.9, 0.0,
					player_img()[(count / (T / player_img().size())) % player_img().size()], 
					TRUE
				);
				if (is_hyper_mode)
				{
					gp::SetDrawBlendModeOf
					(
						gp::DrawRotaGraphF
						(
							gp::level(15), x, y, 0.9, 0.0,
							player_img()[(count / (T / player_img().size())) % player_img().size()], 
							TRUE
						),
						DX_BLENDMODE_ADD,
						255 * pow(sin(count * 0.1), 2)
					);
				}
			}
		}; //lambda, draw_char

		vars->hyper_start_effect.draw();

		//draw locked marker
		for (const auto& marker : vars->locked_enemy_marker_list)
		{
			if (marker && marker->get_flag()) { marker->draw(); }
		}

		//draw options
		for (const auto& onmyo_dama : vars->onmyo_damas) { onmyo_dama.draw(); }

		if (locked_flag)
		{
			gp::SetDrawBlendModeOf(gp::DrawRotaGraphF(gp::level(15), x, y, lock_icon_rate, 0.0, ImagePool::get("../../data/img/lock.png"), TRUE), DX_BLENDMODE_ALPHA, lock_icon_brt);
		}

		if (current_health > 0)
		{
			gp::SetDrawBlendModeOf(gp::DrawRotaGraphF(gp::level(15), x, y, 0.385, 0.0, indicator_img()[current_health - 1], TRUE), DX_BLENDMODE_ALPHA, 100);
		}

		if (current_state == State::NORMAL || current_state == State::ACTION_SELECT)
		{
			draw_lock_range();
			draw_char();
		}
		else if (current_state == State::WARP)
		{
			draw_char();
			gp::SetDrawBlendModeOf(gp::DrawCircle(gp::level(15), static_cast<int>(warp_destination.x()), static_cast<int>(warp_destination.y()), static_cast<int>(30 * (static_cast<Real>(warp_count) / WARP_WAIT_TIME)), gp::sky.get()), DX_BLENDMODE_ALPHA, 150);
		}
		else if (current_state == State::WARP_MOTION)
		{
			int i{0};
			for (const auto& p : trail)
			{
				gp::SetDrawBlendModeOf
				(
					gp::DrawRotaGraphF(gp::level(15), x, y, 0.9, 0.0, player_img()[(count / (T / player_img().size())) % player_img().size()], TRUE),
					DX_BLENDMODE_ALPHA, 255 - 50 * i++
				);
			}
			draw_char();
		}
		else if (current_state == State::IMMORTAL)
		{
			draw_lock_range();
			if (count % 5 == 0) { draw_char(); }
		}
		for (int i = 0; i <= static_cast<int>(100 * (static_cast<float>(warp_charge) / MAX_WARP_CHARGE)); ++i)
		{
			gp::SetDrawBlendModeOf(gp::DrawRotaGraphF(gp::level(15), x, y, 0.5, i * two_pi<Real>() / 100, RING_IMG_HANDLER, true), DX_BLENDMODE_ALPHA, 150);
		}
		vars->draw_lock_gage();

		//print_digit(gp::level(20), pos().x(), pos().y(), 1.0, 255, boost::lexical_cast<char>(static_cast<int>(state())),
		//	ImagePool::get("../../data/img/dot.png"),
		//	ImagePool::get("../../data/img/digit_000.png"),
		//	ImagePool::get("../../data/img/digit_001.png"),
		//	ImagePool::get("../../data/img/digit_002.png"),
		//	ImagePool::get("../../data/img/digit_003.png"),
		//	ImagePool::get("../../data/img/digit_004.png"),
		//	ImagePool::get("../../data/img/digit_005.png"),
		//	ImagePool::get("../../data/img/digit_006.png"),
		//	ImagePool::get("../../data/img/digit_007.png"),
		//	ImagePool::get("../../data/img/digit_008.png"),
		//	ImagePool::get("../../data/img/digit_009.png")
		//);
	} //Player::draw(void) const
} //namespace MyGameProject