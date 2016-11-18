//stage3.cpp

#include "stage3.h"
#include "player.h"
#include "add_get.h"
#include <array>
#include "draw_order.h"
#include "environmental_constants.h"
#include <dxlib.h>
#include <type_traits>
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
	static constexpr auto MAX_COUNT_OF_TRANSITION_1{500};

	namespace ImageHandle
	{
		static std::array<int, 2> momiji_side;
		static int river_bottom{0};
		static int river_surface{0};
		static int top_frame{0};
		static int under_frame{0};
		static int flash{0};
		static int pit{0};
		static int pit_flash{0};
	}

	void Stage3BG::preparation(void)
	{
		ImageHandle::momiji_side[0]  = add_get("../../data/img/stage3bg/momiji_left_side.png" );
		ImageHandle::momiji_side[1]  = add_get("../../data/img/stage3bg/momiji_right_side.png");
		ImageHandle::river_bottom    = add_get("../../data/img/stage3bg/river_bottom.png"     );
		ImageHandle::river_surface   = add_get("../../data/img/stage3bg/river_surface.png"    );
		ImageHandle::under_frame     = add_get("../../data/img/stage3bg/frame1.png"           );
		ImageHandle::top_frame       = add_get("../../data/img/stage3bg/frame0.png"           );
		ImageHandle::flash           = add_get("../../data/img/stage3bg/flash.png"            );
		ImageHandle::pit             = add_get("../../data/img/stage3bg/pit.png"              );
		ImageHandle::pit_flash       = add_get("../../data/img/stage3bg/pit_flash.png"        );
	}

	static constexpr auto WIDTH_OF_RIVER_BOTTOM_GRAPH {372 };
	static constexpr auto HEIGHT_OF_RIVER_BOTTOM_GRAPH{1000};

	struct Stage3BG::Impl 
	{
		Impl(const Player& _player) noexcept 
			:player(_player), 
			momiji_side_graph(1280, 960, true),
			river_graph(WIDTH_OF_RIVER_BOTTOM_GRAPH, HEIGHT_OF_RIVER_BOTTOM_GRAPH, true),
			phase_count_(),
			entrance(640, 480, false)
		{}
		const Player& player;
		int count{0};
		Real y_river_bottom{ 0 }, y_river_surface{ 0 }, y_door{-WH_2<Real>()};
		Real y_pit{0};
		gp::ScopedGraph momiji_side_graph;
		gp::ScopedGraph river_graph;
		Phase phase{Phase::FLYING_OVER_MOMIJI_FORREST};
		gp::ScopedGraph entrance;
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
		auto&& y_pit{impl->y_pit};

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
				if (impl->phase_count(phase) > MAX_COUNT_OF_TRANSITION_0) { phase = Phase::FLYING_OVER_RIVER; }
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
				auto speed1{ speed0 + 1 }; if (speed1 < 0) { speed1 = 0; }
				y_river_bottom  = (y_river_bottom  + speed0) > HEIGHT_OF_RIVER_BOTTOM_GRAPH ?  0 : y_river_bottom  + speed0;
				y_river_surface = (y_river_surface + speed1) > HEIGHT_OF_RIVER_BOTTOM_GRAPH ?  0 : y_river_surface + speed1;
				impl->y_door += speed0;
				if (impl->phase_count(phase) > MAX_COUNT_OF_TRANSITION_1) { phase = Phase::DESCENDING; }
			}break;
			case Phase::DESCENDING:
			{
				y_pit -= 5;
				if (y_pit <= -WH<int>()) { y_pit = 0; }
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
		auto&& y_pit{impl->y_pit};

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

					const auto draw_entrance_graph
					{
						[=](void)
						{
							return 
							gp::DrawRotaGraphF(WW_2<Real>(),(y_river_bottom         ) * SCALE + WH_2<Real>(), SCALE, 0,ImageHandle::river_bottom, true)>>
							gp::DrawRotaGraphF(WW_2<Real>(),(y_river_bottom - HEIGHT) * SCALE + WH_2<Real>(), SCALE, 0,ImageHandle::river_bottom, true)>>

							//Draw entrance
							gp::DrawRotaGraphF(WW_2<Real>(), impl->y_door * SCALE, 0.5, 0,ImageHandle::under_frame, true)>>
							gp::SetDrawBlendModeOf
							(
								gp::DrawRotaGraphF(WW_2<Real>(), (y_river_surface         ) * SCALE + WH_2<Real>(), SCALE, 0,ImageHandle::river_surface, true)>>
								gp::DrawRotaGraphF(WW_2<Real>(), (y_river_surface - HEIGHT) * SCALE + WH_2<Real>(), SCALE, 0,ImageHandle::river_surface, true),
								DX_BLENDMODE_ADD, 140
							)>>
							gp::DrawRotaGraphF(WW_2<Real>(), impl->y_door * SCALE, 0.5, 0,ImageHandle::top_frame, true)>>
							gp::SetDrawBlendModeOf
							(
								gp::DrawRotaGraphF(WW_2<Real>(), impl->y_door * SCALE, 0.5, 0,ImageHandle::flash, true),
								DX_BLENDMODE_ADD, count % 50
							);
						}
					};

					if (current_phase_count < 200) { draw_entrance_graph(); }
					else
					{
						gp::SetDrawScreenOf(draw_entrance_graph(), impl->entrance);
						static constexpr auto DELAY_COUNT{350};
						gp::SetDrawBlendModeOf
						(
							gp::DrawRotaGraph(WW_2<int>(), WH_2<int>(),1.0 +  0.01 * (current_phase_count - 200) , 0.0, impl->entrance, true),
							DX_BLENDMODE_ALPHA, static_cast<int>(current_phase_count < DELAY_COUNT? 255 : 255 * (1 - (current_phase_count - DELAY_COUNT) / 120.f))
						);
					}

				}break;
				case Phase::DESCENDING:
				{
					//Draw base graphich
					gp::SetDrawBlendModeOf
					(
						gp::DrawGraph(0, y_pit            , ImageHandle::pit, true) >>
						gp::DrawGraph(0, y_pit + WH<int>(), ImageHandle::pit, true),
						DX_BLENDMODE_ALPHA, static_cast<int>(current_phase_count < 120? 255 * (current_phase_count / 120.f) : 255)
					);

					gp::SetDrawBlendModeOf
					(
						gp::DrawCircle(gp::level(24), WW_2<int>(), WH_2<int>(), 500, gp::Color(255, 0, 0)),
						DX_BLENDMODE_MULA, 
						static_cast<int>
						(
							current_phase_count < 120? 
							(std::abs(100 * sin(current_phase_count / 30.0)) + 100) * (current_phase_count / 120.f) : 
							(std::abs(100 * sin(current_phase_count / 30.0)) + 100)
						)
					);

					//Draw flash
					gp::SetDrawBlendModeOf
					(
						gp::DrawGraph(0, y_pit            , ImageHandle::pit_flash, true) >>
						gp::DrawGraph(0, y_pit + WH<int>(), ImageHandle::pit_flash, true),
						DX_BLENDMODE_ADD,
						static_cast<int>
						(
							current_phase_count < 120? 
							(std::abs(100 * sin(current_phase_count / 30.0)) + 100) * (current_phase_count / 120.f) : 
							(std::abs(100 * sin(current_phase_count / 30.0)) + 100)
						)
					);
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
			case Phase::TRANSITION_1:
			{
			}break;
			case Phase::DESCENDING:
			{
			}break;
			default:
			{
			}break;
		}
	}
}