#ifndef OCTETPS_APIDB_SCANNER_HPP
#define OCTETPS_APIDB_SCANNER_HPP

#if ! defined(yyFlexLexerOnce)
#include <FlexLexer.h>
#endif

#include "parser.tab.hh"
#include "location.hh"

namespace octetos
{
namespace apidb
{
	namespace mysql
	{
		class Scanner : public yyFlexLexer
		{
		public:
			Scanner(std::istream *in);
			virtual ~Scanner();
			using FlexLexer::yylex;
			virtual int yylex( octetos::apidb::mysql::Parser::semantic_type * const lval, octetos::apidb::mysql::Parser::location_type *location);

		private:
			octetos::apidb::mysql::Parser::semantic_type *yylval = nullptr;
		};
	}

} 
}
#endif
