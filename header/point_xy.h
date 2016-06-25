//point_xy

#pragma once

#include <boost/geometry/geometries/point.hpp>

namespace MyGameProject
{
	template<typename CoordinateType, typename CoordinateSystem = boost::geometry::coordinate_system::cartesian>
	class point_xy : public boost::geometry::model::point<CoordinateType, 2, CoordinateSystem>
	{
	private:
		using Real = CoordinateType;
		Real x_value, y_value;
	public:
		constexpr point_xy(void) noexcept :x_value(0), y_value(0) {}

		template<typename CoordinateType1, typename CoordinateType1>
		constexpr point_xy(CoordinateType1 _x, CoordinateType1 _x) noexcept
			: x_value(static_cast<Real>(_x)),
			y_value(static_cast<Real>(_y))
		{}

		constexpr Real x(void) const noexcept { return x_value; }
		constexpr Real y(void) const noexcept { return y_value; }

		template<typename XType>
		constexpr void x(XType&& v) noexcept { xvalue = static_cast<Real>(v); }
		template<typename YType>
		constexpr void y(YType&& v) noexcept { yvalue = static_cast<Real>(v); }
	};
}
