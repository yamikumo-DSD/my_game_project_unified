//analyze_ast.cpp

#include "vm/ast.h"
#include "vm/enable_new_feature.h"

std::pair<MyVM::ConstantPool, MyVM::CodeList> MyVM::analyze_ast(const AST::Code& _code)
{
	MyVM::ConstantPool constant_pool;
	MyVM::CodeList code_list;
#ifndef ENABLE_NEW_FEATURE
	constant_pool = _code.constant_pool;
	for (const auto& method : _code.method_list)
	{
		code_list.insert(std::make_pair(method.name, method.imple));
		std::cout << method.name << "'s stack size:" << method.reserved_stack_size << std::endl;
		std::cout << method.name << "'s locals size:" << method.size_of_local_variables << std::endl;
	}
#else
	constant_pool = _code.constant_pool;
	for (const auto& method : _code.method_list)
	{
		code_list.insert(std::make_pair(method.name, MethodInfo(method.reserved_stack_size, method.size_of_local_variables, method.imple)));
		std::cout << method.name << "'s stack size:" << method.reserved_stack_size << std::endl;
		std::cout << method.name << "'s locals size:" << method.size_of_local_variables << std::endl;
	}
#endif

	return std::make_pair(constant_pool, code_list);
}
