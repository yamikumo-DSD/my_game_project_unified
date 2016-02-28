//bytecode_parser.h

#pragma once
#pragma warning(disable: 4819)

#include "ast.h"
#include <boost/spirit/include/qi.hpp>
#include <boost/phoenix.hpp>

namespace MyVM
{
	inline unsigned char ui2uc(const unsigned int& _n) { return static_cast<unsigned char>(_n); }

	namespace Parser
	{
		namespace spirit = boost::spirit;
		namespace qi = spirit::qi;
		namespace ascii = qi::ascii;
		namespace phoenix = boost::phoenix;

		template<class Iterator>
		class MethodParser : public qi::grammar<Iterator, AST::Method(void), ascii::blank_type>
		{
		private:
			qi::rule<Iterator, AST::Method(void), ascii::blank_type> body;
			struct Instruction : qi::symbols<char, AST::Byte>
			{
				Instruction(void)
				{
					add
					("next_frame",           RepeatableVM::Instruction::NEXT_FRAME       )       
					("eoc",                  RepeatableVM::Instruction::EOC              )
					("user_extention_0",     RepeatableVM::Instruction::USER_EXTENTION_0 )             
					("summon_mob",           RepeatableVM::Instruction::USER_EXTENTION_0 )             
					("user_extention_1",     RepeatableVM::Instruction::USER_EXTENTION_1 )             
					("user_extention_2",     RepeatableVM::Instruction::USER_EXTENTION_2 )             
					("user_extention_3",     RepeatableVM::Instruction::USER_EXTENTION_3 )             
					("user_extention_4",     RepeatableVM::Instruction::USER_EXTENTION_4 )             
					("display_message",      RepeatableVM::Instruction::USER_EXTENTION_4 )             
					("user_extention_5",     RepeatableVM::Instruction::USER_EXTENTION_5 )             
					("current_count",        RepeatableVM::Instruction::USER_EXTENTION_5 )             
					("ldc",                  RepeatableVM::Instruction::LDC              )
					("iload",                RepeatableVM::Instruction::ILOAD            )  
					("iload_0",              RepeatableVM::Instruction::ILOAD_0          )    
					("iload_1",              RepeatableVM::Instruction::ILOAD_1          )    
					("iload_2",              RepeatableVM::Instruction::ILOAD_2          )    
					("iload_3",              RepeatableVM::Instruction::ILOAD_3          )    
					("fload_0",              RepeatableVM::Instruction::FLOAD_0          )    
					("fload_1",              RepeatableVM::Instruction::FLOAD_1          )    
					("fload_2",              RepeatableVM::Instruction::FLOAD_2          )    
					("fload_3",              RepeatableVM::Instruction::FLOAD_3          )    
					("istore",               RepeatableVM::Instruction::ISTORE           )   
					("istore_0",             RepeatableVM::Instruction::ISTORE_0         )     
					("istore_1",             RepeatableVM::Instruction::ISTORE_1         )     
					("istore_2",             RepeatableVM::Instruction::ISTORE_2         )     
					("istore_3",             RepeatableVM::Instruction::ISTORE_3         )     
					("fload",                RepeatableVM::Instruction::FLOAD            )  
					("fstore",               RepeatableVM::Instruction::FSTORE           )   
					("fstore_0",             RepeatableVM::Instruction::FSTORE_0         )     
					("fstore_1",             RepeatableVM::Instruction::FSTORE_1         )     
					("fstore_2",             RepeatableVM::Instruction::FSTORE_2         )     
					("fstore_3",             RepeatableVM::Instruction::FSTORE_3         )     
					("iadd",                 RepeatableVM::Instruction::IADD             ) 
					("fadd",                 RepeatableVM::Instruction::FADD             ) 
					("isub",                 RepeatableVM::Instruction::ISUB             ) 
					("fsub",                 RepeatableVM::Instruction::FSUB             ) 
					("idiv",                 RepeatableVM::Instruction::IDIV             ) 
					("fdiv",                 RepeatableVM::Instruction::FDIV             ) 
					("imul",                 RepeatableVM::Instruction::IMUL             ) 
					("fmul",                 RepeatableVM::Instruction::FMUL             ) 
					("fcmpg",                RepeatableVM::Instruction::FCMPG            )  
					("fcmpl",                RepeatableVM::Instruction::FCMPL            )  
					("irem",                 RepeatableVM::Instruction::IREM             ) 
					("frem",                 RepeatableVM::Instruction::FREM             ) 
					("i2f",                  RepeatableVM::Instruction::I2F              )
					("f2i",                  RepeatableVM::Instruction::F2I              )
					("ineg",                 RepeatableVM::Instruction::INEG             ) 
					("fneg",                 RepeatableVM::Instruction::FNEG             ) 
					("goto",                 RepeatableVM::Instruction::GOTO             ) 
					("goto_w",               RepeatableVM::Instruction::GOTO_W           )   
					("if_icmp_eq",           RepeatableVM::Instruction::IF_ICMP_EQ       )       
					("if_icmp_ne",           RepeatableVM::Instruction::IF_ICMP_NE       )       
					("if_icmp_lt",           RepeatableVM::Instruction::IF_ICMP_LT       )       
					("if_icmp_ge",           RepeatableVM::Instruction::IF_ICMP_GE       )       
					("if_icmp_gt",           RepeatableVM::Instruction::IF_ICMP_GT       )       
					("if_icmp_le",           RepeatableVM::Instruction::IF_ICMP_LE       )       
					("if_eq",                RepeatableVM::Instruction::IF_EQ            )  
					("if_ne",                RepeatableVM::Instruction::IF_NE            )  
					("if_lt",                RepeatableVM::Instruction::IF_LT            )  
					("if_ge",                RepeatableVM::Instruction::IF_GE            )  
					("if_gt",                RepeatableVM::Instruction::IF_GT            )  
					("if_le",                RepeatableVM::Instruction::IF_LE            )  
					("iinc",                 RepeatableVM::Instruction::IINC             ) 
					("iconst_m1",            RepeatableVM::Instruction::ICONST_M1        )      
					("iconst_0",             RepeatableVM::Instruction::ICONST_0         )     
					("iconst_1",             RepeatableVM::Instruction::ICONST_1         )     
					("iconst_2",             RepeatableVM::Instruction::ICONST_2         )     
					("iconst_3",             RepeatableVM::Instruction::ICONST_3         )     
					("iconst_4",             RepeatableVM::Instruction::ICONST_4         )     
					("iconst_5",             RepeatableVM::Instruction::ICONST_5         )     
					("fconst_0",             RepeatableVM::Instruction::FCONST_0         )     
					("fconst_1",             RepeatableVM::Instruction::FCONST_1         )     
					("fconst_2",             RepeatableVM::Instruction::FCONST_2         )     
					("bipush",               RepeatableVM::Instruction::BIPUSH           )   
					("sipush",               RepeatableVM::Instruction::SIPUSH           )   
					("nop",                  RepeatableVM::Instruction::NOP              )
					("pop",                  RepeatableVM::Instruction::POP              )
					("swap",                 RepeatableVM::Instruction::SWAP             ) 
					("invoke_static",        RepeatableVM::Instruction::INVOKE_STATIC    ) 
					("return",               RepeatableVM::Instruction::RETURN           )
					("ireturn",              RepeatableVM::Instruction::IRETURN          ) 
					("freturn",              RepeatableVM::Instruction::FRETURN          ); 
				}
			}instruction;
			qi::rule<Iterator, AST::Byte(void), ascii::blank_type> unsigned_byte;
			qi::rule<Iterator, AST::Byte(void), ascii::blank_type> inst_or_byte;
			qi::rule<Iterator, char(void)> albar;
			qi::rule<Iterator, std::string(void)> specifier;
			qi::rule<Iterator, int(void), ascii::blank_type> limit_stack;
			qi::rule<Iterator, int(void), ascii::blank_type> limit_locals;
		public:
			MethodParser(void) :base_type(body)
			{
				albar = qi::alpha | qi::char_('_');
				specifier = +albar;
				unsigned_byte = qi::uint_[qi::_val = phoenix::bind(&ui2uc, qi::_1)];
				inst_or_byte = unsigned_byte | instruction;
				limit_stack = qi::lit(".limit") >> qi::lit("stack") >> qi::int_;
				limit_locals = qi::lit(".limit") >> qi::lit("locals") >> qi::int_;

				body = qi::eps[phoenix::bind(&AST::Method::reserved_stack_size, qi::_val) = 16, phoenix::bind(&AST::Method::size_of_local_variables, qi::_val) = 16] >>
					specifier[phoenix::bind(&AST::Method::name, qi::_val) = qi::_1] >> qi::lit(':') >> +qi::eol >>
					*(
						(limit_stack[phoenix::bind(&AST::Method::reserved_stack_size, qi::_val) = qi::_1] >> +qi::eol)
						|
						(limit_locals[phoenix::bind(&AST::Method::size_of_local_variables, qi::_val) = qi::_1] >> +qi::eol)
					) >>
					(inst_or_byte % +qi::eol)[phoenix::bind(&AST::Method::imple, qi::_val) = qi::_1];
			}
		};
	}
}