//bullet_behavior.cpp

#include "bullet_behavior.h"
#include <cmath>
#include "player.h"
#include <exception>
#include <boost/geometry.hpp>
#include <boost/math/constants/constants.hpp>
#include <boost/lexical_cast.hpp>
#include "draw_order.h"
#include "dxlib_wrapper.h"
#include "color.h"

namespace{ using namespace std; using namespace boost::math::constants; using namespace boost::geometry; }

MyGameProject::Bullet::Behavior MyGameProject::straight_course(Real _speed)
{
	return[=](Bullet& _b)->void
	{
		_b.pos().x(_b.pos().x() + _speed * cos(_b.angle()));
		_b.pos().y(_b.pos().y() + _speed * sin(_b.angle()));
	};
}

MyGameProject::Bullet::Behavior MyGameProject::accurate_homing(const MovingObject& _target, Real _speed, int _keep_time)
{
	return[=,&_target](Bullet& _b)->void
	{
		if (boost::geometry::distance(_target.pos(), _b.pos()) > _speed)
		{
			if (_b.get_count() < _keep_time)
			{
				_b.angle(angle_of(_target.pos() - _b.pos()));
			}

			_b.pos().x(_b.pos().x() + _speed * cos(_b.angle()));
			_b.pos().y(_b.pos().y() + _speed * sin(_b.angle()));
		}
	};
}

MyGameProject::Bullet::Behavior MyGameProject::parabola(Real _V0,Real _G)
{
	return[=](Bullet& _b)->void
	{
		const auto t = _b.get_count();
		_b.pos().x(_b.pos().x() + _V0 * cos(_b.angle()));
		_b.pos().y(_b.pos().y() + _V0 * sin(_b.angle()) + _G * t);
	};
}

MyGameProject::Bullet::Behavior MyGameProject::acceleration(Real _V0, Real _a,Real _v_limit)
{
	return[=](Bullet& _b)->void
	{
		const auto t = _b.get_count();
		Real v = 0;
		if (_a <= 0){ v = (_V0 + t * _a) > _v_limit ? (_V0 + t * _a) : _v_limit; }
		else{ v = (_V0 + t * _a) < _v_limit ? (_V0 + t * _a) : _v_limit; }
		_b.pos().x(_b.pos().x() + v * cos(_b.angle()));
		_b.pos().y(_b.pos().y() + v * sin(_b.angle()));
	};
}

MyGameProject::Bullet::Behavior MyGameProject::log_spiral(Real _A, Real _B,Real _theta)
{
	if (_B > 0)
	{
		return[=](Bullet& _b)->void
		{
			const auto s = _b.get_count() - log(_A) / _B;
			const auto t = (1 / _B) * log(1 + s * (_B / sqrt(_A * (pow(_B, 2) + 1))));
			const auto r = _A * exp(_B * t);
			_b.pos().x(_b.pos().x() + r * (_B * cos(t + _theta) - sin(t + _theta)));
			_b.pos().y(_b.pos().y() + r * (_B * sin(t + _theta) + cos(t + _theta)));
		};
	}
	else if (_B < 0)
	{
		return[=](Bullet& _b)->void
		{
			const auto s = _b.get_count() - log(_A) / (-_B);
			const auto t = (1 / (-_B)) * log(1 + s * ((-_B) / sqrt(_A * (pow(_B, 2) + 1))));
			const auto r = _A * exp(-_B * t);
			_b.pos().x(_b.pos().x() + r * (_B * cos(-(t + _theta)) - sin(-(t + _theta))));
			_b.pos().y(_b.pos().y() + r * (_B * sin(-(t + _theta)) + cos(-(t + _theta))));
		};
	}
	else
	{
		throw std::logic_error("_B must not be zero");
	}
}

MyGameProject::Bullet::Behavior MyGameProject::curvature_adjusted_homing(const MovingObject& _target, Real _speed, Real _omega, int _keep_time)
{
	return[=,&_target](Bullet& _b)->void
	{
		auto count = _b.get_count();
		auto speed = abs(_speed);
		auto omega = abs(_omega);if (count > 30 && count < _keep_time){ omega += (count - 30) * static_cast<Real>(0.002); }

		Real theta = angle_of(_target.pos() - _b.pos());
		Real angle = _b.angle();

#if 0
		auto r = speed / omega;
		Point2D PC1(_b.pos().x() + r * cos(angle + half_pi<Real>()), _b.pos().y() + r * sin(angle + half_pi<Real>()));
		Point2D PC2(_b.pos().x() + r * cos(angle - half_pi<Real>()), _b.pos().y() + r * sin(angle - half_pi<Real>()));
		gp::DrawCircle(PC1.x(),PC1.y(),r,gp::sky.get());
		gp::DrawCircle(PC2.x(),PC2.y(), r, gp::sky.get());
#endif

		if (count < _keep_time)
		{
			if (sin(angle - theta) <= 0){_b.angle(angle + omega);}
			else{_b.angle(angle - omega);}
		}

		_b.pos().x(_b.pos().x() + speed * cos(_b.angle()));
		_b.pos().y(_b.pos().y() + speed * sin(_b.angle()));
	};
}