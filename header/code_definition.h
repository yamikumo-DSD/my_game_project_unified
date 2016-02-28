//code_definition.h

#pragma once

#include <string>
#include <boost/variant.hpp>
#include <list>
#include "order_update.h"
#include <vector>
#include "mathematics.h"

namespace MyGameProject
{
#ifdef NEW_ORDER_FORM_TEST
	struct EnemyActPattern
	{
		int pattern_number;
		std::vector<Point2D> arguments;
	};
#endif

	struct CreateMob
	{
#ifdef NEW_ORDER_FORM_TEST
		CreateMob
			(
			const std::string& _name,
			int _register_time,
			const Point2D& _initial_pos,
			double _initial_angle,
			int _pattern_number
			);
		CreateMob
			(
			const std::string& _name,
			int _register_time,
			const Point2D& _initial_pos,
			double _initial_angle,
			int _pattern_number,
			const std::vector<Point2D>& _arguments
			);
#endif
		std::string name;
		int register_time;
		double x, y;
		double initial_angle;
#ifdef NEW_ORDER_FORM_TEST
		EnemyActPattern act_pattern;
#else
		int act_pattern;
#endif
	};

	struct CreateBoss
	{
		std::string name;
		int register_time;
	};

	using Code = boost::variant <CreateMob, CreateBoss>;
	using CodeList = std::list <Code>;
}