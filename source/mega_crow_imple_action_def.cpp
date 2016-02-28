//mega_crow_imple_action_def.cpp

#include "mega_crow_imple.h"
#include <boost/math/constants/constants.hpp>
#include "environmental_constants.h"
#include "diffuse_point.h"
#include "mega_crow.h"
#include "player.h"
#include "find_vacant_object.h"
#include "lazer.h"
#include "bullet.h"
#include "bullet_behavior.h"

namespace MyGameProject
{
	using P = Point2D;
	using namespace boost::math::constants;

	class Invisible : public Bullet
	{
	private:
		const int duration = 0;
		virtual void custom_updater(void) override final
		{
			if (get_count() == duration) { set_flag_off(); }
		}
	public:
		Invisible(const Enemy& _master, const Player& _player, int _duration) 
			:Bullet(_master, _player, _master.pos(), 0, ShapeElement::Circle(150), [](Bullet& _bullet) {_bullet.pos() = _bullet.get_master_ref().pos(); }),
			duration(_duration)
		{
		}
		virtual void draw(void) const override final {/* Do notiong */}
		virtual void hit(void) override final {/* Do notiong  */}
		virtual bool is_resistant(void) const { return true; }
		~Invisible(void) {}
	};

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

	void MegaCrowImple::action(void)
	{
		if (boss.health > 0)
		{
			//aliasses
			auto& pos = boss.pos();
			const auto& player = boss.player_ref();
			auto& bullets = boss.get_bullets_ref();

			static const auto relative_battery_pos_0 = Point2D(75, -100);
			static const auto relative_battery_pos_1 = Point2D(-75, -100);

			static constexpr int T0 = 400;
			static constexpr int T1 = 80;
			static constexpr int T2 = 90;
			static constexpr int T3 = 70;
			static constexpr int T4 = 800;
			static constexpr int T5 = 540;
			static constexpr int T6 = 150;

			//count
			static constexpr auto Tphase = T0 + T1 + T2 + T3 + T4 + T5 + T6;
			const auto count = boss.get_count();
			const auto phase_count = count % (Tphase + 1); // 0 <= phase_count <= Tphase

			tail_index = count % 6 < 3 ? 0 : 1;
			const auto count_28 = count % 28;
			if (count_28 < 7) { string_index = 0; }
			else if (count_28 >= 7 && count_28 < 14) { string_index = 1; }
			else if (count_28 >= 14 && count_28 < 21) { string_index = 2; }
			else { string_index = 1; }

			//Dealing trails.
			if (phase_count == 0)
			{
				boss.immortal_flag = false;
				display_trails = false;
				//initialize trails.
				for (auto& t : trails) { t = pos; }
				//initialize angle histories.
				for (auto& a : angle_histories) { a = img_angle; }
			}
			else
			{
				for (int i = trails.size() - 1; i != 0; --i) { trails[i] = trails[i - 1]; }
				trails[0] = boss.pos();
				for (int i = angle_histories.size() - 1; i != 0; --i) { angle_histories[i] = angle_histories[i - 1]; }
				angle_histories[0] = img_angle;
			}


			if (phase_count >= 0 && phase_count < T0)
			{
				if (phase_count == 0)
				{
					display_trails = false;
					img_angle = 0.f;
					pos = P(XMAX_2, YMAX + 300);
					pal = 255;
					left_wing_index = right_wing_index = 0;
					INIT_V;
				}
				else
				{
					if (phase_count % 80 == 0)
					{
						*find_vacant_object(bullets) = std::make_shared<Lazer>(boss, player, pos + relative_battery_pos_0, angle_of(player.pos() - (pos + relative_battery_pos_0)), [](Bullet& _b) {}, 50, 90);
						*find_vacant_object(bullets) = std::make_shared<Lazer>(boss, player, pos + relative_battery_pos_1, angle_of(player.pos() - (pos + relative_battery_pos_1)), [](Bullet& _b) {}, 50, 90);
					}
				}
			}
			else if (phase_count >= T0 && phase_count < T0 + T1)
			{
				if (phase_count == T0)
				{
					display_trails = false;
					motion0 = create_linear_motion_of_uniform_acceleration_2D(pos, P(pos.x(), YMAX - 50), T1);
					v = motion0.v0();
				}
				else
				{
					v += motion0.a();
				}
			}
			else if (phase_count >= T0 + T1 && phase_count < T0 + T1 + T2)
			{
				if (phase_count == T0 + T1)
				{
					display_trails = false;
					INIT_V;
					*find_vacant_object(bullets) = std::make_shared<Lazer>(boss, player, pos + relative_battery_pos_0, -half_pi<Real>(), [](Bullet& _b) {}, 50, 90, [](const Bullet& _b, Real& _angle) {if (_b.get_count() >= 20) { _angle += 0.04; } });
					*find_vacant_object(bullets) = std::make_shared<Lazer>(boss, player, pos + relative_battery_pos_1, -half_pi<Real>(), [](Bullet& _b) {}, 50, 90, [](const Bullet& _b, Real& _angle) {if (_b.get_count() >= 20) { _angle -= 0.04; } });
				}
				else
				{
					const auto count_100 = count % 100;
					if (count_100 < 40) { left_wing_index = right_wing_index = 0; }
					else if (count_100 >= 40 && count_100 < 50) { left_wing_index = right_wing_index = 1; }
					else if (count_100 >= 50 && count_100 < 60) { left_wing_index = right_wing_index = 2; }
					else if (count_100 >= 60 && count_100 < 70) { left_wing_index = right_wing_index = 3; }
					else if (count_100 >= 70 && count_100 < 80) { left_wing_index = right_wing_index = 2; }
					else if (count_100 >= 80 && count_100 < 90) { left_wing_index = right_wing_index = 1; }
					else { left_wing_index = right_wing_index = 0; }

				}
			}
			else if (phase_count >= T0 + T1 + T2 && phase_count < T0 + T1 + T2 + T3)
			{
				if (phase_count == T0 + T1 + T2)
				{
					display_trails = true;
					*find_vacant_object(bullets) = gp::make_smart<Invisible>(boss, player, T3);
					INIT_V;
					left_wing_index = right_wing_index = 3;
				}
				else
				{
					//v += motion0.a();
					v += P(0, -0.3);
				}
			}
			else if (phase_count >= T0 + T1 + T2 + T3 && phase_count < T0 + T1 + T2 + T4)
			{
				if (phase_count == T0 + T1 + T2 + T3)
				{
					*find_vacant_object(bullets) = gp::make_smart<Invisible>(boss, player, T4);
					display_trails = true;
					INIT_V;
				}
				else
				{
					if (phase_count % 100 == 0)
					{
						const auto theta = gp::safe_rand(-pi<Real>(), pi<Real>());
						pos = P(XMAX_2, YMAX_2) + 1000 * P(cos(theta), sin(theta));
						const auto dir = angle_of(player.pos() - pos);
						static constexpr Real SPEED = 15;
						v = SPEED * P(cos(dir), sin(dir));
						img_angle = dir + half_pi<Real>();
					}

					if (phase_count % 10 == 0)
					{
						static constexpr Real N = 10;
						for (int i = 0; i != N; ++i)
						{
							*find_vacant_object(bullets)
								= Bullet::create("B0", boss, player, pos, i * two_pi<Real>() / N, straight_course(6));
							*find_vacant_object(bullets)
								= Bullet::create("B0", boss, player, pos, i * two_pi<Real>() / N, straight_course(3));
						}
					}
				}
			}
			else if (phase_count >= T0 + T1 + T2 + T3 + T4 && phase_count < T0 + T1 + T2 + T4 + T5)
			{
				if (phase_count == T0 + T1 + T2 + T3 + T4)
				{
					display_trails = false;
					img_angle = 0.f;
					INIT_V;
				}
				else
				{
					if (phase_count == T0 + T1 + T2 + T3 + T4 + 150)
					{
						pal = 0;
						pos = P(XMAX_2, YMAX_2);
					}
					else
					{
						pal += (255 / (T5 - 150.f));
						const auto count_100 = count % 100;
						if (count_100 < 40) { left_wing_index = right_wing_index = 0; }
						else if (count_100 >= 40 && count_100 < 50) { left_wing_index = right_wing_index = 1; }
						else if (count_100 >= 50 && count_100 < 60) { left_wing_index = right_wing_index = 2; }
						else if (count_100 >= 60 && count_100 < 70) { left_wing_index = right_wing_index = 3; }
						else if (count_100 >= 70 && count_100 < 80) { left_wing_index = right_wing_index = 2; }
						else if (count_100 >= 80 && count_100 < 90) { left_wing_index = right_wing_index = 1; }
						else { left_wing_index = right_wing_index = 0; }
					}
				}
			}
			else if (phase_count >= T0 + T1 + T2 + T3 + T4 + T5 && phase_count < T0 + T1 + T2 + T4 + T5 + T6)
			{
				if (phase_count == T0 + T1 + T2 + T3 + T4 + T5)
				{
					//v = P(0, -10);
					display_trails = false;
					INIT_V;
					left_wing_index = right_wing_index = 0;
				}
				else
				{
					v += P(0, +0.3);
					pal -= 2;
					if (phase_count == T0 + T1 + T2 + T3 + T4 + T5 + 40)
					{
						static constexpr Real N = 30;
						for (int i = 0; i != N; ++i)
						{
							*find_vacant_object(bullets)
								= Bullet::create("B0", boss, player, pos, i * two_pi<Real>() / N, straight_course(10));
							*find_vacant_object(bullets)
								= Bullet::create("B0", boss, player, pos, i * two_pi<Real>() / N, straight_course(7));
							*find_vacant_object(bullets)
								= Bullet::create("B0", boss, player, pos, i * two_pi<Real>() / N, straight_course(3));
						}
					}
				}
			}

			boss.pos() += v;
		}
		else
		{
			++count_after_killed;
			if (count_after_killed == 1) { boss.area().get_shape() = ShapeElement::Null(); }
			if (count_after_killed == 400)
			{
				boss.set_flag_off();
			}
		}
	}
}