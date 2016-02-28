//express_debug
#pragma once

#include "msgbox.h"
#include <boost/lexical_cast.hpp>
#include <string>

#define EXPRESS(expr) msgbox_ok(boost::lexical_cast<std::string>(expr).c_str())