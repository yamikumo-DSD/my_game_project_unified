//code_list.h

#pragma once

#include "global_def.h"
#include <string>
#include <map>
#include "enable_new_feature.h"

namespace MyVM
{
#ifndef ENABLE_NEW_FEATURE
	using CodeList = std::map<std::string, ByteCode>;
#else
	struct MethodInfo
	{
		MethodInfo(int _reserved_stack_size, int _size_of_local_variables, const ByteCode& _bytecode) :reserved_stack_size(_reserved_stack_size), size_of_local_variables(_size_of_local_variables), bytecode(_bytecode) {}
		MethodInfo(void) :reserved_stack_size(0), size_of_local_variables(0), bytecode() {}
		int reserved_stack_size;
		int size_of_local_variables;
		ByteCode bytecode;
	};
	using CodeList = std::map<std::string, MethodInfo>;
#endif
}