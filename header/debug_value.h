//debug_value.h

#pragma once

#include "msgbox.h"
#include <boost/lexical_cast.hpp>

namespace MyGameProject
{
	template<typename T>
	void debug_value(T&& _value)
	{
		try
		{
			msgbox_ok(boost::lexical_cast<std::string>(std::forward<T>(_value)).c_str());
		}
		catch (boost::bad_lexical_cast)
		{
			msgbox_ok("That value cannot convert into the basic string form.");
		}
	}
}
