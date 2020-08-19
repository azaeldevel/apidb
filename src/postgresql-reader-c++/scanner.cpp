#include "scanner.hpp"

namespace octetos
{
namespace apidb
{
	namespace postgresql
	{
		Scanner::Scanner(std::istream *in) : yyFlexLexer(in)
		{
		   
		}
		Scanner::~Scanner() 
		{
		}
	}
}
}
