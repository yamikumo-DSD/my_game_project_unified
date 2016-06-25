//hyper_start_effect.h

#pragma once

#include <memory>

namespace MyGameProject
{
	class Player;

	class HyperStartEffect
	{
	private:
		struct Impl; std::unique_ptr<Impl> pimpl;
	public:
		HyperStartEffect(const Player& _player) noexcept;
		HyperStartEffect(HyperStartEffect&& _effect) noexcept;
		HyperStartEffect(const HyperStartEffect& _effect) noexcept;
		HyperStartEffect& operator=(HyperStartEffect&& _effect) noexcept;
		HyperStartEffect& operator=(const HyperStartEffect& _effect) noexcept;
		~HyperStartEffect(void) noexcept;

		void update(void) noexcept;
		void draw(void) noexcept;
		void start_playing(void) noexcept;
		static void preparation(void) noexcept;
	};
}
