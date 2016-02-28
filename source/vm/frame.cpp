//frame.cpp

#include "frame.h"
#include "exception_def.h"
#include <iostream>
#include "bit_calculation.h"
#include <iterator>
#include "express_debug.h"

MyVM::Frame::Frame
(
	std::size_t _stack_size,
	std::size_t _maximum_local_variables_num
)
	:operand_stack(_stack_size + 1),
	local_variables(_maximum_local_variables_num),
	sp()
{
	sp = operand_stack.begin();
}

decltype(MyVM::Frame::sp) MyVM::Frame::get_sp(void) const
{
	return sp;
}

void MyVM::Frame::advance_sp(int _n)
{
	if (_n > 0)
	{
		for (int i = 0; i != _n; ++i)
		{
			if (sp == operand_stack.end()) { throw StackOverFlow("Exception StackOverFlow is thrown."); }
			++sp;
		}
	}
	else if (_n < 0)
	{
		for (int i = 0; i != -_n; ++i)
		{
			if (sp == operand_stack.begin()) { throw StackUnderFlow("Exception StackUnderFlow is thrown."); }
			--sp;
		}
	}
}

MyVM::Word& MyVM::Frame::operator[](std::size_t _offset)
{
	return local_variables[_offset];
}

const MyVM::Word& MyVM::Frame::operator[](std::size_t _offset) const
{
	return local_variables[_offset];
}

MyVM::Word MyVM::Frame::top(void) const { return get_sp()[-1]; }

MyVM::DWord MyVM::Frame::top2(void) const //top dword value is byte1 << 8 | byte2
{
	using namespace BitCalculation;
	T2Byte<Word> byte1, byte2;
	byte1.t = get_sp()[-1]; byte2.t = get_sp()[-2];
	T2Byte<DWord> top_value;
	top_value.field._0 = byte2.field._0;
	top_value.field._1 = byte2.field._1;
	top_value.field._2 = byte2.field._2;
	top_value.field._3 = byte2.field._3;
	top_value.field._4 = byte1.field._0;
	top_value.field._5 = byte1.field._1;
	top_value.field._6 = byte1.field._2;
	top_value.field._7 = byte1.field._3;
	return top_value.t; 
}

void MyVM::Frame::dump_stack(void)
{
		std::cout << "*********** Dump stack ***********" << std::endl;
		int i = 0;
		for (const jint& x : operand_stack)
		{
			std::printf("%x", x); if (i == std::distance(operand_stack.begin(), sp)) { std::cout << "<-sp"; }
			std::cout << std::endl;
			++i;
		}
		std::cout << "**********************************" << std::endl;
}