//load_stage_enemy_list.cpp

#pragma warning(disable: 4819)
#include "play_game.h"
#include <fstream>
#include <exception>
#include "debug_value.h"
#include <boost/format.hpp>
#include "concrete_enemy.h"
#include "enemy_order.h"
#include "shared_object.h"
#include "find_vacant_object.h"
#include "enemy_act_pattern.h"
#include "is_id_enable.h"
#include "se_manager.h"

namespace MyGameProject
{
	namespace Tools
	{
		MobEnemy::ActPattern act_pattern_converter(int _n, const std::vector<Point2D>& _args)
		{
			using namespace MyGameProject;
			switch (_n)
			{
			case 0:   return pattern0;
			case 1:   return pattern1;
			case 2:   return pattern2;
			case 3:   return pattern3;
			case 4:   return pattern4;
			case 5:   return pattern5;
			case 6:   return pattern6;
			case 9:   return pattern9;
			case 10:  return pattern10;
			case 11:  return pattern11;
			case 12:  return pattern12;
			case 13:  return pattern13;
			case 14:  return pattern14;
			case 15:  return pattern15;
			case 16:  return pattern16;
			case 17:  return pattern17;
			case 18:  return pattern18;
			case 19:  return pattern19;
			case 20:  return pattern20;
			case 21:  return pattern21;
			case 100: return pattern100(_args);
			case 101: return pattern101(_args);
			case 102: return pattern102(_args);
			case 103: return pattern103(_args);
			case 104: return pattern104(_args);
			case 105: return pattern105(_args);
			case 106: return pattern106(_args);
			case 107: return pattern107(_args);
			case 108: return pattern108(_args);
			case 109: return pattern109(_args);
			case 110: return pattern110(_args);
			case 111: return pattern111(_args);
			case 112: return pattern112(_args);
			case 113: return pattern113(_args);
			case 114: return pattern114(_args);
			case 115: return pattern115(_args);
			case 116: return pattern116(_args);
			case 117: return pattern117(_args);
			case 118: return pattern118(_args);
			case 119: return pattern119(_args);
			case 120: return pattern120(_args);
			case 121: return pattern121(_args);
			case 122: return pattern122(_args);
			case 123: return pattern123(_args);
			case 124: return pattern124(_args);
			case 125: return pattern125(_args);
			case 126: return pattern126(_args);
			case 127: return pattern127(_args);
			case 128: return pattern128(_args);
			default:  return pattern0;
			}
		}
	}

	void PlayGame::load_stage_enemy_list(int _stage_num)
	{
			//Open lua.
			const auto file_name{(boost::format("../../data/map/script_stage%d.lua") %_stage_num).str()};
			L = decltype(L)(luaL_newstate(), &lua_close);

			luaL_openlibs(L.get());

			//Enable multitasking.
			tasks.initialize(L.get());

			auto add_cfunc
			{
				[this, L = L.get()](lua_CFunction _cfunc, const char* _name)
				{
					lua_pushlightuserdata(L, this);
					lua_pushcclosure(L, _cfunc, 1);
					lua_setglobal(L, _name);
				}
			};
#ifndef ENABLE_THIS_
#define ENABLE_THIS_ auto this_{ static_cast<PlayGame*>(lua_touserdata(_state, lua_upvalueindex(1))) };
#endif

			add_cfunc
			(
				[](lua_State* _state)
				{
					ENABLE_THIS_
					lua_pushinteger(_state, this_->count);
					return 1;
				}, "count"
			);
			add_cfunc
			(
				[](lua_State* _state)
				{
					ENABLE_THIS_
					std::string type{lua_tostring(_state, 1)};
					auto x{static_cast<Real>(lua_tonumber(_state, 2))};
					auto y{static_cast<Real>(lua_tonumber(_state, 3))};
					auto act_pattern_num{static_cast<int>(lua_tointeger(_state, 4))};

					std::vector<Real> args; //arguments for act pattern
					for (int i = 5; i <= lua_gettop(_state); ++i)
					{
						args.emplace_back(lua_tonumber(_state, i));
					}

					const auto modified_size{(args.size() + 1) / 2};
					std::vector<Point2D> args_trully_passed_to_enemy_order(modified_size);
					for (int i = 0; i != 2 * modified_size; i += 2)
					{
						args_trully_passed_to_enemy_order[i / 2].x(static_cast<Real>(args[i    ]));
						args_trully_passed_to_enemy_order[i / 2].y(static_cast<Real>(args[i + 1]));
					}

					std::shared_ptr<MobEnemy> new_mob;
					auto order
					{
						EnemyOrder::make_order
						(
							type, this_->count, Point2D(x, y), 0,
							Tools::act_pattern_converter(act_pattern_num, std::move(args_trully_passed_to_enemy_order))
						)
					};

					if      (type == "Bomber"            ) { new_mob = std::make_unique<Bomber           >(SharedObject::bullets(), *this_->se_manager, std::move(order), *this_->player); }
					else if (type == "Crow"              ) { new_mob = std::make_unique<Crow             >(SharedObject::bullets(), *this_->se_manager, std::move(order), *this_->player); }
					else if (type == "EmergeFromCloud"   ) { new_mob = std::make_unique<EmergeFromCloud  >(SharedObject::bullets(), *this_->se_manager, std::move(order), *this_->player); }
					else if (type == "Crow2"             ) { new_mob = std::make_unique<Crow2            >(SharedObject::bullets(), *this_->se_manager, std::move(order), *this_->player); }
					else if (type == "Crow3"             ) { new_mob = std::make_unique<Crow3            >(SharedObject::bullets(), *this_->se_manager, std::move(order), *this_->player); }
					else if (type == "Gogyo"             ) { new_mob = std::make_unique<Gogyo            >(SharedObject::bullets(), *this_->se_manager, std::move(order), *this_->player); }
					else if (type == "ForcedCrow"        ) { new_mob = std::make_unique<ForcedCrow       >(SharedObject::bullets(), *this_->se_manager, std::move(order), *this_->player); }
					else if (type == "RedEye"            ) { new_mob = std::make_unique<RedEye           >(SharedObject::bullets(), *this_->se_manager, std::move(order), *this_->player); }
					else if (type == "Amenbo"            ) { new_mob = std::make_unique<Amenbo           >(SharedObject::bullets(), *this_->se_manager, std::move(order), *this_->player); }
					else if (type == "Dragonfly"         ) { new_mob = std::make_unique<Dragonfly        >(SharedObject::bullets(), *this_->se_manager, std::move(order), *this_->player); }
					else if (type == "LaserLauncher"     ) { new_mob = std::make_unique<LaserLauncher    >(SharedObject::bullets(), *this_->se_manager, std::move(order), *this_->player); }
					else if (type == "Bomber"            ) { new_mob = std::make_unique<Bomber           >(SharedObject::bullets(), *this_->se_manager, std::move(order), *this_->player); }
					else if (type == "Helli"             ) { new_mob = std::make_unique<Helli            >(SharedObject::bullets(), *this_->se_manager, std::move(order), *this_->player); }
					else if (type == "Fairy_1"           ) { new_mob = std::make_unique<Fairy_1          >(SharedObject::bullets(), *this_->se_manager, std::move(order), *this_->player); }
					else if (type == "Fairy_2"           ) { new_mob = std::make_unique<Fairy_2          >(SharedObject::bullets(), *this_->se_manager, std::move(order), *this_->player); }
					else if (type == "Fairy_3"           ) { new_mob = std::make_unique<Fairy_3          >(SharedObject::bullets(), *this_->se_manager, std::move(order), *this_->player); }
					else if (type == "Fairy_3"           ) { new_mob = std::make_unique<Fairy_3          >(SharedObject::bullets(), *this_->se_manager, std::move(order), *this_->player); }
					else if (type == "MasterAndSlaves"   ) { new_mob = std::make_unique<Master           >(SharedObject::bullets(), *this_->se_manager, std::move(order), *this_->player, SharedObject::enemies()); }
					else if (type == "MasterAndSlaves2"  ) { new_mob = std::make_unique<Master2          >(SharedObject::bullets(), *this_->se_manager, std::move(order), *this_->player, SharedObject::enemies()); }
					else if (type == "SmallBattleShip"   ) { new_mob = std::make_unique<SmallBattleShip  >(SharedObject::bullets(), *this_->se_manager, std::move(order), *this_->player, SharedObject::enemies()); }
					else { throw std::runtime_error("Unknown enemy name."); }

					*find_vacant_object(SharedObject::mob_enemies()) = new_mob;
					*find_vacant_object(SharedObject::enemies()) = new_mob;

					lua_pushinteger(_state, new_mob->get_id());

					return 1;
				}, "new_mob"
			);
			add_cfunc
			(
				[](lua_State* _state)
				{
					ENABLE_THIS_
					std::string type{lua_tostring(_state, 1)};

					std::shared_ptr<Boss> new_boss;

					if(type == "BlackWings") 
					{
						new_boss = std::make_unique<BlackWings>
						(
							SharedObject::bullets(), SharedObject::items(),
							[&se_manager = this_->se_manager](const std::string& _file){se_manager->erect_play_flag_of(_file);}, 
							[&this_](float _speed) {this_->change_play_speed(_speed); },
							*this_->player
						);
					}
					else if(type == "MegaCrow") 
					{
						new_boss = std::make_unique<MegaCrow>
						(
							SharedObject::bullets(), SharedObject::items(),
							[&se_manager = this_->se_manager](const std::string& _file){se_manager->erect_play_flag_of(_file);}, 
							[&this_](float _speed) {this_->change_play_speed(_speed); },
							*this_->player
						);
					}
					else if(type == "Aya") 
					{
						new_boss = std::make_unique<Aya>
						(
							SharedObject::bullets(), SharedObject::items(),
							[&se_manager = this_->se_manager](const std::string& _file){se_manager->erect_play_flag_of(_file);}, 
							[&this_](float _speed) {this_->change_play_speed(_speed); },
							*this_->player
						);
					}
					else if(type == "Brades") 
					{
						new_boss = std::make_unique<Brades>
						(
							SharedObject::bullets(), SharedObject::items(),
							[&se_manager = this_->se_manager](const std::string& _file){se_manager->erect_play_flag_of(_file);}, 
							[&this_](float _speed) {this_->change_play_speed(_speed); },
							*this_->player
						);
					}
					else { throw std::runtime_error("Unknown enemy name."); }

					SharedObject::boss() = new_boss;
					*find_vacant_object(SharedObject::enemies()) = new_boss;

					lua_pushinteger(_state, new_boss->get_id());

					return 1;
				}, "new_boss"
			);
			add_cfunc
			(
				[](lua_State* _state)
				{
					const auto id{static_cast<int>(lua_tointeger(_state, 1))};
					lua_pushboolean(_state, static_cast<int>(is_id_enable(SharedObject::enemies(), id)));
					return 1;
				}, "is_id_enable"
			);
			add_cfunc
			(
				[](lua_State* _state)
				{
					if (auto str = lua_tostring(_state, 1)) //lua_tostring returns NULL if the value is not string or number.
					{ debug_value(str); }
					else
					{ debug_value("The value is of unprintable type."); }
					return 0;
				}, "msgbox"
			);
			add_cfunc
			(
				[](lua_State* _state)
				{
					ENABLE_THIS_
					const auto signal{static_cast<int>(lua_tointeger(_state, 1))};
					this_->back_ground_control->catch_signal(signal);
					return 0;
				}, "notify_bg"
			);
			add_cfunc
			(
				[](lua_State* _state)
				{
					ENABLE_THIS_
					const auto stage_num{static_cast<int>(lua_tointeger(_state, 1))};
					switch (stage_num)
					{
						case 0: this_->stage_proceeding_signal = StageProceedingSignal::GOTO_STAGE0; break;
						case 3: this_->stage_proceeding_signal = StageProceedingSignal::GOTO_STAGE3; break;
						default: throw std::runtime_error((boost::format("Stage%d has not been implemented yet.")).str());
					}
					return 0;
				}, "go_to_stage"
			);
			add_cfunc
			(
				[](lua_State* _state)
				{
					ENABLE_THIS_
					this_->stage_proceeding_signal = StageProceedingSignal::GOTO_NEXT_STAGE;
					return 0;
				}, "go_to_next_stage"
			);
			add_cfunc
			(
				[](lua_State* _state)
				{
					ENABLE_THIS_
					lua_newtable(_state);
					lua_pushnumber(_state, this_->player->pos().x());
					lua_setfield(_state, -2, "x");
					lua_pushnumber(_state, this_->player->pos().y());
					lua_setfield(_state, -2, "y");
					return 1;
				}, "player_data"
			);
			add_cfunc
			(
				[](lua_State* _state)
				{
					Real v[4];

					for (int i = 0; i != 4; ++i)
					{
						v[i] = lua_tonumber(_state, i + 1);
					}

					lua_settop(_state, 0);

					lua_pushnumber(_state, angle_of(Point2D(v[2], v[3]) - Point2D(v[0], v[1])));
					return 1;
				}, "angle_of" //angle_of(from_x, from_y, to_x, to_y)
			);
			add_cfunc
			(
				[](lua_State* _state)
				{
					ENABLE_THIS_
					auto path = lua_tostring(_state, 1);
					this_->se_manager_handle()->erect_play_flag_of(path);
					lua_settop(_state, 0);
					return 0;
				}, "play_se"
			);
			add_cfunc
			(
				[](lua_State* _state)
			{
				ENABLE_THIS_
					lua_settop(_state, 0);
				lua_pushinteger(_state, this_->tasks.count());
				return 1;
			}, "count_task"
			);
			add_cfunc
			(
				[](lua_State* _state)
				{
					ENABLE_THIS_
					lua_settop(_state, 0);
					lua_pushinteger(_state, this_->tasks.count());
					return 1;
				}, "count_task"
			);

			//Register this pointer.
			lua_pushlightuserdata(L.get(), this);
			lua_setglobal(L.get(), "this_game");

			adapt_class(L.get());

			while (luaL_dofile(L.get(), file_name.c_str()))
			{
				auto msg{lua_tostring(L.get(), -1)};
				debug_value(msg);
				debug_value("The script involves grammatical error.\n Please revise it.");
			}

			this_thread = lua_newthread(L.get());
			lua_getglobal(this_thread, "main_loop");

	} //PlayGame::lod_stage_enemy_list
} //namespace MyGameProject
