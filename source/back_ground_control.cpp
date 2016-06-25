//back_ground_control.cpp

#if 0

#include "back_ground_control.h"
#include "boss.h"
#include "debug.h"

namespace MyGameProject
{
	using Signal = BackGroundControl::Signal;

	void BackGroundControl::register_boss(std::shared_ptr<Boss> _boss)
	{
		//be careful that initial value of current_boss_number is -1, not 0.
		if (current_boss_number >= total_boss_number - 1)
		{
			throw BossNumberMismatch();
		}
		else
		{
			++current_boss_number;
			boss = _boss;
		}
	}

	Signal BackGroundControl::get_signal(void) noexcept
	{
		return signal;
	}

	void BackGroundControl::catch_signal(Signal _signal)
	{
		if (boss.expired() || (!boss.expired() && !boss.lock()->get_flag())) //if boss is unavailable
		{
			if (_signal)
			{
				debug_value("Catch signal"); 
				debug_value(*_signal); 
			}
			signal = std::move(_signal);
		}
		else
		{
			if (_signal)
			{
				signal = boost::none;
				throw SignalIgnored(*_signal); 
			}
		}
	}

	void BackGroundControl::update(void)
	{
		if (!boss.expired() && boss.lock()->get_flag()) //if boss is exsisting
		{
			signal = boss.lock()->notify();
			if (signal)
			{
				debug_value("Receive signal from boss obj");
				debug_value(*signal);
			}
			boss_back_ground_updaters[current_boss_number]();
		}
		else //if boss is not exsisting
		{
			normal_back_ground_updater();
		}
	}

	BackGroundControl::~BackGroundControl(void) = default;
}//namespace MyGameProject

#endif