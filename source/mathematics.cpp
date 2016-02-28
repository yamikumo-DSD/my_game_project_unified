//mathematics.cpp

#include "mathematics.h"

const MyGameProject::Point2D MyGameProject::operator+(const Point2D& _lhs, const Point2D& _rhs)
{
	return Point2D(_lhs.x()+_rhs.x(),_lhs.y()+_rhs.y());
}

const MyGameProject::Point2D MyGameProject::operator-(const Point2D& _lhs, const Point2D& _rhs)
{
	return Point2D(_lhs.x() - _rhs.x(), _lhs.y() - _rhs.y());
}

MyGameProject::Point2D& MyGameProject::operator+=(Point2D& _lhs, const Point2D& _rhs)
{
	_lhs = _lhs + _rhs;
	return _lhs;
}

MyGameProject::Point2D& MyGameProject::operator-=(Point2D& _lhs, const Point2D& _rhs)
{
	_lhs = _lhs - _rhs;
	return _lhs;
}

const bool MyGameProject::operator==(const Point2D& _lhs, const Point2D& _rhs)
{
	return (_lhs.x() == _rhs.x()) && (_lhs.y() == _rhs.y());
}