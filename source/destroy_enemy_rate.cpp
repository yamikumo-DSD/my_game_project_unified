//destroy_enemy_rate.cpp

#include "destroy_enemy_rate.h"
#include "decl_static_image_handler.h"
#include "image_pool.h"
#include <type_traits>
#include "lock_weight_to_rate.h"
#include "mathematics.h"
#include "print_digit.h"
#include <boost/lexical_cast.hpp>
#include "debug_value.h"
#include <boost/format.hpp>

#ifndef DER
#define DER DestroyEnemyRate::
#endif

namespace MyGameProject
{
	struct DER Impl
	{
		STATIC_IMAGE_HANDLER(_period_img)
		STATIC_IMAGE_HANDLER(_0_img)
		STATIC_IMAGE_HANDLER(_1_img)
		STATIC_IMAGE_HANDLER(_2_img)
		STATIC_IMAGE_HANDLER(_3_img)
		STATIC_IMAGE_HANDLER(_4_img)
		STATIC_IMAGE_HANDLER(_5_img)
		STATIC_IMAGE_HANDLER(_6_img)
		STATIC_IMAGE_HANDLER(_7_img)
		STATIC_IMAGE_HANDLER(_8_img)
		STATIC_IMAGE_HANDLER(_9_img)

		std::string rate_text;
		Point2D pos;
		int count{0};
		int pal{255};
		bool is_enable{true};

		Impl(float _rate, Point2D _pos) noexcept
			//:rate_text(boost::lexical_cast<std::string>(_rate)),
			:rate_text(boost::str(boost::format("%.1f") % _rate)),
			pos(_pos)
		{}
	};

	DER DestroyEnemyRate(int _x, int _y, float _rate) noexcept
		:pimpl(std::make_unique<Impl>(_rate, Point2D(static_cast<Real>(_x), static_cast<Real>(_y))))
	{
	}

	DER ~DestroyEnemyRate(void) noexcept = default;
#ifndef ALIASES
#define ALIASES \
	auto& count{pimpl->count}; \
	auto& pos{pimpl->pos};
#endif

	void DER draw(void) const noexcept
	{
		ALIASES;
		if (count > 0)
		{
			for (int i = 0; i != 3; ++i)
			{
				try
				{
					print_digit
						(
							gp::level(28), static_cast<int>(pos.x() + 30 * i), static_cast<int>(pos.y()), 1.5, pimpl->pal, pimpl->rate_text[i],
							Impl::_period_img(),
							Impl::_0_img(), Impl::_1_img(), Impl::_2_img(), Impl::_3_img(), Impl::_4_img(),
							Impl::_5_img(), Impl::_6_img(), Impl::_7_img(), Impl::_8_img(), Impl::_9_img()
							);
				}
				catch (Unprintable)
				{
				}
			}
		}
	}

	void DER update(void) noexcept
	{
		ALIASES;
		pos += Point2D(1, -1);
		pimpl->pal -= 10;
		if (count == 20) { pimpl->is_enable = false; }
		++count;
	}

	bool DER get_flag(void) const noexcept { return pimpl->is_enable; }

	template<typename String>
	auto load_digit(String&& _path) noexcept
	{
		ImagePool::add(_path);
		return ImagePool::get(std::forward<String>(_path));
	}

	void DER load_digit_images(void) noexcept
	{
		using namespace std::literals;
		auto root{ "../../data/img/"s };
		Impl::_period_img() = load_digit(root + "dot.png");
		Impl::_0_img() = load_digit(root + "digit_000.png");
		Impl::_1_img() = load_digit(root + "digit_001.png");
		Impl::_2_img() = load_digit(root + "digit_002.png");
		Impl::_3_img() = load_digit(root + "digit_003.png");
		Impl::_4_img() = load_digit(root + "digit_004.png");
		Impl::_5_img() = load_digit(root + "digit_005.png");
		Impl::_6_img() = load_digit(root + "digit_006.png");
		Impl::_7_img() = load_digit(root + "digit_007.png");
		Impl::_8_img() = load_digit(root + "digit_008.png");
		Impl::_9_img() = load_digit(root + "digit_009.png");
	}
}