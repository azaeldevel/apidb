#ifndef OCTETOS_APIDB_SCANNER_POSTGRESQL_HPP
#define OCTETOS_APIDB_SCANNER_POSTGRESQL_HPP

#if ! defined(yyFlexLexerOnce)
#include <FlexLexer.h>
#endif

#include "parser.tab.hh"
#include "location.hh"

namespace octetos
{
namespace apidb
{
	namespace postgresql
	{
		class Scanner : public yyFlexLexer
		{
		public:
			Scanner(std::istream *in);
			virtual ~Scanner();
			using FlexLexer::yylex;
			virtual int yylex( Parser::semantic_type * const lval, Parser::location_type *location);

		private:
			Parser::semantic_type *yylval = nullptr;
		};
	}

} 
}
#endif
