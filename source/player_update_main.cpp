//player_update.cpp

#include "player.h"
#include <boost/math/constants/constants.hpp>
#include "device.h"
#include "player_constants.h"
#include "environmental_constants.h"
#include "find_vacant_object.h"
#include "shot.h"
#include "enemy.h"
#include "player_vars.h"
#include "locked_marker.h"
#include "debug_value.h"

namespace MyGameProject
{
	struct IsNullShapeVisitor : public boost::static_visitor<bool>
	{
		bool operator()(ShapeElement::Null) const { return true; }
		template<class ShapeType>
		bool operator()(ShapeType) const { return false; }
	}is_null_shape;

	inline bool is_lockable(const Enemy& _enemy)
	{
		return !boost::apply_visitor(is_null_shape, _enemy.area().get_shape());
	}

	void Player::update(void)
	{
		using namespace boost::math::constants;

		if (count == 0){ for (auto& p : trail){ p = pos(); } }
		else
		{
			for (int i = trail.size() - 1; i != 0; --i)
			{
				trail[i] = trail[i - 1];
			}
			trail[0] = pos();
		}

		const Point2D prev_p = pos();
		Real speed = 7;
		const bool up_input = device.get_time("Up") > 0;
		const bool down_input = device.get_time("Down") > 0;
		const bool right_input = device.get_time("Right") > 0;
		const bool left_input = device.get_time("Left") > 0;

		if ((up_input || down_input) && (right_input || left_input)){ speed /= static_cast<Real>(std::sqrt(2)); }

		if (current_state == State::WARP)
		{
			if (up_input && warp_destination.y() > 0)            { warp_destination += Point2D(0, -speed); }
			if (down_input && warp_destination.y() < WH<Real>()) { warp_destination += Point2D(0, speed); }
			if (right_input && warp_destination.x() < WW<Real>()){ warp_destination += Point2D(speed, 0); }
			if (left_input && warp_destination.x() > 0)          { warp_destination += Point2D(-speed, 0); }

			--warp_count;
		}
		else if (current_state == State::WARP_MOTION)
		{
			if (warp_destination == pos()){ current_state = State::NORMAL; }
			else
			{
				pos() = pos() + v_warp;
				v_warp = v_warp - Point2D((2 * WARP_GAP / static_cast<Real>(pow(WARP_MOTION_TIME, 2))) * static_cast<Real>(cos(warp_angle)), (2 * WARP_GAP / static_cast<Real>(pow(WARP_MOTION_TIME, 2))) * static_cast<Real>(sin(warp_angle)));
				if (boost::geometry::distance(pos(), warp_destination) <= norm(v_warp)){ pos() = warp_destination; }
			}
		}
		else
		{
			if (up_input && pos().y() > 0)            { pos() += Point2D(0, -speed); }
			if (down_input && pos().y() < WH<Real>()) { pos() += Point2D(0, speed); }
			if (right_input && pos().x() < WW<Real>()){ pos() += Point2D(speed, 0); }
			if (left_input && pos().x() > 0)          { pos() += Point2D(-speed, 0); }
			warp_destination = pos();

			if (warp_charge < MAX_WARP_CHARGE)
			{
				if (++warp_charge >= MAX_WARP_CHARGE)
				{
					erect_se_flag_of("../../data/sound/button04b.mp3"); 
				}
			}
		}

		if (hyper_mode)
		{
			--hyper_mode_count; 
			if (hyper_mode_count < 0){ hyper_mode = false; }
		}

		area().get_physical_state().p = pos();

		velocity = pos() - prev_p;

		if (lock_icon_rate < 1.5)
		{
			lock_icon_rate += static_cast<Real>(0.05); 
			lock_icon_brt -= 7;
		}
		else
		{
			lock_icon_rate = 0; 
			lock_icon_brt = 255;
		}

		if (current_state == State::IMMORTAL)
		{
			--immortal_count;
			if (immortal_count < 0){ current_state = State::NORMAL; immortal_count = 0; }
		}

		//Deal with shots
		if (weapon() == WeaponType::SHORT_RANGE || weapon() == WeaponType::HYPER_SHORT_RANGE)
		{
			if (vars->radius <= 200.f) { vars->radius += 10; }
			else { vars->radius = 200.f; }

			auto& locking_count = vars->short_range_weapon_count;
			if (locking_count == 5)
			{
				for (int i = 0; i != vars->locked_enemy_list.size(); ++i)
				{
					const auto& enemy = enemies[i];
					if (enemy && enemy->get_flag() && is_lockable(*enemy))
					{
						if (norm(pos() - enemy->pos()) <= vars->radius)
						{
							if (enemy->get_health() - 10 * vars->locked_enemy_list[i].m && vars->number_of_locked_enemies < 5)
							{
								++vars->locked_enemy_list[i].m;
								++vars->number_of_locked_enemies;
								vars->locked_enemy_marker_list[i] = gp::make_smart<LockedMarker>(enemies[i], vars->locked_enemy_list[i].m);
								erect_se_flag_of("../../data/sound/button04a.mp3"); 
							}
						}
					}
					else
					{
						vars->number_of_locked_enemies -= vars->locked_enemy_list[i].m;
						vars->locked_enemy_list[i].m = 0;
					}
				}
			}
			else if (locking_count == 20)
			{
				for (int i = 0; i != vars->locked_enemy_list.size(); ++i)
				{
					const auto& enemy = enemies[i];
					if (enemy && enemy->get_flag() && is_lockable(*enemy))
					{
						if (norm(pos() - enemy->pos()) <= vars->radius)
						{
							if (enemy->get_health() - 10 * vars->locked_enemy_list[i].m && vars->number_of_locked_enemies < 10)
							{
								++vars->locked_enemy_list[i].m;
								++vars->number_of_locked_enemies;
								vars->locked_enemy_marker_list[i] = gp::make_smart<LockedMarker>(enemies[i], vars->locked_enemy_list[i].m);
								erect_se_flag_of("../../data/sound/button04a.mp3"); 
							}
						}
					}
					else
					{
						vars->number_of_locked_enemies -= vars->locked_enemy_list[i].m;
						vars->locked_enemy_list[i].m = 0;
					}
				}
			}
			else if (locking_count % 10 == 0)
			{
				for (int i = 0; i != vars->locked_enemy_list.size(); ++i)
				{
					const auto& enemy = enemies[i];
					if (enemy && enemy->get_flag() && is_lockable(*enemy))
					{
						if (norm(pos() - enemy->pos()) <= vars->radius)
						{
							const auto maximum_lock_num = hyper_mode ? 25 : 20;
							if (enemy->get_health() - 10 * vars->locked_enemy_list[i].m && vars->number_of_locked_enemies < maximum_lock_num)
							{
								++vars->locked_enemy_list[i].m;
								++vars->number_of_locked_enemies;
								vars->locked_enemy_marker_list[i] = gp::make_smart<LockedMarker>(enemies[i], vars->locked_enemy_list[i].m);
								erect_se_flag_of("../../data/sound/button04a.mp3"); 
							}
						}
					}
					
					else
					{
						vars->number_of_locked_enemies -= vars->locked_enemy_list[i].m;
						vars->locked_enemy_list[i].m = 0;
					}
				}
			}

			++locking_count;
		}
		else { vars->radius = 0.f;}

		if (vars->is_short_range_weapon_released)
		{ 
			vars->short_range_weapon_count = 0;
			int n = 0;
			for (int i = 0; i != vars->locked_enemy_list.size(); ++i)
			{
				const auto m = vars->locked_enemy_list[i].m;
				vars->locked_enemy_list[i].m = 0;
				for (int j = 0; j != m; ++j)
				{
					if (hyper_mode)
					{
						*find_vacant_object(shots) =
							std::make_shared<HomingShot2>(enemies[i], n++ * (two_pi<Real>() / vars->number_of_locked_enemies), pos());
					}
					else
					{
						*find_vacant_object(shots) =
							std::make_shared<HomingLazer2>(enemies[i], n++ * (two_pi<Real>() / vars->number_of_locked_enemies), pos());
					}
				}
			}
			vars->number_of_locked_enemies = 0;
			for (auto& locked_marker : vars->locked_enemy_marker_list) { locked_marker.reset(); }
		}

		if (shot_if())
		{
			if (count % 4 == 0)
			{
				for (const auto& onmyo_dama : vars->onmyo_damas)
				{
					if (weapon() == WeaponType::HYPER_LONG_RANGE)
					{
						*find_vacant_object(shots) = std::make_shared<HyperShot2>(onmyo_dama.direction, onmyo_dama.pos());
					}
					else if(weapon() == WeaponType::LONG_RANGE)
					{
						*find_vacant_object(shots) = std::make_shared<NormalShot>(onmyo_dama.direction, onmyo_dama.pos());
					}
				}
			} 
			if (count % 6 == 0){ erect_se_flag_of("../../data/sound/shot.mp3"); }
		}

		for (auto& onmyo_dama : vars->onmyo_damas)
		{
			onmyo_dama.update();
		}

		for (auto& marker : vars->locked_enemy_marker_list)
		{
			if (marker && marker->get_flag()) { marker->update(); }
			else { marker.reset(); }
		}

		++count;
	}
}

