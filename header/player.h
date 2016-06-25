//player.h

#ifndef __PLAYER_H
#define __PLAYER_H

#include "object.h"
#include <functional>
#include <string>
#include "smart_ptr.h"

namespace MyGameProject
{
	class Device;
	class Enemy;
	class Shot;
	struct PlayerVars;

	class Player final : public HitableObject, public MovingObject
	{
	public:
		static constexpr int HYPER_MODE_TIME = 700;
		enum class WeaponType
		{
			SHORT_RANGE,
			LONG_RANGE,
			HYPER_SHORT_RANGE,
			HYPER_LONG_RANGE,
		};
		enum class State
		{
			IMMORTAL,
			WARP,
			WARP_MOTION,
			NORMAL,
			ACTION_SELECT,
		};
	private:
		const Device& device;
		Point2D velocity;
		bool shot_flag;
		bool locked_flag;
		int lock_icon_brt;
		Real lock_icon_rate;
		int count;
		WeaponType weapon_type;
		State current_state;
		int selected_weapon_number;
		bool hyper_mode;
		int hyper_mode_count;
		int immortal_count;
		int warp_count;
		Point2D warp_destination;
		int warp_charge;
		Real warp_angle;
		std::array<Point2D, 6> trail;
		Point2D v_warp;
		std::function<void(const std::string&)> erect_se_flag_of;
		int current_health;
		int discharge_button_pressed_count;
		static std::array<int, 8>& indicator_img(void) { static std::array<int, 8> img; return img; }
		static std::array<int, 3>& player_img(void) { static std::array<int, 3> img; return img; }
		std::vector<gp::smart_ptr<Shot>>& shots;
		std::vector<gp::smart_ptr<Enemy>>& enemies;
		std::unique_ptr<PlayerVars> vars;
	public:
		Player
		(
			const Device& _device, 
			std::function<void(const std::string&)> _se_flag_setter,
			std::vector<gp::smart_ptr<Shot>>& _shots,
			std::vector<gp::smart_ptr<Enemy>>& _enemies
		);
		virtual void draw(void) const override;
		virtual void update(void) override;
		void update_mode(void);
		WeaponType weapon(void) const;
		State state(void) const;
		bool hit(void);
		void charge_warp_point(int _point);
		bool is_hyper_mode(void) const;
		int rest_hyper_mode_time(void) const;
		int health(void) const;
		bool shot_if(void) const;
		bool is_locked(void) const;
		const Point2D& v(void) const;
		static void preperation(void);
		virtual ~Player(void);
	};
}

#endif