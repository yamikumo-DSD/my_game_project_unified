//blaxk_wings_imple.cpp

#include "black_wings_imple.h"
#include "black_wings.h"
#include "motion.h"
#include "diffuse_point.h"
#include "environmental_constants.h"
#include "find_vacant_object.h"
#include <boost/math/constants/constants.hpp>
#include "bullet.h"
#include "item.h"
#include "bullet_behavior.h"
#include <array>
#include <cmath>
#include "draw_order.h"
#include "dxlib_wrapper.h"
#include "color.h"
#include "ranged_uniform_random.h"
#include "player.h"
#include "shared_object.h"

namespace
{
	using namespace boost::math::constants;
	using namespace std;
}

MyGameProject::BlackWingsImple::BlackWingsImple(BlackWings& _boss)
	:boss(_boss),
	left_or_right(LEFT),
	v(0,0),
	trail(),
	start_p(0,0),end_p(0,0),
	start_cnt(0),end_cnt(0),
	wave_health({ { 3000 ,4000,5000,2000} }),
	//wave_health({ { 10, 30, 50, 2000 } }),
	wave_count(0),
	to_and_fro_count(0),
	white_out_flag(false)
{
	p1 = p2 = boss.pos();
}

void MyGameProject::BlackWingsImple::set_linear_motion(bool _condition, Point2D _destination, int _term)
{
	if (_condition)
	{
		const auto count = boss.get_count();
		start_p = boss.pos();
		end_p = _destination;
		start_cnt = count;
		end_cnt = count + _term;
	}
}

void MyGameProject::BlackWingsImple::linear_motion(Point2D _departure, Point2D _destination, int _start_count, int _end_count)
{
	const auto count = boss.get_count();
	const auto motion = create_linear_motion_of_uniform_acceleration_2D(_departure, _destination, _end_count - _start_count);
	if (count <= _end_count && count >= _start_count)
	{
		if (count == _start_count)
		{
			boss.pos() = _departure;
			v = motion.v0();
		}
		else
		{
			v = v + motion.a();
			boss.pos() = boss.pos() + v;
		}
	}
}

void MyGameProject::BlackWingsImple::linear_motion(void)
{
	const auto count = boss.get_count();

	if (count <= end_cnt && count >= start_cnt)
	{
		linear_motion(end_p, start_cnt, end_cnt);
		if (count == end_cnt){ start_cnt = 0; end_cnt = 0; }
	}
}

void MyGameProject::BlackWingsImple::wave1(void)
{
	static constexpr int COOL_TIME = 10;

	if (wave_count < COOL_TIME)
	{
		if (wave_count == 0){ set_linear_motion(Point2D(WW_2<Real>(), 100), COOL_TIME); }
	}
	else
	{
		const auto cycle_count = (wave_count - COOL_TIME) % 150; //one cycle = 150 frame

		if (cycle_count < 80)
		{
			if (cycle_count == 0)
			{
				theta = 0;
				set_linear_motion(diffuse_point_rect(Point2D(200, 50), Point2D(WW<Real>() - 200, 100)), 20);
			}

			theta += static_cast<Real>(0.025) * pi<Real>(); //takes 80 frames to rotate 2pi:

			static constexpr Real r = 50;
			p1 = boss.pos() + Point2D(r * cos(theta), r * sin(theta));
			p2 = boss.pos() + Point2D(r * cos(theta + pi<Real>()), r * sin(theta + pi<Real>()));


			if (cycle_count % 3 == 0)
			{
				for (int i = -1; i < 2; ++i)
				{
					*find_vacant_object(SharedObject::bullets()) = Bullet::create("WindSmasher", boss, boss.player_ref(), p1, theta + i * pi<Real>() / 6, straight_course(10));
					*find_vacant_object(SharedObject::bullets()) = Bullet::create("WindSmasher", boss, boss.player_ref(), p2, pi<Real>() + theta + i * pi<Real>() / 6, straight_course(10));
					boss.erect_se_flag_of("../../data/sound/enemy_shot.wav");
				}
			}

		}
	}
	linear_motion();
	++wave_count;
}

void MyGameProject::BlackWingsImple::wave2(void)
{
	static constexpr int COOL_TIME = 100;

	if (wave_count < COOL_TIME)
	{
		if (wave_count == 0){ set_linear_motion(Point2D(WW_2<Real>(), 100), COOL_TIME); }
	}
	else
	{
		const auto cycle_count = (wave_count - COOL_TIME) % 350;
		if (cycle_count >= 100 && cycle_count < 200)
		{
			if (cycle_count % 10 == 0)
			{
				boss.erect_se_flag_of("../../data/sound/enemy_shot.wav");
				for (int i = -1; i < 2; ++i)
				{
					*find_vacant_object(SharedObject::bullets()) = Bullet::create("WindSmasher", boss, boss.player_ref(), boss.pos(), angle_of(boss.player_ref().pos() - boss.pos()) + i * pi<Real>() / 20, straight_course(12));
					boss.erect_se_flag_of("../../data/sound/enemy_shot.wav");
				}
			}
		}
		else if (cycle_count >= 200)
		{
			if (cycle_count == 200){ dst = boss.player_ref().pos(); }
			if (cycle_count == 340){ boss.pos() = dst; }
			boss.erect_se_flag_of("../../data/sound/enemy_shot.wav");
			for (int i = 0; i != 2; ++i)
			{
				*find_vacant_object(SharedObject::bullets())
					= Bullet::create
					(
					"HenyoriElementOrange",
					boss,
					boss.player_ref(), boss.pos(),
					gp::safe_rand(-pi<Real>(), pi<Real>()),
					[=](Bullet& _b)->void
				{
					auto count = _b.get_count();
					auto speed = 5; if (count > 20){ speed = 10; }
					auto omega{ static_cast<Real>(0.1) }; if (count > 30 && count < 100) { omega += (count - 30) * static_cast<Real>(0.002); }

					Real theta = angle_of(dst - _b.pos());
					Real angle = _b.angle();

					if (count < 100)
					{
						if (sin(angle - theta) <= 0){ _b.angle(angle + omega); }
						else{ _b.angle(angle - omega); }
					}

					_b.pos().x(_b.pos().x() + speed * cos(_b.angle()));
					_b.pos().y(_b.pos().y() + speed * sin(_b.angle()));
				}
				);
			}
		}
	}
	linear_motion();
	++wave_count;
}

void MyGameProject::BlackWingsImple::wave3(void)
{
	static constexpr int COOL_TIME = 100;

	if (wave_count < COOL_TIME)
	{
		if (wave_count == 0){ set_linear_motion(Point2D(WW_2<Real>(), WH_2<Real>()), COOL_TIME); }
	}
	else
	{
		const auto cycle_count = wave_count - COOL_TIME;

		const auto c = cycle_count % 500;

		if (c % 500 == 0)
		{
			auto temp
			{
				Bullet::create
				(
				"CameraFrame", 
				boss, boss.player_ref(), 
				boss.pos(),
				0,
				accurate_homing(boss.player_ref(),6.5, 150)
				)
			};
			camera_frame = temp;
			*find_vacant_object(SharedObject::bullets()) = std::move(temp);

			boss.change_play_speed(0.5);
			boss.erect_se_flag_of("../../data/sound/Clock-Second_Hand01-1L.mp3");
		}
		if (c % 500 == 150)
		{
			boss.erect_se_flag_of("../../data/sound/se-033a.mp3");
			boss.change_play_speed(1.0);
			if (!camera_frame.expired() && camera_frame.lock()->get_flag()){ shuttered_point = camera_frame.lock()->pos(); }
		}
		if (c % 500 >= 151 && c % 500 <= 499)
		{
			static constexpr int N = 25;
			if (c % 12 == 0)
			{
				const auto r = gp::safe_rand<Real>(-pi<Real>(),pi<Real>());
				for (int i = 0; i != N; ++i)
				{
					*find_vacant_object(SharedObject::bullets()) = Bullet::create("WindSmasher", boss, boss.player_ref(), shuttered_point, r + i * 2 * pi<Real>() / N, straight_course(4));
					boss.erect_se_flag_of("../../data/sound/enemy_shot.wav");
				}
			}
		}
		if (c % 500 >= 149 && c % 500 <= 151){ white_out_flag = true; }
		else{ white_out_flag = false; }
	}

	linear_motion();
	++wave_count;
}

void MyGameProject::BlackWingsImple::wave4(void)
{
	static constexpr int COOL_TIME = 100;

	if (wave_count < COOL_TIME)
	{
		if (wave_count == 0){ set_linear_motion(Point2D(WW_2<Real>(), 100), COOL_TIME); boss.immortal_flag = true; }
	}
	else
	{
		auto cycle_count = wave_count - COOL_TIME;
		int phase = 0;
		int T = 0;
		if (to_and_fro_count < 5){ T = 60; phase = 0; }
		else if (to_and_fro_count >= 5 && to_and_fro_count < 10){ T = 40; phase = 1; }
		else if (to_and_fro_count >= 10 && to_and_fro_count < 20){ T = 20; phase = 2; }
		else if (to_and_fro_count >= 20 && to_and_fro_count < 35){ T = 15; phase = 3; }
		else{ T = 10; phase = 4; }

		if (cycle_count == 0)
		{
			to_and_fro_count = 0;
			left_or_right = RIGHT;
			set_linear_motion(diffuse_point_rect(Point2D(-100, 50), Point2D(-50, 150)), T);
		}
		else
		{
			if (cycle_count % T == 0)
			{
				left_or_right = !left_or_right;
				set_linear_motion(left_or_right == RIGHT ? diffuse_point_rect(Point2D(-100, 50), Point2D(-50, 150)) : diffuse_point_rect(Point2D(WW<Real>() + 50, 50), Point2D(WW<Real>() + 100, 150)), T);
				++to_and_fro_count;
			}
		}

			switch (phase)
			{
			case 0:
				if (cycle_count % 10 == 0)
				{
					for (int i = 0; i != 6;++i){ *find_vacant_object(SharedObject::bullets()) = Bullet::create("Typical", boss, boss.player_ref(), boss.pos(), half_pi<Real>(),  acceleration(10, static_cast<Real>(-0.1 * i),static_cast<Real>(2 - 0.3 * i))); }
				}
				break;
			case 1:
				if (cycle_count % 5 == 0)
				{
					for (int i = 0; i != 6; ++i){ *find_vacant_object(SharedObject::bullets()) = Bullet::create("Typical", boss, boss.player_ref(), boss.pos(), half_pi<Real>(), acceleration(15, static_cast<Real>(-0.3 * i),static_cast<Real>(4 - 0.6 * i))); }
				}
				break;
			case 2:
				if (cycle_count % 2 == 0)
				{
					for (int i = 0; i != 6; ++i){ *find_vacant_object(SharedObject::bullets()) = Bullet::create("Typical", boss, boss.player_ref(), boss.pos(), half_pi<Real>(), acceleration(20, static_cast<Real>(-0.6 * i),static_cast<Real>(8 - 1.0 * i))); }
				}
				break;
			case 3:
				if (cycle_count % T == 0)
				{
					
					for (int i = 0; i < 4; ++i)
					{
						*find_vacant_object(SharedObject::bullets()) = Bullet::create("Feather", boss, boss.player_ref(), diffuse_point_rect(Point2D(-100, 100), Point2D(WW<Real>() + 100, 120)), gp::safe_rand<Real>(static_cast<Real>(1.5 - 0.2) * pi<Real>(), static_cast<Real>(1.5 + 0.2) * pi<Real>()), parabola(7, static_cast<Real>(0.2)));
					}
				}
				break;
			case 4:
				if (cycle_count % T == 0)
				{
					
					for (int i = 0; i < 4; ++i)
					{
						*find_vacant_object(SharedObject::bullets()) = Bullet::create("Feather", boss, boss.player_ref(), diffuse_point_rect(Point2D(-100, 100), Point2D(WW<Real>() + 100, 120)), gp::safe_rand<Real>(static_cast<Real>(1.5 - 0.2) * pi<Real>(), static_cast<Real>(1.5 + 0.2) * pi<Real>()), parabola(7, static_cast<Real>(0.2)));
					}
				}
				break;			
			}
			if (cycle_count % T == 0){ boss.erect_se_flag_of("../../data/sound/enemy_shot.wav"); }
	}
	linear_motion();
	--boss.health;
	++wave_count;
}

void MyGameProject::BlackWingsImple::action(void)
{
	const auto count = boss.get_count();

	if (count == 0){ for (auto& p : trail){ p = boss.pos(); } }
	else
	{
		for (int i = trail.size() - 1; i != 0; --i)
		{
			trail[i] = trail[i - 1];
		}
		trail[0] = boss.pos();
	}

	if (count <= 100)
	{
		if (count == 0){ wave = 0;}
		set_linear_motion(count == 0, Point2D(WW<Real>() / 2, 100),50);
		linear_motion();
	}

	if (count == 120){ boss.immortal_flag = false; wave = 1; boss.health = wave_health[wave - 1]; }

	if (boss.health < 0)
	{
		boss.change_play_speed(1.f);
		if (wave < WAVE_NUM)
		{
			boss.erect_se_flag_of("../../data/sound/sei_ge_garasu_hibi02.mp3");
			for (auto& bullet : SharedObject::bullets())
			{
				if (bullet && addressof(bullet->get_master_ref()) == addressof(boss))
				{
					*find_vacant_object(SharedObject::items()) = Item::create("ConvertedBullet", boss.player_ref(), bullet->pos());
					bullet.reset();
				}
			}
			++wave; boss.health = wave_health[wave - 1]; wave_count = 0; 
		}
		else
		{
			boss.erect_se_flag_of("../../data/sound/sei_ge_garasu_hibi02.mp3");
			for (auto& bullet : SharedObject::bullets())
			{
				if (bullet && addressof(bullet->get_master_ref()) == addressof(boss))
				{
					*find_vacant_object(SharedObject::items()) = Item::create("ConvertedBullet",boss.player_ref(),bullet->pos());
					bullet.reset();
				}
			}
			boss.set_flag_off(); 
		}
	}

	switch (wave)
	{
	case 0:
		break;
	case 1:
		wave1();break;
	case 2:
		wave2();break;
	case 3:
		wave3();break;
	case 4:
		wave4(); break;
	}
}

void MyGameProject::BlackWingsImple::draw_charactor(void) const
{
	int i = 0;
	for (const auto& p : trail)
	{
		gp::SetDrawBlendModeOf
		(
			gp::DrawCircle(static_cast<Real>(p.x()), static_cast<Real>(p.y()), 14, gp::red),
			DX_BLENDMODE_ALPHA, 100 - 15 * i++
		);
	}

	gp::DrawCircle(static_cast<Real>(boss.pos().x()), static_cast<Real>(boss.pos().y()), 14, gp::black);

	if (wave == 1)
	{
		gp::DrawCircle(static_cast<Real>(p1.x()), static_cast<Real>(p1.y()), 14, gp::red);
		gp::DrawCircle(static_cast<Real>(p2.x()), static_cast<Real>(p2.y()), 14, gp::red);
	}
	if (white_out_flag)
	{
		ScopedDrawBlendMode blend(DX_BLENDMODE_ALPHA,200);
		DrawBox(0,0,WW<int>(),WH<int>(),gp::white.get(),TRUE);
	}
}
