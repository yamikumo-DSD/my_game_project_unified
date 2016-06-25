//stage3.h

#pragma once

#include "back_ground_control.h"
#include <memory>

namespace MyGameProject
{
	class Player;

	class Stage3BG : public BackGroundControl
	{
	private:
		struct Impl; std::unique_ptr<Impl> impl;
		virtual void update(void) noexcept override final;
		virtual void draw(void) const noexcept override final;
		virtual void signal_handler(const Signal& _signal) noexcept override final;
	public:
		Stage3BG(const Player& _player) noexcept;
		virtual ~Stage3BG(void);
		static void preparation(void);
	};
}