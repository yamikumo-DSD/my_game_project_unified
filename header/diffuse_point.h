//diffuse_point.h

#ifndef __DIFFUSE_POINT_H
#define __DIFFUSE_POINT_H

#include "ranged_uniform_random.h"
#include <exception>

namespace MyGameProject
{
	template < class PointType >
	PointType diffuse_point_rect(PointType&& _upper_left, PointType&& _lower_right)
	{
		using CoordType = decltype(_upper_left.x());
		if (_upper_left.x() > _lower_right.x() || _upper_left.y() > _lower_right.y()){ throw std::logic_error("First argument must be on the upper left corner, and second must be on the lower right corner."); }
		gp::RangedUniformRandom gen1(_upper_left.x(), _lower_right.x()), gen2(_upper_left.y(), _lower_right.y());
		return PointType(static_cast<CoordType>(gen1.rand()),static_cast<CoordType>(gen2.rand()));
	}
}

#endif