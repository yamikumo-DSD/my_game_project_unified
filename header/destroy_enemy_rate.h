//destroy_enemy_rate.h

#pragma once

#include <memory>

namespace MyGameProject
{
	class DestroyEnemyRate
	{
	private:
		struct Impl; std::unique_ptr<Impl> pimpl;
	public:
		DestroyEnemyRate(int _x, int _y, float _rate) noexcept;
		~DestroyEnemyRate(void) noexcept;

		void draw(void) const noexcept;
		void update(void) noexcept;

		bool get_flag(void) const noexcept;

		static void load_digit_images(void) noexcept;
	};
} //MyGameProject