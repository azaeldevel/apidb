#include <cctype>
#include <fstream>
#include <cassert>
#include <sstream> 

#include "analyzer-postgresql.hpp"
#include "Errors.hpp"


namespace octetos
{
namespace apidb
{	
namespace postgresql
{
	bool Analyzer::parse(symbols::Symbol* symbol)
	{
		//symbol->outType = parse(symbol->inType);
		//std::cout << "symbol->inType = "<< symbol->inType << std::endl;
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
        std::cout << "Entrando en fase de Análisis PostgreSQL\n";
		/*bool flag = listing();		
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
		}*/
		
		return true;
	}
	Analyzer::Analyzer(const ConfigureProject& config,octetos::db::Connector* conn,core::ActivityProgress* p) : apidb::Analyzer(config,conn,p)
	{
	}
	

    std::string Analyzer::parse(const std::string& str)
	{
		std::cout << "symbol->inType = "<< str << std::endl;
	}
	
	Analyzer::~Analyzer()
    {
        
    }
}
}
}
