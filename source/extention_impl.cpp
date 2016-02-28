//extention_impl

#include "play_game.h"
#include "msgbox.h"
#include "bit_calculation.h"
#include "find_vacant_object.h"
#include "mob_enemy.h"
#include "enemy_order.h"
#include "enemy_act_pattern.h"
#include "se_manager.h"
#include "boss.h"
#include "ranged_uniform_random.h"
#include <cmath>
#include "player.h"
#include "is_id_enable.h"

namespace 
{
	MyGameProject::MobEnemy::ActPattern act_pattern_converter(int _n, const std::vector<MyGameProject::Point2D>& _args)
	{
		using namespace MyGameProject;
		switch (_n)
		{
		case 0  :   return pattern0       ;
		case 1  :   return pattern1       ;
		case 2  :   return pattern2       ;
		case 3  :   return pattern3       ;
		case 4  :   return pattern4       ;
		case 5  :   return pattern5       ;
		case 6  :   return pattern6       ;
		case 9  :   return pattern9       ;
		case 10 :  return pattern10       ;
		case 11 :  return pattern11       ;
		case 12 :  return pattern12       ;
		case 13 :  return pattern13       ;
		case 14 :  return pattern14       ;
		case 15 :  return pattern15       ;
		case 16 :  return pattern16       ;
		case 17 :  return pattern17       ;
		case 18 :  return pattern18       ;
		case 19 :  return pattern19       ;
		case 20 :  return pattern20       ;
		case 21 :  return pattern21       ;
		case 100: return pattern100(_args);
		case 101: return pattern101(_args);
		case 102: return pattern102(_args);
		case 103: return pattern103(_args);
		case 104: return pattern104(_args);
		case 105: return pattern105(_args);
		case 106: return pattern106(_args);
		case 107: return pattern107(_args);
		default :  return pattern0        ;
		}
	}
}

void MyGameProject::PlayGame::user_extention_0(void)
{
	using namespace MyVM;

	auto enemy_type_num =   BitCalculation::bit_cast<jint>  (*(frame_stack.top().get_sp() - 7));
	auto px =               BitCalculation::bit_cast<jfloat>(*(frame_stack.top().get_sp() - 6));
	auto py =               BitCalculation::bit_cast<jfloat>(*(frame_stack.top().get_sp() - 5));
	auto initial_angle =    BitCalculation::bit_cast<jfloat>(*(frame_stack.top().get_sp() - 4));
	auto act_pattern_num =  BitCalculation::bit_cast<jint>  (*(frame_stack.top().get_sp() - 3));
	auto arg1 =             BitCalculation::bit_cast<jfloat>(*(frame_stack.top().get_sp() - 2));
	auto arg2 =             BitCalculation::bit_cast<jfloat>(*(frame_stack.top().get_sp() - 1));

	frame_stack.top().advance_sp(-7);
	++pc;

	std::string enemy_type_name;

	switch (enemy_type_num)
	{
		case 0: enemy_type_name = "EmergeFromCloud";  break;
		case 1: enemy_type_name = "Crow";             break;
		case 2: enemy_type_name = "BlackWings";       break;
		case 3: enemy_type_name = "Crow2";            break;
		case 4: enemy_type_name = "MasterAndSlaves";  break;
		case 5: enemy_type_name = "MasterAndSlaves2"; break;
		default: throw std::runtime_error("Undefined enemy type.");
	}

	if (se_manager)
	{
		auto i =
			(
				*find_vacant_object(mob_enemies) =
				MobEnemy::create
				(
					enemy_type_name,
					bullets,
					*se_manager,
					EnemyOrder::make_order
					(
						enemy_type_name,
						count,
						Point2D(px, py),
						initial_angle,
						::act_pattern_converter(act_pattern_num, { Point2D(arg1, arg2) })
					),
					*player,
					enemies
				)
			);
		*find_vacant_object(enemies) = i;

		*frame_stack.top().get_sp() = BitCalculation::bit_cast<Word>(i->get_id());
		frame_stack.top().advance_sp(1); //Push id on the stack.
	}

}
void MyGameProject::PlayGame::user_extention_1(void)
{
	using namespace MyVM;
	using namespace BitCalculation;
	const auto limit1 = bit_cast<jfloat>(*(frame_stack.top().get_sp() - 1));
	const auto limit2 = bit_cast<jfloat>(*(frame_stack.top().get_sp() - 2));
	const jfloat result = gp::safe_rand(limit1, limit2);
	*(frame_stack.top().get_sp() - 2) = bit_cast<Word>(result);
	frame_stack.top().advance_sp(-1);
	++pc;
}
void MyGameProject::PlayGame::user_extention_2(void)
{
	using namespace MyVM;
	using namespace BitCalculation;
	const auto function_selected = bit_cast<MyVM::Byte>(pc[1]);
	jfloat result = 0.f;

	switch (function_selected)
	{
#ifndef GET_RESULT
#define GET_RESULT(NUMBER_OF_SELECTED_FUNCTION, FUNCTION_NAME) \
	case NUMBER_OF_SELECTED_FUNCTION: result = std::FUNCTION_NAME(bit_cast<jfloat>(*(frame_stack.top().get_sp() - 1))); break;
#endif
		GET_RESULT(0, sin)
		GET_RESULT(1, cos)
		GET_RESULT(2, tan)
		GET_RESULT(3, exp)
		GET_RESULT(4, asin)
		GET_RESULT(5, acos)
		GET_RESULT(6, atan)
		GET_RESULT(7, log)
		default: throw std::runtime_error("The function number is not allowed.");
#undef GET_RESULT
	}

	*(frame_stack.top().get_sp() - 1) = bit_cast<Word>(result);

	pc += 2;
}
void MyGameProject::PlayGame::user_extention_3(void)
{
	using namespace MyVM;
	using namespace BitCalculation;

	*(frame_stack.top().get_sp() + 0) = bit_cast<MyVM::Word>(static_cast<jfloat>(player->pos().x()));
	*(frame_stack.top().get_sp() + 1) = bit_cast<MyVM::Word>(static_cast<jfloat>(player->pos().y()));

	frame_stack.top().advance_sp(2);

	++pc;
}

void MyGameProject::PlayGame::user_extention_4(void)
{
	msgbox_ok("user_extention_4 called.");
	++pc;
}

void MyGameProject::PlayGame::user_extention_5(void)
{
	using namespace MyVM;
	*frame_stack.top().get_sp() = BitCalculation::bit_cast<Word>(count);
	frame_stack.top().advance_sp(1);
	++pc;
}

void MyGameProject::PlayGame::user_extention_6(void)
{
	using namespace MyVM;
	using namespace BitCalculation;
	const auto function_selected = bit_cast<MyVM::Byte>(pc[1]);
	jfloat result = 0.f;

	switch (function_selected)
	{
#ifndef GET_RESULT
#define GET_RESULT(NUMBER_OF_SELECTED_FUNCTION, FUNCTION_NAME) \
	case NUMBER_OF_SELECTED_FUNCTION: result = std::FUNCTION_NAME(bit_cast<jfloat>(*(frame_stack.top().get_sp() - 1)), bit_cast<jfloat>(*(frame_stack.top().get_sp() - 2))); break;
#endif
		GET_RESULT(0, atan2)
		default: throw std::runtime_error("The function number is not allowed.");
#undef GET_RESULT
	}

	*(frame_stack.top().get_sp() - 2) = bit_cast<Word>(result);
	frame_stack.top().advance_sp(-1);

	pc += 2;
}
void MyGameProject::PlayGame::user_extention_7(void)
{
	using namespace MyVM;

	auto enemy_type_num =   BitCalculation::bit_cast<jint>  (*(frame_stack.top().get_sp() - 1));

	frame_stack.top().advance_sp(-1);
	++pc;

	std::string enemy_type_name;

	switch (enemy_type_num)
	{
		case 0: enemy_type_name = "BlackWings";  break;
		case 1: enemy_type_name = "MegaCrow";  break;
		case 2: enemy_type_name = "Aya";  break;
		default: throw std::runtime_error("Undefined boss type.");
	}

	if (se_manager)
	{
		auto i =
			(
				*find_vacant_object(bosses) =
				Boss::create
				(
					enemy_type_name,
					bullets,
					items,
					std::bind(std::mem_fn(&SEManager::erect_play_flag_of), se_manager.get(), std::placeholders::_1),
					std::bind(std::mem_fn(&PlayGame::change_play_speed), this, std::placeholders::_1),
					*player
				)
			);
		*find_vacant_object(enemies) = i;

		*frame_stack.top().get_sp() = BitCalculation::bit_cast<Word>(static_cast<jint>(i->get_id()));
		frame_stack.top().advance_sp(1); //Push id on the stack.
	}

}
void MyGameProject::PlayGame::user_extention_8(void)
{
	using namespace MyVM;

	const int id = BitCalculation::bit_cast<jint>(*(frame_stack.top().get_sp() - 1));
	const auto result = is_id_enable(enemies, id);

	if (result) { *(frame_stack.top().get_sp() - 1) = BitCalculation::bit_cast<Word>(static_cast<jint>(1)); }
	else { *(frame_stack.top().get_sp() - 1) = BitCalculation::bit_cast<Word>(static_cast<jint>(0)); } 

	++pc;
}
void MyGameProject::PlayGame::user_extention_9(void)
{
	using namespace MyVM;
	++pc;
}
void MyGameProject::PlayGame::user_extention_10(void)
{
	using namespace MyVM;
	++pc;
}
void MyGameProject::PlayGame::user_extention_11(void)
{
	using namespace MyVM;
	++pc;
}
void MyGameProject::PlayGame::user_extention_12(void)
{
	using namespace MyVM;
	++pc;
}
void MyGameProject::PlayGame::user_extention_13(void)
{
	using namespace MyVM;
	++pc;
}
void MyGameProject::PlayGame::user_extention_14(void)
{
	using namespace MyVM;
	++pc;
}
void MyGameProject::PlayGame::user_extention_15(void)
{
	using namespace MyVM;
	++pc;
}
void MyGameProject::PlayGame::user_extention_16(void)
{
	using namespace MyVM;
	++pc;
}
