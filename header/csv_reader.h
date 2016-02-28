//csv_reader.h

#ifndef __CSV_READER_H
#define __CSV_READER_H

#include <boost/lexical_cast.hpp>
#include <regex> 
#include <utility>

namespace MyGameProject
{
	namespace CSV
	{
		template<class Container>
		Container read_line(std::string _line,char _delimiter = ',')
		{
			using DataType = typename Container::iterator::value_type;

			if (*(--_line.cend()) != _delimiter){ _line += _delimiter; }
			for (auto i = _line.begin(); i != _line.end() - 1; ++i)
			{
				if (*i == _delimiter && *(i + 1) == _delimiter)
				{
					i = _line.insert(i + 1,'0');
				}
			}

			auto iter = _line.cbegin();
			auto end = _line.cend();

			std::smatch results;

			std::string str = std::string(R"(([^)") + _delimiter + std::string(R"(]*)") + _delimiter + std::string(R"())");
			//std::regex re(R"([^,]*,)");
			std::regex re(str);

			Container data;

			while (std::regex_search(iter, end, results, re))
			{
				std::string each_data = results.str();
				*(--each_data.end()) = '\0'; //Remove delimiter
				try
				{
					data.push_back(boost::lexical_cast<DataType>(each_data.c_str()));
				}
				catch (boost::bad_lexical_cast&)
				{
					throw std::runtime_error("The line possiblly contains invalid values.(fail to boost::lexical_cast)");
				}
				iter = results[0].second;
			}

			return data;
		}

		template<int N,class First,class Second, class... Body>
		std::tuple<First,Second, Body...> line_to_tuple(std::vector<std::string> _data)
		{
			_data.resize(N);
			auto converted_data = std::make_tuple(boost::lexical_cast<First>(_data[_data.size() - sizeof...(Body) - 2]));
			return std::tuple_cat(converted_data, line_to_tuple<N,Second,Body...>(_data));
		}

		template<int N,class Type>
		std::tuple<Type> line_to_tuple(std::vector<std::string> _data)
		{
			auto converted_data = std::make_tuple(boost::lexical_cast<Type>(_data[_data.size() - 1]));
			return converted_data;
		}

		template<class... Types>
		std::tuple<Types...> string_vec_to_tuple(std::vector<std::string> _data)
		{
			return line_to_tuple<sizeof...(Types), Types...>(_data);
		}

		template<class... Types>
		std::tuple<Types...> line_to_tuple(std::string _line,char _delimiter = ',')
		{
			auto data = read_line<std::vector<std::string>>(_line,_delimiter);
			return string_vec_to_tuple<Types...>(data);
		}
	
	}
}

#endif
