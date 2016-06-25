//vm.cpp

#include "vm/vm.h" 
#include <typeindex>
#include <cstdlib>
#include "vm/debug.h"
#include "vm/bit_calculation.h"
#include "vm/exception_def.h"
#include "vm/enable_new_feature.h"

#define EXPRESS(expr) msgbox_ok(boost::lexical_cast<std::string>(expr).c_str())

#define DUMP_STACK
#undef DUMP_STACK
#define DUMP_CODE
#undef DUMP_CODE

using namespace std;
#ifndef ENABLE_NEW_FEATURE
MyVM::RepeatableVM::RepeatableVM
(
	std::size_t _size_of_local_variables,
	std::size_t _reserved_stack_size,
	const ConstantPool& _constant_pool,
	const CodeList& _code_list
	)
	: constant_pool(_constant_pool),
	code_list(_code_list),
	pc(),
	continue_flag(true),
	frame_stack(),
	active_method_info("void", "main", {}),
	invoker_info(),
	pc_to_return()
{
	frame_stack.push(Frame(code_list["main"].reserved_stack_size, code_list["main"].size_of_local_variables, _constant_pool));
	invoker_info.push(SymbolicReference("void", "system", {}));
}
#else
MyVM::RepeatableVM::RepeatableVM
(
	const ConstantPool& _constant_pool,
	const CodeList& _code_list
)
	: constant_pool(_constant_pool),
	code_list(_code_list),
	pc(),
	continue_flag(true),
	frame_stack(),
	active_method_info("void", "main", {}),
	invoker_info(),
	pc_to_return()
{
	frame_stack.push(Frame(code_list["main"].reserved_stack_size, code_list["main"].size_of_local_variables, constant_pool));
	pc = code_list["main"].bytecode.begin();
	invoker_info.push(SymbolicReference("void", "system", {}));
}

#endif

MyVM::RepeatableVM::RepeatableVM(void)
	:constant_pool(),
	code_list(),
	pc(),
	continue_flag(true),
	frame_stack(),
	active_method_info(),
	invoker_info(),
	pc_to_return()
{}

void MyVM::RepeatableVM::reset
(
	const ConstantPool& _constant_pool,
	const CodeList& _code_list
)
{
	constant_pool = _constant_pool;
	code_list = _code_list;
	continue_flag = true;
	frame_stack = decltype(frame_stack)();
	active_method_info = SymbolicReference("void", "main", {});
	invoker_info = decltype(invoker_info)();
	pc_to_return = decltype(pc_to_return)();

	msgbox_ok("before push new frame");
	frame_stack.push(Frame(code_list["main"].reserved_stack_size, code_list["main"].size_of_local_variables, constant_pool));
	msgbox_ok("after push new frame");

	pc = code_list["main"].bytecode.begin();
	invoker_info.push(SymbolicReference("void", "system", {}));
}

void MyVM::RepeatableVM::advance_pc(unsigned int _n)
{
	for (unsigned int i = 0; i != _n; ++i)
	{
		//if (pc == byte_code.end()) { throw ByteCodeOverRun("Exception ByteCodeOverRun is thrown."); }
		++pc;
	}
}


void MyVM::RepeatableVM::execute(void)
{
	if (continue_flag)
	{
		while (*get_pc() != Instruction::EOC && *get_pc() != Instruction::NEXT_FRAME)
		{
#ifdef DUMP_CODE
			//Dump code
			std::cout << "*************** Dump code ***************" << std::endl;
			std::printf( "Instruction code: %x", *get_pc()); std::cout << '(' << active_method_info.method_name << ')' << std::endl;
			std::cout << "*****************************************" << std::endl;
#endif
#ifdef DUMP_CURRENT_METHOD_NAME
			//Display current method anem
			std::cout << active_method_info.method_name << std::endl;
#endif
#ifdef DUMP_STACK
			//Dump stack
			frame_stack.top().dump_stack();
#endif
			switch (*get_pc())
			{
				case Instruction::USER_EXTENTION_0:
					user_extention_0();
					break;
				case Instruction::USER_EXTENTION_1:
					user_extention_1();
					break;
				case Instruction::USER_EXTENTION_2:
					user_extention_2();
					break;
				case Instruction::USER_EXTENTION_3:
					user_extention_3();
					break;
				case Instruction::USER_EXTENTION_4:
					user_extention_4();
					break;
				case Instruction::USER_EXTENTION_5:
					user_extention_5();
					break;
				case Instruction::LDC: 
					{
						std::type_index type = constant_pool[get_pc()[1]].type();
						if (type == typeid(jint))
						{
							auto val = get<jint>(constant_pool[get_pc()[1]]);
							*frame_stack.top().get_sp() = BitCalculation::bit_cast<Word>(val);
							frame_stack.top().advance_sp(1);
						}
						else if (type == typeid(float))
						{
							auto val = get<float>(constant_pool[get_pc()[1]]);
							*frame_stack.top().get_sp() = BitCalculation::bit_cast<Word>(val);
							frame_stack.top().advance_sp(1);
						}
					}
					advance_pc(2);
					break;
				case Instruction::ILOAD:
					*frame_stack.top().get_sp() = frame_stack.top()[get_pc()[1]];
					frame_stack.top().advance_sp(1);
					advance_pc(2);
					break;
#ifndef STATIC_ILOAD
#define STATIC_ILOAD(N) \
					*frame_stack.top().get_sp() = frame_stack.top()[N];\
					frame_stack.top().advance_sp(1);\
					advance_pc(1);\
					break;
				case Instruction::ILOAD_0:
					STATIC_ILOAD(0)
				case Instruction::ILOAD_1:
					STATIC_ILOAD(1)
				case Instruction::ILOAD_2:
					STATIC_ILOAD(2)
				case Instruction::ILOAD_3:
					STATIC_ILOAD(3)
#undef STATIC_ILOAD
#endif
#ifndef STATIC_FLOAD
#define STATIC_FLOAD(N) \
					*frame_stack.top().get_sp() = frame_stack.top()[N];\
					frame_stack.top().advance_sp(1);\
					advance_pc(1);\
					break;
				case Instruction::FLOAD_0:
					STATIC_FLOAD(0)
				case Instruction::FLOAD_1:
					STATIC_FLOAD(1)
				case Instruction::FLOAD_2:
					STATIC_FLOAD(2)
				case Instruction::FLOAD_3:
					STATIC_FLOAD(3)
#undef STATIC_FLOAD
#endif
				case Instruction::ISTORE:
					frame_stack.top().advance_sp(-1);
					frame_stack.top()[get_pc()[1]] = *frame_stack.top().get_sp();
					advance_pc(2);
					break;
#ifndef STATIC_ISTORE
#define STATIC_ISTORE(N) \
					frame_stack.top().advance_sp(-1);\
					frame_stack.top()[N] = *frame_stack.top().get_sp();\
					advance_pc(1);\
					break;
				case Instruction::ISTORE_0:
					STATIC_ISTORE(0)
				case Instruction::ISTORE_1:
					STATIC_ISTORE(1)
				case Instruction::ISTORE_2:
					STATIC_ISTORE(2)
				case Instruction::ISTORE_3:
					STATIC_ISTORE(3)
#undef STATIC_ITORE
#endif
				case Instruction::FLOAD:
					*frame_stack.top().get_sp() = frame_stack.top()[get_pc()[1]];
					frame_stack.top().advance_sp(1);
					advance_pc(2);
					break;
				case Instruction::FSTORE:
					frame_stack.top().advance_sp(-1);
					frame_stack.top()[get_pc()[1]] = *frame_stack.top().get_sp();
					advance_pc(2);
					break;
#ifndef STATIC_FSTORE
#define STATIC_FSTORE(N) \
					frame_stack.top().advance_sp(-1);\
					frame_stack.top()[N] = *frame_stack.top().get_sp();\
					advance_pc(1);\
					break;
				case Instruction::FSTORE_0:
					STATIC_ISTORE(0)
				case Instruction::FSTORE_1:
					STATIC_ISTORE(1)
				case Instruction::FSTORE_2:
					STATIC_ISTORE(2)
				case Instruction::FSTORE_3:
					STATIC_ISTORE(3)
#undef STATIC_FTORE
#endif
#ifndef MATHEMATICAL_BINARY_OPERATOR_WORD_SIZE
#define MATHEMATICAL_BINARY_OPERATOR_WORD_SIZE(op, FourByteType) \
				{ \
					frame_stack.top().advance_sp(-1); \
					auto value1 = BitCalculation::bit_cast<FourByteType>(frame_stack.top().get_sp()[-1]); \
					auto value2 = BitCalculation::bit_cast<FourByteType>(frame_stack.top().get_sp()[0]); \
					auto result = value1 op value2; \
					frame_stack.top().get_sp()[-1] = BitCalculation::bit_cast<Word>(result); \
					advance_pc(1); \
					break; \
				}
				case Instruction::IADD:
					MATHEMATICAL_BINARY_OPERATOR_WORD_SIZE(+, jint)
				case Instruction::FADD:
					MATHEMATICAL_BINARY_OPERATOR_WORD_SIZE(+, jfloat)
				case Instruction::ISUB:
					MATHEMATICAL_BINARY_OPERATOR_WORD_SIZE(-, jint)
				case Instruction::FSUB:
					MATHEMATICAL_BINARY_OPERATOR_WORD_SIZE(-, jfloat)
				case Instruction::IDIV:
					MATHEMATICAL_BINARY_OPERATOR_WORD_SIZE(/, jint)
				case Instruction::FDIV:
					MATHEMATICAL_BINARY_OPERATOR_WORD_SIZE(/, jfloat)
				case Instruction::IMUL:
					MATHEMATICAL_BINARY_OPERATOR_WORD_SIZE(*, jint)
				case Instruction::FMUL:
					MATHEMATICAL_BINARY_OPERATOR_WORD_SIZE(*, jfloat)
				case Instruction::IREM:
					MATHEMATICAL_BINARY_OPERATOR_WORD_SIZE(%, jint)
				case Instruction::FREM:
					throw std::runtime_error("FREM instruction hasn't implemented yet.");
#undef MATHEMATICAL_BINARY_OPERATOR_WORD_SIZE
#endif
				case Instruction::I2F:
				{
					auto value = BitCalculation::bit_cast<jint>(frame_stack.top().get_sp()[-1]);
					auto result = static_cast<jfloat>(value);
					frame_stack.top().get_sp()[-1] = BitCalculation::bit_cast<Word>(result);
					advance_pc(1);
					break;
				}
				case Instruction::F2I:
				{
					auto value = BitCalculation::bit_cast<jfloat>(frame_stack.top().get_sp()[-1]);
					auto result = static_cast<jint>(value);
					frame_stack.top().get_sp()[-1] = BitCalculation::bit_cast<Word>(result);
					advance_pc(1);
					break;
				}
				case Instruction::INEG:
				{
					auto value = BitCalculation::bit_cast<jint>(frame_stack.top().get_sp()[-1]);
					frame_stack.top().get_sp()[-1] = BitCalculation::bit_cast<Word>(-value);
					advance_pc(1);
					break;
				}
				case Instruction::FNEG:
				{
					auto value = BitCalculation::bit_cast<jfloat>(frame_stack.top().get_sp()[-1]);
					frame_stack.top().get_sp()[-1] = BitCalculation::bit_cast<Word>(-value);
					advance_pc(1);
					break;
				}
				case Instruction::GOTO:
				{
					//Destination address(16bit). Branchoffset is (branchbyte1 << 8) | branchbyte2.
					BitCalculation::T2Byte<Short> branch;
					branch.field._0 = get_pc()[2]; //branchbyte2
					branch.field._1 = get_pc()[1]; //branchbyte1
#ifndef ENABLE_NEW_FEATURE
					pc = code_list[active_method_info.method_name].begin() + branch.t;
#else
					pc = code_list[active_method_info.method_name].bytecode.begin() + branch.t;
#endif
					break;
				}
				case Instruction::GOTO_W:
				{
					//Destination address(32bit). Branchoffset is (branchbyte1 << 24) | (branchbyte2 << 16) | (branchbyte3 << 8) | branchbyte4.
					BitCalculation::T2Byte<Word> branch;
					branch.field._0 = get_pc()[4]; //branchbyte2
					branch.field._1 = get_pc()[3]; //branchbyte1
					branch.field._2 = get_pc()[2]; //branchbyte1
					branch.field._3 = get_pc()[1]; //branchbyte1
#ifndef ENABLE_NEW_FEATURE
					pc = code_list[active_method_info.method_name].begin() + branch.t;
#else
					pc = code_list[active_method_info.method_name].bytecode.begin() + branch.t;
#endif
					break;
				}
#ifndef IF_ICMP_IMPLEMENT
#define IF_ICMP_IMPLEMENT(op)\
				{\
					frame_stack.top().advance_sp(-2);\
					auto value1 = BitCalculation::bit_cast<jint>(frame_stack.top().get_sp()[0]);\
					auto value2 = BitCalculation::bit_cast<jint>(frame_stack.top().get_sp()[1]);\
					BitCalculation::T2Byte<Short> branch;\
					branch.field._0 = get_pc()[2];\
					branch.field._1 = get_pc()[1];\
					if (value1 op value2) { pc = code_list[active_method_info.method_name].bytecode.begin() + branch.t; }\
					else { advance_pc(3);}\
					break;\
				}
				case Instruction::IF_ICMP_EQ:
					IF_ICMP_IMPLEMENT(==)
				case Instruction::IF_ICMP_NE:
					IF_ICMP_IMPLEMENT(!=)
				case Instruction::IF_ICMP_LT:
					IF_ICMP_IMPLEMENT(<)
				case Instruction::IF_ICMP_GE:
					IF_ICMP_IMPLEMENT(>=)
				case Instruction::IF_ICMP_GT:
					IF_ICMP_IMPLEMENT(>)
				case Instruction::IF_ICMP_LE:
					IF_ICMP_IMPLEMENT(<= )
#undef IF_ICMP_IMPLEMENT
#endif
#ifndef IF_IMPLEMENT
#define IF_IMPLEMENT(op)\
				{\
					frame_stack.top().advance_sp(-1);\
					auto value = BitCalculation::bit_cast<jint>(*frame_stack.top().get_sp());\
					static constexpr jint zero = 0;\
					BitCalculation::T2Byte<Short> branch;\
					branch.field._0 = get_pc()[2];\
					branch.field._1 = get_pc()[1];\
					if (value op zero) { pc = code_list[active_method_info.method_name].bytecode.begin() + branch.t; }\
					else {advance_pc(3);}\
					break;\
				}
				case Instruction::IF_EQ:
					IF_IMPLEMENT(==)
				case Instruction::IF_NE:
					IF_IMPLEMENT(!=)
				case Instruction::IF_LT:
					IF_IMPLEMENT(<)
				case Instruction::IF_GE:
					IF_IMPLEMENT(>=)
				case Instruction::IF_GT:
					IF_IMPLEMENT(>)
				case Instruction::IF_LE:
					IF_IMPLEMENT(<= )
#undef IF_IMPLEMENT
#endif
#ifndef FCMP_IMPLEMENT
#define FCMP_IMPLEMENT(cmp) \
				{ \
					frame_stack.top().advance_sp(-1); \
					auto value1 = BitCalculation::bit_cast<jfloat>(frame_stack.top().get_sp()[-1]); \
					auto value2 = BitCalculation::bit_cast<jfloat>(frame_stack.top().get_sp()[0]); \
					jint result = (value1 cmp value2) ? 1 : 0; \
					frame_stack.top().get_sp()[-1] = BitCalculation::bit_cast<jint>(result); \
					advance_pc(1); \
					break; \
				}
				case Instruction::FCMPG:
					FCMP_IMPLEMENT(>)
				case Instruction::FCMPL:
					FCMP_IMPLEMENT(<)
#undef FCMP_IMPLEMENT
#endif
				case Instruction::IINC:
					{
						auto const_ = BitCalculation::bit_cast<jchar>(get_pc()[2]); //const is signed byte.
						auto result = BitCalculation::bit_cast<jint>(frame_stack.top()[get_pc()[1]]) + static_cast<jint>(const_); //const is signed-extended to int
						frame_stack.top()[get_pc()[1]] = BitCalculation::bit_cast<Word>(result);
						advance_pc(3);
						break;
					}
#ifndef ICONST_IMPLE
#define ICONST_IMPLE(N)\
					{\
						*frame_stack.top().get_sp() = BitCalculation::bit_cast<Word>(N);\
						frame_stack.top().advance_sp(1);\
						advance_pc(1); \
						break;\
					}
				case Instruction::ICONST_M1:
					ICONST_IMPLE(-1)
				case Instruction::ICONST_0:
					ICONST_IMPLE(0)
				case Instruction::ICONST_1:
					ICONST_IMPLE(1)
				case Instruction::ICONST_2:
					ICONST_IMPLE(2)
				case Instruction::ICONST_3:
					ICONST_IMPLE(3)
				case Instruction::ICONST_4:
					ICONST_IMPLE(4)
				case Instruction::ICONST_5:
					ICONST_IMPLE(5)
#undef ICONST_IMPLE
#endif
#ifndef FCONST_IMPLE
#define FCONST_IMPLE(N)\
					{\
						*frame_stack.top().get_sp() = BitCalculation::bit_cast<Word>(static_cast<jfloat>(N));\
						frame_stack.top().advance_sp(1);\
						advance_pc(1); \
						break;\
					}
				case Instruction::FCONST_0:
					FCONST_IMPLE(0)
				case Instruction::FCONST_1:
					FCONST_IMPLE(1)
				case Instruction::FCONST_2:
					FCONST_IMPLE(2)
#undef FCONST_IMPLE
#endif
				case Instruction::BIPUSH: 
					{
						auto immediate_byte = BitCalculation::bit_cast<jchar>(get_pc()[1]);
						*frame_stack.top().get_sp() = BitCalculation::bit_cast<Word>(static_cast<jint>(immediate_byte));
						msgbox_ok("bipush");
						frame_stack.top().advance_sp(1);
						advance_pc(2);
						break;
					}
				case Instruction::SIPUSH: 
					{
						BitCalculation::T2Byte<jshort> intermediate_value;
						intermediate_value.field._1 = get_pc()[1];
						intermediate_value.field._0 = get_pc()[2];
						*frame_stack.top().get_sp() = BitCalculation::bit_cast<Word>(static_cast<jint>(intermediate_value.t));
						frame_stack.top().advance_sp(1);
						advance_pc(3);
						break;
					}
				case Instruction::NOP:
					advance_pc(1);
					break;
				case Instruction::POP:
					frame_stack.top().advance_sp(-1);
					advance_pc(1);
					break;
				case Instruction::SWAP:
					std::swap(frame_stack.top().get_sp()[-1], frame_stack.top().get_sp()[-2]);
					advance_pc(1);
					break;
				case Instruction::INVOKE_STATIC:
					{
						BitCalculation::T2Byte<Short> index;
						index.field._1 = get_pc()[1];
						index.field._0 = get_pc()[2];
						invoker_info.push(active_method_info); //store invoker's infomation.
						auto symbolic_ref = (active_method_info = get<SymbolicReference>(constant_pool[index.t])); //Problem here
						pc_to_return.push(pc + 3);
#ifndef ENABLE_NEW_FEATURE
						pc = code_list[symbolic_ref.method_name].begin();
#else
						pc = code_list[symbolic_ref.method_name].bytecode.begin();
#endif
						auto& invoker = frame_stack.top();
#ifndef ENABLE_NEW_FEATURE
						frame_stack.push(Frame(10, 10, constant_pool));
#else
						frame_stack.push(Frame(code_list[symbolic_ref.method_name].reserved_stack_size, code_list[symbolic_ref.method_name].size_of_local_variables, constant_pool));
#endif
						int i = 0;
						for (auto t : symbolic_ref.type_of_arguments)
						{
							if (t == "Integer")
							{
								frame_stack.top()[i++] = invoker.top();
								invoker.advance_sp(-1);
							}
							else if (t == "Float")
							{
								frame_stack.top()[i++] = invoker.top();
								invoker.advance_sp(-1);
							}
						}
						break;
					}
				case Instruction::IRETURN:
					{
						active_method_info = invoker_info.top();
						invoker_info.pop();
						auto returned_value = frame_stack.top().top();
						frame_stack.pop();
						*frame_stack.top().get_sp() = returned_value;
						frame_stack.top().advance_sp(1);
						pc = pc_to_return.top();
						pc_to_return.pop();
						break;
					}
				case Instruction::FRETURN:
					{
						active_method_info = invoker_info.top();
						invoker_info.pop();
						auto returned_value = frame_stack.top().top();
						frame_stack.pop();
						*frame_stack.top().get_sp() = returned_value;
						frame_stack.top().advance_sp(1);
						pc = pc_to_return.top();
						pc_to_return.pop();
						break;
					}
				case Instruction::RETURN:
					{
						active_method_info = invoker_info.top();
						invoker_info.pop();
						frame_stack.pop();
						pc = pc_to_return.top();
						pc_to_return.pop();
						break;
					}
				default:
					throw InvalidInstruction("Exception InvalidInstruction is thrown.");
					break;
				}
			}
	
		if (*get_pc() == Instruction::NEXT_FRAME)
		{
			advance_pc(1);
		}
		else if (*get_pc() == Instruction::EOC)
		{
			continue_flag = false;
		}
	}
}


MyVM::Word MyVM::RepeatableVM::variable(int _index) const { return frame_stack.top()[_index]; }

