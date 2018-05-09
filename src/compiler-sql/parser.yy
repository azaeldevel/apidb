%skeleton "lalr1.cc"
%require  "3.0"
%debug 
%defines 
%define api.namespace {MC}
%define parser_class_name {MC_Parser}

%code requires{
   namespace MC {
      class MC_Driver;
      class MC_Scanner;
   }

// The following definitions is missing when %locations isn't used
# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

}

%parse-param { MC_Scanner  &scanner  }
%parse-param { MC_Driver  &driver  }

%code{
   #include <iostream>
   #include <cstdlib>
   #include <fstream>
   
   /* include for all driver functions */
   #include "driver.hpp"

#undef yylex
#define yylex scanner.yylex
}

%define api.value.type variant
%define parse.assert

%token			TINYINT
%token			SMALLINT
%token			MEDIUMINT
%token			INT
%token			INTEGER
%token			BIGINT
%token			BIT
%token			DECIMAL
%token			NUMERIC
%token			REAL
%token			FLOAT
%token			DOUBLE
%token			DATE
%token			DATETIME
%token			TIMESTAMP
%token			TIME
%token			YEAR
%token			CHAR
%token			VARCHAR
%token			BINARY
%token			VARBINARY
%token			BLOB
%token			TEXT
%token			ENUM
%token			SET


%token			COMA
%token			PARENTHESIS_OPEN
%token			PARENTHESIS_CLOSE
%token			SPACE
%token               END    0     "end of file"
%token               UPPER
%token               LOWER
%token <std::string> WORD
%token               NEWLINE
%token               CHARTER
%token               UNKNOW

%locations

%%

declare_type :
	TEXT declare_end|
	INTEGER declare_end|	
	INT declare_end|
	stringVarible declare_end|
	integerVariable declare_end;

stringVarible: stringVariableTypes PARENTHESIS_OPEN INTEGER PARENTHESIS_CLOSE;

stringVariableTypes : VARCHAR | CHAR | TEXT;

integerVariable : integerVariableTypes PARENTHESIS_OPEN INTEGER COMA INTEGER PARENTHESIS_CLOSE;
	
integerVariableTypes : DECIMAL | NUMERIC | REAL | FLOAT | DOUBLE;

declare_end : END | NEWLINE;





%%


void 
MC::MC_Parser::error( const location_type &l, const std::string &err_message )
{
   std::cerr << "Error: " << err_message << " at " << l << "\n";
}
