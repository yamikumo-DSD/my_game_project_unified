#if 0
//enemy_management.cpp

#include "play_game.h"
#include "find_vacant_object.h"
#include "mob_enemy.h"
#include "enemy_order.h"
#include "enemy_act_pattern.h"
#include "se_manager.h"
#include "boss.h"
#include "order_update.h"

#include "grammar_check.h"

#ifdef GRAMMAR_CHECK
	#include "msgbox.h"
	#include <boost/lexical_cast.hpp>
#endif

namespace
{
	using namespace MyGameProject;
}

MobEnemy::ActPattern int_to_act_pattern(int _n)
{
	switch (_n)
	{
	case 0:
		return pattern0;
	case 1:
		return pattern1;
	case 2:
		return pattern2;
	case 3:
		return pattern3;
	case 4:
		return pattern4;
	case 5:
		return pattern5;
	case 6:
		return pattern6;
	default:
		return pattern0;
	}
}

MobEnemy::ActPattern act_pattern_converter(int _n, const std::vector<Point2D>& _args)
{
#ifdef GRAMMAR_CHECK
	msgbox_ok((std::string("act_pattern_converter(int, const std::vector<Point2D>&): ") + boost::lexical_cast<std::string>(_n)).c_str());
#endif
		switch (_n)
		{
		case 0:
			return pattern0;
		case 1:
			return pattern1;
		case 2:
			return pattern2;
		case 3:
			return pattern3;
		case 4:
			return pattern4;
		case 5:
			return pattern5;
		case 6:
			return pattern6;
		case 100:
#ifdef GRAMMAR_CHECK
			msgbox_ok("Calling pattern100");
			msgbox_ok((std::string("act_pattern_converter(int, const std::vector<Point2D>&): _args.size() == ") + boost::lexical_cast<std::string>(_args.size())).c_str());
#endif
			return pattern100(_args);
		case 101:
			return pattern101(_args);
		case 102:
			return pattern102(_args);
		case 103:
			return pattern103(_args);
		case 104:
			return pattern104(_args);
		case 105:
			return pattern105(_args);
		case 106:
			return pattern106(_args);
		case 107:
			return pattern107(_args);
		default:
			return pattern0;
		}
}

void PlayGame::exec(void)
{
	if (!code_list.empty())
	{
		(*this)(code_list.back());
	}
}

void PlayGame::operator()(const Code& _code)
{
	boost::apply_visitor(*this, _code);
}

void PlayGame::operator()(const CreateMob& _cm)
{
	if (count == _cm.register_time)
	{
	#ifdef GRAMMAR_CHECK
		msgbox_ok((std::string("Player::operator()(const CreateMob&): ") + boost::lexical_cast<std::string>(_cm.act_pattern.pattern_number)).c_str());
	#endif
		if (se_manager)
		{
			auto i = 
				(
					*find_vacant_object(mob_enemies) =
						MobEnemy::create
						(
							_cm.name, 
							bullets, 
							*se_manager, 
							EnemyOrder::make_order
							(
								_cm.name,
								_cm.register_time,
								Point2D(_cm.x,_cm.y),
								_cm.initial_angle,
#ifdef NEW_ORDER_FORM_TEST
								//int_to_act_pattern(_cm.act_pattern.pattern_number)
								std::make_shared<MobEnemy::ActPattern>(act_pattern_converter(_cm.act_pattern.pattern_number, _cm.act_pattern.arguments))
#else
								int_to_act_pattern(_cm.act_pattern)
#endif
							),
							*player
						)
				);
			*find_vacant_object(enemies) = i;
		}
		code_list.pop_back();
	}
}

void PlayGame::operator()(const CreateBoss& _cb)
{
	if (count == _cb.register_time)
	{
		//Create a boss and keep the pointer to the object.
		auto i = (*find_vacant_object(bosses)
			= Boss::create
			(
			_cb.name,
			bullets,
			items,
			std::bind(std::mem_fn(&SEManager::erect_play_flag_of), se_manager.get(), std::placeholders::_1),
			std::bind(std::mem_fn(&PlayGame::change_play_speed), this, std::placeholders::_1),
			*player
			));
		//Register the boss in enemies' vector.
		*find_vacant_object(enemies) = i;
		code_list.pop_back();
	}
}
#endif
