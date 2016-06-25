//lock_rate_drawer.h

#pragma once

#include <memory>

namespace MyGameProject
{
	class LockRateDrawer
	{
	private:
		struct Impl; std::unique_ptr<Impl> pimpl;
	public:
		LockRateDrawer(void) noexcept;
		LockRateDrawer(const LockRateDrawer&) noexcept;
		LockRateDrawer(LockRateDrawer&&) noexcept;
		LockRateDrawer& operator=(const LockRateDrawer&) noexcept;
		LockRateDrawer& operator=(LockRateDrawer&&) noexcept;
		~LockRateDrawer(void) noexcept;
		
		void draw(void) const noexcept;
		void update(void) noexcept;
		void set_pos_and_lock_weight(int _x, int _y, int );

		static void load_digit_images(void) noexcept;
	};
} //MyGameProject
