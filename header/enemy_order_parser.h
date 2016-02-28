//enemy_order_parser.h

#pragma once

#include "qi_aliases.h"
#include "smart_ptr.h"
#include "code_definition.h"
#include "order_update.h"

#include "grammar_check.h"

#ifdef GRAMMAR_CHECK
	#include <boost/phoenix.hpp>
	#include "msgbox.h"
	#include <boost/lexical_cast.hpp>
	template<class ValType>
	inline void output(ValType _msg)
	{
		auto msg = boost::lexical_cast<std::string>(_msg);
		msgbox_ok(msg.c_str());
	}
	namespace Phx = boost::phoenix;
#endif

namespace MyGameProject
{
	namespace AST
	{
		enum class OrderClass
		{
			SUMMON_MOB,
			SUMMON_BOSS,
		};

		struct PointLiteral
		{
			double x, y; 
		};
#ifdef NEW_ORDER_FORM_TEST
		struct EnemyActPattern
		{
			int pattern_number;
			std::vector<PointLiteral> arguments;
		};

		struct EnemyOrder
		{
			std::string name;
			int register_time;
			PointLiteral initial_pos;
			double initial_angle;
			EnemyActPattern act_pattern;
		};
#else
		struct EnemyOrder
		{
			std::string name;
			int register_time;
			PointLiteral initial_pos;
			double initial_angle;
			int act_pattern;
		};
#endif

		struct BossOrder
		{
			std::string name;
			int register_time;
		};

		using Argument = boost::variant <EnemyOrder,BossOrder> ;

		struct Order
		{
			OrderClass order_class;
			boost::optional<Argument> argument;
		};

		using OrderList = std::vector <Order> ;

		using EnemyOrderList = std::vector<EnemyOrder>;
	}
}

BOOST_FUSION_ADAPT_STRUCT
(
 	MyGameProject::AST::PointLiteral,
	(double, x)
	(double, y)
)
#ifdef NEW_ORDER_FORM_TEST
	BOOST_FUSION_ADAPT_STRUCT
	(
		MyGameProject::AST::EnemyOrder,
		(std::string, name)
		(int, register_time)
		(MyGameProject::AST::PointLiteral, initial_pos)
		(double, initial_angle)
		(MyGameProject::AST::EnemyActPattern, act_pattern)
	)
#else
	BOOST_FUSION_ADAPT_STRUCT
	(
		MyGameProject::AST::EnemyOrder,
		(std::string, name)
		(int, register_time)
		(MyGameProject::AST::PointLiteral, initial_pos)
		(double, initial_angle)
		(int, act_pattern)
	)
#endif

BOOST_FUSION_ADAPT_STRUCT
(
	MyGameProject::AST::BossOrder,
	(std::string, name)
	(int, register_time)
)

BOOST_FUSION_ADAPT_STRUCT
(
	MyGameProject::AST::Order,
	(MyGameProject::AST::OrderClass, order_class)
	(boost::optional<MyGameProject::AST::Argument>, argument)
)

BOOST_FUSION_ADAPT_STRUCT
(
	MyGameProject::AST::EnemyActPattern,
	(int, pattern_number)
	(std::vector<MyGameProject::AST::PointLiteral>, arguments)
)

namespace MyGameProject
{
	namespace Parser
	{
		struct OrderClass : qi::symbols<char,AST::OrderClass>
		{
			OrderClass(void)
			{
				add("summon_mob", AST::OrderClass::SUMMON_MOB)
					("summon_boss", AST::OrderClass::SUMMON_BOSS);
			}
		};

		template<class Iterator>
		class PointLiteral : public qi::grammar<Iterator,AST::PointLiteral(void),ascii::space_type>
		{
		private:
			qi::rule<Iterator, AST::PointLiteral(void), ascii::space_type> body;
		public:
			PointLiteral(void) :PointLiteral::base_type(body)
			{
				body = 
					qi::lit('(') >> qi::double_ >> qi::lit(',') >> qi::double_ >> qi::lit(')');
			}
		};

		template<class Iterator>
		class Order : public qi::grammar<Iterator, AST::Order(void), ascii::space_type>
		{
		private:
			qi::rule<Iterator, AST::Order(void), ascii::space_type> body;
			qi::rule<Iterator, AST::EnemyOrder(void), ascii::space_type> enemy_order;
			qi::rule<Iterator, AST::BossOrder(void), ascii::space_type> boss_order;
			OrderClass order_class;
			qi::rule<Iterator, std::string(void)> quoted_string;
			PointLiteral<Iterator> point;
			qi::rule<Iterator, double(void),ascii::space_type> angle;
			qi::rule<Iterator, AST::PointLiteral(void), ascii::space_type> initial_pos;
#ifdef NEW_ORDER_FORM_TEST
			qi::rule<Iterator, AST::EnemyActPattern(void), ascii::space_type> enemy_act_pattern;
#endif
		public:
			Order(void) :Order::base_type(body)
			{
				body = qi::lit('[') >> order_class >> qi::lit(':')  >> -(enemy_order | boss_order) >> qi::lit(']');

#ifdef NEW_ORDER_FORM_TEST
#ifdef GRAMMAR_CHECK
				enemy_act_pattern = qi::lit('(') >> qi::int_ >> *(qi::lit(',') >> point) >> qi::lit(')');
#else
				enemy_act_pattern = qi::lit('(') >> qi::int_ >> *(qi::lit(',') >> point) >> qi::lit(')');
#endif

#endif

				enemy_order = 
					qi::lit('{') >>
						quoted_string >> qi::lit(',') >> //Enemy class.
						qi::int_ >> qi::lit(',') >>      //Count at which the enemy appear.
						initial_pos >> qi::lit(',') >>   //Coordinate where the enemy appear.
						angle >> qi::lit(',') >>         //Angle.
#ifdef NEW_ORDER_FORM_TEST
#ifndef GRAMMAR_CHECK
						enemy_act_pattern >>
#else
						enemy_act_pattern[Phx::bind(&output<std::string>, Phx::val(std::string("paser: "))),Phx::bind(&output<int>, Phx::bind(&AST::EnemyActPattern::pattern_number, qi::_1))] >>	
#endif
#else
						qi::int_ >>                      //Enemy action pattern.
#endif
					qi::lit('}');
				boss_order =
					qi::lit('{') >>
						quoted_string >> qi::lit(',') >>
						qi::int_ >>
					qi::lit('}');

				quoted_string = qi::lexeme[qi::lit('"') > +qi::alpha > qi::lit('"')];
				angle = qi::double_[qi::_val = qi::_1] | qi::lit("unused")[qi::_val = 0.0];
				initial_pos = point[qi::_val = qi::_1] | qi::lit("unused")[qi::_val = AST::PointLiteral({0,0})];
			}
		};

		template<class Iterator>
		class OrderList :  public qi::grammar<Iterator,AST::OrderList(void),ascii::space_type>
		{
		private:
			qi::rule<Iterator, AST::OrderList(void), ascii::space_type> body;
			Order<Iterator> order;
		public:
			OrderList(void):OrderList::base_type(body)
			{
				body = *order;
			}
		};
	}
}

namespace MyGameProject
{
	namespace Converter
	{
		class Processor : public boost::static_visitor<void>
		{
		private:
			CodeList& code_list;
		public:
			Processor(CodeList& _code_list);
			void process(const AST::OrderList& _order_list);
			void operator()(const AST::EnemyOrder& _enemy_order);
			void operator()(const AST::BossOrder& _boss_order);
			void operator()(const AST::Order& _order);
		};
	}
}
