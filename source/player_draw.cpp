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
#include "dxsstream.h"

void MyGameProject::Player::draw(void) const
{
	using namespace boost::math::constants;
	static constexpr int T = 20;

	//draw locked marker
	for (const auto& marker : vars->locked_enemy_marker_list)
	{
		if (marker && marker->get_flag()) { marker->draw(); }
	}

	//draw options
	for (const auto& onmyo_dama : vars->onmyo_damas)
	{
		onmyo_dama.draw();
	}

#ifndef DRAW_PLAYER_IMPLE
#define DRAW_PLAYER_IMPLE gp::DrawRotaGraph(gp::level(15), pos().x(), pos().y(), 0.9, 0.0, player_img()[(count / (T / player_img().size())) % player_img().size()], TRUE)
#endif
	if (locked_flag)
	{
		gp::SetDrawBlendModeOf(gp::DrawRotaGraph(gp::level(15), pos().x(), pos().y(), lock_icon_rate, 0.0, ImagePool::get("../../data/img/lock.png"), TRUE), DX_BLENDMODE_ALPHA, lock_icon_brt);
	}

	if (current_health > 0)
	{
		gp::SetDrawBlendModeOf(gp::DrawRotaGraph(gp::level(15), pos().x(), pos().y(), 0.385, 0.0, indicator_img()[current_health - 1], TRUE), DX_BLENDMODE_ALPHA, 100);
	}

	if (current_state == State::NORMAL || current_state == State::ACTION_SELECT)
	{
		gp::SetDrawBlendModeOf(gp::DrawCircle(gp::level(15), pos().x(), pos().y(), vars->radius, gp::blue.get()), DX_BLENDMODE_ALPHA, 100);
		DRAW_PLAYER_IMPLE;
	}
	else if (current_state == State::WARP)
	{
		DRAW_PLAYER_IMPLE;
		gp::SetDrawBlendModeOf(gp::DrawCircle(gp::level(15), warp_destination.x(),warp_destination.y(),30 * (static_cast<Real>(warp_count) / WARP_WAIT_TIME),gp::sky.get()), DX_BLENDMODE_ALPHA, 150);
	}
	else if (current_state == State::WARP_MOTION)
	{
		int i = 0;
		for (const auto& p : trail)
		{
			gp::SetDrawBlendModeOf(DRAW_PLAYER_IMPLE, DX_BLENDMODE_ALPHA, 100 - 15 * i++);
		}
		DRAW_PLAYER_IMPLE;
	}
	else if (current_state == State::IMMORTAL)
	{
		gp::SetDrawBlendModeOf(gp::DrawCircle(gp::level(15), pos().x(), pos().y(), vars->radius, gp::blue.get()), DX_BLENDMODE_ALPHA, 100);
		if (count % 5 == 0)
		{
			DRAW_PLAYER_IMPLE;
		}
	}
	for (int i = 0; i <= static_cast<int>(100 * (static_cast<float>(warp_charge) / MAX_WARP_CHARGE)); ++i)
	{
		gp::SetDrawBlendModeOf(gp::DrawRotaGraph(gp::level(15), pos().x(), pos().y(), 0.5, i * two_pi<Real>() / 100, RING_IMG_HANDLER, true), DX_BLENDMODE_ALPHA, 150);
	}
}