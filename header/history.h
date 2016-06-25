//history.h

#pragma once

#include <array>
#include <tuple>

namespace MyGameProject
{
	template
	<
		unsigned int DEPTH,
		typename... Information
	>
	class HistoryReserver
	{
		public:
		using value_type = std::tuple<Information...>;
		private:
		std::array<value_type, DEPTH> history_series;
		public:
		struct OutOfRange : std::logic_error { using logic_error::logic_error; };
		explicit HistoryReserver(Information... _initial_info) noexcept
			:history_series()
		{
			for (auto& history : history_series) { history = std::make_tuple(_initial_info...); }
		}
		HistoryReserver(void) noexcept
			: history_series()
		{}
		void initialize(Information... _initial_info) noexcept
		{
			for (auto& history : history_series) { history = std::make_tuple(_initial_info...); }
		}
		void shift_forward(Information... _new_top_information) noexcept
		{
			history_series[0] = std::make_tuple(std::move(_new_top_information)...);
			for (int i = DEPTH - 1; i != 0; --i)
			{ history_series[i] = history_series[i - 1]; ; }
		}
		decltype(auto) operator[](std::size_t _n) const noexcept 
		{
			return history_series[_n];
		}
		decltype(auto) at(std::size_t _n) const
		{
			try
			{
				return history_series.at(_n);
			}
			catch (std::out_of_range)
			{
				throw OutOfRange("Over the number of stocked histories.");
			}
		}
		decltype(auto) begin(void) noexcept { return history_series.begin(); }
		decltype(auto) cbegin(void) noexcept { return history_series.cbegin(); }
		decltype(auto) end(void) noexcept { return history_series.end(); }
		decltype(auto) cend(void) noexcept { return history_series.cend(); }
		decltype(auto) rbegin(void) noexcept { return history_series.rbegin(); }
		decltype(auto) crbegin(void) noexcept { return history_series.crbegin(); }
		decltype(auto) rend(void) noexcept { return history_series.rend(); }
		decltype(auto) crend(void) noexcept { return history_series.crend(); }
		constexpr auto size(void) const { return DEPTH; }
	};

	template<unsigned Index, unsigned int DEPTH, typename... Information>
	decltype(auto) get(const HistoryReserver<DEPTH, Information...>& _reserver) noexcept
	{
		return std::get<Index>(_reserver);
	}
}
