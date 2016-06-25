//lock_rate_drawer.cpp

#include "lock_rate_drawer.h"
#include "print_digit.h"
#include "image_pool.h"
#include "decl_static_image_handler.h"

namespace MyGameProject
{
#ifndef LRD
#define LRD LockRateDrawer::
#endif

	struct LockRateUnit
	{
		int count{0};
		bool is_available{false};
	};

	struct LRD Impl
	{
		static constexpr std::size_t MAX_LOCK_RATE_UNIT_NUM{100};
		std::array<LockRateUnit, MAX_LOCK_RATE_UNIT_NUM> lock_rate_units;
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
	}; //struct Impl

	LRD LockRateDrawer(void) noexcept
	:pimpl(std::make_unique<Impl>())
	{ }

	LRD LockRateDrawer(const LockRateDrawer& _gage) noexcept
		:pimpl(std::make_unique<Impl>(*_gage.pimpl)){}

	LRD LockRateDrawer(LockRateDrawer&& _gage) noexcept
		:pimpl(std::move(_gage.pimpl)) {}

	LockRateDrawer& LRD operator=(const LockRateDrawer& _gage) noexcept { *pimpl = *_gage.pimpl; }
	
	LockRateDrawer& LRD operator=(LockRateDrawer&& _gage) noexcept { pimpl = std::move(_gage.pimpl); }

	LRD ~LockRateDrawer(void) noexcept = default;

	template<typename String>
	auto load_digit(String&& _path) noexcept
	{
		ImagePool::add(_path);
		return ImagePool::get(std::forward<String>(_path));
	}

	void LRD load_digit_images(void) noexcept
	{
		using namespace std::literals;
		auto root{"../../data/img/"s};
		Impl::_period_img() = load_digit(root + "dot.png"      );
		Impl::_0_img()      = load_digit(root + "digit_000.png");
		Impl::_1_img()      = load_digit(root + "digit_001.png");
		Impl::_2_img()      = load_digit(root + "digit_002.png");
		Impl::_3_img()      = load_digit(root + "digit_003.png");
		Impl::_4_img()      = load_digit(root + "digit_004.png");
		Impl::_5_img()      = load_digit(root + "digit_005.png");
		Impl::_6_img()      = load_digit(root + "digit_006.png");
		Impl::_7_img()      = load_digit(root + "digit_007.png");
		Impl::_8_img()      = load_digit(root + "digit_008.png");
		Impl::_9_img()      = load_digit(root + "digit_009.png");
	}
}//MyGameProject