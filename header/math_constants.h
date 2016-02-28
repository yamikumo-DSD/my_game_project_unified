//math_constants.h

#ifndef __MATH_CONSTANTS_H
#define __MATH_CONSTANTS_H

#include <boost/math/constants/constants.hpp>

namespace MyGameProject
{
	template<class Real>
	const Real pi(void){ return boost::math::constants::pi<Real>(); }
}

#endif