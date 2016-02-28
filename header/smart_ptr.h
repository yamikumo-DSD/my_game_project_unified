//smart_ptr.h

#ifndef __SMART_PTR_H
#define __SMART_PTR_H

#include <memory>
#include <boost/make_unique.hpp>
#include <type_traits>
#include <boost/optional.hpp>

namespace gp
{
	template<class T>
	using smart_ptr = std::shared_ptr < T > ;

	//Specification 1 for unique_ptr<T>
	template<class T, class... Args>
	typename std::enable_if<std::is_same<smart_ptr<T>, std::unique_ptr<T>>::value, smart_ptr<T> >::type make_smart(Args&&... args)
	{
		return boost::make_unique<T>(args...);
	}

	//Specificaton 2 for shared_ptr<T>
	template<class T, class... Args>
	typename std::enable_if<std::is_same<smart_ptr<T>, std::shared_ptr<T>>::value, smart_ptr<T> >::type make_smart(Args&&... args)
	{
		return std::make_shared<T>(args...);
	}

	//Specificaton 3 for boost::optional<T>
	template<class T, class... Args>
	typename std::enable_if<std::is_same<smart_ptr<T>, boost::optional<T>>::value, smart_ptr<T> >::type make_smart(Args&&... args)
	{
		return boost::optional<T>(args...);
	}
}

#endif