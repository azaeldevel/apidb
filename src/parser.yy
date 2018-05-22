%skeleton "lalr1.cc"
%require  "3.0"
%debug 
%defines 
%define api.namespace {apidb}
%define parser_class_name {Parser}

%code requires
{
	namespace apidb 
	{
      class Driver;
      class Scanner;
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

%parse-param { Scanner  &scanner  }
%parse-param { Driver  &driver  }

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
%token               NEWLINE
%token               UNKNOW
%token			NUMBER

%locations

%%

declare_type :
	integerDeclareVariable declare_end		{
												if((driver.getOutputLenguaje() == Driver::OutputLenguajes::CPP) | (driver.getOutputLenguaje() == Driver::OutputLenguajes::C))
												{
													driver.oneLine = "int";
												}
												else
												{
													driver.message("OutputLenguaje is unknow.");
													driver.oneLine =  "";
												}
											}
	|
	stringDeclareVarible declare_end 		{
												if(driver.getOutputLenguaje() == Driver::OutputLenguajes::CPP)
												{
													driver.oneLine = "std::string";
												}
												else if(driver.getOutputLenguaje() == Driver::OutputLenguajes::C)
												{
													driver.oneLine ="const char*";
												}
												else
												{
													driver.message("OutputLenguaje is unknow.");
													driver.oneLine = "";
												}
											}
	|
	integerTypes declare_end    		{
											if((driver.getOutputLenguaje() == Driver::OutputLenguajes::CPP) | (driver.getOutputLenguaje() == Driver::OutputLenguajes::C))
											{
												driver.oneLine = "int";
											}
											else
											{
												driver.message("OutputLenguaje is unknow.");
												driver.oneLine =  "";
											}
										}
	|
	realDeclareLow declare_end	    		{
												if((driver.getOutputLenguaje() == Driver::OutputLenguajes::CPP) | (driver.getOutputLenguaje() == Driver::OutputLenguajes::C))
												{
													driver.oneLine = "float";
												}
												else
												{
													driver.message("OutputLenguaje is unknow.");
													driver.oneLine =  "";
												}
											}
	|
	realDeclareHigh declare_end			    {
												if((driver.getOutputLenguaje() == Driver::OutputLenguajes::CPP) | (driver.getOutputLenguaje() == Driver::OutputLenguajes::C))
												{
													driver.oneLine = "duble";
												}
												else
												{
													driver.message("OutputLenguaje is unknow.");
													driver.oneLine =  "";
												}
											}						
	;

stringDeclareVarible: stringVariableTypes PARENTHESIS_OPEN NUMBER PARENTHESIS_CLOSE;

integerDeclareVariable: integerTypes PARENTHESIS_OPEN NUMBER PARENTHESIS_CLOSE;

stringVariableTypes : VARCHAR | CHAR | TEXT;

integerTypes : INT | INTEGER;

realDeclareLow : realDeclareTypesLow PARENTHESIS_OPEN NUMBER COMA NUMBER PARENTHESIS_CLOSE;

realDeclareHigh : realDeclareTypesHigh PARENTHESIS_OPEN NUMBER COMA NUMBER PARENTHESIS_CLOSE;

realDeclareTypesLow : NUMERIC | FLOAT;

realDeclareTypesHigh : DECIMAL | REAL | DOUBLE;

declare_end : END | NEWLINE;


%%


void 
apidb::Parser::error( const location_type &l, const std::string &err_message )
{
	driver.oneLine =  "";
	std::cerr << "Error: " << err_message << " at " << l << "\n";
}
