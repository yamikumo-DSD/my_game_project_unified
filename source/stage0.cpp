//stage0.cpp

#include "stage0.h"
#include "pseud3d.h"
#include "image_pool.h"
#include "object.h"
#include "draw_order.h"
#include "ranged_uniform_random.h"
#include "mathematics.h"
#include "environmental_constants.h"
#include <array>
#include <memory>
#include "find_vacant_object.h"
#include "decl_static_image_handler.h"
#include <dxlib.h>
#include "sky.h"
#include "player.h"
#include "debug.h"
#include "color.h"
#include "river.h"
#include "momiji.h"

namespace MyGameProject
{
	using namespace boost::math::constants;

	struct Stage0BG::Impl
	{
		int count{0};
		void draw_intr(void) const;
		void draw_normal(void) const;
		void draw_boss0(void) const;
		void draw_boss1(void) const;
		enum class Mode
		{
			NORMAL,
			BOSS0,
			BOSS1,
		}mode{ Mode::NORMAL };
		ThroughCloud normal_and_boss0;
		Sky boss1;
		River stage_intr;
		Impl(const Player& _player) noexcept :boss1(_player) {}
		bool white_out_start_flag{ false };
		int white_out_count{ 0 };
		static constexpr int WHITE_OUT_DURATION{700};
		int sky_on_timer{0};
		bool is_sky_on_timer_on{false};

		bool is_cloudy{false};
		int cloudy_count{0};

		std::array<std::unique_ptr<Momiji>, 25> leaves;
	};

	void Stage0BG::Impl::draw_intr(void) const
	{
		stage_intr.draw_front();
		stage_intr.draw_rear();
	}

	void Stage0BG::Impl::draw_normal(void) const
	{
		normal_and_boss0.draw_front();
		normal_and_boss0.draw_rear();
	}

	void Stage0BG::Impl::draw_boss0(void) const
	{
		normal_and_boss0.draw_front();
		normal_and_boss0.draw_rear();
	}

	void Stage0BG::Impl::draw_boss1(void) const
	{
		boss1.draw_front();
		boss1.draw_rear();
	}

	Stage0BG::Stage0BG(const Player& _player) noexcept
		: impl(std::make_unique<Impl>(_player))
	{}

	void Stage0BG::signal_handler(const Signal& _signal) noexcept
	{
		switch (_signal)
		{
		case 0:
			impl->mode = Impl::Mode::NORMAL;
			break;
		case 1:
			impl->mode = Impl::Mode::BOSS0;
			break;
		case 2:
			impl->sky_on_timer = impl->count + Impl::WHITE_OUT_DURATION / 2;
			impl->is_sky_on_timer_on = true;
			impl->white_out_start_flag = true;
			break;
		case 3:
			impl->is_cloudy = true;
			break;
		}
	}

	void Stage0BG::stage_intr_updater(void) 
	{
		impl->stage_intr.update();
	}

	void Stage0BG::boss0_updater(void) 
	{
		impl->normal_and_boss0.update();
	}

	void Stage0BG::boss1_updater(void) 
	{
		impl->boss1.update();
	}

	void Stage0BG::normal_back_ground_updater(void) 
	{
		impl->normal_and_boss0.update();
	}

	void Stage0BG::update(void) noexcept
	{
		for (auto& leaf : impl->leaves) 
		{
			if (leaf && leaf->get_flag()) { leaf->update(); } 
			else { leaf.reset(); }
		}
		if(!impl->is_cloudy)
		{
			if (impl->count % 10 == 0) 
			{
				try
				{
					*find_vacant_object(impl->leaves) = std::make_unique<Momiji>(Point2D(gp::safe_rand(0, WW<int>()), -100));
				}
				catch (NoSpace) {}
			}
		}

		if (impl->is_sky_on_timer_on && impl->count == impl->sky_on_timer)
		{
			impl->mode = Impl::Mode::BOSS1;
		}
		if (impl->white_out_start_flag)
		{
			++impl->white_out_count;
			if (impl->white_out_count == Impl::WHITE_OUT_DURATION)
			{
				impl->white_out_start_flag = false;
				impl->white_out_count = 0;
			}
		}
		if (impl->is_cloudy)
		{
			++impl->cloudy_count;
		}

		switch (impl->mode)
		{
			case Impl::Mode::NORMAL:
			{
				stage_intr_updater();
				if (impl->is_cloudy) { normal_back_ground_updater(); }
			}break;
			case Impl::Mode::BOSS0:
			{
				boss0_updater();
			}break;
			case Impl::Mode::BOSS1:
			{
				boss1_updater();
			}break;
		}
		++impl->count;
	} //Stage0BG::update

	void Stage0BG::draw(void) const noexcept
	{
		switch (impl->mode)
		{
			case Impl::Mode::NORMAL:
			{
				impl->draw_intr();
				if (impl->is_cloudy) { impl->draw_normal(); }
			}break;
			case Impl::Mode::BOSS0:
			{
				impl->draw_boss0();
			}break;
			case Impl::Mode::BOSS1:
			{
				impl->draw_boss1();
			}break;
		}

		if (impl->white_out_start_flag)
		{
			gp::SetDrawBlendModeOf
			(
				gp::DrawCircle
				(
					gp::level(24),
					0, 0,
					1000,
					gp::white.get()
				),
				DX_BLENDMODE_ALPHA,
				impl->white_out_count < Impl::WHITE_OUT_DURATION / 2 ?
				impl->white_out_count * 2 * 255.f / Impl::WHITE_OUT_DURATION
				:
				255 * 2 - impl->white_out_count * 255.f * 2 / Impl::WHITE_OUT_DURATION
			);
		}

		for (const auto& leaf : impl->leaves) { if (leaf && leaf->get_flag()) { leaf->draw(); } }
	} //void draw(void) const

	Stage0BG::~Stage0BG(void) = default;

	void Stage0BG::preparation(void)
	{
		Sky::preparation();
		River::preparation();
		Momiji::preparation();
	}
} //namespace MyGameProject