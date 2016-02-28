//is_id_enable.cpp

#include "is_id_enable.h"
#include "enemy.h"
#include <algorithm>

namespace MyGameProject
{
	bool is_id_enable(const std::vector<gp::smart_ptr<Enemy>>& _enemies, int _id_searched)
	{
		return std::find_if
		(
			_enemies.begin(), 
			_enemies.end(), 
			[=](gp::smart_ptr<Enemy> _enemy) 
			{
				if (_enemy && _enemy->get_flag() && _enemy->get_id() == _id_searched) { return true; }
				else { return false; }
			}
		) != _enemies.end();
	}
}