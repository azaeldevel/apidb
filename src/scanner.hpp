#ifndef __MCSCANNER_HPP__
#define __MCSCANNER_HPP__ 1

#if ! defined(yyFlexLexerOnce)
#include <FlexLexer.h>
#endif

#include "parser.tab.hh"
#include "location.hh"

namespace apidb
{
	class MySQLScanner : public yyFlexLexer
	{
	public:
	   
	   MySQLScanner(std::istream *in);
	   virtual ~MySQLScanner();

	   //get rid of override virtual function warning
	   using FlexLexer::yylex;

	   virtual int yylex( apidb::MySQLParser::semantic_type * const lval, apidb::MySQLParser::location_type *location);
	   // YY_DECL defined in mc_lexer.l
	   // Method body created by flex in mc_lexer.yy.cc


	private:
	   /* yyval ptr */
	   apidb::MySQLParser::semantic_type *yylval = nullptr;
	};

} /* end namespace MC */

#endif /* END __MCSCANNER_HPP__ */
