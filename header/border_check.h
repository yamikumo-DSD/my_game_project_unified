#pragma once

#include <type_traits>
#include "environmental_constants.h"

namespace MyGameProject
{
	//for points on the x-y plane
	template<typename P, typename Real = double, std::enable_if_t<std::is_arithmetic<std::decay_t<Real>>::value>* = nullptr>
	inline auto is_within_window(P&& _pos, Real&& _border_margin = 0)
		->decltype(std::declval<P>().x(), std::declval<P>().y(), bool())
	{
		using CoordType = decltype(_pos.x());
		return _pos.x() >= -_border_margin &&
			   _pos.x() <= WW<CoordType>() + _border_margin&&
			   _pos.y() >= -_border_margin &&
			   _pos.y() <= WH<CoordType>() + _border_margin;
	}
}