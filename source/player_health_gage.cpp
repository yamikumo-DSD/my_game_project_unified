//player_health_gage.cpp

#include "player_health_gage.h"
#include "draw_order.h"
#include <dxlib.h>
#include "debug.h"
#include "color.h"
#include "environmental_constants.h"
#include "mathematics.h"
#include <array>
#include <type_traits>
#include "decl_static_image_handler.h"
#include "image_pool.h"
#include <boost/geometry/strategies/transform.hpp>
#include <boost/geometry/strategies/transform/matrix_transformers.hpp>
#include <boost/numeric/ublas/operation.hpp>
#include <boost/geometry/algorithms/transform.hpp> 

#ifndef PHG
#define PHG PlayerHealthGage:: 
#endif

namespace MyGameProject
{
	class LifeIcon
	{
	private:
		int count{0};
		bool is_available{true};
		int x, y;
	public:
		template
		<
			typename XType, typename YType,
			std::enable_if_t<std::is_arithmetic<XType>::value && std::is_arithmetic<YType>::value>* = nullptr
		>
		LifeIcon(XType&& _x, YType&& _y) noexcept : x(std::forward<XType>(_x)), y(std::forward<YType>(_y)) {}
		LifeIcon(void) : x(0), y(0) {}
		template
		<
			typename XType, typename YType,
			std::enable_if_t<std::is_arithmetic<XType>::value && std::is_arithmetic<YType>::value>* = nullptr
		>
		void set_pos(XType&& _x, YType&& _y) noexcept
		{
			 x = std::forward<XType>(_x); 
			 y = std::forward<YType>(_y);
		}
		void update(void) noexcept
		{
			for (auto& t : ts) { t += static_cast<Real>(0.3); }
			++count;
		}
		void damaged(void) noexcept
		{
			is_available = false;
		}
		void extension(void) noexcept
		{
			count = 0;
			is_available = true;
		}
		template < typename PalType, std::enable_if_t<std::is_arithmetic<PalType>::value>* = nullptr >
		void draw(PalType&& _pal) const noexcept
		{
			using namespace std;
			namespace bg = boost::geometry;
			using namespace bg::strategy::transform;
			using boost::numeric::ublas::prod;

			if (is_available)
			{
				gp::SetDrawBlendModeOf
				(
					gp::DrawRotaGraph(gp::level(28), x, y, 0.09, 0.0, img()[0], true),
					DX_BLENDMODE_ADD,
					static_cast<int>(_pal)
				);

				for (int i = 0; i != 4; ++i)
				{
					static constexpr Real A{ 4 }, B{ 23 };
					rotate_transformer<bg::radian, Real, 2, 2> rotate{pi<Real>() / 2 + i * two_pi<Real>() / 8 };
					Point2D result_pos;
					bg::transform(Point2D(A * cos(ts[i]), B * sin(ts[i])), result_pos, rotate);
					gp::SetDrawBlendModeOf
					(
						gp::DrawRotaGraph(gp::level(28), x + result_pos.x(), y + result_pos.y(), 0.03, 0.0, img()[i + 1], true),
						DX_BLENDMODE_MULA,
						static_cast<int>(_pal)
					);
					gp::SetDrawBlendModeOf
					(
						gp::DrawRotaGraph(gp::level(28), x + result_pos.x(), y + result_pos.y(), 0.03, 0.0, img()[i + 1], true),
						DX_BLENDMODE_ADD,
						static_cast<int>(_pal)
					);
				}
			}
		}

		STATIC_IMAGE_HANDLER_LIST(img, 5)
		std::array<Real, 4> ts;

		static void preparation(void) noexcept
		{
			ImagePool::add("../../data/img/life_icon_center.png");
			img()[0] = ImagePool::get("../../data/img/life_icon_center.png");
			ImagePool::add("../../data/img/life_icon_red.png");
			img()[1] = ImagePool::get("../../data/img/life_icon_red.png");
			ImagePool::add("../../data/img/life_icon_black.png");
			img()[2] = ImagePool::get("../../data/img/life_icon_black.png");
			ImagePool::add("../../data/img/life_icon_yellow.png");
			img()[3] = ImagePool::get("../../data/img/life_icon_yellow.png");
			ImagePool::add("../../data/img/life_icon_blue.png");
			img()[4] = ImagePool::get("../../data/img/life_icon_blue.png");
		}
	};

	struct PHG Impl
	{
		int count{0};
		std::size_t health{0};
		std::size_t previous_health{0};
		Real player_y_coordinate{0};

		static constexpr std::size_t MAX_ICON_DISPLAYED{8};
		std::array<LifeIcon, MAX_ICON_DISPLAYED> icons;

		std::size_t icon_index{MAX_ICON_DISPLAYED - 1};
	};

	PHG PlayerHealthGage(void) noexcept
	:pimpl(std::make_unique<Impl>())
	{
		for (int i = 0; i != Impl::MAX_ICON_DISPLAYED; ++i)
		{
			//icons[0] is the one on the right side.
			//icons[Impl::MAX_ICON_DISPLAYED] is the one on the left side.
			pimpl->icons[i].set_pos(WW<int>() - 30 - i * 40, 30);
		}
	}

	PHG PlayerHealthGage(const PlayerHealthGage& _gage) noexcept
		:pimpl(std::make_unique<Impl>(*_gage.pimpl)){}

	PHG PlayerHealthGage(PlayerHealthGage&& _gage) noexcept
		:pimpl(std::move(_gage.pimpl)) {}

	PlayerHealthGage& PHG operator=(const PlayerHealthGage& _gage) noexcept { *pimpl = *_gage.pimpl; }
	
	PlayerHealthGage& PHG operator=(PlayerHealthGage&& _gage) noexcept { pimpl = std::move(_gage.pimpl); }

	PHG ~PlayerHealthGage(void) noexcept = default;

	void PHG draw(void) const noexcept
	{
		auto& count{pimpl->count};
		auto& health{pimpl->health};

		if (count > 0)
		{
			for (const auto& icon : pimpl->icons) { icon.draw(320 * (pimpl->player_y_coordinate / WH<Real>())); }
		}
	}

	void PHG update(std::size_t _current_health, double _player_y_coordinate) noexcept
	{
		auto& count{pimpl->count};
		auto& health{pimpl->health};
		auto& prev_health{pimpl->previous_health};

		prev_health = health;
		health = _current_health;
		pimpl->player_y_coordinate = static_cast<decltype(pimpl->player_y_coordinate)>(_player_y_coordinate);

		for (auto& icon : pimpl->icons) { icon.update(); }

		if (health < prev_health && pimpl->icon_index > 0) 
		{
			pimpl->icons[pimpl->icon_index--].damaged();
		}
		else if (health > prev_health && pimpl->icon_index < Impl::MAX_ICON_DISPLAYED - 1)
		{
			pimpl->icons[pimpl->icon_index++].extension();
		}

		++count;
	}

	void PHG preparation(void) noexcept
	{
		LifeIcon::preparation();
	}
}