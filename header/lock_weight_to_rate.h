//lock_weight_to_rate.h

#pragma once

namespace MyGameProject
{
	inline float lock_weight_to_rate(unsigned int _lock_weight) noexcept
	{
		return 1 + _lock_weight / 10.f;
	}
} //MyGameProject
