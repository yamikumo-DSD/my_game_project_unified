//killed_effect_small.h

#pragma once

#include "decl_static_image_handler.h"
#include "image_pool.h"
#include "object.h"
#include "draw_order.h"
#include "mathematics.h"
#include "ranged_uniform_random.h"
#include "debug_value.h"
#include <dxlib.h>

namespace MyGameProject
{
	//Concrete effect
	class KilledEffectSmall final : public KilledEffect
	{
	public:
		class Particle final : public MovingObject
		{
		private:
			int count{0};
			Point2D origin;
			Real additional_theta;
			static constexpr std::size_t TRAIL_NUM{15};
			std::array<Point2D, TRAIL_NUM> trails;
			enum class Color {Neg = 0, Pos,}color;
			const Real a;
		public:
			Particle(const Point2D& _origin, Real _rot) noexcept
				:origin(_origin),
				additional_theta(_rot),
				trails(),
				color(gp::safe_rand(0.0, 1.0) > 0.5? Color::Neg : Color::Pos),
				a(gp::safe_rand<Real>(5, 8))
			{}
			Particle(void)
				:origin(),
				additional_theta(0),
				trails(),
				color(gp::safe_rand(0.0, 1.0) > 0.5? Color::Neg : Color::Pos),
				a(gp::safe_rand<Real>(5, 8))
			{}
			virtual void update(void) override final
			{
				if (count == 0) { for (auto& p : trails) { p = pos(); } }
				else
				{
					for (int i = trails.size() - 1; i != 0; --i) { trails[i] = trails[i - 1]; }
				}

				auto theta{std::log(a * (count + 1) * two_pi<Real>() / 30)};
				auto r{static_cast<Real>(6 * std::exp(1 * theta))};
				pos() = origin + r * Point2D(cos(theta + additional_theta), sin(theta + additional_theta));

				trails[0] = pos();

				++count;
			}
			virtual void draw(void) const override final
			{
				if (count > 0)
				{
					int i{0};
					for (const auto& trail : trails)
					{
						const auto pal{150 * (1 - count / 30.f) - 7 * i};
						switch (color)
						{
							case Color::Neg:
							{
								gp::SetDrawBlendModeOf
								(
									gp::DrawRotaGraphF
									(
										gp::level(13),
										trail.x(), trail.y(), 0.3 - 0.02 * i, 0.0,
										img()[1], true
									),
									DX_BLENDMODE_SUB, pal
								);
							} break;
							case Color::Pos:
							{
								gp::SetDrawBlendModeOf
								(
									gp::DrawRotaGraphF
									(
										gp::level(13),
										trail.x(), trail.y(), 0.3 - 0.02 * i, 0.0,
										img()[1], true
									),
									DX_BLENDMODE_ALPHA, pal
								);
								gp::SetDrawBlendModeOf
								(
									gp::DrawRotaGraphF
									(
										gp::level(13),
										trail.x(), trail.y(), 0.3 - 0.02 * i, 0.0,
										img()[1], true
									),
									DX_BLENDMODE_ADD, pal
								);
							}
						}
						++i;
					}
				}
			}
		};
	private:
		std::array<Particle, 10> particles;
		virtual void custom_updater(void) noexcept override final
		{
			for (auto& p : particles) { p.update(); }
			if (count() > 100) { set_flag_off(); }
		}
		STATIC_IMAGE_HANDLER_LIST(img, 5)
	public:
		KilledEffectSmall(const Point2D& _killed_pos)
			:KilledEffect(_killed_pos),
			particles
			({
				Particle(_killed_pos, gp::safe_rand<Real>(0, two_pi<Real>())),
				Particle(_killed_pos, gp::safe_rand<Real>(0, two_pi<Real>())),
				Particle(_killed_pos, gp::safe_rand<Real>(0, two_pi<Real>())),
				Particle(_killed_pos, gp::safe_rand<Real>(0, two_pi<Real>())),
				Particle(_killed_pos, gp::safe_rand<Real>(0, two_pi<Real>())),
				Particle(_killed_pos, gp::safe_rand<Real>(0, two_pi<Real>())),
				Particle(_killed_pos, gp::safe_rand<Real>(0, two_pi<Real>())),
				Particle(_killed_pos, gp::safe_rand<Real>(0, two_pi<Real>())),
				Particle(_killed_pos, gp::safe_rand<Real>(0, two_pi<Real>())),
				Particle(_killed_pos, gp::safe_rand<Real>(0, two_pi<Real>()))
			})
		{}

		virtual void draw(void) const override final
		{
			for (const auto& p : particles) { p.draw(); }
		}

		static void preparation(void) noexcept
		{
			using namespace std::literals;
			constexpr auto ROOT{"../../data/img"};
			ImagePool::add(ROOT + "/white_ball_black.png"s ); img()[0] = ImagePool::get(ROOT + "/white_ball_black.png"s );
			ImagePool::add(ROOT + "/white_ball_green.png"s ); img()[1] = ImagePool::get(ROOT + "/white_ball_green.png"s );
			ImagePool::add(ROOT + "/white_ball_red.png"s   ); img()[2] = ImagePool::get(ROOT + "/white_ball_red.png"s   );
			ImagePool::add(ROOT + "/white_ball_white.png"s ); img()[3] = ImagePool::get(ROOT + "/white_ball_white.png"s );
			ImagePool::add(ROOT + "/white_ball_yellow.png"s); img()[4] = ImagePool::get(ROOT + "/white_ball_yellow.png"s);
		}
	};

}
