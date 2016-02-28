//load_stage_enemy_list.cpp

#pragma warning(disable: 4819)
#include "play_game.h"
#include <fstream>
#include <exception>
#include "msgbox.h"
#include "vm/preprocessor.h"
#include "vm/new_parser.h"
#include "vm/new_ast_analyzer.h"

void MyGameProject::PlayGame::load_stage_enemy_list(int _stage_num)
{
	std::ifstream code_input_stream;
	code_input_stream.open("../../data/map/code0.txt");
	std::string code_text;

	if (code_input_stream)
	{
		while (!code_input_stream.eof())
		{
			std::string line;
			std::getline(code_input_stream, line);
			(code_text += line) += '\n';
		}

		code_text = MyVM::preprocess(code_text);
		msgbox_ok(code_text.c_str());

		MyVM::AST::Program ast;

		namespace qi = boost::spirit::qi;
		auto parse_result = qi::phrase_parse(code_text.begin(), code_text.end(), MyVM::ProgramParser<std::string::iterator>(), qi::ascii::blank, ast);

		MyVM::ASTAnalyzer analyzer;
		analyzer.analyze(ast);

		reset(analyzer.get_constant_pool(), analyzer.get_code_list());
	}
	else { throw std::runtime_error("No stage map file."); }
}
