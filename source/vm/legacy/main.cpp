//main.cpp

#include "vm.h"
#include "debug.h"
#include <cstdlib>

#include "bit_calculation.h"
#include <boost/format.hpp>
#include <map>
#include <fstream>
#include "code_parser.h"
#include "preprocessor.h"

class CVM : public MyVM::RepeatableVM
{
	private:
		MyVM::jint count = 0;
		static MyVM::jint id;
		std::map<MyVM::jint,MyVM::jint> id_map;
	public:
		using RepeatableVM::RepeatableVM;
		virtual void user_extention_0(void) override final
		{
			frame_stack.top().advance_sp(-1);
			std::cout << std::endl << "SUMMON_MOB instruction called at count=" << *frame_stack.top().get_sp() << '.' << std::endl;
			std::printf("Mob's type: %x\n", get_pc()[1]);
			std::printf("Behavior type of it: %x\n", get_pc()[2]);
			std::cout << std::endl;
			*frame_stack.top().get_sp() = MyVM::BitCalculation::bit_cast<MyVM::Word>(id++);
			frame_stack.top().advance_sp(1);
			advance_pc(3);
		}
		virtual void user_extention_1(void) override final
		{
			frame_stack.top().advance_sp(-1);
			std::cout << std::endl << "SUMMON_BOSS instruction called at count=" << *frame_stack.top().get_sp() << '.' << std::endl;
			std::printf("Boss's type: %x\n", get_pc()[1]);
			std::cout << std::endl;
			advance_pc(2);
		}
		virtual void user_extention_5(void) override final
		{
			*frame_stack.top().get_sp() = MyVM::BitCalculation::bit_cast<MyVM::Word>(count);
			frame_stack.top().advance_sp(1);
			advance_pc(1);
		}
		virtual void user_extention_2(void) override final
		{
			using namespace MyVM;
			using namespace BitCalculation;
			frame_stack.top().advance_sp(-1);
			id_map.insert
			({
				static_cast<jint>(bit_cast<jchar>(*frame_stack.top().get_sp())), 
				static_cast<jint>(bit_cast<jchar>(get_pc()[1]))
			});
			advance_pc(2);
		}
		virtual void user_extention_4(void) override final
		{
			msgbox_ok("Extention 4");
			advance_pc(1);
		}
		void update(void)
		{
			execute();
			++count;
		}
		void print_count(void)
		{
			std::cout << boost::format("count: %d") %count << std::endl;
		}
		virtual ~CVM(void) {}
		MyVM::jint tag_of(MyVM::jint _id) const 
		{ 
			try { return id_map.at(_id); }
			catch(std::out_of_range) { return 0; }
		}

		enum : MyVM::Byte
		{
			COUNT = MyVM::RepeatableVM::Instruction::USER_EXTENTION_5,
			SUMMON_MOB = MyVM::RepeatableVM::Instruction::USER_EXTENTION_0,
			SUMMON_BOSS = MyVM::RepeatableVM::Instruction::USER_EXTENTION_1,
			MSGBOX = MyVM::RepeatableVM::Instruction::USER_EXTENTION_4,
			TAG_MOB = MyVM::RepeatableVM::Instruction::USER_EXTENTION_2,
		};
};

MyVM::jint CVM::id = 0;


int main(void)
{
	try
	{
		using namespace MyVM;
		using Inst = MyVM::RepeatableVM::Instruction;
		using namespace BitCalculation;
		using namespace Parser;

		//Load code file.
		std::string code_text;
		std::ifstream code_input("code.txt");
		if (code_input)
		{
			while (!code_input.eof())
			{
				std::string line;
				std::getline(code_input, line);
				(code_text += line) += '\n';
			}
		}
		else
		{ throw std::runtime_error("No code file found."); }

		code_text = preprocess(code_text);

		//Parse code text.
		CodeParser<std::string::iterator> code_parser;
		AST::Code code;
		auto result = qi::phrase_parse ( code_text.begin(), code_text.end(), code_parser, ascii::blank, code );
		if (!result) { throw std::runtime_error("Grammatical error in code file."); }
		auto data = analyze_ast(code);

		CVM vm(data.first, data.second);
		for (int i = 0; i != 100; ++i) { vm.print_count(); vm.update(); }

		std::cout << BitCalculation::bit_cast<jfloat>(vm.variable(0)) << std::endl;
	}
	catch (const std::exception& _e) { std::cerr << _e.what() << std::endl; }
	
	std::system("pause");
	return 0;
}