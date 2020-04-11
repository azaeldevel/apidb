#include <cctype>
#include <fstream>
#include <cassert>
#include <sstream> 

#include "analyzer-mysql.hpp"
#include "Errors.hpp"


namespace octetos
{
namespace apidb
{	
namespace mysql
{
	bool Analyzer::parse(symbols::Symbol* symbol)
	{
		//symbol->outType = parse(symbol->inType);
		std::cout << "symbol->inType = "<< symbol->inType << std::endl;
		return false;
	}
	bool Analyzer::parse(symbols::ISpace* ispace)
	{
		if(configureProject.inputLenguaje == InputLenguajes::MySQL)
		{
			if(ispace->what() == symbols::SpaceType::TABLE)
			{
				symbols::Table* tb = (symbols::Table*) ispace;
				for(symbols::Table::iterator it = tb->begin(); it != tb->end(); it++)
				{
					if(parse(it->second) == false) return false;
				}
			}
			else if(ispace->what() == symbols::SpaceType::SPACE)
			{
				if(ispace->what() == symbols::SpaceType::TABLE)
				{
					symbols::Table* tb = (symbols::Table*) ispace;
					for(symbols::Table::iterator it = tb->begin(); it != tb->end(); it++)
					{
						if(parse(it->second) == false) return false;
					}
				}
				else if(ispace->what() == symbols::SpaceType::SPACE)
				{
					symbols::Space* space = (symbols::Space*) ispace;
					//std::cout << "Espacio '" << space->getFullName() << "'" << std::endl;
					for(symbols::Space::iterator it = space->begin(); it != space->end(); it++)
					{
						if(parse(it->second) == false) return false;
					}
				}
			}
		}
		else
		{
			core::Error::write(core::Error("El lenguaje de entrada no esá soportado.",ErrorCodes::ERROR_UNNSOPORTED_INPUTLANGUAGE,__FILE__,__LINE__));
			return false;
		}
		
		return true;
	}
	bool Analyzer::analyze(core::ActivityProgress* progress)
	{
		bool flag = listing();		
		if(flag == false) return false;
		
		for(symbols::SymbolsTable::iterator it = symbolsTable.begin(); it != symbolsTable.end(); it++)
		{
			if(it->second->what() == symbols::SpaceType::SPACE)
			{
				symbols::Space* space = (symbols::Space*)it->second;
				std::cout  << "\n"<< space->getName() << std::endl;
				for(symbols::Space::iterator subIt = space->begin(); subIt != space->end(); subIt++)
				{
					if(subIt->second->what() == symbols::SpaceType::SPACE)
					{
						std::cout << "\t" << ((symbols::Space*)subIt->second)->getName() << std::endl;
					}
					else if(subIt->second->what() == symbols::SpaceType::TABLE)
					{
						std::cout << "\t" << ((symbols::Table*)subIt->second)->getName() << std::endl;
					}
				}
			}
		}
		
		for(std::map<const char*,symbols::ISpace*,symbols::cmp_str>::iterator it = symbolsTable.begin(); it != symbolsTable.end(); it++)
		{
			if(basicSymbols(it->second,progress) == false) return false;
		}
                
		for(std::map<const char*,symbols::ISpace*,symbols::cmp_str>::iterator it = symbolsTable.begin(); it != symbolsTable.end(); it++)
		{
			if(fillKeyType(it->second,progress) == false) return false;
		}
		
		for(std::map<const char*,symbols::ISpace*,symbols::cmp_str>::iterator it = symbolsTable.begin(); it != symbolsTable.end(); it++)
		{
			parse(it->second);
		}
		
		return true;
	}
	Analyzer::Analyzer(const ConfigureProject& config,octetos::db::Connector* conn,core::ActivityProgress* p) : apidb::Analyzer(config,conn,p)
	{
        types type_tinyint;
        type_tinyint.in = "tinyint";
        type_tinyint.out_cpp = "int";
        listtypes.push_back(type_tinyint);
        
        types type_tinyint;
        type_smallint.in = "smallint";
        type_smallint.out_cpp = "int";
        listtypes.push_back(type_smallint);
        
        types type_mediumint;
        type_mediumint.in = "mediumint";
        type_mediumint.out_cpp = "int";
        listtypes.push_back(type_mediumint);
        
        types type_int;
        type_mediumint.in = "int";
        type_mediumint.out_cpp = "int";
        listtypes.push_back(type_int);
        
        types type_integer;
        type_integer.in = "integer";
        type_integer.out_cpp = "int";
        listtypes.push_back(type_integer);
        
        types type_bigint;
        type_bigint.in = "bigint";
        type_bigint.out_cpp = "long";
        listtypes.push_back(type_bigint);
        
        types type_double;
        type_bigint.in = "double";
        type_bigint.out_cpp = "double";
        listtypes.push_back(type_double);
        
        types type_float;
        type_float.in = "float";
        type_float.out_cpp = "float";
        listtypes.push_back(type_float);
        
        types type_decimal;
        type_decimal.in = "decimal";
        type_decimal.out_cpp = "double";
        listtypes.push_back(type_decimal);
        
        types type_date;
        type_date.in = "date";
        type_date.out_cpp = "std::string";
        listtypes.push_back(type_date);
        
        types type_time;
        type_time.in = "time";
        type_time.out_cpp = "std::string";
        listtypes.push_back(type_time);
        
        types type_timestamp;
        type_timestamp.in = "timestamp";
        type_timestamp.out_cpp = "std::string";
        listtypes.push_back(timestamp);
        
        types type_datatime;
        type_datatime.in = "datatime";
        type_datatime.out_cpp = "std::string";
        listtypes.push_back(type_datatime);
        
        
	}
	

		
	/*std::string Analyzer::parse(const std::string& line)
	{
		std::istringstream text(line);
		parse(text);
		return oneLine;
	}
	Analyzer::~Analyzer()
	{
		delete(scanner);
		scanner = nullptr;
		delete(parser);
		parser = nullptr;	   
	}

	void Analyzer::parse( const char * const filename )
	{
	   assert( filename != nullptr );
	   std::ifstream in_file( filename );
	   if( ! in_file.good() )
	   {
		   exit( EXIT_FAILURE );
	   }
	   parse_helper( in_file );
	   return;
	}
	
	void Analyzer::parse( std::istream &stream )
	{
	   if( ! stream.good()  && stream.eof() )
	   {
		   return;
	   }
	   //else
	   parse_helper( stream ); 
	   return;
	}


	void Analyzer::parse_helper( std::istream &stream )
	{
	   delete(scanner);
	   try
	   {
		  scanner = new Scanner( &stream );
	   }
	   catch( std::bad_alloc &ba )
	   {
		  std::cerr << "Failed to allocate scanner: (" << ba.what() << "), exiting!!\n";
		  exit( EXIT_FAILURE );
	   }
	   
	   delete(parser); 
	   try
	   {
		  parser = new Parser(*scanner,*this);
	   }
	   catch( std::bad_alloc &ba )
	   {
		  std::cerr << "Failed to allocate parser: (" <<  ba.what() << "), exiting!!\n";
		  exit( EXIT_FAILURE );
	   }
	   const int accept(0);
	   if( parser->parse() != accept )
	   {
		  std::cerr << "Parse failed!!\n";
	   }
	   return;
	}


	std::ostream& Analyzer::print( std::ostream &stream )
	{
	   return(stream);
	}*/
}
}
}
