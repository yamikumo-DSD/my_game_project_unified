//preprocessor.h

#pragma once

#include <iostream>
#include <boost/spirit/include/qi.hpp>
#include <boost/algorithm/string.hpp>
#include <regex>

namespace MyVM
{
	template<class String>
	auto find_in_range(typename String::iterator first, typename String::iterator last, const String& _str)
	{
		return std::search(first, last, std::begin(_str), std::end(_str));
	}

	namespace qi = boost::spirit::qi;

	template<class Iterator, class String>
	struct MacroParser : public qi::grammar<Iterator, boost::fusion::vector<String, String>(void), qi::ascii::space_type>
	{
		qi::rule<Iterator, boost::fusion::vector<String, String>(void), qi::ascii::space_type> body;
		qi::rule<Iterator, String(void)> search;
		qi::rule<Iterator, String(void)> format;
		MacroParser(void) :MacroParser::base_type(body)
		{
			body = qi::lit('#') >> "macro" >> search >> format;

			search = (qi::alpha | qi::char_('_')) >> *(qi::alnum | qi::char_('_'));

			format = *(qi::char_ | qi::blank);
		}
	};

	template<class String>
	String preprocess(String _code)
	{
		//Remove comment.
		typename String::iterator itr;
		while ((itr = find_in_range(std::begin(_code), std::end(_code), String("//"))) != _code.end())
		{
			_code.replace(itr, find_in_range(itr, _code.end(), String("\n")), "");
		}

		//Deal with macroes.
		std::vector<String> lines;
		boost::split(lines, _code, boost::is_any_of("\n"));

		namespace fusion = boost::fusion;
		std::vector<fusion::vector<String, String>> macroes; //first:identifier, second:contents

		for (auto& line : lines)
		{
			namespace qi = boost::spirit::qi;
			fusion::vector<String, String> temp;
			auto result = qi::phrase_parse
				(
					line.begin(),
					line.end(),
					MacroParser<typename String::iterator, String>(),
					qi::ascii::space,
					temp
				);
			if (result)
			{
				macroes.push_back(temp);
				line = String("");
			}
		}

		_code = String("");
		for (const auto& line : lines)
		{
			(_code += line) += '\n';
		}

		for (auto& macro : macroes)
		{
			boost::replace_all(fusion::at_c<1>(macro), String("\\"), String("\n"));

			auto search = fusion::at_c<0>(macro);
			String expr = String("\\b") + "(" + search + ")" + String("\\b");
			std::regex rex(expr);
			_code = std::regex_replace(_code, rex, fusion::at_c<1>(macro));
		}

		return _code;
	}
}