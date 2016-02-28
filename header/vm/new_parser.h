//new_parser.h

#pragma once
#pragma warning(disable: 4819)

#include "new_ast.h"
#include <boost/spirit/include/qi.hpp>
#include <boost/phoenix.hpp>

namespace MyVM
{
	namespace spirit = boost::spirit;
	namespace qi = spirit::qi;
	namespace ascii = qi::ascii;
	namespace phoenix = boost::phoenix;

	template<class Iterator>
	class ProgramParser : public qi::grammar<Iterator, AST::Program(void), ascii::blank_type>
	{
	private:
		qi::rule<Iterator, AST::Program(void), ascii::blank_type> body;
		qi::rule<Iterator, AST::Function(void), ascii::blank_type> function;
		qi::rule<Iterator, char(void)> albar;
		qi::rule<Iterator, std::string(void)> specifier;
		qi::rule<Iterator, std::string(void)> instruction;
		qi::rule<Iterator, int(void), ascii::blank_type> limit_stack;
		qi::rule<Iterator, int(void), ascii::blank_type> limit_locals;
		qi::rule<Iterator, AST::Mnemonic(void), ascii::blank_type> mnemonic;
		qi::rule<Iterator, AST::Operand(void), ascii::blank_type> operand;
		qi::rule<Iterator, float(void)> float_;
	public:
		ProgramParser(void) :base_type(body)
		{
			float_ = qi::real_parser<float, qi::strict_real_policies<float>>();
			body = -(function % (+qi::eol));

			albar = qi::alpha | qi::char_('_');
			specifier = albar >> *(albar | qi::digit);

			limit_stack = qi::lit(".limit") >> qi::lit("stack") >> qi::int_;
			limit_locals = qi::lit(".limit") >> qi::lit("locals") >> qi::int_;

			instruction = albar >> *(albar | qi::digit);

			operand =
				//(qi::lit("Integer") >> '(' >> qi::int_ >> ')') |
				//(qi::lit("Float") >> '(' >> qi::float_ >> ')') |
				float_ | qi::int_ | specifier;

			mnemonic =
				-(specifier[phoenix::bind(&AST::Mnemonic::tag, qi::_val) = qi::_1] >> ':') >> 
				instruction[phoenix::bind(&AST::Instruction::name, phoenix::bind(&AST::Mnemonic::instruction, qi::_val)) = qi::_1] >>
				-(operand[phoenix::push_back(phoenix::bind(&AST::Mnemonic::operand_list, qi::_val), qi::_1)] % ',');

			function = qi::eps[phoenix::bind(&AST::Function::reserved_stack_size, qi::_val) = 16, phoenix::bind(&AST::Function::size_of_local_variables, qi::_val) = 16] >>
				qi::skip(ascii::space)
				[
					specifier[phoenix::bind(&AST::Function::type_of_returned_value, qi::_val) = qi::_1] >> //Return type
					specifier[phoenix::bind(&AST::Function::name, qi::_val) = qi::_1] >> //Function name
				'(' >>
				-(specifier[phoenix::push_back(phoenix::bind(&AST::Function::type_of_arguments, qi::_val), qi::_1)] % ',') >> //Argumnt list
				')' >>
				'{'
				] >> +qi::eol >>
				-(
					(
						mnemonic[phoenix::push_back(phoenix::bind(&AST::Function::definition, qi::_val), qi::_1)] |
						limit_stack[phoenix::bind(&AST::Function::reserved_stack_size, qi::_val) = qi::_1] |
						limit_locals[phoenix::bind(&AST::Function::size_of_local_variables, qi::_val) = qi::_1]
					) % +qi::eol
				) >>
				+qi::eol >>
				'}';

		}
	};
}