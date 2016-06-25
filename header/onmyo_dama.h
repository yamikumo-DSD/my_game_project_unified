//onmyo_dama.h

#pragma once

#include "player.h"
#include <boost/optional.hpp>
#include "draw_order.h"
#include "dxlib_wrapper.h"
#include "image_pool.h"
#include <boost/math/constants/constants.hpp>
#include "device.h"
#include "add_get.h"
#include "debug.h"

namespace MyGameProject
{
	struct OnmyoDama : public MovingObject
	{
		boost::optional<const Device&> device;
		boost::optional<const Player&> player;
		int count;
		Real direction;
		Real theta;
		Point2D center;
		static constexpr Real R = 50.f;
		static constexpr Real A = 55.f;
		static constexpr Real B = 15.f;
		static constexpr Real ROTATION_SPEED = 15.f;
		static int& img(void) { static int img = 0; return img; }
		static int& base(void) { static int img = 0; return img; }
		static int& top(void) { static int img = 0; return img; }
		static int& hyper_mode(void) { static int img = 0; return img; }
#ifndef OMEGA
#define OMEGA (pi<Real>() / 50)
#endif

		//OnmyoDama(const Player& _player, Real _initial_theta)
		//	:player(_player), count(0), direction(0), theta(_initial_theta)
		//{
		//	using namespace std;
		//	center = R * Point2D(cos(direction), sin(direction));
		//	pos() = player->pos() + center + Point2D(A * cos(theta), B * sin(theta));
		//}
		OnmyoDama(const Device& _device, const Player& _player, Real _initial_theta)
			:player(_player), count(0), direction(0), theta(_initial_theta), device(_device)
		{
			using namespace std;
			center = R * Point2D(cos(direction), sin(direction));
			pos() = player->pos() + center + Point2D(A * cos(theta), B * sin(theta));
		}
		OnmyoDama(void) :player(), count(0), direction(0), theta(0) {}
		virtual void draw(void) const override final
		{
			using namespace boost::math::constants;
			const auto x{ pos().x() }, y{pos().y()};
			const auto scale
			{
				//player->shot_if()?
				//0.5 + 0.12 * std::sin(0.4 * count)
				//:
				0.5
			};

			gp::DrawRotaGraphF(gp::level(15), x, y, scale, count * two_pi<Real>() / 20, base(), true);
			gp::SetDrawBlendModeOf
			(
				gp::DrawRotaGraphF(gp::level(15), x, y, scale, 0, top(), true),
				DX_BLENDMODE_MULA,
				150
			);

			if (player->is_hyper_mode())
			{
				gp::SetDrawBlendModeOf
				(
					gp::DrawRotaGraphF(gp::level(15), x, y, 1.0, 0.3 * count, hyper_mode(), true),
					DX_BLENDMODE_ADD,
					120
				);
			}
		}
		virtual void update(void) override final
		{
			using namespace std;
			using namespace boost::math::constants;

			//update direction
			if (player)
			{
				if (!player->is_locked())
				{
					using P = Point2D;
					const bool up_input = device->get_time("Up") > 0;
					const bool down_input = device->get_time("Down") > 0;
					const bool right_input = device->get_time("Right") > 0;
					const bool left_input = device->get_time("Left") > 0;
					const auto conceptual_player_v
					{
						up_input   * P( 0,-1) + 
						down_input * P( 0, 1) + 
						right_input* P( 1, 0) + 
						left_input * P(-1, 0)
					};

					auto player_dir = angle_of(conceptual_player_v);
					if (player_dir > -0.001 && player_dir < 0.001) { player_dir = two_pi<Real>(); }
					const auto px = player->pos().x(), py = player->pos().y();
					if (norm(conceptual_player_v) > 0 && player->state() != Player::State::WARP)
					{
						if (abs(direction - player_dir + half_pi<Real>()) > OMEGA)
						{
							if (cos(player_dir - direction) > 0) 
							{
								direction -= OMEGA; 
								if (direction < 0) { direction += two_pi<Real>(); }
							}
							else if (cos(player_dir - direction) < 0) 
							{
								direction += OMEGA; 
								if (direction > two_pi<Real>()) { direction -= two_pi<Real>(); }
							}
						}
						else 
						{
							direction = player_dir - half_pi<Real>();
						}
					}
				}

				center = R * Point2D(cos(direction - half_pi<Real>()), sin(direction - half_pi<Real>()));

				namespace bg = boost::geometry;
				using namespace bg::strategy::transform;
				using boost::numeric::ublas::prod;

				Point2D rational_pos;
				rotate_transformer<bg::radian, Real, 2, 2> rotate(-direction);
				bg::transform(Point2D(A * cos(theta), B * sin(theta)), rational_pos, rotate);

				pos() = player->pos() + center + rational_pos;

				theta += ROTATION_SPEED;
				++count;
			}
		}
		static void preperation(void)
		{
			ImagePool::add("../../data/img/option.png");
			img() = ImagePool::get("../../data/img/option.png");
			ImagePool::add("../../data/img/onmyodama_base.png");
			base() = ImagePool::get("../../data/img/onmyodama_base.png");
			ImagePool::add("../../data/img/onmyodama_top.png");
			top() = ImagePool::get("../../data/img/onmyodama_top.png");
			hyper_mode() = add_get("../../data/img/particle.png");
		}
		virtual ~OnmyoDama(void) {}
	};
}