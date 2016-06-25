//base_rate_drawer.h

#pragma once

#include <memory>

namespace MyGameProject
{
	class BaseRateDrawer
	{
	private:
		struct Impl; std::unique_ptr<Impl> pimpl;
	public:
		BaseRateDrawer(void) noexcept;
		BaseRateDrawer(const BaseRateDrawer&) noexcept;
		BaseRateDrawer(BaseRateDrawer&&) noexcept;
		BaseRateDrawer& operator=(const BaseRateDrawer&) noexcept;
		BaseRateDrawer& operator=(BaseRateDrawer&&) noexcept;
		~BaseRateDrawer(void) noexcept;

		void update(double _base_rate) noexcept;
		void draw(void) const noexcept;
		static void preparation(void) noexcept;
	};
} //namespace MyGameProject
