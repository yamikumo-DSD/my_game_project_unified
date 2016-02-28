//find_nearest_object.h

#ifndef __FIND_NEAREST_OBJECT_H
#define __FIND_NEAREST_OBJECT_H

#include <boost/geometry.hpp>
#include <iterator>
#include <algorithm>

namespace MyGameProject
{
	template < class BaseObject, class TargetPtrContainer >
	typename TargetPtrContainer::iterator::value_type find_nearest_object(const BaseObject& _base,TargetPtrContainer& _targets)
	{
		using namespace boost::geometry;

		auto i = _targets.begin();
		auto nearest_itr = i;

		while (i != _targets.end())
		{
			if (*i && *(i + 1))
			{
				if (distance(_base.pos(), (*i)->pos()) > distance(_base.pos(), (*(i + 1))->pos()))
				{
					nearest_itr = i + 1;
				}
			}
			std::advance(i,1);
		}

		if (nearest_itr != _targets.end()){ return *nearest_itr; }
		return TargetPtrContainer::iterator::value_type();
	}
}

#endif