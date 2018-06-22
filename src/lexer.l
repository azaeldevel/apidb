%{
/* C++ string header, for string ops below */
#include <string>

/* Implementation of yyFlexScanner */ 
#include "scanner.hpp"
#undef  YY_DECL
#define YY_DECL int apidb::mysql::Scanner::yylex(apidb::mysql::Parser::semantic_type * const lval, apidb::mysql::Parser::location_type *loc )

/* typedef to make the returns for the tokens shorter */
using token = apidb::mysql::Parser::token;

/* define yyterminate as this instead of NULL */
#define yyterminate() return( token::END )

/* msvc2010 requires that we exclude this header file. */
#define YY_NO_UNISTD_H

/* update location on matching */
#define YY_USER_ACTION loc->step(); loc->columns(yyleng);

%}

%option debug
%option nodefault
%option yyclass="apidb::mysql::Scanner"
%option noyywrap
%option c++

D			[0-9]
L			[a-zA-Z_]

%%
%{
	/** Code executed at the beginning of yylex **/
	yylval = lval;
%}


[T|t][I|i][N|n][Y|y][I|i]+[N|n][T|t]					{return token::TINYINT;}
[S|s][M|m][A|a][L|l][L|l][I|i][N|n][T|t]				{return token::SMALLINT;}
[M|m][E|e][D|d][I|i][U|u][M|m][I|i][N|n][T|t]			{return token::MEDIUMINT;}
[I|i][N|n][T|t]											{return token::INT;}
[I|i][N|n][T|t][E|e][G|g][E|e][R|r]						{return token::INTEGER;}
[B|b][I|i][G|g][I|i][N|n][T|t]							{return token::BIGINT;}
"REAL"			{return token::REAL;}
[D|d][O|o][U|u][B|b][L|l][E|e]							{return token::DOUBLE;}
[F|f][L|l][O|o][A|a][T|t]								{return token::FLOAT;}
[D|d][E|e][C|c][I|i][M|m][A|a][L|l]						{return token::DECIMAL;}
[D|d][A|a][T|t][E|e]									{return token::DATE;}
[T|t][I|i][M|m][E|e]									{return token::TIME;}
[T|t][I|i][M|m][E|e][S|s][T|t][A|a][M|m][P|p]			{return token::TIMESTAMP;}
[D|d][A|a][T|t][E|e][T|t][I|i][M|m][E|e]				{return token::DATETIME;}
"YEAR"			{return token::YEAR;}
[C|c][H|h][A|a][R|r]									{return token::CHAR;}
[V|v][A|a][R|r][C|c][H|h][A|a][R|r]						{return token::VARCHAR;}
"BINARY"		{return token::BINARY;}
"VARBINARY"		{return token::VARBINARY;}
"TINYBLOB"		{return token::TINYBLOB;}
"BLOB"			{return token::BLOB;}
"MEDIUMBLOB"	{return token::MEDIUMBLOB;}
"LONGBLOB"		{return token::LONGBLOB;}
[T|t][I|i][N|n][Y|y][T|t][E|e][X|x][T|t]			{return token::TINYTEXT;}
[T|t][E|e][X|x][T|t]								{return token::TEXT;}
[M|m][E|e][D|d][I|i][U|u][M|m][T|t][E|e][X|x][T|t]	{return token::MEDIUMTEXT;}
"LONGTEXT"		{return token::LONGTEXT;}
[E|e][N|n][U|u][M|m]								{return token::ENUM;}
[S|s][E|e][T|t]										{return token::SET;}
"BIT"			{return token::BIT;}
"COLLATE"		{return token::COLLATE;}
"STRING"		{return token::STRING;}
[U|u][N|n][S|s][I|i][G|g][N|n][E|e][D|d]			{return token::UNSIGNED;}
"ZEROFILL"		{return token::ZEROFILL;}

"("				{return token::PARENTHESIS_OPEN;}
")"				{return token::PARENTHESIS_CLOSE;}
","				{return token::COMA;}
	
{D}+					{return token::INTNUM;}
L?\'(\\.|[^\\'])*\'		{return token::STRING;}
\n          	{
				   // Update line number
				   loc->lines();
				   return token::NEWLINE;
				}
				
[ ]+			{return token::SPACE;}				
.				{return token::UNKNOW;}

%%


