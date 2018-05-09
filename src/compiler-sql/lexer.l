%{
/* C++ string header, for string ops below */
#include <string>

/* Implementation of yyFlexScanner */ 
#include "scanner.hpp"
#undef  YY_DECL
#define YY_DECL int apibd::Scanner::yylex( apibd::Parser::semantic_type * const lval, apibd::Parser::location_type *loc )

/* typedef to make the returns for the tokens shorter */
using token = apibd::Parser::token;

/* define yyterminate as this instead of NULL */
#define yyterminate() return( token::END )

/* msvc2010 requires that we exclude this header file. */
#define YY_NO_UNISTD_H

/* update location on matching */
#define YY_USER_ACTION loc->step(); loc->columns(yyleng);

%}

%option debug
%option nodefault
%option yyclass="apibd::Scanner"
%option noyywrap
%option c++

%%
%{          /** Code executed at the beginning of yylex **/
            yylval = lval;
%}


"TINYINT"				{return token::TINYINT;}
"SMALLINT"				{return token::SMALLINT;}
"MEDIUMINT"				{return token::MEDIUMINT;}
"INT"					{return token::INT;}
"INTEGER"				{return token::INTEGER;}
"BIGINT"				{return token::BIGINT;}
"BIT"					{return token::BIT;}

"DECIMAL"				{return token::DECIMAL;}
"NUMERIC"				{return token::NUMERIC;}
"FLOAT"					{return token::FLOAT;}
"DOUBLE"				{return token::DOUBLE;}

"DATE"					{return token::DATE;}
"DATETIME"				{return token::DATETIME;}
"TIMESTAMP"				{return token::TIMESTAMP;}
"TIME"					{return token::TIME;}
"YEAR"					{return token::YEAR;}

"CHAR"					{return token::CHAR;}
"VARCHAR"				{return token::VARCHAR;}
"BINARY"				{return token::BINARY;}
"VARBINARY"				{return token::VARBINARY;}
"BLOB"					{return token::BLOB;}
"TEXT"					{return token::TEXT;}
"ENUM"					{return token::ENUM;}
"SET"					{return token::SET;}
","						{return token::COMA;}
"("						{return token::PARENTHESIS_OPEN;}
")"						{return token::PARENTHESIS_CLOSE;}
[0123456789]+           {return token::INTEGER;}
[ \t]+					{return token::SPACE;}
		



\n          {
               // Update line number
               loc->lines();
               return( token::NEWLINE );
            }

.           {
               return( token::UNKNOW );
            }
%%


