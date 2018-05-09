%{
/**
 * 
 *  This file is part of apidb.
 * 
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * 
 * */

#include <stdio.h>
#include "lex-sql.h"
%}
%pointer

%%
"TINYINT"				{printf("TYPE_INT_BYTES1\n");return TYPE_INT_BYTES1;}
"SMALLINT"				{printf("TYPE_INT_BYTES2\n");return TYPE_INT_BYTES2;}
"MEDIUMINT"				{printf("TYPE_INT_BYTES3\n");return TYPE_INT_BYTES3;}
"INT"					{printf("TYPE_INT_BYTES4\n");return TYPE_INT_BYTES4;}
"INTEGER"				{printf("TYPE_INT_BYTES4\n");return TYPE_INT_BYTES4;}
"BIGINT"				{printf("TYPE_BIGINT\n");return TYPE_INT_BYTES8;}
"BIT"					{printf("ITYPE_NT_BYTES1\n");return TYPE_BIT;}

"DECIMAL"				{printf("TYPE_DOUBLE\n");return TYPE_DOUBLE;}
"NUMERIC"				{printf("TYPE_DOUBLE\n");return TYPE_DOUBLE;}
"FLOAT"					{printf("TYPE_FLOAT\n");return TYPE_FLOAT;}
"DOUBLE"				{printf("TYPE_DOUBLE\n");return TYPE_DOUBLE;}

"DATE"					{printf("STRING\n");return TYPE_STRING;}
"DATETIME"				{printf("TYPE_STRING\n");return TYPE_STRING;}
"TIMESTAMP"				{printf("TYPE_STRING\n");return TYPE_STRING;}
"TIME"					{printf("TYPE_STRING\n");return TYPE_STRING;}
"YEAR"					{printf("TYPE_INT_BYTES2\n");return TYPE_INT_BYTES2;}

"CHAR"					{printf("TYPE_CHAR\n");return TYPE_CHAR;}
"VARCHAR"				{printf("TYPE_STRING\n");return TYPE_STRING;}
"BINARY"				{printf("TYPE_STRING\n");return TYPE_STRING;}
"VARBINARY"				{printf("TYPE_STRING\n");return TYPE_STRING;}
"BLOB"					{printf("TYPE_STRING\n");return TYPE_STRING;}
"TEXT"					{printf("TYPE_STRING\n");return TYPE_STRING;}
"ENUM"					{printf("TYPE_STRING\n");return TYPE_STRING;}
"SET"					{printf("TYPE_STRING\n");return TYPE_STRING;}
","						{printf("COMA\n");return COMA;}
"("						{printf("PARENTESIS ABIERTO\n");return PARENTHESIS_OPEN;}
")"						{printf("PARENTESIS CERRADO\n");return PARENTHESIS_CLOSE;}
[0123456789]+           {printf("NUMERO\n");return NUMBER;}
[ \t]+					{printf("SPACE\n");return SPACE;}
.						{printf("?\n");return UNKNOW;}


\n						{printf("Bye\n");return END;}	
		
%%
int main( int argc, char **argv )
{
    ++argv, --argc;  /* skip over program name */
    if ( argc > 0 )
            yyin = fopen( argv[0], "r" );
    else
            yyin = stdin;
	int tk = UNKNOW;
	do
	{
		tk = yylex();
	}
	while(tk != END);
    
    return 0;
}
