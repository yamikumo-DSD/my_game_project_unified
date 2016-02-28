//ast.h

#pragma once

#include "vm.h"
#include <boost/fusion/include/adapt_struct.hpp>
#include <string>
#include <utility>

namespace MyVM
{
	namespace AST
	{
		using Byte = MyVM::Byte;
		using ByteCode = std::vector<AST::Byte>;
		using Constant = MyVM::Unit;
		using SymbolicReference = MyVM::SymbolicReference;
		using ConstantPool = std::vector<Constant>;

		struct Method
		{
			std::string name;
			ByteCode imple;
			int reserved_stack_size;
			int size_of_local_variables;
		};

		struct Code
		{
			ConstantPool constant_pool;
			std::vector<Method> method_list;
		};
	}

	std::pair<MyVM::ConstantPool, MyVM::CodeList> analyze_ast(const AST::Code& _code);
}

BOOST_FUSION_ADAPT_STRUCT
(
	MyVM::AST::SymbolicReference,
	(std::string, type_of_returned_value)
	(std::string, method_name)
	(std::vector<std::string>, type_of_arguments)
)
BOOST_FUSION_ADAPT_STRUCT
(
	MyVM::AST::Method,
	(std::string, name)
	(MyVM::AST::ByteCode, imple)
	(int, reserved_stack_size)
	(int, size_of_local_variables)
)
BOOST_FUSION_ADAPT_STRUCT
(
	MyVM::AST::Code,
	(MyVM::AST::ConstantPool, constant_pool)
	(std::vector<MyVM::AST::Method>, method_list)
)