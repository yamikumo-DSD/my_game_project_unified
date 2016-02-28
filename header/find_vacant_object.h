//find_vacant_object.h

#ifndef __FIND_VACANT_OBJECT
#define __FIND_VACANT_OBJECT

#include <algorithm>
#include <exception>

namespace MyGameProject
{
	template<class Container>
	auto find_vacant_object(Container& _objs)->decltype(_objs.begin())
	{
		auto i = std::find_if
		(
			_objs.begin(),
			_objs.end(),
			[](decltype(*_objs.begin()) _1)->bool
			{
				return (_1.get() == nullptr || _1->get_flag() == false);
			}
		);

		if (i == _objs.end()){ throw std::runtime_error("Over the limitation."); }
		else{ return i; }
	}
}

#endif
