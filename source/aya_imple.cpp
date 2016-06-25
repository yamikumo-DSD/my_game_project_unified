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
#include "diffuse_point.h"
#include "shared_object.h"
#include "forced_crow.h"
#include "border_check.h"
#include "add_get.h"

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

	struct ForcedCrowBulletAction
	{
		int delay{0};
		Point2D dst;
		Real angle{0};
		std::function<void(const std::string&)> erect_se_flag_of;
		const Player& player;
		bool return_flag{false};
		ForcedCrowBulletAction(int _delay, const Player& _player, decltype(erect_se_flag_of) _se_flag_erecter) noexcept
			:delay(_delay), dst(), player(_player), erect_se_flag_of(_se_flag_erecter)
		{}
		void operator()(Bullet& _b) noexcept
		{
			const auto count{_b.get_count()};
			auto& pos{_b.pos()};
			if (count < delay)
			{
				pos += 8 * Point2D(cos(_b.angle()), sin(_b.angle()));
			}
			else if (count == delay) { dst = player.pos(); angle = angle_of(dst - pos); }
			else
			{
				pos += 11 * Point2D(cos(angle), sin(angle));
				if (is_within_window(_b.pos(), 100))
				{
					try
					{
						if (count % 2 == 0)
						{
							//_color: false->blue, true->red
							auto create_bullets
							{
								[&_b = _b, dir = angle](auto&& _theta, bool _color)
								{
									*find_vacant_object(SharedObject::bullets()) =
										Bullet::create
										(
											_color? "B2" : "B3", *SharedObject::boss().lock(), _b.get_player_ref(), 
											_b.pos(), dir + _theta,
											[](Bullet& _b)
											{
												const auto count{_b.get_count()};
												const auto v{6 - 0.16f * count};

												_b.pos() += v * Point2D(cos(_b.angle()), sin(_b.angle()));
											}
										);
								}
							};
							create_bullets( pi<Real>() / 80, false);
							create_bullets(-pi<Real>() / 80, true );
							create_bullets( pi<Real>() / 50, true );
							create_bullets(-pi<Real>() / 50, false);
						}
					}
					catch (...) {}
					if (!return_flag) { erect_se_flag_of("../../data/sound/enemy_shot_01.wav"); return_flag = true; }
				}
			}
		}
	};

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
		static constexpr Real rc = 50.f;
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

	BulletStorm::BulletStorm
	(
		const Enemy& _master,
		const Player& _player,
		const Point2D& _initial_pos,
		Real _initial_angle,
		Behavior _behavior
	) noexcept
		:Bullet(_master, _player, _initial_pos, _initial_angle, ShapeElement::Null(),_behavior),
		registered_bullets()
	{}

	BulletStorm::~BulletStorm(void) noexcept = default;

	void BulletStorm::draw(void) const {/*do nothing*/}

	int BulletStorm::count_bullet_accessible(void) const noexcept
	{
		int result{ 0 };
		for (auto&& bullet : registered_bullets)
		{
			if (!bullet.expired()) { ++result; }
		}
		return result;
	}

	void BulletStorm::custom_updater(void)
	{
		const auto count{get_count()};

		if (count == 1)
		{
			registered_bullets.push_back
			(
				*find_vacant_object(SharedObject::bullets()) =
				Bullet::create
				(
					"Feather", get_master_ref(), get_player_ref(),
					pos(), 0,
					[&pos = pos()](Bullet& _b){_b.pos() = pos; }
				)
			);
			for (int i = 0; i != 10; ++i)
			{
				registered_bullets.push_back
				(
					*find_vacant_object(SharedObject::bullets()) =
					std::make_unique<B2_>
					(
						get_master_ref(), get_player_ref(),
						pos(), 0,
						[i = i, &center = pos()](Bullet& _b)
						{
							using P = Point2D;
							const auto count{ _b.get_count() };
							const auto t{ 0.05 * count };

							_b.pos() = center + 100 * sin(t / 3) * P(cos(t + i * two_pi<Real>() / 10), sin(t + i * two_pi<Real>() / 10));
						},
						[this](void) {return v(); }
					)
				);
				registered_bullets.push_back
				(
					*find_vacant_object(SharedObject::bullets()) =
					std::make_unique<B3_>
					(
						get_master_ref(), get_player_ref(),
						pos(), 0,
						[i = i, &center = pos()](Bullet& _b)
						{
							using P = Point2D;
							const auto count{ _b.get_count() };
							const auto t{ 0.05 * count };

							_b.pos() = center + 200 * sin(t / 3) * P(cos(t + i * two_pi<Real>() / 10), sin(t + i * two_pi<Real>() / 10));
						},
						[this](void) {return v(); }
					)
				);
			}
		}

		if (count > 1 && count_bullet_accessible() == 0)
		{
			set_flag_off();
		}
	}

	B2_::B2_
	(
		const Enemy& _master,
		const Player& _player,
		const Point2D& _initial_pos,
		Real _initial_angle,
		Behavior _behavior,
		decltype(v_base) v_base
	) noexcept
		:B234(_master, _player, _initial_pos, _initial_angle, _behavior),
		v_base(v_base)
	{}

	void B2_::draw(void) const
	{
		gp::DrawRotaGraphF
		(
			gp::level(12),
			pos().x(), pos().y(), 1.5, angle_of(v() - v_base()) + boost::math::constants::half_pi<Real>(),
			img_handle()[(get_count() / 4) % 3], true
		);
	}

	void B2_::preparation(void) noexcept
	{
		img_handle()[0] = add_get("../../data/img/b2_001.png");
		img_handle()[1] = add_get("../../data/img/b2_002.png");
		img_handle()[2] = add_get("../../data/img/b2_003.png");
	}

	B3_::B3_
	(
		const Enemy& _master,
		const Player& _player,
		const Point2D& _initial_pos,
		Real _initial_angle,
		Behavior _behavior,
		decltype(v_base) v_base
	) noexcept
		:B234(_master, _player, _initial_pos, _initial_angle, _behavior),
		v_base(v_base)
	{}

	void B3_::draw(void) const
	{
		gp::DrawRotaGraphF
		(
			gp::level(12),
			pos().x(), pos().y(), 1.5, angle_of(v() - v_base()) + boost::math::constants::half_pi<Real>(),
			img_handle()[(get_count() / 4) % 3], true
		);
	}

	void B3_::preparation(void) noexcept
	{
		img_handle()[0] = add_get("../../data/img/b3_001.png");
		img_handle()[1] = add_get("../../data/img/b3_002.png");
		img_handle()[2] = add_get("../../data/img/b3_003.png");
	}

	void AyaImple::action(void)
	{
		//counts
		const auto count = boss.get_count();

		pre_pos = boss.pos();

		//aliases
		auto& pos = boss.pos();
		const auto& player = boss.player_ref();
		auto& bullets = SharedObject::bullets();

		static constexpr int T0 = 1000;
		static constexpr int T1 = 1000;

		switch (wave)
		{
			case 0:
			{
				constexpr Real INIT_MAG = 5.f;
				constexpr Real FARTHEST_MAG = 0.25f;
				const P FARTHEST_POS(XMAX_2, 220.f);
				const P INITIAL_POS(XMAX_2, 50.f);
				constexpr int T = 60;

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
						if (wave_count[0] >= 100 && wave_count[0] < 900)
						{
							if (wave_count[0] % 10 == 0)
							{
								const Point2D SD_xy{player.pos() - Point2D(boss.pos().x(), boss.pos().y())};
								const Point2D v_xy{-(-10.f) / 500 * SD_xy}; //-(z component of velocity) / (initial z coordinate) * (SD vector)
								*find_vacant_object(bullets) = std::make_shared<RedEdges>
								(
									boss, player,
									boss.pos(), 0,
									[v_xy](Bullet& _b)
									{
										_b.pos() += v_xy;
									}
								);
								const auto theta{gp::safe_rand<Real>(static_cast<Real>(0), two_pi<Real>())};
								const Point2D SD_xy_{player.pos() - Point2D(boss.pos().x(), boss.pos().y()) + 400 * Point2D(cos(theta), sin(theta))};
								const Point2D v_xy_{-(-10.f) / 500 * SD_xy_}; //-(z component of velocity) / (initial z coordinate) * (SD vector)
								*find_vacant_object(bullets) = std::make_shared<RedEdges>
								(
									boss, player,
									boss.pos(), 0,
									[v_xy_](Bullet& _b)
									{
										_b.pos() += v_xy_;
									}
								);
							}
							if (wave_count[0] % 5 == 0)
							{
								static constexpr int N{25};
								for (int i = 0; i != N; ++i)
								{
									*find_vacant_object(bullets) = std::make_shared<ComingBullet>
									(
										boss, player,
										boss.pos(), i * two_pi<Real>() / N,
										curvature_adjusted_homing(player, 8, 0.04, 600)
									);
								}

								boss.erect_se_flag_of("../../data/sound/enemy_shot_01.wav");
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
					boss.immortal_flag = false;
					boss.area().get_shape() = ShapeElement::Circle(15);
					INIT_V;
					pos = INITIAL_POS;
				}
				else
				{
					static constexpr auto STAY_TIME_0   { 500 };
					static constexpr auto TRAVEL_TIME_0 { 30  };
					static constexpr auto STAY_TIME_1   { 500 };
					static constexpr auto TRAVEL_TIME_1 { 50  };
					static constexpr auto STAY_TIME_2   { 500 };
					static constexpr auto TRAVEL_TIME_2 { 30  };
					static constexpr auto STAY_TIME_3   { 1200 };

					if (boss.get_health() <= 1000)
					{
						wave = 4;
					}

					if (wave_count[2] < STAY_TIME_0)
					{
						const auto local_count = wave_count[2];
						if (local_count == 10)
						{
							static constexpr auto N{10};
							for (int i = 0; i != N; ++i)
							{
								*find_vacant_object(SharedObject::bullets()) =
									std::make_unique<ForcedCrowBullet>
									(
										boss, player, 
										pos, i * two_pi<Real>() / N,
										ForcedCrowBulletAction(100 + i * 20, player, boss.erect_se_flag_of)
									);
							}
							boss.erect_se_flag_of("../../data/sound/rocket_going.wav");
						}
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
						if (local_count == 10)
						{
							static constexpr auto N{10};
							for (int i = 0; i != N; ++i)
							{
								*find_vacant_object(SharedObject::bullets()) =
									std::make_unique<ForcedCrowBullet>
									(
										boss, player, 
										pos, i * two_pi<Real>() / N,
										ForcedCrowBulletAction(100 + i * 20, player, boss.erect_se_flag_of)
									);
							}
							boss.erect_se_flag_of("../../data/sound/rocket_going.wav");
						}
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
						if (local_count == 10)
						{
							static constexpr auto N{10};
							for (int i = 0; i != N; ++i)
							{
								*find_vacant_object(SharedObject::bullets()) =
									std::make_unique<ForcedCrowBullet>
									(
										boss, player, 
										pos, i * two_pi<Real>() / N,
										ForcedCrowBulletAction(100 + i * 20, player, boss.erect_se_flag_of)
									);
							}
							boss.erect_se_flag_of("../../data/sound/rocket_going.wav");
						}
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
						else 
						{
							if (local_count > 200 && local_count < STAY_TIME_3 - 100)
							{
								try
								{
									if (local_count % 4 == 0)
									{
										static constexpr auto N{ 10 };
										for (int i = 0; i != N; ++i)
										{
											*find_vacant_object(SharedObject::bullets()) =
												Bullet::create("B2", boss, player, pos, local_count * 0.02 + i * two_pi<Real>() / N, straight_course(6));
										}
										if (local_count > 800)
										{
											for (int i = 0; i != N; ++i)
											{
												*find_vacant_object(SharedObject::bullets()) =
													Bullet::create("B3", boss, player, pos, -local_count * 0.02 + i * two_pi<Real>() / N, straight_course(6));
											}
										}
										boss.erect_se_flag_of("../../data/sound/enemy_shot_03.wav");
									}
									if (local_count > 400 && local_count % 60 == 0)
									{
										static constexpr auto N{ 20 };
										const auto theta{gp::safe_rand(static_cast<Real>(0), two_pi<Real>())};
										for (int i = 0; i != N; ++i)
										{
											*find_vacant_object(SharedObject::bullets()) =
												Bullet::create("Feather", boss, player, pos, theta + i * two_pi<Real>() / N, straight_course(4));
										}
										boss.erect_se_flag_of("../../data/sound/enemy_shot_01.wav");
									}
								}
								catch (...) {}
							}
						}
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
					if (boss.get_health() <= 1000)
					{
						wave = 4;
					}

					if (wave_count[3] < 150 * 5)
					{
						if (wave_count[3] % 150 == 0)
						{
							pos = P(player.pos().x(), -100);
							*find_vacant_object(bullets) = std::make_shared<Lazer>(boss, player, boss.pos() + P(50, -20), -half_pi<Real>(), [&boss = this->boss](Bullet& _b) {_b.pos() = boss.pos() + P(50 * sin(boss.get_count() / 7.f), -20); }, 50, 60);
							*find_vacant_object(bullets) = std::make_shared<Lazer>(boss, player, boss.pos() + P(-50, -20), -half_pi<Real>(), [&boss = this->boss](Bullet& _b) {_b.pos() = boss.pos() + P(-50 * sin(boss.get_count() / 7.f), -20); }, 50, 60);
							boss.erect_se_flag_of("../../data/sound/launcher1.mp3");
						}
						else
						{
							pos += P(0, 10); 
							//if (wave_count[3] % 10 == 0 && pos.y() > -50 && pos.y() <= YMAX + 50)
							//{
							//	*find_vacant_object(bullets) = std::make_shared<Lazer> ( boss, player, pos + P(40, 0), 0, [](Bullet& _b) {}, 50, 60 );
							//	*find_vacant_object(bullets) = std::make_shared<Lazer> ( boss, player, pos + P(-40, 0), pi<Real>(), [](Bullet& _b) {}, 50, 60 );
							//}
						}
					}
					else if (wave_count[3] >= 150 * 5 && wave_count[3] < 150 * 5 + 100 * 5)
					{
						if ((wave_count[3] - 150 * 5) % 100 == 0)
						{
							pos = P(player.pos().x() , -100);
							*find_vacant_object(bullets) = std::make_shared<Lazer>(boss, player, boss.pos() + P(50, -20), -half_pi<Real>(), [&boss = this->boss](Bullet& _b) {_b.pos() = boss.pos() + P(50 * sin(boss.get_count() / 7.f), -20); }, 50, 60, 20);
							*find_vacant_object(bullets) = std::make_shared<Lazer>(boss, player, boss.pos() + P(-50, -20), -half_pi<Real>(), [&boss = this->boss](Bullet& _b) {_b.pos() = boss.pos() + P(-50 * sin(boss.get_count() / 7.f), -20); }, 50, 60, 20);
							boss.erect_se_flag_of("../../data/sound/launcher1.mp3");
						}
						else 
						{
							pos += P(0, 12.5); 
							//if (wave_count[3] % 9 == 0 && pos.y() > -50 && pos.y() <= YMAX + 50)
							//{
							//	*find_vacant_object(bullets) = std::make_shared<Lazer> ( boss, player, pos + P(40, 0), 0, [](Bullet& _b) {}, 50, 60 );
							//	*find_vacant_object(bullets) = std::make_shared<Lazer> ( boss, player, pos + P(-40, 0), pi<Real>(), [](Bullet& _b) {}, 50, 60 );
							//}
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
							if (pos.y() <= WH<Real>() + 10 && pos.y() >= WH<Real>() - 10)
							{
								static constexpr Real N = 30;
								for (int i = 0; i != N; ++i)
								{
									*find_vacant_object(bullets)
										= Bullet::create(i % 2 == 0? "B0" : "B1", boss, player, pos, i * two_pi<Real>() / N, straight_course(7));
									*find_vacant_object(bullets)
										= Bullet::create(i % 2 == 0? "B0" : "B1", boss, player, pos, i * two_pi<Real>() / N, straight_course(3));
								boss.erect_se_flag_of("../../data/sound/dive2.wav");
								}
							}
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
				static constexpr int TRAVEL_TIME = 240;
				static const P DST_POS(XMAX_2, 150);
				if (wave_count[4] == 0)
				{
					INIT_V;
					motion = create_linear_motion_of_uniform_acceleration_2D(pos, DST_POS, TRAVEL_TIME);
					v = motion.v0();
				}
				else
				{
					pos += v;
					if (wave_count[4] < TRAVEL_TIME)
					{
						v += motion.a();
					}
					else
					{
						++wave;
					}
				}
				++wave_count[4];
			}break;
			case 5:
			{
				static const P INITIAL_POS(XMAX_2, 150);
				static constexpr int STAY_TIME = 180;
				static constexpr int GET_OUT_TIME = 50;

				if (wave_count[5] == 0)
				{
					INIT_V;
					pos = INITIAL_POS;
				}
				else
				{
					if (wave_count[5] < STAY_TIME)
					{
						// do nothing.
					}
					else
					{
						const auto count = wave_count[5] - STAY_TIME;

						if (count == 0)
						{
							wave5_dst_pos = P(-100, gp::safe_rand<Real>(0, 180));
							boss.erect_se_flag_of("../../data/sound/rocket_going.wav");
						}
						else if(count > 0 && count < GET_OUT_TIME)
						{
							static constexpr Real A0 = 0.5f;
							const auto angle_to_dst = angle_of(wave5_dst_pos - INITIAL_POS);
							pos += v; v += A0 * P(cos(angle_to_dst), sin(angle_to_dst));
						}
						else
						{
							const auto count_ = count - GET_OUT_TIME;

							constexpr int TOTAL_LEVEL_NUM = 4;
							constexpr std::array<int, TOTAL_LEVEL_NUM> TRAVEL_TIME_LEVEL = {100, 80, 40, 20};
							constexpr std::array<int, TOTAL_LEVEL_NUM> REPEAT_TIMES_LEVEL = {6, 6, 6, 40};
							constexpr std::array<Real, TOTAL_LEVEL_NUM> VELOCITY_LEVEL = {15, 20, 25, 50};

							auto unit_v_to_dst = [&pos](const P& _dst) 
							{
								const auto angle_to_dst = angle_of(_dst - pos);
								return P(cos(angle_to_dst), sin(angle_to_dst));
							};

							if 
							(
								count_ <
									TRAVEL_TIME_LEVEL[0] * REPEAT_TIMES_LEVEL[0]
							) //Condition
							{
								if (count_ % (2 * TRAVEL_TIME_LEVEL[0]) == 0) // If the boss is on the left side...
								{
									pos = wave5_dst_pos; // To fix overrun position.
									// next position must be on the right side.
									wave5_dst_pos = P(XMAX + 100, gp::safe_rand<Real>(0, 180));
									v = VELOCITY_LEVEL[0] * unit_v_to_dst(wave5_dst_pos);
									boss.erect_se_flag_of("../../data/sound/rocket_going.wav");
								}
								else if (count_ % (2 * TRAVEL_TIME_LEVEL[0]) == TRAVEL_TIME_LEVEL[0])// If the boss is on the right side...
								{
									pos = wave5_dst_pos; // To fix overrun position.
									// next position must be on the left side.
									wave5_dst_pos = P(-100, gp::safe_rand<Real>(0, 180));
									v = VELOCITY_LEVEL[0] * unit_v_to_dst(wave5_dst_pos);
									boss.erect_se_flag_of("../../data/sound/rocket_going.wav");
								}
								else
								{
									pos += v;
								}
								if (count_ % 20 == 0)
								{
									if (is_within_window(boss.pos(), 50))
									{
										for (int i = 0; i != 4; ++i)
										{
											*find_vacant_object(bullets) = Bullet::create("B2", boss, boss.player_ref(), pos + diffuse_point_rect(P(-100, 0), P(100, 100)), gp::safe_rand<Real>(static_cast<Real>(1.5 - 0.2) * pi<Real>(), static_cast<Real>(1.5 + 0.2) * pi<Real>()), parabola(7, static_cast<Real>(0.2)));
										}
										boss.erect_se_flag_of("../../data/sound/enemy_shot_01.wav");
									}
								}
							}
							else if 
							(
								count_ >=
									TRAVEL_TIME_LEVEL[0] * REPEAT_TIMES_LEVEL[0]
								&& 
								count_ <
									TRAVEL_TIME_LEVEL[0] * REPEAT_TIMES_LEVEL[0] + 
									TRAVEL_TIME_LEVEL[1] * REPEAT_TIMES_LEVEL[1]
							) //Condition
							{
								if
								(
									count_ ==
										TRAVEL_TIME_LEVEL[0] * REPEAT_TIMES_LEVEL[0] 
									||
									count_ % (2 * TRAVEL_TIME_LEVEL[1]) == 0
								) // If the boss is on the left side...
								{
									pos = wave5_dst_pos; // To fix overrun position.
									// next position must be on the right side.
									wave5_dst_pos = P(XMAX + 100, gp::safe_rand<Real>(0, 180));
									v = VELOCITY_LEVEL[1] * unit_v_to_dst(wave5_dst_pos);
									boss.erect_se_flag_of("../../data/sound/rocket_going.wav");
								}
								else if (count_ % (2 * TRAVEL_TIME_LEVEL[1]) == TRAVEL_TIME_LEVEL[1])// If the boss is on the right side...
								{
									pos = wave5_dst_pos; // To fix overrun position.
									// next position must be on the left side.
									wave5_dst_pos = P(-100, gp::safe_rand<Real>(0, 180));
									v = VELOCITY_LEVEL[1] * unit_v_to_dst(wave5_dst_pos);
									boss.erect_se_flag_of("../../data/sound/rocket_going.wav");
								}
								else
								{
									pos += v;
								}
								if (count_ % 10 == 0)
								{
									if (is_within_window(pos, 50))
									{
										for (int i = 0; i != 4; ++i)
										{
											*find_vacant_object(bullets) = Bullet::create(i % 2 == 0 ? "B2" : "B3", boss, boss.player_ref(), pos + diffuse_point_rect(P(-100, 0), P(100, 100)), gp::safe_rand<Real>(static_cast<Real>(1.5 - 0.2) * pi<Real>(), static_cast<Real>(1.5 + 0.2) * pi<Real>()), parabola(7, static_cast<Real>(0.2)));
										}
										boss.erect_se_flag_of("../../data/sound/enemy_shot_01.wav");
									}
								}
							}
							else if 
							(
								count_ >=
									TRAVEL_TIME_LEVEL[0] * REPEAT_TIMES_LEVEL[0] + 
									TRAVEL_TIME_LEVEL[1] * REPEAT_TIMES_LEVEL[1] 
								&&
								count_ <
									TRAVEL_TIME_LEVEL[0] * REPEAT_TIMES_LEVEL[0] + 
									TRAVEL_TIME_LEVEL[1] * REPEAT_TIMES_LEVEL[1] + 
									TRAVEL_TIME_LEVEL[2] * REPEAT_TIMES_LEVEL[2]
							) //Condition
							{
								if 
								(
									count_ ==
										TRAVEL_TIME_LEVEL[0] * REPEAT_TIMES_LEVEL[0] +
										TRAVEL_TIME_LEVEL[1] * REPEAT_TIMES_LEVEL[1]
									||
									count_ % (2 * TRAVEL_TIME_LEVEL[2]) == 0
								) // If the boss is on the left side...
								{
									pos = wave5_dst_pos; // To fix overrun position.
									// next position must be on the right side.
									wave5_dst_pos = P(XMAX + 100, gp::safe_rand<Real>(0, 180));
									v = VELOCITY_LEVEL[2] * unit_v_to_dst(wave5_dst_pos);
									boss.erect_se_flag_of("../../data/sound/rocket_going.wav");
								}
								else if (count_ % (2 * TRAVEL_TIME_LEVEL[2]) == TRAVEL_TIME_LEVEL[2])// If the boss is on the right side...
								{
									pos = wave5_dst_pos; // To fix overrun position.
									// next position must be on the left side.
									wave5_dst_pos = P(-100, gp::safe_rand<Real>(0, 180));
									v = VELOCITY_LEVEL[2] * unit_v_to_dst(wave5_dst_pos);
									boss.erect_se_flag_of("../../data/sound/rocket_going.wav");
								}
								else
								{
									pos += v;
								}

								if (count_ % TRAVEL_TIME_LEVEL[2] == 0)
								{
									for (int i = 0; i != 5; ++i)
									{
										*find_vacant_object(bullets) = Bullet::create("Feather", boss, boss.player_ref(), diffuse_point_rect(Point2D(-100, 100), Point2D(WW<Real>() + 100, 120)), gp::safe_rand<Real>(static_cast<Real>(1.5 - 0.2) * pi<Real>(), static_cast<Real>(1.5 + 0.2) * pi<Real>()), parabola(7, static_cast<Real>(0.2)));
									}
									boss.erect_se_flag_of("../../data/sound/enemy_shot_01.wav");
								}
							}
							else if 
							(
								count_ >=
									TRAVEL_TIME_LEVEL[0] * REPEAT_TIMES_LEVEL[0] + 
									TRAVEL_TIME_LEVEL[1] * REPEAT_TIMES_LEVEL[1] +
									TRAVEL_TIME_LEVEL[2] * REPEAT_TIMES_LEVEL[2]
								&&
								count_ <
									TRAVEL_TIME_LEVEL[0] * REPEAT_TIMES_LEVEL[0] + 
									TRAVEL_TIME_LEVEL[1] * REPEAT_TIMES_LEVEL[1] + 
									TRAVEL_TIME_LEVEL[2] * REPEAT_TIMES_LEVEL[2] + 
									TRAVEL_TIME_LEVEL[3] * REPEAT_TIMES_LEVEL[3]
							) //Condition
							{
								if 
								(
									count_ == 
										TRAVEL_TIME_LEVEL[0] * REPEAT_TIMES_LEVEL[0] + 
										TRAVEL_TIME_LEVEL[1] * REPEAT_TIMES_LEVEL[1] +
										TRAVEL_TIME_LEVEL[2] * REPEAT_TIMES_LEVEL[2] 
									||
									count_ % (2 * TRAVEL_TIME_LEVEL[3]) == 0
								) // If the boss is on the left side...
								{
									const auto pre_pos{wave5_dst_pos};
									pos = wave5_dst_pos; // To fix overrun position.
									// next position must be on the right side.
									wave5_dst_pos = P(XMAX + 100, gp::safe_rand<Real>(0, 230));
									v = VELOCITY_LEVEL[3] * unit_v_to_dst(wave5_dst_pos);
									boss.erect_se_flag_of("../../data/sound/rocket_going.wav");
									*find_vacant_object(bullets) = std::make_shared<Lazer>
										(
											boss, player, 
											boss.pos(), angle_of(pre_pos - wave5_dst_pos),
											[&boss = this->boss](Bullet& _b) {if(_b.get_count() < 20){_b.pos() = boss.pos(); }},
											50, 40, 50
										);
								}
								else if (count_ % (2 * TRAVEL_TIME_LEVEL[3]) == TRAVEL_TIME_LEVEL[3])// If the boss is on the right side...
								{
									const auto pre_pos{wave5_dst_pos};
									pos = wave5_dst_pos; // To fix overrun position.
									// next position must be on the left side.
									wave5_dst_pos = P(-100, gp::safe_rand<Real>(0, 230));
									v = VELOCITY_LEVEL[3] * unit_v_to_dst(wave5_dst_pos);
									boss.erect_se_flag_of("../../data/sound/rocket_going.wav");
									*find_vacant_object(bullets) = std::make_shared<Lazer>
										(
											boss, player, 
											boss.pos(), angle_of(pre_pos - wave5_dst_pos),
											[&boss = this->boss](Bullet& _b) {if(_b.get_count() < 20){_b.pos() = boss.pos(); }},
											50, 40, 50
										);
								}
								else
								{
									pos += v;
								}

								if (count_ % TRAVEL_TIME_LEVEL[3] == 0)
								{
									for (int i = 0; i != 2; ++i)
									{
										*find_vacant_object(bullets) = //Bullet::create("Feather", boss, boss.player_ref(), diffuse_point_rect(P(-100, 100), P(WW<Real>() + 100, 120)), gp::safe_rand<Real>(static_cast<Real>(1.5 - 0.2) * pi<Real>(), static_cast<Real>(1.5 + 0.2) * pi<Real>()), parabola(7, static_cast<Real>(0.2)));
											std::make_unique<BulletStorm>(boss, boss.player_ref(), diffuse_point_rect(P(-100, 100), P(WW<Real>() + 100, 200)), gp::safe_rand<Real>(static_cast<Real>(1.5 - 0.2) * pi<Real>(), static_cast<Real>(1.5 + 0.2) * pi<Real>()), parabola(7, static_cast<Real>(0.1)));
									}
								}
								if (count_ % (TRAVEL_TIME_LEVEL[3] / 2) == 0) {boss.erect_se_flag_of("../../data/sound/enemy_shot_01.wav");}
							}
						}
					}
				}
				++wave_count[5];
			}break;
		}

	}

	const Point2D& AyaImple::velocity(void) const noexcept { return boss.pos() - pre_pos; }

	BackGroundControl::OptionalSignal AyaImple::notify(void) const noexcept
	{
		return boost::none;
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
				else if (c_40 >= 10 && c_40 < 20) { i = 1; }
				else if (c_40 >= 20 && c_40 < 30) { i = 2; }
				else { i = 1; }

				gp::DrawRotaGraphF(gp::level(11), pos.x(), pos.y(), mag, 0.0, wings_img()[i], true);
				gp::DrawRotaGraphF(gp::level(11), pos.x(), pos.y(), mag, 0.0, skirt_img()[i], true);
				gp::DrawRotaGraphF(gp::level(11), pos.x(), pos.y(), mag, 0.0, standing_img(), true);
				gp::DrawRotaGraphF(gp::level(11), pos.x(), pos.y(), mag, 0.0, bombom_img()[i], true);
			}
		}
		else
		{
			if (norm(velocity()) < 3.0)
			{
				const int c = wave_count[wave];
				const int c_40 = c % 40;
				int i = 0;
				if (c_40 < 10) { i = 0; }
				else if (c_40 >= 10 && c_40 < 20) { i = 1; }
				else if (c_40 >= 20 && c_40 < 30) { i = 2; }
				else { i = 1; }

				gp::DrawRotaGraphF(gp::level(11), pos.x(), pos.y(), mag, 0.0, wings_img()[i], true);
				gp::DrawRotaGraphF(gp::level(11), pos.x(), pos.y(), mag, 0.0, skirt_img()[i], true);
				gp::DrawRotaGraphF(gp::level(11), pos.x(), pos.y(), mag, 0.0, standing_img(), true);
				gp::DrawRotaGraphF(gp::level(11), pos.x(), pos.y(), mag, 0.0, bombom_img()[i], true);
			}
			else
			{
				gp::DrawRotaGraphF(gp::level(11), pos.x(), pos.y(), mag, half_pi<Real>() + angle_of(velocity()), twist_img()[(wave_count[wave] / 4) % 8], true);
			}
		}
	}

	void AyaImple::preperation(void)
	{
		static auto add_and_get
		{
			[](auto&& _path)
			{
				ImagePool::add(_path);
				return ImagePool::get(std::forward<decltype(_path)>(_path));
			}
		};

		approach_img()[0] = add_and_get("../../data/img/aya2/00.png"          );
		approach_img()[1] = add_and_get("../../data/img/aya2/01.png"          );
		approach_img()[2] = add_and_get("../../data/img/aya2/02.png"          );
		approach_img()[3] = add_and_get("../../data/img/aya2/03.png"          );

		standing_img()    = add_and_get("../../data/img/aya2/04_body.png"     );
		wings_img()[0]    = add_and_get("../../data/img/aya2/04_wing00.png"   );
		wings_img()[1]    = add_and_get("../../data/img/aya2/04_wing01.png"   );
		wings_img()[2]    = add_and_get("../../data/img/aya2/04_wing02.png"   );
		bombom_img()[0]   = add_and_get("../../data/img/aya2/04_bombom00.png" );
		bombom_img()[1]   = add_and_get("../../data/img/aya2/04_bombom01.png" );
		bombom_img()[2]   = add_and_get("../../data/img/aya2/04_bombom02.png" );
		skirt_img()[0]    = add_and_get("../../data/img/aya2/04_skirt00.png"  );
		skirt_img()[1]    = add_and_get("../../data/img/aya2/04_skirt01.png"  );
		skirt_img()[2]    = add_and_get("../../data/img/aya2/04_skirt02.png"  );

		twist_img()[0]    = add_and_get("../../data/img/aya2/04.png"          );
		twist_img()[1]    = add_and_get("../../data/img/aya2/03.png"          );
		twist_img()[2]    = add_and_get("../../data/img/aya2/02.png"          );
		twist_img()[3]    = add_and_get("../../data/img/aya2/01.png"          );
		twist_img()[4]    = add_and_get("../../data/img/aya2/05.png"          );
		twist_img()[5]    = add_and_get("../../data/img/aya2/01_mirror.png"   );
		twist_img()[6]    = add_and_get("../../data/img/aya2/02_mirror.png"   );
		twist_img()[7]    = add_and_get("../../data/img/aya2/03_mirror.png"   );

		ForcedCrowBullet::preparation();
		ComingBullet::preparation();
		RedEdges::preparation();
		B2_::preparation();
		B3_::preparation();
	}
}
