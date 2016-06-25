//frame.h

#pragma once

#include "vm/global_def.h"
#include "vm/constant_pool.h"
#include "vm/debug.h"

namespace MyVM
{
	class Frame
	{
	private:
		std::vector<Word> operand_stack;
		std::vector<Word> local_variables;
		std::vector<Word>::iterator sp;
		const ConstantPool& constant_pool;
	public:
		Frame
		(
			std::size_t _stack_size,
			std::size_t _maximum_local_variables_num,
			const ConstantPool& _constant_pool_ref
		);
		void advance_sp(int _n);
		decltype(sp) get_sp(void) const;
		decltype(sp) get_sp(void);
		Word& operator[](std::size_t _offset);
		const Word& operator[](std::size_t _offset) const;
		Word top(void) const;
		DWord top2(void) const;
		void dump_stack(void) ; //to stdout
		~Frame(void);
	};
}
