//player.cpp

#include "player.h"
#include "device.h"
#include "image_pool.h"
#include "environmental_constants.h"
#include <boost/math/constants/constants.hpp>
#include "msgbox.h"
#include "onmyo_dama.h"
#include "player_vars.h"
#include "player_constants.h"
#include "locked_marker.h"
#include "debug_value.h"
#include "shared_object.h"

namespace MyGameProject
{
	Player::Player
		(
			const Device& _device,
			std::function<void(const std::string&)> _se_flag_setter,
			std::vector<gp::smart_ptr<Shot>>& _shots,
			std::vector<gp::smart_ptr<Enemy>>& _enemies
			)
		:HitableObject(HitBox(ShapeElement::Circle(6), PhysicalState(0, Point2D(WW_2<Real>(), WH<Real>() - 100)))),
		MovingObject(Point2D(WW_2<Real>(), WH<Real>() - 100)),
		shot_flag(false),
		locked_flag(false),
		device(_device),
		lock_icon_brt(255),
		lock_icon_rate(0),
		count(0),
		weapon_type(WeaponType::LONG_RANGE),
		selected_weapon_number(0),
		hyper_mode(false),
		hyper_mode_count(0),
		erect_se_flag_of(_se_flag_setter),
		current_state(State::NORMAL),
		immortal_count(0),
		current_health(MAX_HEALTH),
		velocity(Point2D(0, 0)),
		warp_count(0),
		warp_destination(),
		warp_charge(MAX_WARP_CHARGE),
		warp_angle(0),
		discharge_button_pressed_count(0),
		shots(_shots),
		enemies(_enemies),
		vars(std::make_unique<PlayerVars>(*this))
	{
		using namespace boost::math::constants;
		for (int i = 0; i != vars->onmyo_damas.size(); ++i)
		{
			vars->onmyo_damas[i] = OnmyoDama(device, *this, i * two_pi<Real>() / vars->onmyo_damas.size());
		}
		vars->locked_enemy_list.resize(SharedObject::enemies().size());
		vars->number_of_locked_enemies = 0;
		vars->locked_enemy_marker_list.resize(SharedObject::enemies().size());
	} //Player::Player(...)


	Player::WeaponType Player::weapon(void) const
	{
		if (!hyper_mode)
		{
			if (device.get_time("X") > 0) { return WeaponType::SHORT_RANGE; }
			else { return WeaponType::LONG_RANGE; }
		}
		else
		{
			if (device.get_time("X") > 0) { return WeaponType::HYPER_SHORT_RANGE; }
			else { return WeaponType::HYPER_LONG_RANGE; }
		}
	}

	Player::State Player::state(void) const { return current_state; }


	int Player::rest_hyper_mode_time(void) const { return is_hyper_mode() ? hyper_mode_count : 0; }

	bool Player::hit(void)
	{
		static constexpr int IMMORTAL_TIME = 180;

		switch (current_state)
		{
		case State::NORMAL:
			current_state = State::IMMORTAL;
			current_health = current_health - 1 > 0 ? current_health - 1 : 0;
			erect_se_flag_of("../../data/sound/sei_ge_garasu_hibi02.mp3");
			immortal_count = IMMORTAL_TIME;
			return true;
		case State::WARP:
			return false;
		case State::WARP_MOTION:
			return false;
		case State::IMMORTAL:
			return false;
		case State::ACTION_SELECT:
			return false;
		default:
			return false;
		}
	} //Player::hit

	void Player::charge_warp_point(int _point)
	{
		if (warp_charge < MAX_WARP_CHARGE)
		{
			if ((warp_charge += _point) >= MAX_WARP_CHARGE)
			{
				if (warp_charge > MAX_WARP_CHARGE) { warp_charge = MAX_WARP_CHARGE; }
				erect_se_flag_of("../../data/sound/button04b.mp3");
			}
		}
	}

	const Point2D& Player::v(void) const { return velocity; }

	bool Player::shot_if(void) const { return shot_flag; }

	int Player::health(void) const { return current_health; }

	bool Player::is_hyper_mode(void) const { return hyper_mode; }

	bool Player::is_locked(void) const { return locked_flag; }

	void Player::preperation(void)
	{
		ImagePool::add("../../data/img/lock_range_level0.png");
		ImagePool::add("../../data/img/lock_range_level1.png");
		ImagePool::add("../../data/img/lock_range_level2.png");
		PlayerVars::lock_range_img_level()[0] = ImagePool::get("../../data/img/lock_range_level0.png");
		PlayerVars::lock_range_img_level()[1] = ImagePool::get("../../data/img/lock_range_level1.png");
		PlayerVars::lock_range_img_level()[2] = ImagePool::get("../../data/img/lock_range_level2.png");

		ImagePool::add("../../data/img/player_dot_0.png");
		ImagePool::add("../../data/img/player_dot_1.png");
		ImagePool::add("../../data/img/player_dot_2.png");
		player_img()[0] = ImagePool::get("../../data/img/player_dot_0.png");
		player_img()[1] = ImagePool::get("../../data/img/player_dot_1.png");
		player_img()[2] = ImagePool::get("../../data/img/player_dot_2.png");
		ImagePool::add("../../data/img/lock.png");
		ImagePool::add_multi("../../data/img/health_indicator.png", 10, 5, 2, 400, 400);
		int i = 0;
		for (auto& img : indicator_img())
		{
			using namespace std; using namespace boost;
			img = ImagePool::get("../../data/img/health_indicator.png@"s + lexical_cast<string>(i));
			++i;
		}
		ImagePool::add("../../data/img/ring.png");
		RING_IMG_HANDLER = ImagePool::get("../../data/img/ring.png");
		OnmyoDama::preperation();
		LockedMarker::preperation();
		HyperStartEffect::preparation();
		PlayerVars::load_lock_gage_images();
	} //Player::preperation

	Player::~Player(void) = default;

	void PlayerVars::load_lock_gage_images(void) noexcept
	{
		static auto add_and_get
		{
			[](auto&& _file_path)
			{
				ImagePool::add(_file_path);
				return ImagePool::get(std::forward<decltype(_file_path)>(_file_path));
			}
		};

		using namespace std::literals;
		static const auto ROOT{"../../data/img/lock_gage/"s};
		lock_gage_img()[LockGageColor::BLUE] = add_and_get(ROOT + "blue.png");
		lock_gage_img()[LockGageColor::GREEN] = add_and_get(ROOT + "green.png");
		lock_gage_img()[LockGageColor::YELLOW] = add_and_get(ROOT + "yellow.png");
		lock_gage_frame_img() = add_and_get(ROOT + "frame.png");
		lock_gage_unavailable_img() = add_and_get(ROOT + "unavailable.png");
	}

} //namespace MyGameProject
