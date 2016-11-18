//pattern114.cpp

#include "pattern114.h"
#include <typeinfo>
#include "bomber.h"
#include "debug_value.h"

#define DEFINE_ENEMY_PTR(L) auto enemy{ static_cast<Enemy*>(lua_touserdata(L, lua_upvalueindex(1))) };

namespace MyGameProject
{
	using P = Point2D;
	//struct Pattern114 final : Pattern
	//{
	//	Bomber* bomber;
	//	OVERRIDE_PARENTHESIS
	//	{
	//		ALIASES;
	//		if (count == 0)
	//		{
	//			bomber = dynamic_cast<Bomber*>(&_enemy);
	//		}
	//
	//		if (bomber)
	//		{
	//			if (count % 200 == 0) { bomber->open_shell(); }
	//			else if (count % 200 == 100) { bomber->close_shell(); }
	//		}
    //
	//		if (count % 200 > 60 && count % 200 < 100)
	//		{
	//			if (count % 3 == 0)
	//			{
	//				auto dir{gp::safe_rand<Real>(0, two_pi<Real>())};
	//				register_bullet("B2", _enemy, player, pos + 50 * P(cos(dir), sin(dir)), dir, straight_course(5));
	//			}
	//		}
	//		if (count % 300 < 150) { pos += P(2, 1); }
	//		else if (count % 300 >= 150) { pos += P(-2, 1); }
	//	};
	//};

	MobEnemy::ActPattern pattern114(const std::vector<Point2D>& _args) 
	{
		if (_args.size() == 1)
		{
			return Pattern114(get_n_th_arg(_args, 0));
		}
		else
		{
			throw std::runtime_error("Pattern 114 needs 1 arguments.");
		}
	}

	void Pattern114::preparation(void)
	{
		using P = Point2D;

		register_enemy_memfunc
		(
			[](lua_State* _state)
			{
				DEFINE_ENEMY_PTR(_state)
				lua_pushinteger(_state, static_cast<lua_Integer>(enemy->get_count()));
				return 1;
			}, "get_count"
		);
		register_enemy_memfunc
		(
			[](lua_State* _state)
			{
				DEFINE_ENEMY_PTR(_state)
				enemy->pos() = P(static_cast<Real>(lua_tonumber(_state, -2)), static_cast<Real>(lua_tonumber(_state, -1)));
				return 0;
			}, "set_pos"
		);
		register_enemy_memfunc
		(
			[](lua_State* _state)
			{
				DEFINE_ENEMY_PTR(_state)
				lua_pushnumber(_state, enemy->pos().x());
				lua_pushnumber(_state, enemy->pos().y());
				return 2;
			}, "get_pos"
		);
		register_enemy_memfunc
		(
			[](lua_State* _state)
			{
				DEFINE_ENEMY_PTR(_state)
				lua_pushboolean(_state, typeid(*enemy) == typeid(Bomber));
				return 1;
			}, "is_bomber"
		);
		register_enemy_memfunc
		(
			[](lua_State* _state)
			{
				DEFINE_ENEMY_PTR(_state)
				auto b{dynamic_cast<Bomber*>(enemy)};
				if (b) { b->open_shell(); }
				return 0;
			}, "open_shell"
		);
		register_enemy_memfunc
		(
			[](lua_State* _state)
			{
				DEFINE_ENEMY_PTR(_state)
				auto b{dynamic_cast<Bomber*>(enemy)};
				if (b) { b->close_shell(); }
				return 0;
			}, "close_shell"
		);
		register_enemy_memfunc
		(
			[](lua_State* _state)
			{
				debug_value("POPUP");
				return 0;
			}, "popup"
		);
		register_free_func
		(
			[](lua_State* _state)
			{
				auto this_{ to_this(_state) };
				this_->print_x();
				return 0;
			}, "print_x"
		);
		apply_script("../../data/script/pattern114.lua", "main_loop");
	}
}