//preprocessor.h

#pragma once

#include <string>
#include <algorithm>

namespace MyVM
{
	template<class String>
	auto find_in_range(typename String::iterator first, typename String::iterator last, const String& _str)
	{
		return std::search(first, last, std::begin(_str), std::end(_str));
	}

	//String class must be a standard string class.
	template<class String>
	String preprocess(String _code)
	{
		//Remove comment.
		typename String::iterator itr;
		while ((itr = find_in_range(_code.begin(), _code.end(), String("//"))) != _code.end())
		{
			_code.replace(itr, find_in_range(itr, _code.end(), String("\n")), "");
		}
		return _code;
	}
}
