//player_health_gage.h

#pragma once

#include <memory>

namespace MyGameProject
{
	class PlayerHealthGage
	{
	private:
		struct Impl; std::unique_ptr<Impl> pimpl;
	public:
		PlayerHealthGage(void) noexcept;
		PlayerHealthGage(const PlayerHealthGage&) noexcept;
		PlayerHealthGage(PlayerHealthGage&&) noexcept;
		PlayerHealthGage& operator=(const PlayerHealthGage&) noexcept;
		PlayerHealthGage& operator=(PlayerHealthGage&&) noexcept;
		~PlayerHealthGage(void) noexcept;

		void draw(void) const noexcept;
		void update(std::size_t _current_health, double _player_y_coordinate) noexcept;

		static void preparation(void) noexcept;
	}; //class PlayerHealthGage
} //namespace MyGameProject