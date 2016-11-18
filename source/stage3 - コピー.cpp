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
#include "debug.h"
#include "color.h"

namespace MyGameProject
{
	enum class Phase
	{
		FLYING_OVER_MOMIJI_FORREST = 0,
		TRANSITION_0,
		FLYING_OVER_RIVER,
		TRANSITION_1,
		DESCENDING,
	};

	static constexpr auto NUM_OF_PHASE{static_cast<int>(Phase::DESCENDING) + 1};
	static constexpr auto MAX_COUNT_OF_TRANSITION_0{100};

	namespace ImageHandle
	{
		static std::array<int, 2> momiji_side;
		static int momiji_tree{0};
		static int river_bottom{0};
		static int river_surface{0};

		static int frame{0};
		static int left_door{0};
		static int right_door{0};
		static int ring{0};
	}

	void Stage3BG::preparation(void)
	{
		ImageHandle::momiji_side[0]  = add_get("../../data/img/stage3bg/momiji_left_side.png");
		ImageHandle::momiji_side[1]  = add_get("../../data/img/stage3bg/momiji_right_side.png");
		ImageHandle::momiji_tree     = add_get("../../data/img/stage3bg/momiji_tree.png");
		ImageHandle::river_bottom    = add_get("../../data/img/stage3bg/river_bottom.png");
		ImageHandle::river_surface   = add_get("../../data/img/stage3bg/river_surface.png");
		ImageHandle::frame           = add_get("../../data/img/stage3bg/frame.png");
		ImageHandle::left_door       = add_get("../../data/img/stage3bg/left_door.png");
		ImageHandle::right_door      = add_get("../../data/img/stage3bg/right_door.png");
		ImageHandle::ring            = add_get("../../data/img/stage3bg/ring.png");
	}

	static constexpr auto WIDTH_OF_RIVER_BOTTOM_GRAPH {372 };
	static constexpr auto HEIGHT_OF_RIVER_BOTTOM_GRAPH{1000};

	struct Stage3BG::Impl 
	{
		Impl(const Player& _player) noexcept 
			:player(_player), 
			momiji_side_graph(1280, 960, true),
			river_graph(WIDTH_OF_RIVER_BOTTOM_GRAPH, HEIGHT_OF_RIVER_BOTTOM_GRAPH, true),
			phase_count_()
		{}
		const Player& player;
		int count{0};
		Real y_river_bottom{ 0 }, y_river_surface{ 0 }, y_door{-WH_2<Real>()};
		gp::ScopedGraph momiji_side_graph;
		gp::ScopedGraph river_graph;
		Phase phase{Phase::FLYING_OVER_MOMIJI_FORREST};
	private:std::array<int, NUM_OF_PHASE> phase_count_;
	public:auto& phase_count(Phase _phase) noexcept
		{
			return phase_count_[static_cast<int>(_phase)];
		}
	};

	Stage3BG::Stage3BG(const Player& _player) noexcept
		:impl(std::make_unique<Impl>(_player))
	{
	}

	Stage3BG::~Stage3BG(void) = default;

	void Stage3BG::update(void) noexcept
	{
		auto&& count{impl->count};
		auto&& phase{impl->phase};
		auto&& y_river_bottom{impl->y_river_bottom};
		auto&& y_river_surface{impl->y_river_surface};

		switch (phase)
		{
			case Phase::FLYING_OVER_MOMIJI_FORREST:
			{
				y_river_bottom  = (y_river_bottom  + 3) > HEIGHT_OF_RIVER_BOTTOM_GRAPH ?  0 : y_river_bottom  + 3;
				y_river_surface = (y_river_surface + 4) > HEIGHT_OF_RIVER_BOTTOM_GRAPH ?  0 : y_river_surface + 4;
			}break;
			case Phase::TRANSITION_0:
			{
				y_river_bottom  = (y_river_bottom  + 3) > HEIGHT_OF_RIVER_BOTTOM_GRAPH ?  0 : y_river_bottom  + 3;
				y_river_surface = (y_river_surface + 4) > HEIGHT_OF_RIVER_BOTTOM_GRAPH ?  0 : y_river_surface + 4;
				if (impl->phase_count(phase) > MAX_COUNT_OF_TRANSITION_0)
				{
					phase = Phase::FLYING_OVER_RIVER;
				}
			}break;
			case Phase::FLYING_OVER_RIVER:
			{
				y_river_bottom  = (y_river_bottom  + 3) > HEIGHT_OF_RIVER_BOTTOM_GRAPH ?  0 : y_river_bottom  + 3;
				y_river_surface = (y_river_surface + 4) > HEIGHT_OF_RIVER_BOTTOM_GRAPH ?  0 : y_river_surface + 4;
			}break;
			case Phase::TRANSITION_1:
			{
				static constexpr auto COUNT_UNTIL_STOP{221};
				auto speed0{ 3 - 3.f / COUNT_UNTIL_STOP * impl->phase_count(phase) }; if (speed0 < 0) { speed0 = 0; }
				auto speed1{ 4 - 4.f / COUNT_UNTIL_STOP * impl->phase_count(phase) }; if (speed1 < 0) { speed1 = 0; }
				y_river_bottom  = (y_river_bottom  + speed0) > HEIGHT_OF_RIVER_BOTTOM_GRAPH ?  0 : y_river_bottom  + speed0;
				y_river_surface = (y_river_surface + speed1) > HEIGHT_OF_RIVER_BOTTOM_GRAPH ?  0 : y_river_surface + speed1;
				impl->y_door += speed0;
			}break;
			default:
			{
			}break;
		}

		++impl->phase_count(phase);
		++count;
	}

	void Stage3BG::draw(void) const noexcept
	{
		auto&& count              {impl->count             };
		auto&& phase              {impl->phase             };
		auto&& current_phase_count{impl->phase_count(phase)};
		auto&& y_river_bottom{impl->y_river_bottom};
		auto&& y_river_surface{impl->y_river_surface};

		if (count > 0)
		{
			using namespace gp;
			auto& momiji_side_graph{impl->momiji_side_graph};
			auto& river_graph{impl->river_graph};

#ifndef MAKE_RIVER_SIDE_GRAPH
#define MAKE_RIVER_SIDE_GRAPH \
			SetDrawScreenOf\
			(\
				gp::DrawGraph(0, (11 * count) % 960 - 960, ImageHandle::momiji_side[0], true) >>\
				gp::DrawGraph(0, (11 * count) % 960 - 960, ImageHandle::momiji_side[1], true) >>\
				gp::DrawGraph(0, (11 * count) % 960      , ImageHandle::momiji_side[0], true) >>\
				gp::DrawGraph(0, (11 * count) % 960      , ImageHandle::momiji_side[1], true) >>\
\
				gp::DrawGraph(0, (16 * count) % 960 - 960, ImageHandle::momiji_side[0], true) >>\
				gp::DrawGraph(0, (16 * count) % 960 - 960, ImageHandle::momiji_side[1], true) >>\
				gp::DrawGraph(0, (16 * count) % 960      , ImageHandle::momiji_side[0], true) >>\
				gp::DrawGraph(0, (16 * count) % 960      , ImageHandle::momiji_side[1], true),\
				momiji_side_graph\
			);
#endif
#ifndef MAKE_RIVER_GRAPH
#define MAKE_RIVER_GRAPH\
			SetDrawScreenOf\
			(\
				gp::DrawGraph(0, y_river_bottom - HEIGHT_OF_RIVER_BOTTOM_GRAPH, ImageHandle::river_bottom,  true) >>\
				gp::DrawGraph(0, y_river_bottom                               , ImageHandle::river_bottom,  true) >>\
\
				gp::SetDrawBlendModeOf\
				(\
					gp::DrawGraph(0, y_river_surface - HEIGHT_OF_RIVER_BOTTOM_GRAPH, ImageHandle::river_surface, true) >>\
					gp::DrawGraph(0, y_river_surface                               , ImageHandle::river_surface, true),\
					DX_BLENDMODE_ADD,\
					140\
				),\
\
				river_graph\
			);
#endif

			static constexpr auto final_scale_of_river_graph{2.0 + 0.006 * MAX_COUNT_OF_TRANSITION_0};
			switch (phase)
			{
				case Phase::FLYING_OVER_MOMIJI_FORREST:
				{
					MAKE_RIVER_SIDE_GRAPH
					MAKE_RIVER_GRAPH
					gp::DrawRotaGraphF(gp::level(0), WW_2<Real>(), WH_2<Real>(), 2.0, 0.0, river_graph, true);
					gp::DrawRotaGraphF(gp::level(0), WW_2<Real>(), WH_2<Real>(), 0.8, 0.0, momiji_side_graph, true);
				}break;
				case Phase::TRANSITION_0:
				{
					MAKE_RIVER_SIDE_GRAPH
					MAKE_RIVER_GRAPH
					gp::DrawRotaGraphF(gp::level(0), WW_2<Real>(), WH_2<Real>(), 2.0 + 0.006 * current_phase_count, 0.0, river_graph, true);
					gp::SetDrawBlendModeOf
					(
						gp::DrawRotaGraphF(gp::level(0), WW_2<Real>(), WH_2<Real>(), 0.8 + 0.006 * current_phase_count, 0.0, momiji_side_graph, true),
						DX_BLENDMODE_ALPHA,
						255 - current_phase_count * 2
					);
				}break;
				case Phase::FLYING_OVER_RIVER:
				{
					static constexpr auto HEIGHT{HEIGHT_OF_RIVER_BOTTOM_GRAPH};
					static constexpr auto SCALE{final_scale_of_river_graph};
					gp::DrawRotaGraphF(WW_2<Real>(),(y_river_bottom         ) * SCALE + WH_2<Real>(), SCALE, 0,ImageHandle::river_bottom, true);
					gp::DrawRotaGraphF(WW_2<Real>(),(y_river_bottom - HEIGHT) * SCALE + WH_2<Real>(), SCALE, 0,ImageHandle::river_bottom, true);
					gp::SetDrawBlendModeOf
					(
						gp::DrawRotaGraphF(WW_2<Real>(),(y_river_surface         ) * SCALE + WH_2<Real>(), SCALE, 0,ImageHandle::river_surface, true)>>
						gp::DrawRotaGraphF(WW_2<Real>(),(y_river_surface - HEIGHT) * SCALE + WH_2<Real>(), SCALE, 0,ImageHandle::river_surface, true),
						DX_BLENDMODE_ADD, 140
					);
				}break;
				case Phase::TRANSITION_1:
				{
					static constexpr auto HEIGHT{HEIGHT_OF_RIVER_BOTTOM_GRAPH};
					static constexpr auto SCALE{final_scale_of_river_graph};
					static constexpr auto COUNT_UNTIL_DOOR_OPEN{400};
					static constexpr auto TIME_TO_DOOR_GETTING_READY{150.f};
					static constexpr auto COUNT_UNTIL_DOOR_GET_READY{COUNT_UNTIL_DOOR_OPEN - TIME_TO_DOOR_GETTING_READY};

					const auto vibration
					{
						(current_phase_count > COUNT_UNTIL_DOOR_GET_READY && current_phase_count <= COUNT_UNTIL_DOOR_OPEN)? gp::safe_rand<int>(-3, 3) : 0
					};

					gp::DrawRotaGraphF(WW_2<Real>(),(y_river_bottom         ) * SCALE + WH_2<Real>(), SCALE, 0,ImageHandle::river_bottom, true);
					gp::DrawRotaGraphF(WW_2<Real>(),(y_river_bottom - HEIGHT) * SCALE + WH_2<Real>(), SCALE, 0,ImageHandle::river_bottom, true);

					auto door_scale{1.0};
					if (current_phase_count > COUNT_UNTIL_DOOR_GET_READY && current_phase_count <= COUNT_UNTIL_DOOR_OPEN)
					{
						door_scale = 1.0 + 0.0005 * (current_phase_count - COUNT_UNTIL_DOOR_GET_READY);
					}
					else if (current_phase_count > COUNT_UNTIL_DOOR_OPEN)
					{
						door_scale = 1.0 + 0.0005 * (COUNT_UNTIL_DOOR_OPEN - COUNT_UNTIL_DOOR_GET_READY);
					}

					{
						int width{ 0 }, height{ 0 };
						DxLib::GetGraphSize(ImageHandle::left_door, &width, &height);
						const auto src_x
						{
							current_phase_count > COUNT_UNTIL_DOOR_OPEN?
							width / 500.f * (current_phase_count - COUNT_UNTIL_DOOR_OPEN) : 0
						};
						const auto trimmed
						{
							gp::DerivationGraph
							(
								src_x        , 0     ,
								width - src_x, height,
								ImageHandle::left_door
							)
						};
						gp::DrawRotaGraphF(vibration + (width - src_x) / 2, vibration + impl->y_door * SCALE, door_scale, 0,trimmed , true);
					}

					//gp::DrawRotaGraphF(vibration + WW_2<Real>() + (current_phase_count > COUNT_UNTIL_DOOR_OPEN? current_phase_count - COUNT_UNTIL_DOOR_OPEN : 0),vibration + impl->y_door * SCALE, door_scale, 0,ImageHandle::right_door, true);
					//gp::DrawRotaGraphF(vibration + WW_2<Real>()                                                                                                 ,vibration + impl->y_door * SCALE, door_scale, 0,ImageHandle::frame     , true);

					gp::SetDrawBlendModeOf
					(
						gp::DrawRotaGraphF(vibration + WW_2<Real>(), vibration + (y_river_surface         ) * SCALE + WH_2<Real>(), SCALE, 0,ImageHandle::river_surface, true)>>
						gp::DrawRotaGraphF(vibration + WW_2<Real>(), vibration + (y_river_surface - HEIGHT) * SCALE + WH_2<Real>(), SCALE, 0,ImageHandle::river_surface, true),
						DX_BLENDMODE_ADD, 
						current_phase_count < COUNT_UNTIL_DOOR_GET_READY? 
						140 : (140 - 140 / TIME_TO_DOOR_GETTING_READY * (current_phase_count - COUNT_UNTIL_DOOR_GET_READY))
					);

					static const auto r = [](const int _c)
					{
						return _c / 50.f - _c / 50;
					};
					static const auto p = [](const int _c)
					{
						return 255 * (1 - (_c / 50.f - _c / 50));
					};
					if (current_phase_count > COUNT_UNTIL_DOOR_GET_READY && current_phase_count <= COUNT_UNTIL_DOOR_OPEN)
					{
						gp::SetDrawBlendModeOf
						(
							gp::DrawRotaGraphF(vibration + WW_2<Real>(), vibration + WH_2<Real>(), r(current_phase_count - COUNT_UNTIL_DOOR_GET_READY), 0, ImageHandle::ring, true),
							DX_BLENDMODE_ADD, p(current_phase_count - COUNT_UNTIL_DOOR_GET_READY)
						);
					}

				}break;
				default:
				{
				}break;
			}
			momiji_side_graph.clear();
		}
	}

	void Stage3BG::signal_handler(const Signal& _signal) noexcept
	{
		auto&& phase{impl->phase};

		switch (phase)
		{
			case Phase::FLYING_OVER_MOMIJI_FORREST:
			{
				switch (_signal)
				{
					case 0:
					{
						phase = Phase::TRANSITION_0;
					}break;
					default:
					{
					}break;
				}
			}break;
			case Phase::TRANSITION_0:
			{
			}break;
			case Phase::FLYING_OVER_RIVER:
			{
				switch (_signal)
				{
					case 0:
					{
						phase = Phase::TRANSITION_1;
					}break;
					default:
					{
					}break;
				}
			}break;
			default:
			{
			}break;
		}
	}
}