//print_digit.cpp

#include "print_digit.h"
#include <dxlib.h>
#include "debug.h"

namespace MyGameProject
{
	void print_digit
	(
		const gp::Level& _level, 
		int _x, int _y, double _size, int _pal,
		char _digit,
		int _period_img_handle,
		int _0_img_handle,
		int _1_img_handle,
		int _2_img_handle,
		int _3_img_handle,
		int _4_img_handle,
		int _5_img_handle,
		int _6_img_handle,
		int _7_img_handle,
		int _8_img_handle,
		int _9_img_handle
	)
	{
		switch (_digit)
		{
		case '.': gp::SetDrawBlendModeOf(gp::DrawRotaGraph(_level, _x, _y, _size, 0.0, _period_img_handle, true), DX_BLENDMODE_ALPHA, _pal); break;
		case '0': gp::SetDrawBlendModeOf(gp::DrawRotaGraph(_level, _x, _y, _size, 0.0, _0_img_handle     , true), DX_BLENDMODE_ALPHA, _pal); break;
		case '1': gp::SetDrawBlendModeOf(gp::DrawRotaGraph(_level, _x, _y, _size, 0.0, _1_img_handle     , true), DX_BLENDMODE_ALPHA, _pal); break;
		case '2': gp::SetDrawBlendModeOf(gp::DrawRotaGraph(_level, _x, _y, _size, 0.0, _2_img_handle     , true), DX_BLENDMODE_ALPHA, _pal); break;
		case '3': gp::SetDrawBlendModeOf(gp::DrawRotaGraph(_level, _x, _y, _size, 0.0, _3_img_handle     , true), DX_BLENDMODE_ALPHA, _pal); break;
		case '4': gp::SetDrawBlendModeOf(gp::DrawRotaGraph(_level, _x, _y, _size, 0.0, _4_img_handle     , true), DX_BLENDMODE_ALPHA, _pal); break;
		case '5': gp::SetDrawBlendModeOf(gp::DrawRotaGraph(_level, _x, _y, _size, 0.0, _5_img_handle     , true), DX_BLENDMODE_ALPHA, _pal); break;
		case '6': gp::SetDrawBlendModeOf(gp::DrawRotaGraph(_level, _x, _y, _size, 0.0, _6_img_handle     , true), DX_BLENDMODE_ALPHA, _pal); break;
		case '7': gp::SetDrawBlendModeOf(gp::DrawRotaGraph(_level, _x, _y, _size, 0.0, _7_img_handle     , true), DX_BLENDMODE_ALPHA, _pal); break;
		case '8': gp::SetDrawBlendModeOf(gp::DrawRotaGraph(_level, _x, _y, _size, 0.0, _8_img_handle     , true), DX_BLENDMODE_ALPHA, _pal); break;
		case '9': gp::SetDrawBlendModeOf(gp::DrawRotaGraph(_level, _x, _y, _size, 0.0, _9_img_handle     , true), DX_BLENDMODE_ALPHA, _pal); break;
		default: throw Unprintable("Unprintable");
		}
	}
} //namespace MyGameProject