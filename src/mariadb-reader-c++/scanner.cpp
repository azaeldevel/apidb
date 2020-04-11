#include "scanner.hpp"

namespace octetos
{
namespace apidb
{
	namespace mariadb
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
