//enemy_order_parser.cpp

#include "enemy_order_parser.h"
#include "enemy_order.h"
#include "enemy_act_pattern.h"
#include "play_game.h"
#include "order_update.h"

#include "grammar_check.h"

#ifdef GRAMMAR_CHECK
	#include <boost/lexical_cast.hpp>
	#include "msgbox.h"
#endif

MyGameProject::Converter::Processor::Processor(CodeList& _code_list)
	:code_list(_code_list)
{}

void MyGameProject::Converter::Processor::process(const AST::OrderList& _order_list)
{
	for(const auto& order : _order_list)
	{
		(*this)(order);
	}
}
void MyGameProject::Converter::Processor::operator()(const AST::EnemyOrder& _order)
{
#ifdef GRAMMAR_CHECK
	msgbox_ok((std::string("Converter::Processor::operator(const AST::EnemyOrder&): ") + boost::lexical_cast<std::string>(_order.act_pattern.pattern_number)).c_str());
#endif
	code_list.push_front
	(
		CreateMob
		(
			_order.name,
			_order.register_time,
			Point2D(static_cast<Real>(_order.initial_pos.x), static_cast<Real>(_order.initial_pos.y)),
			_order.initial_angle,
#ifdef NEW_ORDER_FORM_TEST
			_order.act_pattern.pattern_number,
			[&](void)->std::vector<Point2D>
			{
				std::vector<Point2D> result;
				for (const auto& arg : _order.act_pattern.arguments)
				{
					result.push_back(Point2D(static_cast<Real>(arg.x), static_cast<Real>(arg.y)));
				}
				return result;
			}()
#else
			_order.act_pattern
#endif
		)
	);
}
void MyGameProject::Converter::Processor::operator()(const AST::BossOrder& _order)
{
	code_list.push_front
	(
		CreateBoss
		({
			_order.name,
			_order.register_time,
		})
	);
}
void MyGameProject::Converter::Processor::operator()(const AST::Order& _order)
{
	boost::apply_visitor(*this,*(_order.argument));
}
