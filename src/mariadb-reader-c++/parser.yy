%skeleton "lalr1.cc"
%require  "3.0"
%debug 
%defines 
%define api.namespace {octetos::apidb::mariadb}
%define parser_class_name {Parser}

%code requires
{
        namespace octetos
        {
                namespace apidb 
                {
                        namespace mariadb
                        {
                                class Analyzer;
                                class Scanner;
                        }
                }
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

%parse-param { mariadb::Scanner  &scanner  }
%parse-param { mariadb::Analyzer  &driver  }

%code
{
	#include <iostream>
	#include <cstdlib>
	#include <fstream>
	
	/* include for all driver functions */
	#include "analyzer.hpp"
	
	#undef yylex
	#define yylex scanner.yylex
}

%define api.value.type variant
%define parse.assert

%token TINYINT
%token SMALLINT
%token MEDIUMINT
%token INT
%token INTEGER
%token BIGINT
%token REAL
%token DOUBLE
%token FLOAT
%token DECIMAL
%token DATE
%token TIME
%token TIMESTAMP
%token DATETIME
%token YEAR
%token CHAR
%token VARCHAR
%token BINARY
%token VARBINARY
%token TINYBLOB
%token BLOB
%token MEDIUMBLOB
%token LONGBLOB
%token TINYTEXT
%token TEXT
%token MEDIUMTEXT
%token LONGTEXT
%token ENUM
%token SET
%token COLLATE
%token BIT
%token STRING
%token INTNUM
%token UNSIGNED
%token ZEROFILL

%token 		PARENTHESIS_OPEN
%token 		PARENTHESIS_CLOSE
%token 		COMA
%token 		SIMPLE_COMMILLA
%token 		SPACE
%token               END    0     "end of file"
%token               NEWLINE
%token               UNKNOW

%start data_type
%locations

%%

data_type: BIT opt_length end
	| TINYINT opt_length opt_uz end
		{
			if((driver.getConfigureProject().outputLenguaje == OutputLenguajes::CPP) | (driver.getConfigureProject().outputLenguaje== OutputLenguajes::C) |  (driver.getConfigureProject().outputLenguaje== OutputLenguajes::JAVA))
			{
				driver.oneLine = "short";
			}
			else
			{
                                BuildException fail("OutputLenguaje is unknow.");
				driver.getOutput().add(fail);
				driver.oneLine =  "";
			}
		}
	| SMALLINT opt_length opt_uz end
		{
			if((driver.getConfigureProject().outputLenguaje== OutputLenguajes::CPP) | (driver.getConfigureProject().outputLenguaje== OutputLenguajes::C))
			{
				driver.oneLine = "unsigned char";
			}
			else if(driver.getConfigureProject().outputLenguaje== OutputLenguajes::JAVA)
			{
				driver.oneLine = "byte";
			}
			else
			{
                                BuildException fail("OutputLenguaje is unknow.");
				driver.getOutput().add(fail);
				driver.oneLine =  "";
			}
		}
	| MEDIUMINT opt_length opt_uz end
		{
			if((driver.getConfigureProject().outputLenguaje == OutputLenguajes::CPP) | (driver.getConfigureProject().outputLenguaje== OutputLenguajes::C))
			{
				driver.oneLine = "int";
			}
			else if(driver.getConfigureProject().outputLenguaje== OutputLenguajes::JAVA)
			{
				driver.oneLine = "int";
			}
			else
			{
                                BuildException fail("OutputLenguaje is unknow.");
				driver.getOutput().add(fail);
				driver.oneLine =  "";
			}
		}
	| INT opt_length opt_uz end
		{
			if((driver.getConfigureProject().outputLenguaje == OutputLenguajes::CPP) | (driver.getConfigureProject().outputLenguaje == OutputLenguajes::C))
			{
				driver.oneLine = "int";
			}
			else if(driver.getConfigureProject().outputLenguaje== OutputLenguajes::JAVA)
			{
				driver.oneLine = "int";
			}
			else
			{
                                BuildException fail("OutputLenguaje is unknow.");
				driver.getOutput().add(fail);
				driver.oneLine =  "";
			}
		}
	| INTEGER opt_length opt_uz end
		{
			if((driver.getConfigureProject().outputLenguaje == OutputLenguajes::CPP) | (driver.getConfigureProject().outputLenguaje== OutputLenguajes::C))
			{
				driver.oneLine = "int";
			}
			else if(driver.getConfigureProject().outputLenguaje== OutputLenguajes::JAVA)
			{
				driver.oneLine = "int";
			}
			else
			{
                                BuildException fail("OutputLenguaje is unknow.");
				driver.getOutput().add(fail);
				driver.oneLine =  "";
			}
		}
	| BIGINT opt_length opt_uz end
		{
			if((driver.getConfigureProject().outputLenguaje == OutputLenguajes::CPP) | (driver.getConfigureProject().outputLenguaje == OutputLenguajes::C))
			{
				driver.oneLine = "long";
			}
			else if(driver.getConfigureProject().outputLenguaje== OutputLenguajes::JAVA)
			{
				driver.oneLine = "long";
			}
			else
			{
                                BuildException fail("OutputLenguaje is unknow.");
				driver.getOutput().add(fail);
				driver.oneLine =  "";
			}
		}
	| REAL opt_length opt_uz end
	| DOUBLE opt_length opt_uz end
		{
			if((driver.getConfigureProject().outputLenguaje == OutputLenguajes::CPP) | (driver.getConfigureProject().outputLenguaje == OutputLenguajes::C))
			{
				driver.oneLine = "double";
			}
			else if(driver.getConfigureProject().outputLenguaje== OutputLenguajes::JAVA)
			{
				driver.oneLine = "double";
			}
			else
			{
                                BuildException fail("OutputLenguaje is unknow.");
				driver.getOutput().add(fail);
				driver.oneLine =  "";
			}
		}
	| FLOAT opt_length opt_uz end
		{
			if((driver.getConfigureProject().outputLenguaje == OutputLenguajes::CPP) | (driver.getConfigureProject().outputLenguaje == OutputLenguajes::C))
			{
				driver.oneLine = "float";
			}
			else if(driver.getConfigureProject().outputLenguaje== OutputLenguajes::JAVA)
			{
				driver.oneLine = "float";
			}
			else
			{
                                BuildException fail("OutputLenguaje is unknow.");
				driver.getOutput().add(fail);
				driver.oneLine =  "";
			}
		}
	| DECIMAL opt_length opt_uz end
		{
            printf("Advertencia: APIDB no maneja correctamente el tipo de dato decimal, se recomoenda Float o Double segun corresponda.");
			if((driver.getConfigureProject().outputLenguaje == OutputLenguajes::CPP) | (driver.getConfigureProject().outputLenguaje == OutputLenguajes::C))
			{
				driver.oneLine = "double";
			}
			else if(driver.getConfigureProject().outputLenguaje== OutputLenguajes::JAVA)
			{
				driver.oneLine = "double";
			}
			else
			{
                                BuildException fail("OutputLenguaje is unknow.");
				driver.getOutput().add(fail);
				driver.oneLine =  "";
			}
		}
	| DATE end
		{
			if(driver.getConfigureProject().outputLenguaje == OutputLenguajes::CPP)
			{
				driver.oneLine = "std::string";
			}
			else if(driver.getConfigureProject().outputLenguaje== OutputLenguajes::JAVA)
			{
				driver.oneLine = "String";
			}
			else
			{
                                BuildException fail("OutputLenguaje is unknow.");
				driver.getOutput().add(fail);
				driver.oneLine =  "";
			}
		}
	| TIME end
		{
			if(driver.getConfigureProject().outputLenguaje == OutputLenguajes::CPP)
			{
				driver.oneLine = "std::string";
			}
			else if(driver.getConfigureProject().outputLenguaje== OutputLenguajes::JAVA)
			{
				driver.oneLine = "String";
			}
			else
			{
                                BuildException fail("OutputLenguaje is unknow.");
				driver.getOutput().add(fail);
				driver.oneLine =  "";
			}
		}
	| TIMESTAMP end
		{
			if(driver.getConfigureProject().outputLenguaje == OutputLenguajes::CPP)
			{
				driver.oneLine = "std::string";
			}
			else if(driver.getConfigureProject().outputLenguaje== OutputLenguajes::JAVA)
			{
				driver.oneLine = "String";
			}
			else
			{
                                BuildException fail("OutputLenguaje is unknow.");
				driver.getOutput().add(fail);
				driver.oneLine =  "";
			}
		}
	| DATETIME end
		{
			if(driver.getConfigureProject().outputLenguaje == OutputLenguajes::CPP)
			{
				driver.oneLine = "std::string";
			}
			else if(driver.getConfigureProject().outputLenguaje== OutputLenguajes::JAVA)
			{
				driver.oneLine = "String";
			}
			else
			{
                                BuildException fail("OutputLenguaje is unknow.");
				driver.getOutput().add(fail);
				driver.oneLine =  "";
			}
		}
	| YEAR end
		{
			if(driver.getConfigureProject().outputLenguaje == OutputLenguajes::CPP)
			{
				driver.oneLine = "std::string";
			}
			else if(driver.getConfigureProject().outputLenguaje== OutputLenguajes::JAVA)
			{
				driver.oneLine = "String";
			}
			else
			{
                                BuildException fail("OutputLenguaje is unknow.");
				driver.getOutput().add(fail);
				driver.oneLine =  "";
			}
		}
	| CHAR opt_length opt_csc end
		{
			if(driver.getConfigureProject().outputLenguaje == OutputLenguajes::CPP)
			{
				driver.oneLine = "std::string";
			}
			else if(driver.getConfigureProject().outputLenguaje== OutputLenguajes::JAVA)
			{
				driver.oneLine = "String";
			}
			else
			{
                                BuildException fail("OutputLenguaje is unknow.");
				driver.getOutput().add(fail);
				driver.oneLine =  "";
			}
		}
	| VARCHAR opt_length opt_csc end
		{
			if(driver.getConfigureProject().outputLenguaje == OutputLenguajes::CPP)
			{
				driver.oneLine = "std::string";
			}
			else if(driver.getConfigureProject().outputLenguaje== OutputLenguajes::JAVA)
			{
				driver.oneLine = "String";
			}
			else
			{
                                BuildException fail("OutputLenguaje is unknow.");
				driver.getOutput().add(fail);
				driver.oneLine =  "";
			}
		}
	| BINARY opt_length end
	| VARBINARY PARENTHESIS_OPEN INTNUM PARENTHESIS_CLOSE end
	| TINYBLOB end
	| BLOB end
	| MEDIUMBLOB end
	| LONGBLOB end
	| TINYTEXT opt_binary opt_csc end
		{
			if(driver.getConfigureProject().outputLenguaje == OutputLenguajes::CPP)
			{
				driver.oneLine = "std::string";
			}
			else if(driver.getConfigureProject().outputLenguaje== OutputLenguajes::JAVA)
			{
				driver.oneLine = "String";
			}
			else
			{
                                BuildException fail("OutputLenguaje is unknow.");
				driver.getOutput().add(fail);
				driver.oneLine =  "";
			}
		}
	| TEXT opt_binary opt_csc end
		{
			if(driver.getConfigureProject().outputLenguaje == OutputLenguajes::CPP)
			{
				driver.oneLine = "std::string";
			}
			else if(driver.getConfigureProject().outputLenguaje== OutputLenguajes::JAVA)
			{
				driver.oneLine = "String";
			}
			else
			{
                                BuildException fail("OutputLenguaje is unknow.");
				driver.getOutput().add(fail);
				driver.oneLine =  "";
			}
		}
	| MEDIUMTEXT opt_binary opt_csc end
		{
			if(driver.getConfigureProject().outputLenguaje == OutputLenguajes::CPP)
			{
				driver.oneLine = "std::string";
			}
			else if(driver.getConfigureProject().outputLenguaje== OutputLenguajes::JAVA)
			{
				driver.oneLine = "String";
			}
			else
			{
                                BuildException fail("OutputLenguaje is unknow.");
				driver.getOutput().add(fail);
				driver.oneLine =  "";
			}
		}
	| LONGTEXT opt_binary opt_csc end
		{
			if(driver.getConfigureProject().outputLenguaje == OutputLenguajes::CPP)
			{
				driver.oneLine = "std::string";
			}
			else if(driver.getConfigureProject().outputLenguaje== OutputLenguajes::JAVA)
			{
				driver.oneLine = "String";
			}
			else
			{
				BuildException fail("OutputLenguaje is unknow.");
				driver.getOutput().add(fail);
				driver.oneLine =  "";
			}
		}
	| ENUM PARENTHESIS_OPEN enum_list PARENTHESIS_CLOSE opt_csc end	
		{
			if(driver.getConfigureProject().outputLenguaje == OutputLenguajes::CPP)
			{
				driver.oneLine = "std::string";
			}
			else if(driver.getConfigureProject().outputLenguaje== OutputLenguajes::JAVA)
			{
				driver.oneLine = "String";
			}
			else
			{
				BuildException fail("OutputLenguaje is unknow.");
				driver.getOutput().add(fail);
				driver.oneLine =  "";
			}
		}
	| SET PARENTHESIS_OPEN enum_list PARENTHESIS_CLOSE opt_csc end
		{
			if(driver.getConfigureProject().outputLenguaje == OutputLenguajes::CPP)
			{
				driver.oneLine = "std::string";
			}
			else if(driver.getConfigureProject().outputLenguaje== OutputLenguajes::JAVA)
			{
				driver.oneLine = "String";
			}
			else
			{
				BuildException fail("OutputLenguaje is unknow.");
				driver.getOutput().add(fail);
				driver.oneLine =  "";
			}
		}

enum_list: STRING
	| enum_list COMA STRING
	
opt_binary: /* empty */
	| BINARY
	
opt_length: /* empty */
	| PARENTHESIS_OPEN INTNUM PARENTHESIS_CLOSE
	| PARENTHESIS_OPEN INTNUM COMA INTNUM PARENTHESIS_CLOSE
	
opt_uz: /* empty */
	| opt_uz SPACE UNSIGNED
	| opt_uz SPACE ZEROFILL
	
opt_csc: /* empty */
	| opt_csc CHAR SET STRING
	| opt_csc COLLATE STRING
end : NEWLINE|END|';'


%%


void 
octetos::apidb::mariadb::Parser::error(const location_type &l, const std::string &err_message)
{
	driver.oneLine =  "";
	std::cerr << "Error: " << err_message << " at " << l << "\n";
}
