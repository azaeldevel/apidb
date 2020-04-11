#ifndef OCTETPS_APIDB_SCANNER_MARIADB_HPP
#define OCTETPS_APIDB_SCANNER_MARIADB_HPP

#if ! defined(yyFlexLexerOnce)
#include <FlexLexer.h>
#endif

#include "parser.tab.hh"
#include "location.hh"

namespace octetos
{
namespace apidb
{
	namespace mariadb
	{
		class Scanner : public yyFlexLexer
		{
		public:
			Scanner(std::istream *in);
			virtual ~Scanner();
			using FlexLexer::yylex;
			virtual int yylex( octetos::apidb::mariadb::Parser::semantic_type * const lval, octetos::apidb::mariadb::Parser::location_type *location);

		private:
			octetos::apidb::mariadb::Parser::semantic_type *yylval = nullptr;
		};
	}

} 
}
#endif
