//sky.h

#pragma once

#include "back_ground.h"
#include <memory>
#include <boost/optional.hpp>

namespace MyGameProject
{
	class Player;

	class Sky : public BackGround
	{
	private:
		struct Impl;
		std::unique_ptr<Impl> impl;
		virtual void custom_updater(void) override final;
		boost::optional<const Player&> player;
	public:
		//following to Item 22 of Effective modern c++, declare special functions in pimpl idiom.
		Sky(const Player& _player) noexcept;
		Sky(const Sky& _sky) noexcept;
		Sky(Sky&& _sky) noexcept;
		Sky& operator=(const Sky& _lhs) noexcept;
		Sky& operator=(Sky&& _lhs) noexcept;

		virtual void draw_rear(void) const;
		virtual void draw_front(void) const;
		virtual ~Sky(void);

		static void preparation(void);
	};
}
