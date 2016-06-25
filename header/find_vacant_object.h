//find_vacant_object.h

#ifndef __FIND_VACANT_OBJECT
#define __FIND_VACANT_OBJECT

#include <algorithm>
#include <exception>
#include <type_traits>

namespace MyGameProject
{
	namespace Impl
	{
		template<typename T>
		struct is_weak_ptr
		{
			static constexpr auto value{ false };
		};

		template<typename T>
		struct is_weak_ptr<std::weak_ptr<T>>
		{
			static constexpr auto value{ true };
		};
	}

	class NoSpace : public std::runtime_error
	{
	public: 
		NoSpace(void) :runtime_error("There is no space in the container to put the new object.") {}
		virtual ~NoSpace(void) = default;
	};

	//non-weak_ptr ver.
	template<typename Container>
	auto find_vacant_object(Container& _objs, std::enable_if_t<!Impl::is_weak_ptr<typename Container::value_type>::value>* = nullptr)
	{
		auto i = std::find_if
		(
			_objs.begin(),
			_objs.end(),
			[](auto&& _1)
			{
				return (!_1 || _1->get_flag() == false);
			}
		);

		if (i == _objs.end()){ throw NoSpace(); }
		else{ return i; }
	}

	//weak_ptr ver.
	template<typename Container>
	auto find_vacant_object(Container& _objs, std::enable_if_t<Impl::is_weak_ptr<typename Container::value_type>::value>* = nullptr)
	{
		auto i = std::find_if
		(
			_objs.begin(),
			_objs.end(),
			[](auto&& _1)
			{
				if (_1.expired()) { return true; }
				else { return !_1.lock()->get_flag(); }
			}
		);

		if (i == _objs.end()) { throw NoSpace(); }
		else{ return i; }
	}
}

#endif
