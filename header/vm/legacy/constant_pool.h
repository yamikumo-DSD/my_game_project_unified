//constant_pool.h

#pragma once

#include <boost/variant.hpp>
#include <vector>
#include <cstdint>
#include "vm/global_def.h"
#include <string>

namespace MyVM
{
	using boost::get;
	struct SymbolicReference
	{
		std::string type_of_returned_value;
		std::string method_name;
		std::vector<std::string> type_of_arguments;
		SymbolicReference
		(
			const std::string& _type_of_returned_value,
			const std::string& _method_name,
			const std::initializer_list<std::string>& _type_of_arguments
		) :method_name(_method_name), type_of_returned_value(_type_of_returned_value), type_of_arguments(_type_of_arguments) {}
		SymbolicReference(void) {}
	};
	using Unit = boost::variant<jbool, jint, jfloat, SymbolicReference>;
	using ConstantPool = std::vector<Unit>;
}