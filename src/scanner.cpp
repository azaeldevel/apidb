#include "scanner.hpp"

namespace octetos
{
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
}
