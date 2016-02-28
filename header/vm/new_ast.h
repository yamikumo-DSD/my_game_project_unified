//new_ast.h

#pragma once

#include <string>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/variant.hpp>
#include "global_def.h"
#include <boost/optional.hpp>

namespace MyVM
{
	namespace AST
	{
		struct Instruction
		{
			std::string name;
		};

		using Operand = boost::variant<jint, jfloat, std::string>;

		struct Mnemonic
		{
			Instruction instruction;
			std::vector<Operand> operand_list;
			boost::optional<std::string> tag;
		};

		struct Function
		{
			std::string name;
			int reserved_stack_size;
			int size_of_local_variables;
			std::string type_of_returned_value;
			std::vector<std::string> type_of_arguments;
			std::vector<Mnemonic> definition;
		};

		using Program = std::vector<Function>;
	}
}

#ifndef ADAPT
#define ADAPT(NAME, ATTRIBUTES) BOOST_FUSION_ADAPT_STRUCT(NAME, ATTRIBUTES)
#endif

ADAPT
(
	MyVM::AST::Instruction,
	(std::string, name)
)

ADAPT
(
	MyVM::AST::Mnemonic,
	(MyVM::AST::Instruction, instruction)
	(std::vector<MyVM::AST::Operand>, operand_list)
	(boost::optional<std::string>, tag)
)

ADAPT
(
	MyVM::AST::Function,
	(std::string, name)
	(int, reserved_stack_size)
	(int, size_of_local_variables)
	(std::string, type_of_returned_value)
	(std::vector<std::string>, type_of_arguments)
	(std::vector<MyVM::AST::Mnemonic>, definition)
)

#undef ADAPT