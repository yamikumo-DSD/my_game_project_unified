//score_drawer.cpp

#include "score_drawer.h"
#include "decl_static_image_handler.h"
#include <boost/lexical_cast.hpp>
#include "image_pool.h"
#include "print_digit.h"
#include "debug.h"

namespace MyGameProject
{
#ifndef SD 
#define SD ScoreDrawer:: 
#endif
	struct SD Impl 
	{
		STATIC_IMAGE_HANDLER_LIST(digit, 11)
		double score{1.0};
	};

	SD ScoreDrawer(void) noexcept
		:pimpl(std::make_unique<Impl>())
	{}

	SD ScoreDrawer(const ScoreDrawer& _brd) noexcept
		:pimpl(std::make_unique<Impl>(*_brd.pimpl))
	{ }

	SD ScoreDrawer(ScoreDrawer&& _brd) noexcept
		: pimpl(std::move(_brd.pimpl))
	{ }

	ScoreDrawer& SD operator=(const ScoreDrawer& _brd) noexcept
	{
		*pimpl = *_brd.pimpl;
	}

	ScoreDrawer& SD operator=(ScoreDrawer&& _brd) noexcept
	{
		pimpl = std::move(_brd.pimpl);
	}

	SD ~ScoreDrawer(void) noexcept = default;

	void SD update(double _score)noexcept
	{
		pimpl->score = _score;
	}

	void SD draw(void) const noexcept
	{
		try
		{
			const auto text{ boost::lexical_cast<std::string>(static_cast<int>(pimpl->score)) };
			int i{ 0 };
			for (auto c : text)
			{
				print_digit
				(
					gp::level(28), 15 + 12.6 * i, 20, 0.5, 255, c,
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

	void SD preparation(void) noexcept
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
