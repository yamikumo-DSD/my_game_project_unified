//load_stage_enemy_list.cpp

#pragma warning(disable: 4819)
#include "play_game.h"
#include <fstream>
#include <exception>
#include "msgbox.h"
#include "vm/preprocessor.h"
#include "vm/new_parser.h"
#include "vm/new_ast_analyzer.h"
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>

#define DUMP_CODE

namespace MyGameProject
{
	template<typename Dst, typename Src>
	inline decltype(auto) lexical_cast(Src&& _src)
	{
		return boost::lexical_cast<Dst>(std::forward<Src>(_src));
	}

	struct ConstantVisitor : boost::static_visitor<void>
	{
		std::ostream& os;
		int count;
		ConstantVisitor(std::ostream& _os, int _counter) :os(_os), count(_counter){}
		void operator()(int _n) const
		{
			os << '[' << count << "] " << _n << " as int." << std::endl;
		}
		void operator()(float _f) const
		{
			os << '[' << count << "] " << _f << " as float." << std::endl;
		}
		void operator()(bool _b) const
		{
			os << '[' << count << "] " << _b << " as bool." << std::endl;
		}
		void operator()(MyVM::SymbolicReference _ref) const
		{
			os << '[' << count << "] " << _ref.method_name << " as symbolic reference." << std::endl;
		}
	};

	void PlayGame::load_stage_enemy_list(int _stage_num)
	{
			using namespace std::literals;
			std::ifstream code_input_stream;
			code_input_stream.open("../../data/map/code"s + lexical_cast<char>(_stage_num) + ".txt");
			std::string code_text;

			if (code_input_stream)
			{
				while (!code_input_stream.eof())
				{
					std::string line;
					std::getline(code_input_stream, line);
					(code_text += std::move(line)) += '\n';
				}

				code_text = MyVM::preprocess(std::move(code_text));

				MyVM::AST::Program ast;

				namespace qi = boost::spirit::qi;
				auto parse_result = qi::phrase_parse(code_text.begin(), code_text.end(), MyVM::ProgramParser<std::string::iterator>(), qi::ascii::blank, ast);

				MyVM::ASTAnalyzer analyzer;
				analyzer.analyze(std::move(ast));

	#ifdef DUMP_CODE
				std::ofstream log_constant_pool{ "constant_pool"s + lexical_cast<char>(_stage_num) + ".txt" };
				int i{0};
				for (const auto& constant : analyzer.get_constant_pool())
				{
					boost::apply_visitor(ConstantVisitor(log_constant_pool, i++), constant);
				}
				std::ofstream log_code_list{"code_list"s + lexical_cast<char>(_stage_num) + ".txt"};
				for (const auto& code_list : analyzer.get_code_list())
				{
					log_code_list << boost::format("function name = %1% consists of\n") %code_list.first;
					int i{0};
					for (const auto& code : code_list.second.bytecode)
					{
						log_code_list << boost::format("[%1%]: %2%\n") %i++ %static_cast<int>(code);
					}
				}
	#endif

				reset(analyzer.get_constant_pool(), analyzer.get_code_list());
			}
			else { throw std::runtime_error("No stage map file."); }
	} //PlayGame::lod_stage_enemy_list
} //namespace MyGameProject
