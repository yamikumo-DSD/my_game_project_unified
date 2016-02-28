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

MyGameProject::Player::Player
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
	velocity(Point2D(0,0)),
	warp_count(0),
	warp_destination(),
	warp_charge(MAX_WARP_CHARGE),
	warp_angle(0),
	discharge_button_pressed_count(0),
	shots(_shots),
	enemies(_enemies),
	vars(std::make_unique<PlayerVars>())
{
	using namespace boost::math::constants;
	for (int i = 0; i != vars->onmyo_damas.size(); ++i)
	{ vars->onmyo_damas[i] = OnmyoDama(*this, i * two_pi<Real>() / vars->onmyo_damas.size()); }
	vars->locked_enemy_list.resize(enemies.size());
	vars->number_of_locked_enemies = 0;
	vars->locked_enemy_marker_list.resize(enemies.size());
}


MyGameProject::Player::WeaponType MyGameProject::Player::weapon(void) const
{
	if (!hyper_mode)
	{
#if 0
		switch (selected_weapon_number)
		{
		case 0:
			return WeaponType::LONG_RANGE;
		case 1:
			return WeaponType::SHORT_RANGE;
		default:
			return WeaponType::LONG_RANGE;
		}
#endif
		if (device.get_time("X") > 0){ return WeaponType::SHORT_RANGE; }
		else{ return WeaponType::LONG_RANGE; }
	}
	else
	{
#if 0
		switch (selected_weapon_number)
		{
		case 0:
			return WeaponType::HYPER_LONG_RANGE;
		case 1:
			return WeaponType::HYPER_SHORT_RANGE;
		default:
			return WeaponType::HYPER_LONG_RANGE;
		}
#endif
		if (device.get_time("X") > 0){ return WeaponType::HYPER_SHORT_RANGE; }
		else{ return WeaponType::HYPER_LONG_RANGE; }
	}
}

MyGameProject::Player::State MyGameProject::Player::state(void) const
{
	return current_state;
}


int MyGameProject::Player::rest_hyper_mode_time(void) const
{
	if (is_hyper_mode()){ return hyper_mode_count; }
	else{ return 0; }
}



void MyGameProject::Player::hit(void)
{
	static constexpr int IMMORTAL_TIME = 180;

	switch (current_state)
	{
	case State::NORMAL:
		current_state = State::IMMORTAL;
		current_health = current_health - 1 > 0 ? current_health - 1 : 0;
		erect_se_flag_of("../../data/sound/sei_ge_garasu_hibi02.mp3");
		immortal_count = IMMORTAL_TIME;
		break;
	case State::WARP:
		break;
	case State::WARP_MOTION:
		break;
	case State::IMMORTAL:
		break;
	case State::ACTION_SELECT:
		break;
	default:
		break;
	}
}

void MyGameProject::Player::charge_warp_point(int _point)
{
	if (warp_charge < MAX_WARP_CHARGE)
	{
		if ((warp_charge += _point) >= MAX_WARP_CHARGE)
		{
			if (warp_charge > MAX_WARP_CHARGE){ warp_charge = MAX_WARP_CHARGE; }
			erect_se_flag_of("../../data/sound/button04b.mp3"); 
		}
	}
}

const MyGameProject::Point2D& MyGameProject::Player::v(void) const
{
	return velocity;
}

bool MyGameProject::Player::shot_if(void) const
{
	return shot_flag;
}

int MyGameProject::Player::health(void) const
{
	return current_health;
}

bool MyGameProject::Player::is_hyper_mode(void) const
{
	return hyper_mode;
}

bool MyGameProject::Player::is_locked(void) const
{
	return locked_flag;
}

void MyGameProject::Player::preperation(void)
{
	ImagePool::add("../../data/img/player_dot_0.png");
	ImagePool::add("../../data/img/player_dot_1.png");
	ImagePool::add("../../data/img/player_dot_2.png");
	player_img()[0] = ImagePool::get("../../data/img/player_dot_0.png");
	player_img()[1] = ImagePool::get("../../data/img/player_dot_1.png");
	player_img()[2] = ImagePool::get("../../data/img/player_dot_2.png");
	ImagePool::add("../../data/img/lock.png");
	ImagePool::add_multi("../../data/img/health_indicator.png",10,5,2,400,400);
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

}

MyGameProject::Player::~Player(void){}

