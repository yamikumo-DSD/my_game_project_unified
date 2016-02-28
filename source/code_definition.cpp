//code_definition.cpp

#include "code_definition.h"

#include "grammar_check.h"
#ifdef GRAMMAR_CHECK
	#include "msgbox.h"
	#include <boost/lexical_cast.hpp>
#endif

MyGameProject::CreateMob::CreateMob
(
const std::string& _name,
int _register_time,
const Point2D& _initial_pos,
double _initial_angle,
int _pattern_number,
const std::vector<Point2D>& _arguments
) 
:name(_name),
register_time(_register_time),
x(_initial_pos.x()),
y(_initial_pos.y()),
initial_angle(_initial_angle)
{
	act_pattern.pattern_number = _pattern_number;
	act_pattern.arguments = _arguments;
#ifdef GRAMMAR_CHECK
	msgbox_ok((std::string("CreateMob(args...): ") + boost::lexical_cast<std::string>(act_pattern.pattern_number)).c_str());
#endif
}

MyGameProject::CreateMob::CreateMob
(
const std::string& _name,
int _register_time,
const Point2D& _initial_pos,
double _initial_angle,
int _pattern_number
)
:name(_name),
register_time(_register_time),
x(_initial_pos.x()),
y(_initial_pos.y()),
initial_angle(_initial_angle)
{
	act_pattern.pattern_number = _pattern_number;
}

