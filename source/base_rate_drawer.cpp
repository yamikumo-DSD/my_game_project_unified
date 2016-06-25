//base_rate_drawer.cpp

#include "base_rate_drawer.h"
#include "decl_static_image_handler.h"
#include <boost/lexical_cast.hpp>
#include "image_pool.h"
#include "print_digit.h"
#include "debug.h"

namespace MyGameProject
{
#ifndef BRD 
#define BRD BaseRateDrawer:: 
#endif
	struct BRD Impl 
	{
		STATIC_IMAGE_HANDLER_LIST(digit, 11)
		double base_rate{1.0};
	};

	BRD BaseRateDrawer(void) noexcept
		:pimpl(std::make_unique<Impl>())
	{}

	BRD BaseRateDrawer(const BaseRateDrawer& _brd) noexcept
		:pimpl(std::make_unique<Impl>(*_brd.pimpl))
	{ }

	BRD BaseRateDrawer(BaseRateDrawer&& _brd) noexcept
		: pimpl(std::move(_brd.pimpl))
	{ }

	BaseRateDrawer& BRD operator=(const BaseRateDrawer& _brd) noexcept
	{
		*pimpl = *_brd.pimpl;
	}

	BaseRateDrawer& BRD operator=(BaseRateDrawer&& _brd) noexcept
	{
		pimpl = std::move(_brd.pimpl);
	}

	BRD ~BaseRateDrawer(void) noexcept = default;

	void BRD update(double _base_rate)noexcept
	{
		pimpl->base_rate = _base_rate;
	}

	void BRD draw(void) const noexcept
	{
		try
		{
			const auto text{ boost::lexical_cast<std::string>(static_cast<int>(pimpl->base_rate)) };
			int i{ 0 };
			for (auto c : text)
			{
				print_digit
				(
					gp::level(28), 60 + 25 * i, 53, 1.0, 255, c,
					Impl::digit()[10], Impl::digit()[0], Impl::digit()[1], Impl::digit()[2], Impl::digit()[3], Impl::digit()[4],
					Impl::digit()[5], Impl::digit()[6], Impl::digit()[7], Impl::digit()[8], Impl::digit()[9]
				);
				++i;
			}
		}
		catch (Unprintable)
		{
		}
	}

	void BRD preparation(void) noexcept
	{
		static auto add_and_get
		{
			[](auto&& _file_path)
			{
				ImagePool::add(_file_path);
				return ImagePool::get(std::forward<decltype(_file_path)>(_file_path));
			}
		};

		using namespace std::literals;

		for (int i = 0; i != 10; ++i)
		{
			Impl::digit()[i] = add_and_get("../../data/img/base_rate_digit_00"s + boost::lexical_cast<std::string>(i) + ".png");
		}
		Impl::digit()[10] = 0;
	}
}//namespace MyGameProject