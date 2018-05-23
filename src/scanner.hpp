#ifndef __MCSCANNER_HPP__
#define __MCSCANNER_HPP__ 1

#if ! defined(yyFlexLexerOnce)
#include <FlexLexer.h>
#endif

#include "parser.tab.hh"
#include "location.hh"

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
			virtual int yylex( apidb::mysql::Parser::semantic_type * const lval, apidb::mysql::Parser::location_type *location);

		private:
			apidb::mysql::Parser::semantic_type *yylval = nullptr;
		};
	}

} /* end namespace MC */

#endif /* END __MCSCANNER_HPP__ */
