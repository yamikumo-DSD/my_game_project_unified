//aya_imple.cpp

#include "aya_imple.h"
#include "aya.h"
#include "debug_value.h"
#include "draw_order.h"
#include "color.h"
#include "player.h"
#include "bullet.h"
#include "environmental_constants.h"
#include "concrete_bullet.h"
#include "find_vacant_object.h"
#include "bullet_behavior.h"
#include "ranged_uniform_random.h"
#include "image_pool.h"

#ifndef XMAX
#define XMAX WW<Real>()
#endif
#ifndef YMAX
#define YMAX WH<Real>()
#endif
#ifndef XMAX_2
#define XMAX_2 WW_2<Real>()
#endif
#ifndef YMAX_2
#define YMAX_2 WH_2<Real>()
#endif
#ifndef INIT_V 
#define INIT_V v.x(0); v.y(0);
#endif

namespace MyGameProject
{
	using namespace boost::math::constants;
	using P = Point2D;

	CloseUpBullet::CloseUpBullet
	(
		const Enemy& _master,
		const Player& _player,
		const Point2D& _initial_pos,
		Real _initial_angle,
		int _wait_time
	)
		:Bullet
		(
			_master,
			_player,
			_initial_pos,
			_initial_angle,
			ShapeElement::Null(), 
			[_wait_time](Bullet& _b)
			{
				const auto count = _b.get_count();
				auto& pos = _b.pos();

				if (count < T)
				{
					pos += SPEED_0 * P(cos(_b.angle()), sin(_b.angle()));
				}
				else if (count >= T && count < T + _wait_time)
				{
					//stay...
				}
				else if (count == T + _wait_time)
				{
					_b.angle(angle_of(_b.get_player_ref().pos() - pos));
				}
				else
				{
					pos += SPEED_1 * P(cos(_b.angle()), sin(_b.angle()));
				}
			}
		),
		wait_time(_wait_time)
	{}

	void CloseUpBullet::draw(void) const
	{
		gp::SetDrawBlendModeOf
		(
			gp::DrawCircle(gp::level(12), pos().x(), pos().y(), r, gp::blue.get()),
			DX_BLENDMODE_ALPHA, 150
		);
	}

	void CloseUpBullet::custom_updater(void)
	{
		static constexpr Real rc = 150.f;
		static constexpr Real r0 = 10.f;
		static constexpr int tc = 30;

		const auto count = get_count();
		const auto t = static_cast<Real>(0.8 * (count - T - wait_time));

		if (count >= T + wait_time)
		{
			static constexpr Real k = (rc - r0) / (tc * tc);
			r = r0 + k * t * t;
		}
		if (t > tc)
		{
			area().get_shape() = ShapeElement::Circle(150);
		}
		if (t > tc + 10)
		{
			set_flag_off();
		}
	}

	void AyaImple::action(void)
	{
		//counts
		const auto count = boss.get_count();

		//aliases
		auto& pos = boss.pos();
		const auto& player = boss.player_ref();
		auto& bullets = boss.get_bullets_ref();

		static constexpr int T0 = 1000;
		static constexpr int T1 = 1000;

		switch (wave)
		{
			case 0:
			{
				static constexpr Real INIT_MAG = 5.f;
				static constexpr Real FARTHEST_MAG = 0.25f;
				static const P FARTHEST_POS(XMAX_2, 220.f);
				static const P INITIAL_POS(XMAX_2, 50.f);
				static constexpr int T = 60;

				if (wave_count[0] == 0)
				{
					INIT_V;
					pos = INITIAL_POS;
					mag = INIT_MAG;
					boss.area().get_shape() = ShapeElement::Null();
				}
				else
				{
					if (wave_count[0] < T)
					{
						static constexpr Real dmag = (INIT_MAG - FARTHEST_MAG) / T;
						//mag = (mag - dmag) > FARTHEST_MAG? mag - dmag : FARTHEST_MAG;
						mag = FARTHEST_MAG + (INIT_MAG - FARTHEST_MAG) * exp(-wave_count[0] / 10.f);
						pos.y(pos.y() - (INITIAL_POS.y() - FARTHEST_POS.y()) / T);
					}
					else
					{
						const auto t = (wave_count[0] - T) / 20.f;
						pos = FARTHEST_POS + P(3 * sin(5 * t), 3 * cos(3 * t));
						if (wave_count[0] == 80)
						{
							static constexpr int N = 8;
							for (int i = 0; i != N; ++i)
							{
								*find_vacant_object(bullets) = std::make_shared<CloseUpBullet>(boss, player, boss.pos(), i * two_pi<Real>() / N, 40 * i);
							}
						}
						else if (wave_count[0] == 480)
						{
							static constexpr int N = 10;
							for (int i = 0; i != N; ++i)
							{
								*find_vacant_object(bullets) = std::make_shared<CloseUpBullet>(boss, player, boss.pos(), i * two_pi<Real>() / N, 20 * i);
							}
						}
						else if (wave_count[0] > 750 && wave_count[0] <= 900)
						{
							if (wave_count[0] % 20 == 0)
							{
								*find_vacant_object(bullets) = std::make_shared<CloseUpBullet>(boss, player, boss.pos(), gp::safe_rand<Real>(0, two_pi<Real>()), 0);
							}
						}
					}
					if (wave_count[0] == T0) {wave_count[0] = 0;  ++wave; }
				}
				++wave_count[0];
			}break;
			case 1:
			{
				static const P INITIAL_POS{XMAX_2, 220.f};
				static constexpr auto INIT_MAG = static_cast<Real>(0.25f);
				static constexpr auto DST_MAG = static_cast<Real>(0.6);
				static constexpr auto T = 120;
				static constexpr auto dmag = static_cast<Real>((DST_MAG - INIT_MAG) / static_cast<Real>(T));

				if (wave_count[1] == 0) // Initialize all.
				{
					INIT_V;
					mag = INIT_MAG;
					pos = INITIAL_POS;
				}
				else if(wave_count[1] > 0 && wave_count[1] != T)
				{
					mag += dmag; // Finally(when wave_count[1] is equal to T), mag reaches to DST_MAG.
				}
				else
				{
					 wave_count[1] = 0;++wave;
				}
				++wave_count[1];
			}break;
			case 2:
			{
				static const P INITIAL_POS{XMAX_2, 220.f};
				static const std::array<P, 3> DST_POS{P(100, 100), P(XMAX - 100, 100), P(XMAX_2, YMAX_2)};
				static const P POS_AFTER_VANISHED{XMAX_2, -300}; // Out of display 
				if (wave_count[2] == 0)
				{
					boss.area().get_shape() = ShapeElement::Circle(15);
					INIT_V;
					pos = INITIAL_POS;
				}
				else
				{
					static constexpr auto STAY_TIME_0   { 100 };
					static constexpr auto TRAVEL_TIME_0 { 30  };
					static constexpr auto STAY_TIME_1   { 200 };
					static constexpr auto TRAVEL_TIME_1 { 50  };
					static constexpr auto STAY_TIME_2   { 200 };
					static constexpr auto TRAVEL_TIME_2 { 30  };
					static constexpr auto STAY_TIME_3   { 400 };

					if (wave_count[2] < STAY_TIME_0)
					{
						const auto local_count = wave_count[2];
						/*do nothing*/
					}
					else if (wave_count[2] >= STAY_TIME_0 && wave_count[2] < STAY_TIME_0 + TRAVEL_TIME_0)
					{
						const auto local_count = wave_count[2] - STAY_TIME_0;
						if (local_count == 0)
						{
							motion = create_linear_motion_of_uniform_acceleration_2D(INITIAL_POS, DST_POS[0], TRAVEL_TIME_0);
							v = motion.v0();
						}
						else{ v += motion.a();  pos += v; }
					}
					else if (wave_count[2] >= STAY_TIME_0 + TRAVEL_TIME_0 && wave_count[2] < STAY_TIME_0 + TRAVEL_TIME_0 + STAY_TIME_1)
					{
						const auto local_count = wave_count[2] - STAY_TIME_0 - TRAVEL_TIME_0;
						/*do nothing*/
					}
					else if (wave_count[2] >= STAY_TIME_0 + TRAVEL_TIME_0 + STAY_TIME_1 && wave_count[2] < STAY_TIME_0 + TRAVEL_TIME_0 + STAY_TIME_1 + TRAVEL_TIME_1)
					{
						const auto local_count = wave_count[2] - STAY_TIME_0 - TRAVEL_TIME_0 - STAY_TIME_1;
						if (local_count == 0)
						{
							motion = create_linear_motion_of_uniform_acceleration_2D(DST_POS[0], DST_POS[1], TRAVEL_TIME_1);
							v = motion.v0();
						}
						else{ v += motion.a();  pos += v; }
					}
					else if (wave_count[2] >= STAY_TIME_0 + TRAVEL_TIME_0 + STAY_TIME_1 + TRAVEL_TIME_1 && wave_count[2] < STAY_TIME_0 + TRAVEL_TIME_0 + STAY_TIME_1 + TRAVEL_TIME_1 + STAY_TIME_2)
					{
						const auto local_count = wave_count[2] - STAY_TIME_0 - TRAVEL_TIME_0 - STAY_TIME_1 - TRAVEL_TIME_1;
						/*do nothing*/
					}
					else if (wave_count[2] >= STAY_TIME_0 + TRAVEL_TIME_0 + STAY_TIME_1 + TRAVEL_TIME_1 + STAY_TIME_2 && wave_count[2] < STAY_TIME_0 + TRAVEL_TIME_0 + STAY_TIME_1 + TRAVEL_TIME_1 + STAY_TIME_2 + TRAVEL_TIME_2)
					{
						const auto local_count = wave_count[2] - STAY_TIME_0 - TRAVEL_TIME_0 - STAY_TIME_1 - TRAVEL_TIME_1 - STAY_TIME_2;
						if (local_count == 0)
						{
							motion = create_linear_motion_of_uniform_acceleration_2D(DST_POS[1], DST_POS[2], TRAVEL_TIME_2);
							v = motion.v0();
						}
						else{ v += motion.a();  pos += v; }
					}
					else if (wave_count[2] >= STAY_TIME_0 + TRAVEL_TIME_0 + STAY_TIME_1 + TRAVEL_TIME_1 + STAY_TIME_2 + TRAVEL_TIME_2 && wave_count[2] < STAY_TIME_0 + TRAVEL_TIME_0 + STAY_TIME_1 + TRAVEL_TIME_1 + STAY_TIME_2 + TRAVEL_TIME_2 + STAY_TIME_3)
					{
						const auto local_count = wave_count[2] - STAY_TIME_0 - TRAVEL_TIME_0 - STAY_TIME_1 - TRAVEL_TIME_1 - STAY_TIME_2 - TRAVEL_TIME_2;
						if (local_count == STAY_TIME_3 - 100) {pos = POS_AFTER_VANISHED; }
					}
					else{wave_count[2] = 0; ++wave; }
				}
				++wave_count[2];
			}break;
			case 3:
			{
				static const P INITIAL_POS{XMAX_2, -100.f };
				static const P DST_POS    {XMAX_2, 220.f  };

				if (wave_count[3] == 0)
				{
					pos = INITIAL_POS;
					++wave2_index;
				}
				else
				{
					if (wave_count[3] < 150 * 5)
					{
						if (wave_count[3] % 150 == 0)
						{
							pos = P(player.pos().x(), -100);
						}
						else
						{
							pos += P(0, 10); 
							if (wave_count[3] % 10 == 0 && pos.y() > -50 && pos.y() <= YMAX + 50)
							{
								*find_vacant_object(bullets) = std::make_shared<Lazer> ( boss, player, pos + P(40, 0), 0, [](Bullet& _b) {}, 50, 60 );
								*find_vacant_object(bullets) = std::make_shared<Lazer> ( boss, player, pos + P(-40, 0), pi<Real>(), [](Bullet& _b) {}, 50, 60 );
							}
						}
					}
					else if (wave_count[3] >= 150 * 5 && wave_count[3] < 150 * 5 + 100 * 5)
					{
						if ((wave_count[3] - 150 * 5) % 100 == 0)
						{
							pos = P(player.pos().x() , -100);
						}
						else 
						{
							pos += P(0, 12.5); 
							if (wave_count[3] % 9 == 0 && pos.y() > -50 && pos.y() <= YMAX + 50)
							{
								*find_vacant_object(bullets) = std::make_shared<Lazer> ( boss, player, pos + P(40, 0), 0, [](Bullet& _b) {}, 50, 60 );
								*find_vacant_object(bullets) = std::make_shared<Lazer> ( boss, player, pos + P(-40, 0), pi<Real>(), [](Bullet& _b) {}, 50, 60 );
							}
						}
					}
					else if (wave_count[3] >= 150 * 5 + 100 * 5 && wave_count[3] < 150 * 5 + 100 * 5 + 60 * 5)
					{
						if (wave_count[3] == 150 * 5 + 100 * 5 || (wave_count[3] - 150 * 5 - 100 * 5) % 60 == 0)
						{
							pos = P(player.pos().x() , -100);
						}
						else
						{
							pos += P(0, 15); 
						}
					}
					else
					{
						if (wave_count[3] == 150 * 5 + 100 * 5 + 60 * 5)
						{
							pos = P(XMAX_2, -200);
							motion = create_linear_motion_of_uniform_acceleration_2D(pos, DST_POS, 100);
							v = motion.v0();
						}
						else
						{
							if (wave_count[3] == 150 * 5 + 100 * 5 + 60 * 5 + 100)
							{
								wave = 2;
								wave_count[3] = 0;
							}
							v += motion.a();
							pos += v;
						}
					}
				}
				++wave_count[3];
			}break;
			case 4:
			{

			}break;
		}
	}

	void AyaImple::draw_charactor(void) const
	{
		const auto& pos = boss.pos();
		const auto count = boss.get_count();

		
		if (wave == 0)
		{
			if (wave_count[0] < 60)
			{
				int i = 0;
				if      (wave_count[0] <  30                       ) { i = 0; }
				else if (wave_count[0] >= 30  && wave_count[0] < 40) { i = 1; }
				else if (wave_count[0] >= 40 && wave_count[0] < 50) { i = 2; }
				else                                                { i = 3; }
				gp::DrawRotaGraphF(gp::level(11), pos.x(), pos.y(), mag, 0.0, approach_img()[i], true);
			}
			else
			{
				const int c = wave_count[0];
				const int c_40 = c % 40;
				int i = 0;
				if (c_40 < 10) { i = 0; }
				else if (c_40 >= 10 && c_40 < 20) { i = 2; }
				else if (c_40 >= 20 && c_40 < 30) { i = 0; }
				else { i = 1; }

				gp::DrawRotaGraphF(gp::level(11), pos.x(), pos.y(), mag, 0.0, wings_img()[i], true);
				gp::DrawRotaGraphF(gp::level(11), pos.x(), pos.y(), mag, 0.0, standing_img(), true);
			}
		}
		else
		{
			const int c = wave_count[wave];
			const int c_40 = c % 40;
			int i = 0;
			if (c_40 < 10) { i = 0; }
			else if (c_40 >= 10 && c_40 < 20) { i = 2; }
			else if (c_40 >= 20 && c_40 < 30) { i = 0; }
			else { i = 1; }

			gp::DrawRotaGraphF(gp::level(11), pos.x(), pos.y(), mag, 0.0, wings_img()[i], true);
			gp::DrawRotaGraphF(gp::level(11), pos.x(), pos.y(), mag, 0.0, standing_img(), true);
		}
	}

	void AyaImple::preperation(void)
	{
		ImagePool::add("../../data/img/aya/_0_dot.png");
		ImagePool::add("../../data/img/aya/_1_dot.png");
		ImagePool::add("../../data/img/aya/_2_dot.png");
		ImagePool::add("../../data/img/aya/_3_dot.png");
		ImagePool::add("../../data/img/aya/_4_dot.png");

		ImagePool::add("../../data/img/aya/_4_wing_0_dot.png");
		ImagePool::add("../../data/img/aya/_4_wing_1_dot.png");
		ImagePool::add("../../data/img/aya/_4_wing_2_dot.png");

		approach_img()[0] = ImagePool::get("../../data/img/aya/_0_dot.png");
		approach_img()[1] = ImagePool::get("../../data/img/aya/_1_dot.png");
		approach_img()[2] = ImagePool::get("../../data/img/aya/_2_dot.png");
		approach_img()[3] = ImagePool::get("../../data/img/aya/_3_dot.png");
		standing_img()    = ImagePool::get("../../data/img/aya/_4_dot.png");

		wings_img()[0]    = ImagePool::get("../../data/img/aya/_4_wing_0_dot.png");
		wings_img()[1]    = ImagePool::get("../../data/img/aya/_4_wing_1_dot.png");
		wings_img()[2]    = ImagePool::get("../../data/img/aya/_4_wing_2_dot.png");
	}
}
