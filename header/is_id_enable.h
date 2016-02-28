//is_id_enable.h

#pragma once

#include <vector>
#include "smart_ptr.h"

namespace MyGameProject
{
	class Enemy;

	bool is_id_enable(const std::vector<gp::smart_ptr<Enemy>>& _enemies, int _id_searched);
}