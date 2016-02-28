//locked_marker.cpp

#include "locked_marker.h"
#include "enemy.h"
#include "image_pool.h"
#include "draw_order.h"
#include "dxlib_wrapper.h"
#include "mathematics.h"
#include "drawblendmode.h"
#include <boost/optional.hpp>
#include <boost/math/constants/constants.hpp>
#include "debug_value.h"
#include "dxsstream.h"

namespace MyGameProject
{
	struct Trail
	{
		int count;
		Real angle;
		Real opacity;
		Trail(void) :angle(0), opacity(0), count(0){}
	};

	struct LockedMarkerVars
	{
		int count;
		static int& img(void) { static int img = 0; return img; }
		static constexpr std::size_t TRAIL_NUM = 5;
		Trail head;
		std::array<boost::optional<Trail>, TRAIL_NUM> trails;

		LockedMarkerVars(void)
		:head(),
		trails(),
		count(0)
		{}
	};

	LockedMarker::LockedMarker(decltype(target) _target, int _m)
		:target(_target),
		vars(std::make_unique<LockedMarkerVars>()),
		m(_m)
	{}

	void LockedMarker::update(void)
	{
		using namespace boost::math::constants;
		if (target && target->get_flag())
		{
			pos() = target->pos();

			if      (vars->count == 5)  { vars->trails[0] = Trail(); vars->trails[0]->angle = vars->head.angle;}
			else if (vars->count == 10) { vars->trails[1] = Trail(); vars->trails[1]->angle = vars->head.angle;}
			else if (vars->count == 20) { vars->trails[2] = Trail(); vars->trails[2]->angle = vars->head.angle;}
			else if (vars->count == 30) { vars->trails[3] = Trail(); vars->trails[3]->angle = vars->head.angle;}
			else if (vars->count == 40) { vars->trails[4] = Trail(); vars->trails[4]->angle = vars->head.angle;}
			//update head
			vars->head.angle += two_pi<Real>() / 100;
			++vars->head.count;
			//update trails
			for (auto& trail : vars->trails)
			{
				if (trail)
				{
					trail->opacity = 255.f - trail->count * (255.f / 20.f);
					++trail->count;
				}
			}
		}
		else
		{
			set_flag_off();
		}
		++vars->count;
	}

	void LockedMarker::draw(void) const
	{
		//draw head
		gp::DrawRotaGraph
		(
				gp::level(25),
				pos().x(),
				pos().y(),
				1.0,
				vars->head.angle,
				LockedMarkerVars::img(),
				true
		);
		//draw trails
		if (vars->trails[0])
		{
				const auto count = vars->count;
				gp::SetDrawBlendModeOf
				(
					gp::DrawRotaGraph
					(
							gp::level(25),
							pos().x(),
							pos().y(),
							[count](void) 
							{
								if (count <= 20) { return 3 - count / 10.f; }
								else { return 1.f; }
							}(),
							vars->trails[0]->angle,
							LockedMarkerVars::img(),
							true
					),
					DX_BLENDMODE_ALPHA,
					static_cast<int>(vars->trails[0]->opacity)
				);
		}
		for (const auto& trail : vars->trails)
		{
			if (trail)
			{
				const auto count = vars->count;
				gp::SetDrawBlendModeOf
				(
					gp::DrawRotaGraph
					(
							gp::level(25),
							pos().x(),
							pos().y(),
							[count](void) 
							{
								if (count <= 20) { return 3 - count / 10.f; }
								else { return 1.f; }
							}(),
							trail->angle,
							LockedMarkerVars::img(),
							true
					),
					DX_BLENDMODE_ALPHA,
					static_cast<int>(trail->opacity)
				);
			}
		}
		gp::dxout(pos().x(), pos().y()) << gp::red << m << gp::end;
	}

	LockedMarker::~LockedMarker(void) {}

	void LockedMarker::preperation(void)
	{
		ImagePool::add("../../data/img/locked_marker.png");
		LockedMarkerVars::img() = ImagePool::get("../../data/img/locked_marker.png");
	}
}