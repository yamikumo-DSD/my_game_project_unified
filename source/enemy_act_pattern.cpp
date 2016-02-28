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
#include "msgbox.h"
#include "motion.h"
#include "environmental_constants.h"

namespace
{
	using namespace boost::math::constants;
	using namespace MyGameProject;
}

MyGameProject::MobEnemy::ActPattern MyGameProject::default_pattern = [](MyGameProject::MobEnemy& _enemy)->void{};

MyGameProject::MobEnemy::ActPattern MyGameProject::pattern0 = [](MyGameProject::MobEnemy& _enemy)->void
{
	const auto count = _enemy.get_count();

	if (count == 0){ _enemy.velocity() = Point2D(9,-3); }
	_enemy.velocity().x(_enemy.velocity().x() - static_cast<Real>(0.1)); 
	if (count > 70){ _enemy.velocity().y(_enemy.velocity().y() + static_cast<Real>(0.05)); }

	if (count % 8 == 0)
	{
		*find_vacant_object(_enemy.get_bullets_ref()) = Bullet::create("Cutter", _enemy, _enemy.player_ref(), _enemy.pos(), pi<Real>() / 2, straight_course(9));
		_enemy.get_se_manager_ref().erect_play_flag_of("../../data/sound/enemy_shot.wav");
	}

	_enemy.pos() += _enemy.velocity();
};

MyGameProject::MobEnemy::ActPattern MyGameProject::pattern1 = [](MyGameProject::MobEnemy& _enemy)->void
{
	const auto count = _enemy.get_count();

	if (count == 0){ _enemy.velocity() = Point2D(-9, -3); }
	_enemy.velocity().x(_enemy.velocity().x() + static_cast<Real>(0.1));
	if (count > 70){ _enemy.velocity().y(_enemy.velocity().y() + static_cast<Real>(0.05)); }

	if (count % 8 == 0)
	{
		*find_vacant_object(_enemy.get_bullets_ref()) = Bullet::create("Cutter", _enemy, _enemy.player_ref(), _enemy.pos(), pi<Real>() / 2, straight_course(9));
		_enemy.get_se_manager_ref().erect_play_flag_of("../../data/sound/enemy_shot.wav");
	}
	_enemy.pos() += _enemy.velocity();
};

MyGameProject::MobEnemy::ActPattern MyGameProject::pattern2 = [](MyGameProject::MobEnemy& _enemy)->void
{
	const auto count = _enemy.get_count();

	if (count == 0){ _enemy.velocity() = Point2D(0, -5); }

	if (count % 4 == 0)
	{
		*find_vacant_object(_enemy.get_bullets_ref()) = Bullet::create("Cutter", _enemy, _enemy.player_ref(), _enemy.pos(), pi<Real>() / 2, straight_course(9));
		_enemy.get_se_manager_ref().erect_play_flag_of("../../data/sound/enemy_shot.wav");
	}

	_enemy.pos() += _enemy.velocity();
};

MyGameProject::MobEnemy::ActPattern MyGameProject::pattern3 = [](MyGameProject::MobEnemy& _enemy)->void
{
	const auto count = _enemy.get_count();

	if (count == 0){ _enemy.velocity() = Point2D(0, 2); }

	if (count % 60 == 0)
	{
		gp::RangedUniformRandom generator(-pi<Real>(),pi<Real>());
		Real r = static_cast<Real>(generator.rand());
		static constexpr int N = 10;
		for (int i = 0; i < N;++i)
		{
			*find_vacant_object(_enemy.get_bullets_ref()) = Bullet::create("Typical", _enemy, _enemy.player_ref(), _enemy.pos(),r + i * (2 * pi<Real>() / N), straight_course(4));
		}
		_enemy.get_se_manager_ref().erect_play_flag_of("../../data/sound/enemy_shot.wav");
	}

	_enemy.pos() += _enemy.velocity();
};

MyGameProject::MobEnemy::ActPattern MyGameProject::pattern4 = [](MyGameProject::MobEnemy& _enemy)->void
{
	const auto count = _enemy.get_count();

	//Motion control.
	if (count == 0){ _enemy.velocity() = Point2D(0, -2); }
	else if (count >= 180 && count <= 200)
	{
		_enemy.velocity() = Point2D(0, count / 10.f - 20); 
	}
	
	//Bullet control.
	if (count > 200 && count <= 600)
	{
		if (count % 60 == 0)
		{
			*find_vacant_object(_enemy.get_bullets_ref()) =
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

MyGameProject::MobEnemy::ActPattern MyGameProject::pattern5 = [](MyGameProject::MobEnemy& _enemy)->void
{
};

MyGameProject::MobEnemy::ActPattern MyGameProject::pattern6 = [](MyGameProject::MobEnemy& _enemy)->void
{
};

namespace MyGameProject
{
	struct Pattern9Vars {LinearMotionOfUniformAcceleration2D<Point2D, int> motion; };
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
}

void MyGameProject::Pattern9::operator()(MobEnemy& _enemy)
{
	const auto count = _enemy.get_count();
	static constexpr int T = 30;
	static constexpr int WAIT_TIME = 1000;

	if (count == 0)
	{
		vars = std::make_shared<Pattern9Vars>();
		static const Point2D destination(WW<Real>() - 210, 150);
		vars->motion = create_linear_motion_of_uniform_acceleration_2D(_enemy.pos(), destination, T);
		_enemy.velocity() = vars->motion.v0();
	}
	else
	{
		if (count < T) { _enemy.velocity() += vars->motion.a(); }
		else if (count >= T && count < T + WAIT_TIME) { _enemy.velocity().x(0), _enemy.velocity().y(0);}
		else if (count >= T + WAIT_TIME) { _enemy.velocity() += Point2D(-0.2, 0); }
	}
	_enemy.pos() += _enemy.velocity();
}

void MyGameProject::Pattern10::operator()(MobEnemy& _enemy) 
{
	const auto count = _enemy.get_count();
	static constexpr int T = 30;
	static constexpr int WAIT_TIME = 1000;

	if (count == 0)
	{
		vars = std::make_shared<Pattern10Vars>();
		static const Point2D destination(210, 150);
		vars->motion = create_linear_motion_of_uniform_acceleration_2D(_enemy.pos(), destination, T);
		_enemy.velocity() = vars->motion.v0();
	}
	else
	{
		if (count < T) { _enemy.velocity() += vars->motion.a(); }
		else if (count >= T && count < T + WAIT_TIME) { _enemy.velocity().x(0), _enemy.velocity().y(0);}
		else if (count >= T + WAIT_TIME) { _enemy.velocity() += Point2D(0.2, 0); }
	}
	_enemy.pos() += _enemy.velocity();
}

void MyGameProject::Pattern11::operator()(MobEnemy& _enemy) {}
void MyGameProject::Pattern12::operator()(MobEnemy& _enemy) {}
void MyGameProject::Pattern13::operator()(MobEnemy& _enemy) {}
void MyGameProject::Pattern14::operator()(MobEnemy& _enemy) {}
void MyGameProject::Pattern15::operator()(MobEnemy& _enemy) {}
void MyGameProject::Pattern16::operator()(MobEnemy& _enemy) {}
void MyGameProject::Pattern17::operator()(MobEnemy& _enemy) {}
void MyGameProject::Pattern18::operator()(MobEnemy& _enemy) {}
void MyGameProject::Pattern19::operator()(MobEnemy& _enemy) {}
void MyGameProject::Pattern20::operator()(MobEnemy& _enemy) {}
void MyGameProject::Pattern21::operator()(MobEnemy& _enemy) {}

MyGameProject::Pattern9::Pattern9(void) {}
MyGameProject::Pattern10::Pattern10(void) {}
MyGameProject::Pattern11::Pattern11(void) {}
MyGameProject::Pattern12::Pattern12(void) {}
MyGameProject::Pattern13::Pattern13(void) {}
MyGameProject::Pattern14::Pattern14(void) {}
MyGameProject::Pattern15::Pattern15(void) {}
MyGameProject::Pattern16::Pattern16(void) {}
MyGameProject::Pattern17::Pattern17(void) {}
MyGameProject::Pattern18::Pattern18(void) {}
MyGameProject::Pattern19::Pattern19(void) {}
MyGameProject::Pattern20::Pattern20(void) {}
MyGameProject::Pattern21::Pattern21(void) {}

MyGameProject::Pattern9 MyGameProject::pattern9;
MyGameProject::Pattern10 MyGameProject::pattern10;
MyGameProject::Pattern11 MyGameProject::pattern11;
MyGameProject::Pattern12 MyGameProject::pattern12;
MyGameProject::Pattern13 MyGameProject::pattern13;
MyGameProject::Pattern14 MyGameProject::pattern14;
MyGameProject::Pattern15 MyGameProject::pattern15;
MyGameProject::Pattern16 MyGameProject::pattern16;
MyGameProject::Pattern17 MyGameProject::pattern17;
MyGameProject::Pattern18 MyGameProject::pattern18;
MyGameProject::Pattern19 MyGameProject::pattern19;
MyGameProject::Pattern20 MyGameProject::pattern20;
MyGameProject::Pattern21 MyGameProject::pattern21;

MobEnemy::ActPattern MyGameProject::pattern100(const std::vector<Point2D>& _args)
{
	Real v0;
	if (_args.size() == 1) { v0 = _args[0].x(); }
	else { throw std::runtime_error("No argument is not allowed."); }

	return [&, v0](MyGameProject::MobEnemy& _enemy)->void
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
				*find_vacant_object(_enemy.get_bullets_ref()) =
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
MobEnemy::ActPattern MyGameProject::pattern101(const std::vector<Point2D>& _args)
{
	Real v0;
	Real a;
	if (_args.size() == 1) { v0 = _args[0].x(); a = _args[0].y(); }
	else { throw std::runtime_error("this pattarn must take arguments."); }

	return [&, v0, a](MyGameProject::MobEnemy& _enemy)->void
	{
		const auto count = _enemy.get_count();

		//Motion control.
		if (count == 0) { _enemy.velocity() = Point2D(0, v0); }
		else { _enemy.velocity() -= Point2D(0, a); }

		_enemy.pos() += _enemy.velocity();
	};
}
MobEnemy::ActPattern MyGameProject::pattern102(const std::vector<Point2D>& _args)
{
	Point2D v;
	if (_args.size() == 1) { v.x(_args[0].x()); v.y(_args[0].y());}
	else { throw std::runtime_error("pattarn 102 must take arguments."); }

	return [&, v](MyGameProject::MobEnemy& _enemy)->void
	{
		const auto count = _enemy.get_count();

		//Motion control.
		static constexpr int WAIT_TIME = 300;
		if (count == 0) { _enemy.velocity() = Point2D(0, 0); }
		else if (count >= WAIT_TIME) { _enemy.velocity() = v; }

		_enemy.pos() += _enemy.velocity();
	};
}

struct Pattern103 : public MyGameProject::Pattern
{
	Real a, PD, v0, theta;
	const Point2D dst;
	static constexpr int WAIT_TIME = 250;
	static constexpr Real T = 50.f;
	Pattern103(const Point2D& _destination):dst(_destination) {}
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
			else if(count >= T + WAIT_TIME) { _enemy.velocity() += Point2D(0, 0.2f); }

		}

		_enemy.pos() += _enemy.velocity();
	}
};

MobEnemy::ActPattern MyGameProject::pattern103(const std::vector<Point2D>& _args)
{
	if (_args.size() == 1) { return Pattern103(Point2D(_args[0].x(), _args[0].y())); }
	else { throw std::runtime_error("pattarn 103 must take an arguments."); }
}
MobEnemy::ActPattern MyGameProject::pattern104(const std::vector<Point2D>& _args)
{
	using namespace std;

	Real a = 0.f, theta = 0.f;
	if (_args.size() == 1) { a = _args[0].x(); theta = _args[0].y(); }
	else { throw std::runtime_error("pattarn 104 must take an arguments."); }

	return [=](MyGameProject::MobEnemy& _enemy)
	{
		const auto count = _enemy.get_count();

		static constexpr int WAIT_TIME = 220;

		if (count == 0) { _enemy.velocity() = Point2D(0, 0); }
		else if (count > WAIT_TIME) { _enemy.velocity() += a * Point2D(cos(theta), sin(theta)); }

		_enemy.pos() += _enemy.velocity();
	};
}
//Straight forward
MobEnemy::ActPattern MyGameProject::pattern105(const std::vector<Point2D>& _args)
{
	using namespace std;

	Real v = 0.f, theta = 0.f;
	if (_args.size() == 1) { v = _args[0].x(); theta = _args[0].y(); }
	else { throw std::runtime_error("pattarn 105 must take an arguments."); }

	return [=](MyGameProject::MobEnemy& _enemy)
	{
		const auto count = _enemy.get_count();

		if (count == 0) { _enemy.velocity() = v * Point2D(cos(theta), sin(theta)); }

		_enemy.pos() += _enemy.velocity();
	};
}

MobEnemy::ActPattern MyGameProject::pattern106(const std::vector<Point2D>& _args)
{
	return pattern0;
}
MobEnemy::ActPattern MyGameProject::pattern107(const std::vector<Point2D>& _args)
{
	return pattern0;
}
