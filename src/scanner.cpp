#include "scanner.hpp"

namespace apidb
{
	namespace scanner
	{
		MySQL::MySQL(std::istream *in) : yyFlexLexer(in)
		{
		   
		}
		MySQL::~MySQL() 
		{
		   
		}
	}
}
