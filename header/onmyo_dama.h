//onmyo_dama.h

#pragma once

#include "player.h"
#include <boost/optional.hpp>
#include "draw_order.h"
#include "dxlib_wrapper.h"
#include "image_pool.h"
#include <boost/math/constants/constants.hpp>

namespace MyGameProject
{
	struct OnmyoDama : public MovingObject
	{
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
#ifndef OMEGA
#define OMEGA (pi<Real>() / 50)
#endif

		OnmyoDama(const Player& _player, Real _initial_theta)
			:player(_player), count(0), direction(0), theta(_initial_theta)
		{
			using namespace std;
			center = R * Point2D(cos(direction), sin(direction));
			pos() = player->pos() + center + Point2D(A * cos(theta), B * sin(theta));
		}
		OnmyoDama(void) :player(), count(0), direction(0), theta(0) {}
		virtual void draw(void) const override final
		{
			using namespace boost::math::constants;

			gp::DrawRotaGraph(gp::level(12), pos().x(),pos().y(),0.5,count * two_pi<Real>() / 20,base(),true);
			gp::SetDrawBlendModeOf
			(
				gp::DrawRotaGraph(gp::level(12), pos().x(),pos().y(),0.5,0,top(),true),
				DX_BLENDMODE_MULA, 
				150
			);
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
					const auto player_dir = angle_of(player->v());
					const auto px = player->pos().x(), py = player->pos().y();
					if (norm(player->v()) > 0)
					{
						if (abs(direction - player_dir) >= OMEGA)
						{
							if (cos(player_dir - direction) > 0) { direction -= OMEGA; }
							if (cos(player_dir - direction) < 0) { direction += OMEGA; }
						}
						else { direction = player_dir; }
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
		}
		virtual ~OnmyoDama(void) {}
	};
}