//debug_value.h

#pragma once

#include "msgbox.h"
#include <boost/lexical_cast.hpp>

namespace MyGameProject
{
	template<class T>
	void debug_value(const T& _value)
	{
		msgbox_ok(boost::lexical_cast<std::string>(_value).c_str());
	}
}
