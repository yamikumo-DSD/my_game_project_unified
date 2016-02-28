//vm.h

#pragma once

#include "constant_pool.h"
#include "debug.h"
#include "global_def.h"
#include "frame.h"
#include <stack>
#include "code_list.h"
#include "enable_new_feature.h"

#pragma warning(disable: 4369)

namespace MyVM
{
	class RepeatableVM
	{
	private:
		bool continue_flag;
	protected:
		std::stack<Frame> frame_stack;
		ConstantPool constant_pool;
		ByteCode::const_iterator pc; //A program counter.
		void advance_pc(unsigned int _n);
		auto get_pc(void) const { return pc; }
		CodeList code_list;
		SymbolicReference active_method_info;
		std::stack<SymbolicReference> invoker_info;
		std::stack<decltype(pc)> pc_to_return;//Point a position on invoker's bytecode.
	public:
		enum Instruction : Byte //Original java code and extended code.
		{
			NEXT_FRAME = 203,        //0xcb: Extended code.
			EOC = 204,               //0xcc: Extended code.
			USER_EXTENTION_0  = 205, //0xcd: Extended code.
			USER_EXTENTION_1  = 206, //0xce: Extended code.
			USER_EXTENTION_2  = 207, //0xcf: Extended code.
			USER_EXTENTION_3  = 208, //0xd0: Extended code.
			USER_EXTENTION_4  = 209, //0xd1: Extended code.
			USER_EXTENTION_5  = 210, //0xd2: Extended code.
			USER_EXTENTION_6  = 211, //0xd3: Extended code.
			USER_EXTENTION_7  = 212, //0xd4: Extended code.
			USER_EXTENTION_8  = 213, //0xd5: Extended code.
			USER_EXTENTION_9  = 214, //0xd6: Extended code.
			USER_EXTENTION_10 = 215, //0xd7: Extended code.
			USER_EXTENTION_11 = 216, //0xd8: Extended code.
			USER_EXTENTION_12 = 217, //0xd9: Extended code.
			USER_EXTENTION_13 = 218, //0xda: Extended code.
			USER_EXTENTION_14 = 219, //0xdb: Extended code.
			USER_EXTENTION_15 = 220, //0xdc: Extended code.
			USER_EXTENTION_16 = 221, //0xdd: Extended code.

			LDC = 18,                //0x12: Push item from constant pool onto operand stack. pc[0]:LDC, pc[1]:index 
			ILOAD = 21,              //0x15: Load int from local variables. pc[0]:ILOAD, pc[1]:index
			ILOAD_0 = 26,            //0x1a: Load int from local variables. pc[0]:ILOAD_0
			ILOAD_1 = 27,            //0x1b: Load int from local variables. pc[0]:ILOAD_1
			ILOAD_2 = 28,            //0x1c: Load int from local variables. pc[0]:ILOAD_2
			ILOAD_3 = 29,            //0x1d: Load int from local variables. pc[0]:ILOAD_3
			ISTORE = 54,             //0x36: Store int into local variables. pc[0]:ISTORE, pc[1]:index
			ISTORE_0 = 59,           //0x3b: Store int into local variables. pc[0]:ISTORE_0
			ISTORE_1 = 60,           //0x3c: Store int into local variables. pc[0]:ISTORE_1
			ISTORE_2 = 61,           //0x3d: Store int into local variables. pc[0]:ISTORE_2
			ISTORE_3 = 62,           //0x3b: Store int into local variables. pc[0]:ISTORE_3
			FLOAD = 23,              //0x17: Load float from local variables. pc[0]:FLOAD, pc[1]:index
			FLOAD_0 = 34,            //0x22: Load float from local variables. pc[0]:FLOAD_0
			FLOAD_1 = 35,            //0x23: Load float from local variables. pc[0]:FLOAD_1
			FLOAD_2 = 36,            //0x24: Load float from local variables. pc[0]:FLOAD_2
			FLOAD_3 = 37,            //0x25: Load float from local variables. pc[0]:FLOAD_3
			FSTORE = 56,             //0x38: Store float into local variables. pc[0]:FSTORE, pc[1]:index
			FSTORE_0 = 67,           //0x43: Store float into local variables. pc[0]:FSTORE_0
			FSTORE_1 = 68,           //0x44: Store float into local variables. pc[0]:FSTORE_1
			FSTORE_2 = 69,           //0x45: Store float into local variables. pc[0]:FSTORE_2
			FSTORE_3 = 70,           //0x46: Store float into local variables. pc[0]:FSTORE_3
			IADD = 96,               //0x60: Add int on stack. pc[0]:IADD
			FADD = 98,               //0x62: Add float on stack. pc[0]:FADD
			ISUB = 100,              //0x64: Subtract int on stack(value on little address - value on big address). pc[0]:ISUB
			FSUB = 102,              //0x66: Subtract float on stack(value on little address - value on big address). pc[0]:FSUB
			IDIV = 108,              //0x6c: Divide int on stack(value on little address - value on big address). pc[0]:IDIV
			FDIV = 110,              //0x6e: Divide float on stack(value on little address - value on big address). pc[0]:FDIV
			IMUL = 104,              //0x68: Multiply int on stack(value on little address - value on big address). pc[0]:IDIV
			FMUL = 106,              //0x6a: Multiply float on stack(value on little address - value on big address). pc[0]:FMUL
			FCMPG = 150,             //0x96: Compare float. pc[0]:FCMPG
			FCMPL = 149,             //0x95: Compare float. pc[0]:FCMPL
			IREM = 112,              //0x70: Remainder int on stack(value on little address - value on big address). pc[0]:IREM
			FREM = 114,              //0x72: Remainder float on stack(value on little address - value on big address). pc[0]:FREM
			I2F = 134,               //0x86: Convert int to float on stack. pc[0]:I2F
			F2I = 139,               //0x8b: Convert float to int on stack. pc[0]:F2I
			INEG = 116,              //0x74: Negate int. pc[0]:INEG
			FNEG = 118,              //0x76: Negate float. pc[0]:FNEG
			GOTO = 167,              //0xa7: Branch always. pc[0]:GOTO, pc[1]:branchbyte1, pc[2]:branchbyte2
			GOTO_W = 200,            //0xc8: Branch always. pc[0]:GOTO_@, pc[1]:branchbyte1, pc[2]:branchbyte2, pc[3]:branchbyte3, pc[4]:branchbyte4 
			IF_ICMP_EQ = 159,        //0x9f: Branch if int comparison succeeds. pc[0]:IF_ICMP_EQ, pc[1]:branchbyte1, pc[2]:branchbyte2
			IF_ICMP_NE = 160,        //0xa0: Branch if int comparison succeeds. pc[0]:IF_ICMP_NE, pc[1]:branchbyte1, pc[2]:branchbyte2
			IF_ICMP_LT = 161,        //0xa1: Branch if int comparison succeeds. pc[0]:IF_ICMP_LT, pc[1]:branchbyte1, pc[2]:branchbyte2
			IF_ICMP_GE = 162,        //0xa2: Branch if int comparison succeeds. pc[0]:IF_ICMP_GE, pc[1]:branchbyte1, pc[2]:branchbyte2
			IF_ICMP_GT = 163,        //0xa3: Branch if int comparison succeeds. pc[0]:IF_ICMP_GT, pc[1]:branchbyte1, pc[2]:branchbyte2
			IF_ICMP_LE = 164,        //0xa4: Branch if int comparison succeeds. pc[0]:IF_ICMP_LE, pc[1]:branchbyte1, pc[2]:branchbyte2
			IF_EQ = 153,             //0x99: Branch if int comparison with zero succeeds. pc[0]:IF_EQ, pc[1]:branchbyte1, pc[2]:branchbyte2
			IF_NE = 154,             //0x9a: Branch if int comparison with zero succeeds. pc[0]:IF_NE, pc[1]:branchbyte1, pc[2]:branchbyte2
			IF_LT = 155,             //0x9b: Branch if int comparison with zero succeeds. pc[0]:IF_LT, pc[1]:branchbyte1, pc[2]:branchbyte2
			IF_GE = 156,             //0x9c: Branch if int comparison with zero succeeds. pc[0]:IF_GE, pc[1]:branchbyte1, pc[2]:branchbyte2
			IF_GT = 157,             //0x9d: Branch if int comparison with zero succeeds. pc[0]:IF_GT, pc[1]:branchbyte1, pc[2]:branchbyte2
			IF_LE = 158,             //0x9e: Branch if int comparison with zero succeeds. pc[0]:IF_LE, pc[1]:branchbyte1, pc[2]:branchbyte2
			IINC = 132,              //0x84: Increment local variable by constant. pc[0]:IINC, pc[1]:index, pc[2]:const
			ICONST_M1 = 2,           //0x02: Push int constant. pc[0]:ICONST_M1
			ICONST_0 = 3,            //0x03: Push int constant. pc[0]:ICONST_0
			ICONST_1 = 4,            //0x04: Push int constant. pc[0]:ICONST_1
			ICONST_2 = 5,            //0x05: Push int constant. pc[0]:ICONST_2
			ICONST_3 = 6,            //0x06: Push int constant. pc[0]:ICONST_3
			ICONST_4 = 7,            //0x07: Push int constant. pc[0]:ICONST_4
			ICONST_5 = 8,            //0x08: Push int constant. pc[0]:ICONST_5
			FCONST_0 = 11,           //0x0b: Push 0.0 as float constant. pc[0]:FCONST_0
			FCONST_1 = 12,           //0x0c: Push 1.0 as float constant. pc[0]:FCONST_1
			FCONST_2 = 13,           //0x0d: Push 2.0 as float constant. pc[0]:FCONST_2
			BIPUSH = 16,             //0x10: Push byte as sign-extended int value. pc[0]:BIPUSH, pc[1]:byte
			SIPUSH = 17,             //0x11: Push short. pc[0]:SIPUSH, pc[1]:byte1, pc[2]:byte2
			NOP = 0,                 //0x00: Do nothing. pc[0]:NOP.
			POP = 87,                //0x57: Pop the top operand stack value. The pop instruction must not be used unless value is a value of a category 1 computational type. pc[0]:POP
			SWAP = 95,               //0x5f: Swap the top two values on the operand stack.The swap instruction must not be used unless value1 and value2 are both values of a category 1 computational type. pc[0]:SWAP
			INVOKE_STATIC = 184,     //0xb8: Invoke a method. pc[0]:INVOKE_STATIC, pc[1]:indexbyte1, pc[2]:indexbyte2
			RETURN = 177,            //0xb1: Return void from method. pc[0]:RETURN
			IRETURN = 172,           //0xac: Return int from method. pc[0]:IRETURN
			FRETURN = 174,           //0xae: Return float from method. pc[0]:FRETURN
		};
#ifndef ENABLE_NEW_FEATURE
		RepeatableVM
		(
			std::size_t _size_of_local_variables,
			std::size_t _reserved_stack_size,
			const ConstantPool& _constant_pool,
			const CodeList& _code_list
		);
#else
		RepeatableVM
		(
			const ConstantPool& _constant_pool,
			const CodeList& _code_list
		);
#endif
		RepeatableVM(void);
		void reset
		(
			const ConstantPool& _constant_pool,
			const CodeList& _code_list
		);
		void execute(void);
		Word variable(int _index) const;
		virtual void user_extention_0(void) { ++pc; }
		virtual void user_extention_1(void) { ++pc; }
		virtual void user_extention_2(void) { ++pc; }
		virtual void user_extention_3(void) { ++pc; }
		virtual void user_extention_4(void) { ++pc; }
		virtual void user_extention_5(void) { ++pc; }
		virtual void user_extention_6(void) { ++pc; }
		virtual void user_extention_7(void) { ++pc; }
		virtual void user_extention_8(void) { ++pc; }
		virtual void user_extention_9(void) { ++pc; }
		virtual void user_extention_10(void) { ++pc; }
		virtual void user_extention_11(void) { ++pc; }
		virtual void user_extention_12(void) { ++pc; }
		virtual void user_extention_13(void) { ++pc; }
		virtual void user_extention_14(void) { ++pc; }
		virtual void user_extention_15(void) { ++pc; }
		virtual void user_extention_16(void) { ++pc; }
		virtual ~RepeatableVM(void){}
	};
}