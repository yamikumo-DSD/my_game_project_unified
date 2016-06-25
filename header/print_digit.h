//print_digit.h

#pragma once

#include "draw_order.h"
#include <exception>

namespace MyGameProject
{
	class Unprintable : public std::logic_error
	{
	public:
		using logic_error::logic_error;
	};

	//3rd argument must be '0', '1', '2', '3', '4', '5', '6', '6', '7', '8', '9', or '.'.
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
	);
}
