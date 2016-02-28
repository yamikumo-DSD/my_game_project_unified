//new_ast_analyzer.cpp

#include "new_ast_analyzer.h"
#include <cmath>
#include "bit_calculation.h"

using namespace MyVM;

ASTAnalyzer::ASTAnalyzer(void)
:constant_pool(),
code_list(),
current_function_name(),
constant_index(0u),
function_index_map(),
taged_mnemonic_list_for_goto(),
taged_mnemonic_list_for_goto_w()
{}

ConstantPool ASTAnalyzer::get_constant_pool(void) const { return constant_pool; }

CodeList ASTAnalyzer::get_code_list(void) const { return code_list; }

void ASTAnalyzer::analyze(const AST::Program& _program)
{
	if (_program.size() == 0) { throw std::runtime_error("The program is empty."); }
	for (const auto& function : _program)
	{
		analyze_function(function);
	}
}

void ASTAnalyzer::analyze_function(const AST::Function& _function)
{
	current_function_name = _function.name;

	//Check tags first.
	int i = 0;
	for (const auto& mnemonic : _function.definition)
	{
		if (mnemonic.tag)
		{
			if (i <= std::pow(2, 16) - 1)
			{
				BitCalculation::T2Byte<Short> branch; branch.t = i;
				std::tuple<std::string, std::string, Byte, Byte> temp;
				std::get<0>(temp) = current_function_name;
				std::get<1>(temp) = *mnemonic.tag;
				std::get<2>(temp) = branch.field._1;
				std::get<3>(temp) = branch.field._0;
				taged_mnemonic_list_for_goto.push_back(temp);
			}
			else
			{
				BitCalculation::T2Byte<Word> branch; branch.t = i;
				std::tuple<std::string, std::string, Byte, Byte, Byte, Byte> temp;
				std::get<0>(temp) = current_function_name;
				std::get<1>(temp) = *mnemonic.tag;
				std::get<2>(temp) = branch.field._3;
				std::get<3>(temp) = branch.field._2;
				std::get<4>(temp) = branch.field._1;
				std::get<5>(temp) = branch.field._0;
				taged_mnemonic_list_for_goto_w.push_back(temp);
			}
		}
		if      (mnemonic.instruction.name == "ldc"               ) { i += 2; }
		else if (mnemonic.instruction.name == "invoke_static"     ) { i += 3; }
		else if (mnemonic.instruction.name == "goto"              ) { i += 3; }
		else if (mnemonic.instruction.name == "goto_w"            ) { i += 5; }
		else if (mnemonic.instruction.name == "next_frame"        ) { i += 1; }
		else if (mnemonic.instruction.name == "if_icmp_eq"        ) { i += 3; }
		else if (mnemonic.instruction.name == "if_icmp_ne"        ) { i += 3; }
		else if (mnemonic.instruction.name == "if_icmp_lt"        ) { i += 3; }
		else if (mnemonic.instruction.name == "if_icmp_ge"        ) { i += 3; }
		else if (mnemonic.instruction.name == "if_icmp_gt"        ) { i += 3; }
		else if (mnemonic.instruction.name == "if_icmp_le"        ) { i += 3; }
		else if (mnemonic.instruction.name == "if_eq"             ) { i += 3; }
		else if (mnemonic.instruction.name == "if_ne"             ) { i += 3; }
		else if (mnemonic.instruction.name == "if_lt"             ) { i += 3; }
		else if (mnemonic.instruction.name == "if_ge"             ) { i += 3; }
		else if (mnemonic.instruction.name == "if_gt"             ) { i += 3; }
		else if (mnemonic.instruction.name == "if_le"             ) { i += 3; }
		else if (mnemonic.instruction.name == "bipush"            ) { i += 2; }
		else if (mnemonic.instruction.name == "sipush"            ) { i += 3; }
		else if (mnemonic.instruction.name == "iinc"              ) { i += 3; }
		else if (mnemonic.instruction.name == "iload"             ) { i += 2; }
		else if (mnemonic.instruction.name == "iload_0"           ) { i += 1; }
		else if (mnemonic.instruction.name == "iload_1"           ) { i += 1; }
		else if (mnemonic.instruction.name == "iload_1"           ) { i += 1; }
		else if (mnemonic.instruction.name == "iload_3"           ) { i += 1; }
		else if (mnemonic.instruction.name == "eoc"               ) { i += 1; }
		else if (mnemonic.instruction.name == "istore"            ) { i += 2; }
		else if (mnemonic.instruction.name == "istore_0"          ) { i += 1; }
		else if (mnemonic.instruction.name == "istore_1"          ) { i += 1; }
		else if (mnemonic.instruction.name == "istore_2"          ) { i += 1; }
		else if (mnemonic.instruction.name == "istore_3"          ) { i += 1; }
		else if (mnemonic.instruction.name == "fload"             ) { i += 2; }
		else if (mnemonic.instruction.name == "fload_0"           ) { i += 1; }
		else if (mnemonic.instruction.name == "fload_1"           ) { i += 1; }
		else if (mnemonic.instruction.name == "fload_2"           ) { i += 1; }
		else if (mnemonic.instruction.name == "fload_3"           ) { i += 1; }
		else if (mnemonic.instruction.name == "fstore"            ) { i += 2; }
		else if (mnemonic.instruction.name == "fstore_0"          ) { i += 1; }
		else if (mnemonic.instruction.name == "fstore_1"          ) { i += 1; }
		else if (mnemonic.instruction.name == "fstore_2"          ) { i += 1; }
		else if (mnemonic.instruction.name == "fstore_3"          ) { i += 1; }
		else if (mnemonic.instruction.name == "iadd"              ) { i += 1; }
		else if (mnemonic.instruction.name == "fadd"              ) { i += 1; }
		else if (mnemonic.instruction.name == "isub"              ) { i += 1; }
		else if (mnemonic.instruction.name == "fsub"              ) { i += 1; }
		else if (mnemonic.instruction.name == "idiv"              ) { i += 1; }
		else if (mnemonic.instruction.name == "fdiv"              ) { i += 1; }
		else if (mnemonic.instruction.name == "imul"              ) { i += 1; }
		else if (mnemonic.instruction.name == "fmul"              ) { i += 1; }
		else if (mnemonic.instruction.name == "fcmpg"             ) { i += 1; }
		else if (mnemonic.instruction.name == "fcmpl"             ) { i += 1; }
		else if (mnemonic.instruction.name == "irem"              ) { i += 1; }
		else if (mnemonic.instruction.name == "frem"              ) { i += 1; }
		else if (mnemonic.instruction.name == "i2f"               ) { i += 1; }
		else if (mnemonic.instruction.name == "f2i"               ) { i += 1; }
		else if (mnemonic.instruction.name == "ineg"              ) { i += 1; }
		else if (mnemonic.instruction.name == "fneg"              ) { i += 1; }
		else if (mnemonic.instruction.name == "iconst_m1"         ) { i += 1; }
		else if (mnemonic.instruction.name == "iconst_0"          ) { i += 1; }
		else if (mnemonic.instruction.name == "iconst_1"          ) { i += 1; }
		else if (mnemonic.instruction.name == "iconst_2"          ) { i += 1; }
		else if (mnemonic.instruction.name == "iconst_3"          ) { i += 1; }
		else if (mnemonic.instruction.name == "iconst_4"          ) { i += 1; }
		else if (mnemonic.instruction.name == "iconst_5"          ) { i += 1; }
		else if (mnemonic.instruction.name == "fconst_0"          ) { i += 1; }
		else if (mnemonic.instruction.name == "fconst_1"          ) { i += 1; }
		else if (mnemonic.instruction.name == "fconst_2"          ) { i += 1; }
		else if (mnemonic.instruction.name == "nop"               ) { i += 1; }
		else if (mnemonic.instruction.name == "pop"               ) { i += 1; }
		else if (mnemonic.instruction.name == "swap"              ) { i += 1; }
		else if (mnemonic.instruction.name == "return"            ) { i += 1; }
		else if (mnemonic.instruction.name == "ireturn"           ) { i += 1; }
		else if (mnemonic.instruction.name == "freturn"           ) { i += 1; }
		else if (mnemonic.instruction.name == "user_extention_0"  ) { i += 1; }
		else if (mnemonic.instruction.name == "summon_mob"        ) { i += 1; }
		else if (mnemonic.instruction.name == "user_extention_1"  ) { i += 1; }
		else if (mnemonic.instruction.name == "user_extention_2"  ) { i += 2; }
		else if (mnemonic.instruction.name == "math_func_uni"     ) { i += 2; }
		else if (mnemonic.instruction.name == "user_extention_3"  ) { i += 1; }
		else if (mnemonic.instruction.name == "player_position"   ) { i += 1; }
		else if (mnemonic.instruction.name == "user_extention_4"  ) { i += 1; }
		else if (mnemonic.instruction.name == "display_message"   ) { i += 1; }
		else if (mnemonic.instruction.name == "user_extention_5"  ) { i += 1; }
		else if (mnemonic.instruction.name == "current_count"     ) { i += 1; }
		else if (mnemonic.instruction.name == "user_extention_6"  ) { i += 2; }
		else if (mnemonic.instruction.name == "math_func_bi"      ) { i += 2; }
		else if (mnemonic.instruction.name == "user_extention_7"  ) { i += 1; }
		else if (mnemonic.instruction.name == "summon_boss"       ) { i += 1; }
		else if (mnemonic.instruction.name == "user_extention_8"  ) { i += 1; }
		else if (mnemonic.instruction.name == "is_id_enable"      ) { i += 1; }
		else if (mnemonic.instruction.name == "user_extention_9"  ) { i += 1; }
		else if (mnemonic.instruction.name == "user_extention_10" ) { i += 1; }
		else if (mnemonic.instruction.name == "user_extention_11" ) { i += 1; }
		else if (mnemonic.instruction.name == "user_extention_12" ) { i += 1; }
		else if (mnemonic.instruction.name == "user_extention_13" ) { i += 1; }
		else if (mnemonic.instruction.name == "user_extention_14" ) { i += 1; }
		else if (mnemonic.instruction.name == "user_extention_15" ) { i += 1; }
		else if (mnemonic.instruction.name == "user_extention_16" ) { i += 1; }
		else 
		{
			using namespace std;
			throw std::runtime_error("Undefined instruction name:"s + mnemonic.instruction.name); 
		}
	}

	//Push back information of new function.
	SymbolicReference new_func_ref;
	new_func_ref.type_of_returned_value = _function.type_of_returned_value;
	new_func_ref.method_name = _function.name;
	new_func_ref.type_of_arguments = _function.type_of_arguments;
	constant_pool.push_back(new_func_ref);
	function_index_map.insert(std::make_pair(current_function_name, constant_index));
	++constant_index;

	code_list.insert(std::make_pair(current_function_name, MethodInfo()));
	code_list[current_function_name].reserved_stack_size = _function.reserved_stack_size;
	code_list[current_function_name].size_of_local_variables = _function.size_of_local_variables;
	for (const auto& mnemonic : _function.definition)
	{
		analyze_mnemonic(mnemonic);
	}
}

void ASTAnalyzer::analyze_mnemonic(const AST::Mnemonic& _mnemonic)
{
	////////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////                          MACRO DEFINITIONS                           ////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef OPERAND_NUM_CHECK
#define OPERAND_NUM_CHECK(N) assert(_mnemonic.operand_list.size() == N)
#endif

#ifndef IF_NO_ARG
#define IF_NO_ARG(INSTRUCTION) \
	OPERAND_NUM_CHECK(0); \
	code_list[current_function_name].bytecode.push_back(RepeatableVM::Instruction::INSTRUCTION);
#endif

#ifndef ANALYZE_JUMPER
#define ANALYZE_JUMPER(INSTRUCTION) \
		OPERAND_NUM_CHECK(1);\
		code_list[current_function_name].bytecode.push_back(RepeatableVM::Instruction::INSTRUCTION);\
		auto cfn = current_function_name;\
		auto tag = boost::get<std::string>(_mnemonic.operand_list[0]);\
		auto result = std::find_if\
			(\
				taged_mnemonic_list_for_goto.begin(), \
				taged_mnemonic_list_for_goto.end(),\
				[cfn, tag](const std::tuple<std::string, std::string, Byte, Byte>& _taged)\
				{\
					if (std::get<0>(_taged) == cfn && std::get<1>(_taged) == tag) { return true; }\
					else { return false; }\
				}\
			);\
		code_list[current_function_name].bytecode.push_back(std::get<2>(*result));\
		code_list[current_function_name].bytecode.push_back(std::get<3>(*result));
#endif

#ifndef LOAD_AND_STORE_NOINDEX
#define LOAD_AND_STORE_NOINDEX(INSTRUCTION)\
		OPERAND_NUM_CHECK(1);\
		const auto index = boost::get<jint>(_mnemonic.operand_list[0]);\
		if (index >= 0 && index <= std::pow(2, 8) - 1)\
		{\
			code_list[current_function_name].bytecode.push_back(RepeatableVM::Instruction::INSTRUCTION);\
			code_list[current_function_name].bytecode.push_back(static_cast<Byte>(index));\
		}\
		else \
		{\
			throw std::runtime_error("Index must be in 0 to 255.");\
		}
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////                          MACRO     END                               ////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////

	if (_mnemonic.instruction.name == "ldc")
	{
		OPERAND_NUM_CHECK(1);
		code_list[current_function_name].bytecode.push_back(RepeatableVM::Instruction::LDC);
		code_list[current_function_name].bytecode.push_back(static_cast<Byte>(constant_index));
		if (_mnemonic.operand_list[0].type() == typeid(jint))
		{
			constant_pool.push_back(boost::get<jint>(_mnemonic.operand_list[0]));
		}
		else if (_mnemonic.operand_list[0].type() == typeid(jfloat))
		{
			constant_pool.push_back(boost::get<jfloat>(_mnemonic.operand_list[0]));
		}
		++constant_index;
	}
	else if (_mnemonic.instruction.name == "invoke_static")
	{
		OPERAND_NUM_CHECK(1);
		code_list[current_function_name].bytecode.push_back(RepeatableVM::Instruction::INVOKE_STATIC);
		BitCalculation::T2Byte<Short> temp; temp.t = function_index_map[boost::get<std::string>(_mnemonic.operand_list[0])];
		code_list[current_function_name].bytecode.push_back(temp.field._1);
		code_list[current_function_name].bytecode.push_back(temp.field._0);
	}
	else if (_mnemonic.instruction.name == "bipush")
	{
		OPERAND_NUM_CHECK(1);
		const auto value = boost::get<jint>(_mnemonic.operand_list[0]);
		if (value <= std::pow(2, 8) - 1)
		{
			code_list[current_function_name].bytecode.push_back(RepeatableVM::Instruction::BIPUSH);
			code_list[current_function_name].bytecode.push_back(static_cast<Byte>(value));
		}
		else
		{
			throw std::runtime_error("Operand of bipush instruction must be 255 or under.");
		}
	}
	else if (_mnemonic.instruction.name == "sipush")
	{
		OPERAND_NUM_CHECK(1);
		const auto value = boost::get<jint>(_mnemonic.operand_list[0]);
		if (value <= std::pow(2, 16) - 1)
		{
			BitCalculation::T2Byte<jshort> temp; temp.t = value;
			code_list[current_function_name].bytecode.push_back(RepeatableVM::Instruction::SIPUSH);
			code_list[current_function_name].bytecode.push_back(temp.field._1);
			code_list[current_function_name].bytecode.push_back(temp.field._0);
		}
		else
		{
			throw std::runtime_error("Operand of sipush instruction must be 65535 or under.");
		}
	}
	else if (_mnemonic.instruction.name == "iinc")
	{
		OPERAND_NUM_CHECK(2);
		const auto index = boost::get<jint>(_mnemonic.operand_list[0]);
		const auto const_ = boost::get<jint>(_mnemonic.operand_list[1]);

		if (index <= std::pow(2, 8) - 1)
		{
			if (const_ <= 127 && const_ >= -128)
			{
				code_list[current_function_name].bytecode.push_back(RepeatableVM::Instruction::IINC);
				code_list[current_function_name].bytecode.push_back(static_cast<Byte>(index));
				code_list[current_function_name].bytecode.push_back(BitCalculation::bit_cast<Byte>(static_cast<jchar>(const_)));
			}
			else
			{
				throw std::runtime_error("Constant must be in -128 to 127.");
			}
		}
		else
		{
				throw std::runtime_error("Index must be under 255.");
		}
	}
	else if (_mnemonic.instruction.name == "user_extention_2" || _mnemonic.instruction.name == "math_func_uni") 
	{
		OPERAND_NUM_CHECK(1);
		const auto function_selected = boost::get<jint>(_mnemonic.operand_list[0]);
		if (function_selected >= 0 && function_selected <= std::pow(2, 8) - 1)
		{
				code_list[current_function_name].bytecode.push_back(RepeatableVM::Instruction::USER_EXTENTION_2);
				code_list[current_function_name].bytecode.push_back(static_cast<Byte>(function_selected));
		}
		else
		{
			throw std::runtime_error("Number of function must be in from 0 to 255.");
		}
	}
	else if (_mnemonic.instruction.name == "user_extention_6" || _mnemonic.instruction.name == "math_func_bi") 
	{
		OPERAND_NUM_CHECK(1);
		const auto function_selected = boost::get<jint>(_mnemonic.operand_list[0]);
		if (function_selected >= 0 && function_selected <= std::pow(2, 8) - 1)
		{
				code_list[current_function_name].bytecode.push_back(RepeatableVM::Instruction::USER_EXTENTION_6);
				code_list[current_function_name].bytecode.push_back(static_cast<Byte>(function_selected));
		}
		else
		{
			throw std::runtime_error("Number of function must be in from 0 to 255.");
		}
	}
	else if (_mnemonic.instruction.name == "goto"             ) { ANALYZE_JUMPER(GOTO)            }
	else if (_mnemonic.instruction.name == "if_icmp_eq"       ) { ANALYZE_JUMPER(IF_ICMP_EQ)      }
	else if (_mnemonic.instruction.name == "if_icmp_ne"       ) { ANALYZE_JUMPER(IF_ICMP_NE)      }
	else if (_mnemonic.instruction.name == "if_icmp_lt"       ) { ANALYZE_JUMPER(IF_ICMP_LT)      }
	else if (_mnemonic.instruction.name == "if_icmp_ge"       ) { ANALYZE_JUMPER(IF_ICMP_GE)      }
	else if (_mnemonic.instruction.name == "if_icmp_gt"       ) { ANALYZE_JUMPER(IF_ICMP_GT)      }
	else if (_mnemonic.instruction.name == "if_icmp_le"       ) { ANALYZE_JUMPER(IF_ICMP_LE)      }
	else if (_mnemonic.instruction.name == "if_eq"            ) { ANALYZE_JUMPER(IF_EQ)           }
	else if (_mnemonic.instruction.name == "if_ne"            ) { ANALYZE_JUMPER(IF_NE)           }
	else if (_mnemonic.instruction.name == "if_lt"            ) { ANALYZE_JUMPER(IF_LT)           }
	else if (_mnemonic.instruction.name == "if_ge"            ) { ANALYZE_JUMPER(IF_GE)           }
	else if (_mnemonic.instruction.name == "if_gt"            ) { ANALYZE_JUMPER(IF_GT)           }
	else if (_mnemonic.instruction.name == "if_le"            ) { ANALYZE_JUMPER(IF_LE)           }
	else if (_mnemonic.instruction.name == "iload"            ) { LOAD_AND_STORE_NOINDEX(ILOAD)   }
	else if (_mnemonic.instruction.name == "istore"           ) { LOAD_AND_STORE_NOINDEX(ISTORE)  }
	else if (_mnemonic.instruction.name == "fload"            ) { LOAD_AND_STORE_NOINDEX(FLOAD)   }
	else if (_mnemonic.instruction.name == "fstore"           ) { LOAD_AND_STORE_NOINDEX(FSTORE)  }
	else if (_mnemonic.instruction.name == "next_frame"       ) { IF_NO_ARG(NEXT_FRAME)           }
	else if (_mnemonic.instruction.name == "iload_0"          ) { IF_NO_ARG(ILOAD_0)              }
	else if (_mnemonic.instruction.name == "iload_1"          ) { IF_NO_ARG(ILOAD_1)              }
	else if (_mnemonic.instruction.name == "iload_1"          ) { IF_NO_ARG(ILOAD_2)              }
	else if (_mnemonic.instruction.name == "iload_3"          ) { IF_NO_ARG(ILOAD_3)              }
	else if (_mnemonic.instruction.name == "eoc"              ) { IF_NO_ARG(EOC)                  }
	else if (_mnemonic.instruction.name == "istore_0"         ) { IF_NO_ARG(ISTORE_0)             }
	else if (_mnemonic.instruction.name == "istore_1"         ) { IF_NO_ARG(ISTORE_1)             }
	else if (_mnemonic.instruction.name == "istore_2"         ) { IF_NO_ARG(ISTORE_2)             }
	else if (_mnemonic.instruction.name == "istore_3"         ) { IF_NO_ARG(ISTORE_3)             }
	else if (_mnemonic.instruction.name == "fload_0"          ) { IF_NO_ARG(FLOAD_0)              }
	else if (_mnemonic.instruction.name == "fload_1"          ) { IF_NO_ARG(FLOAD_1)              }
	else if (_mnemonic.instruction.name == "fload_2"          ) { IF_NO_ARG(FLOAD_2)              }
	else if (_mnemonic.instruction.name == "fload_3"          ) { IF_NO_ARG(FLOAD_3)              }
	else if (_mnemonic.instruction.name == "fstore_0"         ) { IF_NO_ARG(FSTORE_0)             }
	else if (_mnemonic.instruction.name == "fstore_1"         ) { IF_NO_ARG(FSTORE_1)             }
	else if (_mnemonic.instruction.name == "fstore_2"         ) { IF_NO_ARG(FSTORE_2)             }
	else if (_mnemonic.instruction.name == "fstore_3"         ) { IF_NO_ARG(FSTORE_3)             }
	else if (_mnemonic.instruction.name == "iadd"             ) { IF_NO_ARG(IADD)                 }
	else if (_mnemonic.instruction.name == "fadd"             ) { IF_NO_ARG(FADD)                 }
	else if (_mnemonic.instruction.name == "isub"             ) { IF_NO_ARG(ISUB)                 }
	else if (_mnemonic.instruction.name == "fsub"             ) { IF_NO_ARG(FSUB)                 }
	else if (_mnemonic.instruction.name == "idiv"             ) { IF_NO_ARG(IDIV)                 }
	else if (_mnemonic.instruction.name == "fdiv"             ) { IF_NO_ARG(FDIV)                 }
	else if (_mnemonic.instruction.name == "imul"             ) { IF_NO_ARG(IMUL)                 }
	else if (_mnemonic.instruction.name == "fmul"             ) { IF_NO_ARG(FMUL)                 }
	else if (_mnemonic.instruction.name == "fcmpg"            ) { IF_NO_ARG(FCMPG)                }
	else if (_mnemonic.instruction.name == "fcmpl"            ) { IF_NO_ARG(FCMPL)                }
	else if (_mnemonic.instruction.name == "irem"             ) { IF_NO_ARG(IREM)                 }
	else if (_mnemonic.instruction.name == "frem"             ) { IF_NO_ARG(FREM)                 }
	else if (_mnemonic.instruction.name == "i2f"              ) { IF_NO_ARG(I2F)                  }
	else if (_mnemonic.instruction.name == "f2i"              ) { IF_NO_ARG(F2I)                  }
	else if (_mnemonic.instruction.name == "ineg"             ) { IF_NO_ARG(INEG)                 }
	else if (_mnemonic.instruction.name == "fneg"             ) { IF_NO_ARG(FNEG)                 }
	else if (_mnemonic.instruction.name == "iconst_m1"        ) { IF_NO_ARG(ICONST_M1)            }
	else if (_mnemonic.instruction.name == "iconst_0"         ) { IF_NO_ARG(ICONST_0)             }
	else if (_mnemonic.instruction.name == "iconst_1"         ) { IF_NO_ARG(ICONST_1)             }
	else if (_mnemonic.instruction.name == "iconst_2"         ) { IF_NO_ARG(ICONST_2)             }
	else if (_mnemonic.instruction.name == "iconst_3"         ) { IF_NO_ARG(ICONST_3)             }
	else if (_mnemonic.instruction.name == "iconst_4"         ) { IF_NO_ARG(ICONST_4)             }
	else if (_mnemonic.instruction.name == "iconst_5"         ) { IF_NO_ARG(ICONST_5)             }
	else if (_mnemonic.instruction.name == "fconst_0"         ) { IF_NO_ARG(FCONST_0)             }
	else if (_mnemonic.instruction.name == "fconst_1"         ) { IF_NO_ARG(FCONST_1)             }
	else if (_mnemonic.instruction.name == "fconst_2"         ) { IF_NO_ARG(FCONST_2)             }
	else if (_mnemonic.instruction.name == "nop"              ) { IF_NO_ARG(NOP)                  }
	else if (_mnemonic.instruction.name == "pop"              ) { IF_NO_ARG(POP)                  }
	else if (_mnemonic.instruction.name == "swap"             ) { IF_NO_ARG(SWAP)                 }
	else if (_mnemonic.instruction.name == "return"           ) { IF_NO_ARG(RETURN)               }
	else if (_mnemonic.instruction.name == "ireturn"          ) { IF_NO_ARG(IRETURN)              }
	else if (_mnemonic.instruction.name == "freturn"          ) { IF_NO_ARG(FRETURN)              }
	else if (_mnemonic.instruction.name == "user_extention_0" ) { IF_NO_ARG(USER_EXTENTION_0)     }
	else if (_mnemonic.instruction.name == "summon_mob"       ) { IF_NO_ARG(USER_EXTENTION_0)     }
	else if (_mnemonic.instruction.name == "user_extention_1" ) { IF_NO_ARG(USER_EXTENTION_1)     }
	else if (_mnemonic.instruction.name == "user_extention_3" ) { IF_NO_ARG(USER_EXTENTION_3)     }
	else if (_mnemonic.instruction.name == "player_position"  ) { IF_NO_ARG(USER_EXTENTION_3)     }
	else if (_mnemonic.instruction.name == "user_extention_4" ) { IF_NO_ARG(USER_EXTENTION_4)     }
	else if (_mnemonic.instruction.name == "display_message"  ) { IF_NO_ARG(USER_EXTENTION_4)     }
	else if (_mnemonic.instruction.name == "user_extention_5" ) { IF_NO_ARG(USER_EXTENTION_5)     }
	else if (_mnemonic.instruction.name == "current_count"    ) { IF_NO_ARG(USER_EXTENTION_5)     }
	else if (_mnemonic.instruction.name == "user_extention_7" ) { IF_NO_ARG(USER_EXTENTION_7)     }
	else if (_mnemonic.instruction.name == "summon_boss"      ) { IF_NO_ARG(USER_EXTENTION_7)     }
	else if (_mnemonic.instruction.name == "user_extention_8" ) { IF_NO_ARG(USER_EXTENTION_8)     }
	else if (_mnemonic.instruction.name == "is_id_enable"     ) { IF_NO_ARG(USER_EXTENTION_8)     }
	else if (_mnemonic.instruction.name == "user_extention_9" ) { IF_NO_ARG(USER_EXTENTION_9)     }
	else if (_mnemonic.instruction.name == "user_extention_10" ) { IF_NO_ARG(USER_EXTENTION_10)     }
	else if (_mnemonic.instruction.name == "user_extention_11" ) { IF_NO_ARG(USER_EXTENTION_11)     }
	else if (_mnemonic.instruction.name == "user_extention_12" ) { IF_NO_ARG(USER_EXTENTION_12)     }
	else if (_mnemonic.instruction.name == "user_extention_13" ) { IF_NO_ARG(USER_EXTENTION_13)     }
	else if (_mnemonic.instruction.name == "user_extention_14" ) { IF_NO_ARG(USER_EXTENTION_14)     }
	else if (_mnemonic.instruction.name == "user_extention_15" ) { IF_NO_ARG(USER_EXTENTION_15)     }
	else if (_mnemonic.instruction.name == "user_extention_16" ) { IF_NO_ARG(USER_EXTENTION_16)     }
	else 
	{
		using namespace std;
		throw std::runtime_error("Undefined instruction name:"s + _mnemonic.instruction.name); 
	}
}