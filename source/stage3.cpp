//stage3.cpp

#include "stage3.h"
#include "player.h"
#include "add_get.h"
#include <array>
#include "draw_order.h"
#include "environmental_constants.h"
#include <dxlib.h>
#include "object.h"
#include <type_traits>
#include "find_vacant_object.h"
#include "diffuse_point.h"

namespace MyGameProject
{
	namespace ImageHandle
	{
		static std::array<int, 2> momiji_side;
		static int momiji_tree{ 0 };
		static int river_bottom{0};
		static int river_surface{0};
	}

	void Stage3BG::preparation(void)
	{
		ImageHandle::momiji_side[0] = add_get("../../data/img/stage3bg/momiji_left_side.png");
		ImageHandle::momiji_side[1] = add_get("../../data/img/stage3bg/momiji_right_side.png");
		ImageHandle::momiji_tree = add_get("../../data/img/stage3bg/momiji_tree.png");
		ImageHandle::river_bottom = add_get("../../data/img/stage3bg/river_bottom.png");
		ImageHandle::river_surface = add_get("../../data/img/stage3bg/river_surface.png");
	}

	struct Stage3BG::Impl 
	{
		Impl(const Player& _player) noexcept 
			:player(_player), 
			momiji_side_graph(1280, 960, true),
			river_graph(372, 1000, true)
		{}
		const Player& player;
		int count{0};
		gp::ScopedGraph momiji_side_graph;
		gp::ScopedGraph river_graph;
	};

	Stage3BG::Stage3BG(const Player& _player) noexcept
		:impl(std::make_unique<Impl>(_player))
	{
	}

	Stage3BG::~Stage3BG(void) = default;

	void Stage3BG::update(void) noexcept
	{
		auto&& count{impl->count};

		++count;
	}

	void Stage3BG::draw(void) const noexcept
	{
		auto&& count{impl->count};

		if (count > 0)
		{
			using namespace gp;
			auto& momiji_side_graph{impl->momiji_side_graph};
			auto& river_graph{impl->river_graph};
			//Make river side graphic object.
			SetDrawScreenOf
			(
				gp::DrawGraph(0, (11 * count) % 960 - 960, ImageHandle::momiji_side[0], true) >>
				gp::DrawGraph(0, (11 * count) % 960 - 960, ImageHandle::momiji_side[1], true) >>
				gp::DrawGraph(0, (11 * count) % 960      , ImageHandle::momiji_side[0], true) >>
				gp::DrawGraph(0, (11 * count) % 960      , ImageHandle::momiji_side[1], true) >>

				gp::DrawGraph(0, (16 * count) % 960 - 960, ImageHandle::momiji_side[0], true) >>
				gp::DrawGraph(0, (16 * count) % 960 - 960, ImageHandle::momiji_side[1], true) >>
				gp::DrawGraph(0, (16 * count) % 960      , ImageHandle::momiji_side[0], true) >>
				gp::DrawGraph(0, (16 * count) % 960      , ImageHandle::momiji_side[1], true),
				momiji_side_graph
			);
			//Make river graphic object.
			SetDrawScreenOf
			(
				gp::DrawGraph(0, (3 * count) % 1000 - 1000, ImageHandle::river_bottom,  true) >>
				gp::DrawGraph(0, (3 * count) % 1000       , ImageHandle::river_bottom,  true) >>

				gp::SetDrawBlendModeOf
				(
					gp::DrawGraph(gp::safe_rand<int>(0, 2), (4 * count) % 1000 - 1000 + gp::safe_rand<int>(0, 2), ImageHandle::river_surface, true) >>
					gp::DrawGraph(gp::safe_rand<int>(0, 2), (4 * count) % 1000        + gp::safe_rand<int>(0, 2), ImageHandle::river_surface, true),
					DX_BLENDMODE_ADD,
					140
				),

				river_graph
			);
			if (count < 400)
			{
				gp::DrawRotaGraphF(gp::level(0), WW_2<Real>(), WH_2<Real>(), 2.0, 0.0, river_graph, true);
				gp::DrawRotaGraphF(gp::level(0), WW_2<Real>(), WH_2<Real>(), 0.8, 0.0, momiji_side_graph, true);
			}
			else if(count >= 400 && count < 500)
			{
				gp::DrawRotaGraphF(gp::level(0), WW_2<Real>(), WH_2<Real>(), 2.0 + 0.006 * (count - 400), 0.0, river_graph, true);
				gp::DrawRotaGraphF(gp::level(0), WW_2<Real>(), WH_2<Real>(), 0.8 + 0.006 * (count - 400), 0.0, momiji_side_graph, true);
			}
			else
			{
				static constexpr auto final_scale_of_river_graph{2.0 + 0.006 * (500 - 400)};
				gp::DrawRotaGraphF(gp::level(0), WW_2<Real>(), WH_2<Real>(), final_scale_of_river_graph, 0.0, river_graph, true);
				gp::SetDrawBlendModeOf
				(
					gp::DrawRotaGraphF(gp::level(0), WW_2<Real>(), WH_2<Real>(), 0.8 + 0.006 * (count - 400), 0.0, momiji_side_graph, true),
					DX_BLENDMODE_ALPHA,
					255 - (count - 500) * 2
				);
			}
			//for (auto&& momiji_tree : impl->momiji_trees) { if (momiji_tree && momiji_tree->get_flag()) { momiji_tree->draw(); } }
			momiji_side_graph.clear();
		}
	}

	void Stage3BG::signal_handler(const Signal& _signal) noexcept
	{}
}