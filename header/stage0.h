//stage0.h

#pragma once

#include "back_ground_control.h"
#include <memory>

namespace MyGameProject
{
	class Player;

	class Stage0BG : public BackGroundControl
	{
	private:
		struct Impl;
		std::unique_ptr<Impl> impl;
		void stage_intr_updater(void);
		void normal_back_ground_updater(void);
		void boss0_updater(void);
		void boss1_updater(void);
		virtual void update(void) noexcept override final;
		virtual void draw(void) const noexcept override final;
		virtual void signal_handler(const Signal& _signal) noexcept override final;
	public:
		Stage0BG(const Player& _player) noexcept;
		virtual ~Stage0BG(void);
		static void preparation(void);
	};
}