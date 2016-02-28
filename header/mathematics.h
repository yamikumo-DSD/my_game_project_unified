//mathematics.h

#ifndef __MATHEMATICS_H
#define __MATHEMATICS_H

#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/math/constants/constants.hpp>
#include <cmath>

namespace MyGameProject
{
	using Real = float;
	namespace bg = boost::geometry;
	using Point2D = bg::model::d2::point_xy <Real>;

	template<class VectorType>
	Real angle_of(const VectorType& _v)
	{
		Real theta = static_cast<Real>(std::atan2(_v.y(), _v.x()));
		if (theta < 0){ theta = 2 * boost::math::constants::pi<Real>() - abs(theta); }
		return theta;
	}

	template<class VectorType>
	Real norm(const VectorType& _v)
	{
		return sqrt(_v.x() * _v.x() + _v.y() * _v.y());
	}

	const Point2D operator+(const Point2D& _lhs, const Point2D& _rhs);
	template<class RealType>
	const Point2D operator*(const RealType& _k, const Point2D& _p)
	{
		return Point2D(static_cast<Real>(_k) * _p.x(), static_cast<Real>(_k) * _p.y());
	}
	template<class RealType>
	const Point2D operator*(const Point2D& _p, const RealType& _k)
	{
		return _k * _p;
	}
	const Point2D operator-(const Point2D& _lhs, const Point2D& _rhs);
	Point2D& operator+=(Point2D& _lhs, const Point2D& _rhs);
	Point2D& operator-=(Point2D& _lhs, const Point2D& _rhs);
	const bool operator==(const Point2D& _lhs, const Point2D& _rhs);
}

#endif