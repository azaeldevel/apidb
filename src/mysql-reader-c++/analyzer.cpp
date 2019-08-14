#include <cctype>
#include <fstream>
#include <cassert>
#include <sstream> 

#include "analyzer.hpp"
#include "../Errors.hpp"


namespace octetos
{
namespace apidb
{	
namespace mysql
{
	bool Analyzer::parse(symbols::Symbol* symbol)
	{
		symbol->outType = parse(symbol->inType);
		//std::cout << symbol->inType << " -> " << symbol->outType << std::endl;
		return true;
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
			toolkit::Error::write(toolkit::Error("El lenguaje de entrada no esá soportado.",ErrorCodes::ERROR_UNNSOPORTED_INPUTLANGUAGE,__FILE__,__LINE__));
			return false;
		}
		
		return true;
	}
	bool Analyzer::analyze(toolkit::ActivityProgress* progress)
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
	Analyzer::Analyzer(const ConfigureProject& config,octetos::toolkit::clientdb::Connector* conn,toolkit::ActivityProgress* p) : apidb::Analyzer(config,conn,p)
	{
	}
	

		
	std::string Analyzer::parse(const std::string& line)
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
	}
}
}
}
