//color.h

#ifndef __MYLIB_GP_COLOR_H
#define __MYLIB_GP_COLOR_H

#include "DxLib.h"

namespace gp
{
	class Color
	{
	public:
		using Byte = unsigned char;

	private:
		Byte r, g, b;

	public:
		constexpr Color(Byte _r, Byte _g, Byte _b) noexcept :r(_r), g(_g), b(_b) {}
		constexpr Color(const Color& _rhs) noexcept = default;
		constexpr Color(Color&& _rhs) noexcept = default;
		Color& operator=(const Color& _rhs) noexcept = default;
		Color& operator=(Color&& _rhs) noexcept = default;
		~Color(void) noexcept = default;
		template<typename ColorType>
		constexpr Color operator+(ColorType&& _rhs) const noexcept
		{
			return Color
			(
				r + _rhs.r,
				g + _rhs.g,
				b + _rhs.b
			);
		}

		constexpr auto red(void)   const noexcept { return r; }
		constexpr auto green(void) const noexcept { return g; }
		constexpr auto blue(void)  const noexcept { return b; }
		template<typename R, typename G, typename B>
		void set(R&& _r, G&& _g, B&& _b) noexcept
		{
			r = static_cast<Byte>(std::forward<R>(_r));
			g = static_cast<Byte>(std::forward<G>(_g));
			b = static_cast<Byte>(std::forward<B>(_b));
		}
		auto get(void) const noexcept { return DxLib::GetColor(r, g, b); }
		operator int() const noexcept{ return DxLib::GetColor(r, g, b); }
	};

	constexpr Color red(255, 0, 0);
	constexpr Color green(0, 255, 0);
	constexpr Color blue(0, 0, 255);
	constexpr Color black(0, 0, 0);
	constexpr Color white(255, 255, 255);
	constexpr Color sky(120, 255, 255);
}

#endif
