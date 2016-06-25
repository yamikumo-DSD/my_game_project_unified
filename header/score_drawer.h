//score_drawer.h

#pragma once

#include <memory>

namespace MyGameProject
{
	class ScoreDrawer
	{
	private:
		struct Impl; std::unique_ptr<Impl> pimpl;
	public:
		ScoreDrawer(void) noexcept;
		ScoreDrawer(const ScoreDrawer&) noexcept;
		ScoreDrawer(ScoreDrawer&&) noexcept;
		ScoreDrawer& operator=(const ScoreDrawer&) noexcept;
		ScoreDrawer& operator=(ScoreDrawer&&) noexcept;
		~ScoreDrawer(void) noexcept;

		void update(double _score) noexcept;
		void draw(void) const noexcept;
		static void preparation(void) noexcept;
	};
} //namespace MyGameProject
