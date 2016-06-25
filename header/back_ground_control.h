//back_ground_control.h

#pragma once

#include <boost/optional.hpp>
#include <queue>

namespace MyGameProject
{
	class BackGroundControl
	{
	public:
		using Signal = int;
		using OptionalSignal = boost::optional<Signal>;
		using SignalHandler = std::function<void(Signal)>;
		using SignalQueue = std::queue<Signal>;

		//Helper class.
		//Provide interface for continuous signal receiving.
		class SignalSender
		{
		public:
			virtual BackGroundControl::OptionalSignal notify(void) const noexcept = 0;
		};
	private:
		SignalQueue signals;
		virtual void signal_handler(const Signal& _signal) noexcept = 0;
		virtual void update(void) noexcept = 0;
	public:
		BackGroundControl(void) noexcept
			: signals()
		{}
		//Handle and flush all signals on the queue, and update.
		void apply(void) noexcept
		{
			while (!signals.empty())
			{
				signal_handler(signals.front());
				signals.pop();
			}
			update();
		}
		//For continuous signal receiving.
		void wait_signal(const OptionalSignal& _opt_signal) noexcept
		{
			if (_opt_signal) { signals.push(*_opt_signal); }
		}
		//For occational signal receiving.
		void catch_signal(Signal _signal) noexcept
		{
			signals.push(_signal);
		}
		virtual void draw(void) const noexcept = 0;
		virtual ~BackGroundControl(void) noexcept = default;

	};//class BackGroundControl
}//namespace MyGameProject
