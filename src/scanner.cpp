#include "scanner.hpp"

namespace apidb
{
	namespace mysql
	{
		Scanner::Scanner(std::istream *in) : yyFlexLexer(in)
		{
		   
		}
		Scanner::~Scanner() 
		{
		   
		}
	}
}
