//river.h

#pragma once

#include "back_ground.h"
#include <memory>

namespace MyGameProject
{
	class River : public BackGround
	{
	private:
		struct Impl;
		std::unique_ptr<Impl> impl;
		virtual void custom_updater(void) override final;
	public:
		River(void) noexcept;
		River(const River& _sky) noexcept;
		River(River&& _sky) noexcept;
		River& operator=(const River& _lhs) noexcept;
		River& operator=(River&& _lhs) noexcept;

		virtual void draw_rear(void) const;
		virtual void draw_front(void) const;
		virtual ~River(void);

		static void preparation(void);
	};
}
