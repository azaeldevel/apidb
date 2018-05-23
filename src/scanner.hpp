#ifndef __MCSCANNER_HPP__
#define __MCSCANNER_HPP__ 1

#if ! defined(yyFlexLexerOnce)
#include <FlexLexer.h>
#endif

#include "parser.tab.hh"
#include "location.hh"

namespace apidb
{
	namespace scanner
	{
		class MySQL : public yyFlexLexer
		{
		public:
		   
		   MySQL(std::istream *in);
		   virtual ~MySQL();

		   //get rid of override virtual function warning
		   using FlexLexer::yylex;

		   virtual int yylex( apidb::parser::MySQL::semantic_type * const lval, apidb::parser::MySQL::location_type *location);
		   // YY_DECL defined in mc_lexer.l
		   // Method body created by flex in mc_lexer.yy.cc


		private:
		   /* yyval ptr */
		   apidb::parser::MySQL::semantic_type *yylval = nullptr;
		};
	}

} /* end namespace MC */

#endif /* END __MCSCANNER_HPP__ */
