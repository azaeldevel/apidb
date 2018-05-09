%{
/* C++ string header, for string ops below */
#include <string>

/* Implementation of yyFlexScanner */ 
#include "scanner.hpp"
#undef  YY_DECL
#define YY_DECL int MC::MC_Scanner::yylex( MC::MC_Parser::semantic_type * const lval, MC::MC_Parser::location_type *loc )

/* typedef to make the returns for the tokens shorter */
using token = MC::MC_Parser::token;

/* define yyterminate as this instead of NULL */
#define yyterminate() return( token::END )

/* msvc2010 requires that we exclude this header file. */
#define YY_NO_UNISTD_H

/* update location on matching */
#define YY_USER_ACTION loc->step(); loc->columns(yyleng);

%}

%option debug
%option nodefault
%option yyclass="MC::MC_Scanner"
%option noyywrap
%option c++

%%
%{          /** Code executed at the beginning of yylex **/
            yylval = lval;
%}


"TINYINT"				{printf("TINYINT\n");return token::TINYINT;}
"SMALLINT"				{printf("SMALLINT\n");return token::SMALLINT;}
"MEDIUMINT"				{printf("MEDIUMINT\n");return token::MEDIUMINT;}
"INT"					{printf("INT\n");return token::INT;}
"INTEGER"				{printf("INTEGER\n");return token::INTEGER;}
"BIGINT"				{printf("TYPE_BIGINT\n");return token::BIGINT;}
"BIT"					{printf("BIGINT\n");return token::BIT;}

"DECIMAL"				{printf("DECIMAL\n");return token::DECIMAL;}
"NUMERIC"				{printf("NUMERIC\n");return token::NUMERIC;}
"FLOAT"					{printf("FLOAT\n");return token::FLOAT;}
"DOUBLE"				{printf("DOUBLE\n");return token::DOUBLE;}

"DATE"					{printf("DATE\n");return token::DATE;}
"DATETIME"				{printf("DATETIME\n");return token::DATETIME;}
"TIMESTAMP"				{printf("TIMESTAMP\n");return token::TIMESTAMP;}
"TIME"					{printf("TIME\n");return token::TIME;}
"YEAR"					{printf("YEAR\n");return token::YEAR;}

"CHAR"					{printf("CHAR\n");return token::CHAR;}
"VARCHAR"				{printf("VARCHAR\n");return token::VARCHAR;}
"BINARY"				{printf("BINARY\n");return token::BINARY;}
"VARBINARY"				{printf("VARBINARY\n");return token::VARBINARY;}
"BLOB"					{printf("BLOB\n");return token::BLOB;}
"TEXT"					{printf("TEXT\n");return token::TEXT;}
"ENUM"					{printf("ENUM\n");return token::ENUM;}
"SET"					{printf("SET\n");return token::SET;}
","						{printf("COMA\n");return token::COMA;}
"("						{printf("PARENTESIS ABIERTO\n");return token::PARENTHESIS_OPEN;}
")"						{printf("PARENTESIS CERRADO\n");return token::PARENTHESIS_CLOSE;}
[0123456789]+           {printf("NUMERO\n");return token::INTEGER;}
[ \t]+					{printf("SPACE\n");return token::SPACE;}
		
		
[a-z]       {
               return( token::LOWER );
            }

[A-Z]       {
               return( token::UPPER );
            }

[a-zA-Z]+   {
               /**
                * Section 10.1.5.1 of the 3.0.2 Bison Manual says the 
                * following should work:
                * yylval.build( yytext );
                * but it doesn't.
                * ref: http://goo.gl/KLn0w2
                */
               yylval->build< std::string >( yytext );
               return( token::WORD );
            }

\n          {
               // Update line number
               loc->lines();
               return( token::NEWLINE );
            }

.           {
               return( token::UNKNOW );
            }
%%


