#include "scanner.hpp"

namespace apidb
{

   MySQLScanner::MySQLScanner(std::istream *in) : yyFlexLexer(in)
   {
	   
   }
   MySQLScanner::~MySQLScanner() 
   {
	   
   }
}
