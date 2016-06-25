//enemy_act_pattern.cpp

#include "enemy_act_pattern.h"
#include "bullet.h"
#include "find_vacant_object.h"
#include <boost/math/constants/constants.hpp>
#include "bullet_behavior.h"
#include "mathematics.h"
#include "se_manager.h"
#include "ranged_uniform_random.h"
#include "player.h"
#include "motion.h"
#include "environmental_constants.h"
#include "shared_object.h"
#include "debug.h"

namespace MyGameProject
{
	using namespace boost::math::constants;

	MobEnemy::ActPattern default_pattern = [](MobEnemy& _enemy)->void {};

	MobEnemy::ActPattern pattern0 = [](MobEnemy& _enemy)->void
	{
		const auto count = _enemy.get_count();

		if (count == 0) { _enemy.velocity() = Point2D(9, -3); }
		_enemy.velocity().x(_enemy.velocity().x() - static_cast<Real>(0.1));
		if (count > 70) { _enemy.velocity().y(_enemy.velocity().y() + static_cast<Real>(0.05)); }

		if (count % 8 == 0)
		{
			*find_vacant_object(SharedObject::bullets()) = Bullet::create("Cutter", _enemy, _enemy.player_ref(), _enemy.pos(), pi<Real>() / 2, straight_course(9));
			_enemy.get_se_manager_ref().erect_play_flag_of("../../data/sound/enemy_shot.wav");
		}

		_enemy.pos() += _enemy.velocity();
	};

	MobEnemy::ActPattern pattern1 = [](MobEnemy& _enemy)->void
	{
		const auto count = _enemy.get_count();

		if (count == 0) { _enemy.velocity() = Point2D(-9, -3); }
		_enemy.velocity().x(_enemy.velocity().x() + static_cast<Real>(0.1));
		if (count > 70) { _enemy.velocity().y(_enemy.velocity().y() + static_cast<Real>(0.05)); }

		if (count % 8 == 0)
		{
			*find_vacant_object(SharedObject::bullets()) = Bullet::create("Cutter", _enemy, _enemy.player_ref(), _enemy.pos(), pi<Real>() / 2, straight_course(9));
			_enemy.get_se_manager_ref().erect_play_flag_of("../../data/sound/enemy_shot.wav");
		}
		_enemy.pos() += _enemy.velocity();
	};

	MobEnemy::ActPattern pattern2 = [](MobEnemy& _enemy)->void
	{
		const auto count = _enemy.get_count();

		if (count == 0) { _enemy.velocity() = Point2D(0, -5); }

		if (count % 4 == 0)
		{
			*find_vacant_object(SharedObject::bullets()) = Bullet::create("Cutter", _enemy, _enemy.player_ref(), _enemy.pos(), pi<Real>() / 2, straight_course(9));
			_enemy.get_se_manager_ref().erect_play_flag_of("../../data/sound/enemy_shot.wav");
		}

		_enemy.pos() += _enemy.velocity();
	};

	MobEnemy::ActPattern pattern3 = [](MobEnemy& _enemy)->void
	{
		const auto count = _enemy.get_count();

		if (count == 0) { _enemy.velocity() = Point2D(0, 2); }

		if (count % 60 == 0)
		{
			gp::RangedUniformRandom generator(-pi<Real>(), pi<Real>());
			Real r = static_cast<Real>(generator.rand());
			static constexpr int N = 10;
			for (int i = 0; i < N; ++i)
			{
				*find_vacant_object(SharedObject::bullets()) = Bullet::create("Typical", _enemy, _enemy.player_ref(), _enemy.pos(), r + i * (2 * pi<Real>() / N), straight_course(4));
			}
			_enemy.get_se_manager_ref().erect_play_flag_of("../../data/sound/enemy_shot.wav");
		}

		_enemy.pos() += _enemy.velocity();
	};

	MobEnemy::ActPattern pattern4 = [](MobEnemy& _enemy)->void
	{
		const auto count = _enemy.get_count();

		//Motion control.
		if (count == 0) { _enemy.velocity() = Point2D(0, -2); }
		else if (count >= 180 && count <= 200)
		{
			_enemy.velocity() = Point2D(0, count / 10.f - 20);
		}

		//Bullet control.
		if (count > 200 && count <= 600)
		{
			if (count % 60 == 0)
			{
				*find_vacant_object(SharedObject::bullets()) =
					Bullet::create
					(
						"Typical",
						_enemy,
						_enemy.player_ref(),
						_enemy.pos(),
						angle_of(_enemy.player_ref().pos() - _enemy.pos()),
						straight_course(10)
						);
			}
		}

		_enemy.pos() += _enemy.velocity();
	};

	MobEnemy::ActPattern pattern5 = [](MobEnemy& _enemy)->void
	{
	};

	MobEnemy::ActPattern pattern6 = [](MobEnemy& _enemy)->void
	{
	};

		struct Pattern9Vars { LinearMotionOfUniformAcceleration2D<Point2D, int> motion; };
		struct Pattern10Vars { LinearMotionOfUniformAcceleration2D<Point2D, int> motion; };
		struct Pattern11Vars {};
		struct Pattern12Vars {};
		struct Pattern13Vars {};
		struct Pattern14Vars {};
		struct Pattern15Vars {};
		struct Pattern16Vars {};
		struct Pattern17Vars {};
		struct Pattern18Vars {};
		struct Pattern19Vars {};
		struct Pattern20Vars {};
		struct Pattern21Vars {};

	void Pattern9::operator()(MobEnemy& _enemy)
	{
		const auto count = _enemy.get_count();
		static constexpr int T = 30;
		static constexpr int WAIT_TIME = 1000;

		if (count == 0)
		{
			vars = std::make_shared<Pattern9Vars>();
			static const Point2D destination(WW_2<Real>(), WH_2<Real>());
			vars->motion = create_linear_motion_of_uniform_acceleration_2D(_enemy.pos(), destination, T);
			_enemy.velocity() = vars->motion.v0();
		}
		else
		{
			if (count < T) { _enemy.velocity() += vars->motion.a(); }
			else if (count >= T && count < T + WAIT_TIME) { _enemy.velocity().x(0), _enemy.velocity().y(0); }
			else if (count >= T + WAIT_TIME) { _enemy.velocity() += Point2D(-0.2, 0); }
		}
		_enemy.pos() += _enemy.velocity();
	}

	void Pattern10::operator()(MobEnemy& _enemy)
	{
		const auto count = _enemy.get_count();
		static constexpr int T = 30;
		static constexpr int WAIT_TIME = 1000;

		if (count == 0)
		{
			vars = std::make_shared<Pattern10Vars>();
			static const Point2D destination(WW_2<Real>(), WH_2<Real>());
			vars->motion = create_linear_motion_of_uniform_acceleration_2D(_enemy.pos(), destination, T);
			_enemy.velocity() = vars->motion.v0();
		}
		else
		{
			if (count < T) { _enemy.velocity() += vars->motion.a(); }
			else if (count >= T && count < T + WAIT_TIME) { _enemy.velocity().x(0), _enemy.velocity().y(0); }
			else if (count >= T + WAIT_TIME) { _enemy.velocity() += Point2D(0.2, 0); }
		}
		_enemy.pos() += _enemy.velocity();
	}

	void Pattern11::operator()(MobEnemy& _enemy) {}
	void Pattern12::operator()(MobEnemy& _enemy) {}
	void Pattern13::operator()(MobEnemy& _enemy) {}
	void Pattern14::operator()(MobEnemy& _enemy) {}
	void Pattern15::operator()(MobEnemy& _enemy) {}
	void Pattern16::operator()(MobEnemy& _enemy) {}
	void Pattern17::operator()(MobEnemy& _enemy) {}
	void Pattern18::operator()(MobEnemy& _enemy) {}
	void Pattern19::operator()(MobEnemy& _enemy) {}
	void Pattern20::operator()(MobEnemy& _enemy) {}
	void Pattern21::operator()(MobEnemy& _enemy) {}

	Pattern9::Pattern9(void) {}
	Pattern10::Pattern10(void) {}
	Pattern11::Pattern11(void) {}
	Pattern12::Pattern12(void) {}
	Pattern13::Pattern13(void) {}
	Pattern14::Pattern14(void) {}
	Pattern15::Pattern15(void) {}
	Pattern16::Pattern16(void) {}
	Pattern17::Pattern17(void) {}
	Pattern18::Pattern18(void) {}
	Pattern19::Pattern19(void) {}
	Pattern20::Pattern20(void) {}
	Pattern21::Pattern21(void) {}

	Pattern9 pattern9;
	Pattern10 pattern10;
	Pattern11 pattern11;
	Pattern12 pattern12;
	Pattern13 pattern13;
	Pattern14 pattern14;
	Pattern15 pattern15;
	Pattern16 pattern16;
	Pattern17 pattern17;
	Pattern18 pattern18;
	Pattern19 pattern19;
	Pattern20 pattern20;
	Pattern21 pattern21;

#include <type_traits>

	//Minimum _n is not 1, but 0.
	template<typename Integer, std::enable_if_t<std::is_integral<std::decay_t<Integer>>::value>* = nullptr>
	inline decltype(auto) get_n_th_arg(const std::vector<Point2D>& _args, Integer&& _n)
	{
		using namespace std::literals;
		try
		{
			if (_n % 2 == 0) { return _args.at(_n / 2).x(); }
			else { return _args.at(_n / 2).y(); }
		}
		catch (std::out_of_range) 
		{
			throw std::out_of_range("The number of arguments is less than "s + boost::lexical_cast<std::string>(_n + 1) + '.'); 
		}
	}

	template<typename RealType, std::enable_if_t<std::is_arithmetic<std::decay_t<RealType>>::value>* = nullptr>
	inline auto unit_vec(RealType&& _angle) noexcept
	{
		return Point2D(static_cast<Real>(std::cos(std::forward<RealType>(_angle))), static_cast<Real>(std::sin(std::forward<RealType>(_angle))));
	}

	template<typename... Ts>
	inline void register_bullet(Ts&&... _args) noexcept
	{
		try { *find_vacant_object(SharedObject::bullets()) = Bullet::create(std::forward<Ts>(_args)...); }
		catch (NoSpace) {}
	}

#ifndef OVERRIDE_PARENTHESIS
#define OVERRIDE_PARENTHESIS virtual void operator()(MobEnemy& _enemy) override final
#endif

#ifndef ALIASES
	//Alias player, count, and the enemy's position.
#define ALIASES decltype(auto) player(_enemy.player_ref()); const auto count{_enemy.get_count()}; decltype(auto) pos(_enemy.pos());
#endif

	MobEnemy::ActPattern pattern100(const std::vector<Point2D>& _args)
	{
		Real v0;
		if (_args.size() == 1) { v0 = _args[0].x(); }
		else { throw std::runtime_error("No argument is not allowed."); }

		return [&, v0](MobEnemy& _enemy)->void
		{
			const auto count = _enemy.get_count();

			//Motion control.
			if (count == 0) { _enemy.velocity() = Point2D(0, v0); }
			else if (count >= 180 && count <= 200) { _enemy.velocity() = Point2D(0, count / 10.f - 20); }

			//Bullet control.
			if (count > 200 && count <= 600)
			{
				if (count % 60 == 0)
				{
					*find_vacant_object(SharedObject::bullets()) =
						Bullet::create
						(
							"Typical",
							_enemy,
							_enemy.player_ref(),
							_enemy.pos(),
							angle_of(_enemy.player_ref().pos() - _enemy.pos()),
							straight_course(10)
							);
				}
			}

			_enemy.pos() += _enemy.velocity();
		};
	}
	MobEnemy::ActPattern pattern101(const std::vector<Point2D>& _args)
	{
		Real v0;
		Real a;
		if (_args.size() == 1) { v0 = _args[0].x(); a = _args[0].y(); }
		else { throw std::runtime_error("this pattarn must take arguments."); }

		return [&, v0, a](MobEnemy& _enemy)->void
		{
			const auto count = _enemy.get_count();

			//Motion control.
			if (count == 0) { _enemy.velocity() = Point2D(0, v0); }
			else { _enemy.velocity() -= Point2D(0, a); }

			_enemy.pos() += _enemy.velocity();
		};
	}
	MobEnemy::ActPattern pattern102(const std::vector<Point2D>& _args)
	{
		Point2D v;
		if (_args.size() == 1) { v.x(_args[0].x()); v.y(_args[0].y()); }
		else { throw std::runtime_error("pattarn 102 must take arguments."); }

		return [&, v](MobEnemy& _enemy)->void
		{
			const auto count = _enemy.get_count();

			//Motion control.
			static constexpr int WAIT_TIME = 300;
			if (count == 0) { _enemy.velocity() = Point2D(0, 0); }
			else if (count >= WAIT_TIME) { _enemy.velocity() = v; }

			_enemy.pos() += _enemy.velocity();
		};
	}

	struct Pattern103 : public Pattern
	{
		Real a, PD, v0, theta;
		const Point2D dst;
		static constexpr int WAIT_TIME = 250;
		static constexpr Real T = 50.f;
		Pattern103(const Point2D& _destination) :dst(_destination) {}
		virtual void operator()(MobEnemy& _enemy) override final
		{
			const auto count = _enemy.get_count();

			if (count == 0)
			{
				PD = norm(dst - _enemy.pos());
				a = -2 * PD / (T * T);
				v0 = -a * T;

				theta = angle_of(dst - _enemy.pos());
				_enemy.velocity() = v0 * Point2D(cos(theta), sin(theta));
			}
			else
			{
				if (count <= T)
				{
					_enemy.velocity() = (v0 + a * count) * Point2D(cos(theta), sin(theta));
					if (count == T) { _enemy.velocity() = Point2D(0, 0); }
				}
				else if (count > T && count <= T + WAIT_TIME) { _enemy.velocity() = Point2D(0, -0.5); }
				else if (count >= T + WAIT_TIME) { _enemy.velocity() += Point2D(0, 0.2f); }

			}

			_enemy.pos() += _enemy.velocity();
		}
	};

	MobEnemy::ActPattern pattern103(const std::vector<Point2D>& _args)
	{
		if (_args.size() == 1) { return Pattern103(Point2D(_args[0].x(), _args[0].y())); }
		else { throw std::runtime_error("pattarn 103 must take an arguments."); }
	}
	MobEnemy::ActPattern pattern104(const std::vector<Point2D>& _args)
	{
		using namespace std;

		Real a = 0.f, theta = 0.f;
		if (_args.size() == 1) { a = _args[0].x(); theta = _args[0].y(); }
		else { throw std::runtime_error("pattarn 104 must take an arguments."); }

		return [=](MobEnemy& _enemy)
		{
			const auto count = _enemy.get_count();
			const auto pos{ _enemy.pos() };
			decltype(auto) player = _enemy.player_ref();

			static constexpr int WAIT_TIME = 220;

			if (count == 0) { _enemy.velocity() = Point2D(0, 0); }
			else if (count == WAIT_TIME)
			{
				_enemy.get_se_manager_ref().erect_play_flag_of("../../data/sound/enemy_shot_01.wav");
				for (int i = 0; i != 5; ++i)
				{
					*find_vacant_object(SharedObject::bullets()) = Bullet::create("B1", _enemy, player, pos, angle_of(player.pos() - pos), straight_course(7 + 2 * i));
				}
			}
			else if (count > WAIT_TIME) { _enemy.velocity() += a * Point2D(cos(theta), sin(theta)); }

			_enemy.pos() += _enemy.velocity();
		};
	}
	//Straight forward
	MobEnemy::ActPattern pattern105(const std::vector<Point2D>& _args)
	{
		using namespace std;

		Real v = 0.f, theta = 0.f;
		if (_args.size() == 1) { v = _args[0].x(); theta = _args[0].y(); }
		else { throw std::runtime_error("pattarn 105 must take an arguments."); }

		return [=](MobEnemy& _enemy)
		{
			const auto count = _enemy.get_count();

			if (count == 0) { _enemy.velocity() = v * Point2D(cos(theta), sin(theta)); }

			_enemy.pos() += _enemy.velocity();
		};
	}

	struct Pattern106 : Pattern
	{
		enum class AttackType { RADIATION = 0, AIMING } attack_type;
		LinearMotionOfUniformAcceleration2D<Point2D, int> motion;
		static constexpr auto T0{ 180 };
		static constexpr auto WAIT_TIME{ 40 };
		Point2D v;
		Point2D dst, org;
		Real dir{ 0 };
		int count{ 0 };
		Pattern106(const Point2D& _dst, AttackType _attack_type)
			:dst(_dst), attack_type(_attack_type), v() {}
		virtual void operator()(MobEnemy& _enemy) override final
		{
			if (count == 0)
			{
				org = _enemy.pos();
				dir = angle_of(dst - org);
				motion = create_linear_motion_of_uniform_acceleration_2D(org, dst, T0);
				v = motion.v0();
			}
			else if (count < T0)
			{
				v += motion.a();
				_enemy.pos() += v;
			}
			else if (count >= T0 && count < T0 + WAIT_TIME)
			{
				switch (attack_type)
				{
				case AttackType::AIMING:
				{
					if (count % 5 == 0)
					{
						*find_vacant_object(SharedObject::bullets()) =
							Bullet::create("B4", _enemy, _enemy.player_ref(), _enemy.pos(), angle_of(_enemy.player_ref().pos() - _enemy.pos()), straight_course(5));
						*find_vacant_object(SharedObject::bullets()) =
							Bullet::create("B4", _enemy, _enemy.player_ref(), _enemy.pos(), pi<Real>() / 9 + angle_of(_enemy.player_ref().pos() - _enemy.pos()), straight_course(5));
						*find_vacant_object(SharedObject::bullets()) =
							Bullet::create("B4", _enemy, _enemy.player_ref(), _enemy.pos(), -pi<Real>() / 9 + angle_of(_enemy.player_ref().pos() - _enemy.pos()), straight_course(5));
					}
				} break;
				case AttackType::RADIATION:
				{
					if (count % 20 == 0)
					{
						static constexpr auto N{ 20 };
						const auto theta{ gp::safe_rand<Real>(0, two_pi<Real>()) };
						for (int i = 0; i != N; ++i)
						{
							*find_vacant_object(SharedObject::bullets()) =
								Bullet::create("B4", _enemy, _enemy.player_ref(), _enemy.pos(), theta + i * two_pi<Real>() / N, straight_course(5));
						}
					}
				} break;
				}
			}
			else
			{
				v = (count - T0 - WAIT_TIME) / 3 * Point2D(cos(pi<Real>() + dir), sin(pi<Real>() + dir));
				_enemy.pos() += v;
			}
			++count;
		}
	};

	//After going to destination pos, return to original pos.
	//Shot some B4 when it starts returning.
	MobEnemy::ActPattern pattern106(const std::vector<Point2D>& _args)
	{
		Pattern106::AttackType attack_type;
		switch (static_cast<int>(_args.at(1).x()))
		{
		case 0: attack_type = Pattern106::AttackType::RADIATION; break;
		case 1: attack_type = Pattern106::AttackType::AIMING; break;
		default: throw std::runtime_error("Undefine attack type.");
		}
		if (_args.size() == 2)
		{
			return Pattern106
				(
					Point2D(_args.at(0).x(), _args.at(0).y()),
					std::move(attack_type)
					);
		}
		else
		{
			throw std::runtime_error("pattern106 needs 3 arguments");
		}
	}

	struct Pattern107 : Pattern
	{
		Real speed, dir{0};
		int duration;
		Pattern107(Real _speed, int _chasing_time)
			:speed(_speed), duration(_chasing_time)
		{}
		virtual void operator()(MobEnemy& _enemy) override final
		{
			const auto count{_enemy.get_count()};
			decltype(auto) player(_enemy.player_ref());
			if (count < duration)
			{
				auto theta{angle_of(player.pos() - _enemy.pos())};
				if (sin(dir - theta) <= 0) { dir += 0.05f; }
				else { dir -= 0.05f; }

				if (abs(dir - theta) < 0.1f) { dir = theta; }

				if (count > 100)
				{
					if (count % 5 == 0)
					{
						*find_vacant_object(SharedObject::bullets()) =
							Bullet::create("B3", _enemy, player, _enemy.pos() + 15 * Point2D(cos(dir), sin(dir)), dir, straight_course(10));
					}
					if (count % 7 == 0)
					{
						_enemy.get_se_manager_ref().erect_play_flag_of("../../data/sound/enemy_shot_02.wav");
					}
				}
			}
			_enemy.pos() += speed * Point2D(cos(dir), sin(dir));
		}
	};

	//Chase player for specified frame and go.
	MobEnemy::ActPattern pattern107(const std::vector<Point2D>& _args)
	{
		if (_args.size() == 1)
		{
			return Pattern107(_args.at(0).x(), static_cast<int>(_args.at(0).y()));
		}
		else
		{
			throw std::runtime_error("pattern107 needs 2 arguments");
		}
	}

	struct Pattern108 : Pattern
	{
		enum class AttackType {AIMING_SPLASH = 0, AIMING_LINEAR_1_WAY, AIMING_3_WAY, AIMING_BULLETS_SHEET};
		const AttackType attack_type;
		static constexpr auto TIME_TO_FIRST_DST{90};
		const Point2D first_dst, second_dst;
		const int wait_time;
		LinearMotionOfUniformAcceleration2D<Point2D, int> motion;
		Point2D v;

		Pattern108(AttackType _attack_type, const Point2D& _first_dst, int _wait_time, const Point2D& _second_dst) noexcept
			:attack_type(_attack_type),
			first_dst(_first_dst),
			wait_time(_wait_time),
			second_dst(_second_dst)
		{ }

		OVERRIDE_PARENTHESIS
		{
			ALIASES;

			if (count == 0)
			{
				motion = create_linear_motion_of_uniform_acceleration_2D(pos, first_dst, TIME_TO_FIRST_DST);
				v = motion.v0();
			}
			else if (count < TIME_TO_FIRST_DST)
			{
				v += motion.a();
			}
			else if (count >= TIME_TO_FIRST_DST && count < TIME_TO_FIRST_DST + wait_time)
			{
				v = Point2D(0, 0);
				switch (attack_type)
				{
					case AttackType::AIMING_SPLASH:
					{
						if (count % 50 == 0)
						{
							_enemy.get_se_manager_ref().erect_play_flag_of("../../data/sound/enemy_shot_01.wav");
							for (int i = 0; i != 70; ++i)
							{
								*find_vacant_object(SharedObject::bullets()) =
								Bullet::create
								(
									i % 2 == 0 ? "B2" : "B3",
									_enemy, player,
									pos, angle_of(player.pos() - pos) + gp::safe_rand(-0.3, 0.3),
									straight_course(gp::safe_rand<Real>(5, 15))
								);
							}
						}
					}break;
					case AttackType::AIMING_LINEAR_1_WAY:
					{
						if (count % 100 == 0)
						{
							_enemy.get_se_manager_ref().erect_play_flag_of("../../data/sound/enemy_shot_01.wav");
							for (int i = 0; i != 8; ++i)
							{
								*find_vacant_object(SharedObject::bullets()) =
								Bullet::create
								(
									"B0",
									_enemy, player,
									pos, angle_of(player.pos() - pos),
									straight_course(5 + i * static_cast<Real>(1.5))
								);
							}
						}
					}break;
					case AttackType::AIMING_3_WAY:
					{
						if (count % 10 == 0)
						{
							*find_vacant_object(SharedObject::bullets()) =
							Bullet::create
							(
								"B3",
								_enemy, player,
								pos, angle_of(player.pos() - pos),
								straight_course(8)
							);
							*find_vacant_object(SharedObject::bullets()) =
							Bullet::create
							(
								"B3",
								_enemy, player,
								pos, angle_of(player.pos() - pos) + static_cast<Real>(0.1),
								straight_course(8)
							);
							*find_vacant_object(SharedObject::bullets()) =
							Bullet::create
							(
								"B3",
								_enemy, player,
								pos, angle_of(player.pos() - pos) - static_cast<Real>(0.1),
								straight_course(8)
							);
						}
					}break;
					case AttackType::AIMING_BULLETS_SHEET:
					{
						if (count < TIME_TO_FIRST_DST + wait_time / 2)
						{
							if (count % 4 == 0)
							{
								static constexpr std::array<Real, 11> Ds{ -65, -52, -39, -26, -13, 0, 13, 26, 39, 52, 65 };
								for (const auto D : Ds)
								{
									const auto dir{ angle_of(player.pos() - pos) };
									register_bullet("B2", _enemy, player, pos + D * unit_vec(dir + half_pi<Real>()), dir, straight_course(10));
								}
							}
							if (count % 4 == 0)
							{
								_enemy.get_se_manager_ref().erect_play_flag_of("../../data/sound/enemy_shot_02.wav");
							}
						}
					}break;
				}
			}
			else if (count >= TIME_TO_FIRST_DST + wait_time)
			{
				const auto dir{angle_of(second_dst - first_dst)};
				v = static_cast<Real>(0.06) * (count - TIME_TO_FIRST_DST - wait_time) * Point2D(cos(dir), sin(dir));
			}

			pos += v;
		}
	};

	//Go to first destination, stay for a while and release some bullets, and go to second destination.
	MobEnemy::ActPattern pattern108(const std::vector<Point2D>& _args) 
	{
		if (_args.size() == 3)
		{
			Pattern108::AttackType attack_type;
			switch (static_cast<int>(get_n_th_arg(_args, 0)))
			{
			case 0: attack_type = Pattern108::AttackType::AIMING_SPLASH; break;
			case 1: attack_type = Pattern108::AttackType::AIMING_LINEAR_1_WAY; break;
			case 2: attack_type = Pattern108::AttackType::AIMING_3_WAY; break;
			case 3: attack_type = Pattern108::AttackType::AIMING_BULLETS_SHEET; break;
			default: throw std::runtime_error("Undefined attack type.");
			}
			return Pattern108
			(
				std::move(attack_type),
				Point2D(get_n_th_arg(_args, 1), get_n_th_arg(_args, 2)),
				static_cast<int>(get_n_th_arg(_args, 3)),
				Point2D(get_n_th_arg(_args, 4), get_n_th_arg(_args, 5))
			);
		}
		else
		{
			throw std::runtime_error("Pattern108 needs 6 arguments.");
		}
	}

	struct Pattern109 final : Pattern
	{
		enum class AttackType {VERTICAL = 0, AIMING,};
		const AttackType attack_type;
		const Point2D dst;
		Point2D v;
		static constexpr auto SPEED{static_cast<Real>(4)};

		Pattern109(AttackType _attack_type, const Point2D& _dst) noexcept
			:attack_type(_attack_type), dst(_dst), v()
		{}

		virtual void operator()(MobEnemy& _enemy) override final
		{
			ALIASES;
			if (count == 0)
			{
				v = SPEED * unit_vec(angle_of(dst - pos));
			}
			else
			{
				switch (attack_type)
				{
					case AttackType::VERTICAL:
					{
						if (count % 50 > 20 && count % 50 < 40)
						{
							if (count % 4 == 0)
							{
								_enemy.get_se_manager_ref().erect_play_flag_of("../../data/sound/enemy_shot_02.wav");
							}
							if (count % 3 == 0)
							{
								register_bullet("B2", _enemy, player, pos, half_pi<Real>(), straight_course(8));
								register_bullet("B2", _enemy, player, pos, -half_pi<Real>(), straight_course(8));
							}
						}
					}break;
					case AttackType::AIMING:
					{
						if (count % 50 > 20 && count % 50 < 40)
						{
							if (count % 4 == 0)
							{
								_enemy.get_se_manager_ref().erect_play_flag_of("../../data/sound/enemy_shot_02.wav");
							}
							register_bullet("B2", _enemy, player, pos, angle_of(player.pos() - pos), straight_course(8));
						}
					}break;
				}

				pos += v;
			}
		}
	};

	//Release bullets while heading to specified dst.
	MobEnemy::ActPattern pattern109(const std::vector<Point2D>& _args)
	{
		if (_args.size() == 2)
		{
			Pattern109::AttackType attack_type;
			switch (static_cast<int>(get_n_th_arg(_args, 0)))
			{
			case 0: attack_type = Pattern109::AttackType::VERTICAL; break;
			case 1: attack_type = Pattern109::AttackType::AIMING; break;
			}
			return Pattern109(std::move(attack_type), Point2D(get_n_th_arg(_args, 1), get_n_th_arg(_args, 2)));
		}
		else
		{
			throw std::runtime_error("Pattern 109 needs 3 arguments.");
		}
	}

	struct Pattern110 final : Pattern
	{
		OVERRIDE_PARENTHESIS
		{
			ALIASES;

		}
	};

	MobEnemy::ActPattern pattern110(const std::vector<Point2D>& _args) { return pattern0; }
	MobEnemy::ActPattern pattern111(const std::vector<Point2D>& _args) { return pattern0; }
	MobEnemy::ActPattern pattern112(const std::vector<Point2D>& _args) { return pattern0; }
	MobEnemy::ActPattern pattern113(const std::vector<Point2D>& _args) { return pattern0; }
	MobEnemy::ActPattern pattern114(const std::vector<Point2D>& _args) { return pattern0; }
	MobEnemy::ActPattern pattern115(const std::vector<Point2D>& _args) { return pattern0; }
	MobEnemy::ActPattern pattern116(const std::vector<Point2D>& _args) { return pattern0; }
	MobEnemy::ActPattern pattern117(const std::vector<Point2D>& _args) { return pattern0; }
	MobEnemy::ActPattern pattern118(const std::vector<Point2D>& _args) { return pattern0; }
	MobEnemy::ActPattern pattern119(const std::vector<Point2D>& _args) { return pattern0; }
	MobEnemy::ActPattern pattern120(const std::vector<Point2D>& _args) { return pattern0; }
	MobEnemy::ActPattern pattern121(const std::vector<Point2D>& _args) { return pattern0; }
	MobEnemy::ActPattern pattern122(const std::vector<Point2D>& _args) { return pattern0; }
	MobEnemy::ActPattern pattern123(const std::vector<Point2D>& _args) { return pattern0; }
	MobEnemy::ActPattern pattern124(const std::vector<Point2D>& _args) { return pattern0; }
	MobEnemy::ActPattern pattern125(const std::vector<Point2D>& _args) { return pattern0; }
	MobEnemy::ActPattern pattern126(const std::vector<Point2D>& _args) { return pattern0; }
	MobEnemy::ActPattern pattern127(const std::vector<Point2D>& _args) { return pattern0; }
	MobEnemy::ActPattern pattern128(const std::vector<Point2D>& _args) { return pattern0; }
}
