#ifndef __MCSCANNER_HPP__
#define __MCSCANNER_HPP__ 1

#if ! defined(yyFlexLexerOnce)
#include <FlexLexer.h>
#endif

#include "parser.tab.hh"
#include "location.hh"

namespace apidb
{
class Scanner : public yyFlexLexer
{
public:
   
   Scanner(std::istream *in) : yyFlexLexer(in)
   {
	   
   };
   virtual ~Scanner() 
   {
	   
   };

   //get rid of override virtual function warning
   using FlexLexer::yylex;

   virtual int yylex( apidb::Parser::semantic_type * const lval, apidb::Parser::location_type *location);
   // YY_DECL defined in mc_lexer.l
   // Method body created by flex in mc_lexer.yy.cc


private:
   /* yyval ptr */
   apidb::Parser::semantic_type *yylval = nullptr;
};

} /* end namespace MC */

#endif /* END __MCSCANNER_HPP__ */
