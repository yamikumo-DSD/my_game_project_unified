//mathematics.h

#ifndef __MATHEMATICS_H
#define __MATHEMATICS_H

#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/math/constants/constants.hpp>
#include <cmath>
#include <type_traits>

namespace MyGameProject
{
	using Real = float;
	namespace bg = boost::geometry;
	using Point2D = bg::model::d2::point_xy <Real>;

#ifndef MY_GAME_PROJECT_PI
#define MY_GAME_PROJECT_PI 3.14159265358979323846264338327950288419716939937510L
#endif
	template<typename T, std::enable_if_t<std::is_arithmetic<T>::value>* = nullptr>
	inline constexpr T pi(void)
	{
		return static_cast<T>(MY_GAME_PROJECT_PI);
	}
	template<typename T, std::enable_if_t<std::is_arithmetic<T>::value>* = nullptr>
	inline constexpr T half_pi(void)
	{
		return static_cast<T>(MY_GAME_PROJECT_PI / 2);
	}
	template<typename T, std::enable_if_t<std::is_arithmetic<T>::value>* = nullptr>
	inline constexpr T two_pi(void)
	{
		return static_cast<T>(2 * MY_GAME_PROJECT_PI);
	}

	template<typename RealType, std::enable_if_t<std::is_arithmetic<std::decay_t<RealType>>::value>* = nullptr>
	inline auto elem(RealType&& _angle) noexcept
	{
		return Point2D(static_cast<Real>(std::cos(std::forward<RealType>(_angle))), static_cast<Real>(std::sin(std::forward<RealType>(_angle))));
	}

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