//new_ast_analyzer.h

#pragma once

#include "new_ast.h"
#include "vm.h"
#include <utility>

namespace MyVM
{
	class ASTAnalyzer
	{
	private:
		ConstantPool constant_pool;
		CodeList code_list;
		std::string current_function_name;
		Short constant_index;
		std::map<std::string, Short> function_index_map;
		//tuple<funtion name, tag, data...>
		std::vector<std::tuple<std::string, std::string, Byte, Byte>> taged_mnemonic_list_for_goto;
		//tuple<funtion name, tag, data...>
		std::vector<std::tuple<std::string, std::string, Byte, Byte, Byte, Byte>> taged_mnemonic_list_for_goto_w;
		void analyze_mnemonic(const AST::Mnemonic& _mnemonic);
		void analyze_function(const AST::Function& _function);
	public:
		ASTAnalyzer(void);
		void analyze(const AST::Program& _program);
		ConstantPool get_constant_pool(void) const;
		CodeList get_code_list(void) const;
	};
}