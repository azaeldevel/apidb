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
		bool flag = listing();		
		if(!flag) return false;
		
		for(symbols::SymbolsTable::iterator it = symbolsTable.begin(); it != symbolsTable.end(); it++)
		{
			if(it->second->what() == symbols::SpaceType::SPACE)
			{
				symbols::Space* space = (symbols::Space*)it->second;
				//std::cout  << "\n"<< space->getName() << std::endl;
				for(symbols::Space::iterator subIt = space->begin(); subIt != space->end(); subIt++)
				{
					if(subIt->second->what() == symbols::SpaceType::SPACE)
					{
						//std::cout << "\t" << ((symbols::Space*)subIt->second)->getName() << std::endl;
					}
					else if(subIt->second->what() == symbols::SpaceType::TABLE)
					{
						//std::cout << "\t" << ((symbols::Table*)subIt->second)->getName() << std::endl;
					}
				}
			}
		}
		
		for(std::map<const char*,symbols::ISpace*,symbols::cmp_str>::iterator it = symbolsTable.begin(); it != symbolsTable.end(); it++)
		{
			if(!basicSymbols(it->second,progress)) return false;
		}
        
		/*for(std::map<const char*,symbols::ISpace*,symbols::cmp_str>::iterator it = symbolsTable.begin(); it != symbolsTable.end(); it++)
		{
			if(fillKeyType(it->second,progress) == false) return false;
		}
		
		for(std::map<const char*,symbols::ISpace*,symbols::cmp_str>::iterator it = symbolsTable.begin(); it != symbolsTable.end(); it++)
		{
			parse(it->second);
		}*/
		
		return false;
	}
	Analyzer::Analyzer(const ConfigureProject& config,octetos::db::Connector* conn,core::ActivityProgress* p) : apidb::Analyzer(config,conn,p)
	{
	}
	

    std::string Analyzer::parse(const std::string& str)
	{
        return "";
	}
	
	Analyzer::~Analyzer()
    {
        
    }

	bool Analyzer::listing()
	{
		std::string db = connector->getDatconection()->getDatabase();
		std::string str = "SELECT TABLE_NAME FROM information_schema.tables WHERE TABLE_SCHEMA = 'public' and table_catalog ='";
		str = str + db + "' and TABLE_TYPE = 'BASE TABLE' ORDER BY TABLE_NAME ASC";
		octetos::db::Datresult* dt = connector->execute(str);
		//std::cout << str  <<std::endl;
		if(dt != NULL) 
		{
			symbols::SymbolsTable::iterator itGlobal = symbolsTable.find(configureProject.name.c_str());
			if(itGlobal == symbolsTable.end())
			{
				core::Error::write(core::Error("No se encontró Espacion Global",ErrorCodes::ANALYZER_FAIL,__FILE__,__LINE__));
				return false;
			}
			symbols::Space* spaceGlobal = (symbols::Space*)(itGlobal->second);
			spaceGlobal->clear();
			if(spaceGlobal == NULL)
			{
				core::Error::write(core::Error("No se encontró Espacion Global",ErrorCodes::ANALYZER_FAIL,__FILE__,__LINE__));
				return false;
			}
			while (dt->nextRow())
			{
				symbols::Table* prw = new symbols::Table(symbols::getFirstName(dt->getString(0)));
				std::string upper = dt->getString(0);
				upper[0] = toupper(upper[0]);
				prw->upperName = upper;
				prw->space = symbols::getSpacePatch(dt->getString(0));
				prw->fullname = dt->getString(0);
				int level = symbols::getSpaceLevel(prw->fullname);
				//std::cout << "Presesando : "<< level  << " - " << prw->fullname << std::endl;
				if(level == 0)
				{
					spaceGlobal->addTable(prw);
				}
				else if(level > 0 and configureProject.namespace_detect.compare("emulate") == 0)
				{
					//std::cout << "\nNested Tabla : " << prw->fullname << std::endl;
					std::string spacePath = symbols::getSpacePatch(dt->getString(0));
					//std::cout << "Space path : " << spacePath << std::endl;
					symbols::Space* space = spaceGlobal->findSpace(spacePath);
					if(space == NULL)
					{
						//std::cout << "Agregando espacio '" << spacePath << "' en '" << spaceGlobal->getName() << "' Analyzer::listing" << std::endl;  
						space = spaceGlobal->addSpace(spacePath);
						if(space != NULL)
						{
							space->addTable(prw);
						}
						else
						{
							std::string msg = "Fallo la creacion del espacion '";
							msg += spacePath + "'";
							core::Error::write(core::Error(msg,ErrorCodes::ANALYZER_FAIL,__FILE__,__LINE__));
							return false;
						}
					}
					else
					{		
						//std::cout << prw->fullname << " -> '" << space->getName() << "'" << std::endl;
						//std::pair<const char*, symbols::ISpace*> newInser(prw->fullname.c_str(),prw);
						//space->insert(newInser);
						space->addTable(prw);
					}
				}
				else if(level > 0 and configureProject.namespace_detect.compare("reject") == 0)
				{
					core::Error::write(core::Error("Usted asigno la opción 'Nombre de espcaio detectado' con el valor 'reject', está opcion impedira la contrucción del código fuente mientras haya puntos de lo nombres de tablas.",ErrorCodes::ANALYZER_FAIL_NAMESPCE_DETECTED,__FILE__,__LINE__));
					return false;
				}
				else if(configureProject.namespace_detect.empty() or configureProject.namespace_detect.compare("¿?") == 0)
				{
					core::Error::write(core::Error("Los nombre de las tablas contiene punto, esto provocra errores de compilación.\nPara solucionar esté incoveniente APIDB le propone le emulaciónn de espacios, asignando 'Deteción de nombre de espacio' = 'Emular', de esta forma APIDB creará espacio de nombre equivalentes en su lenguaje.",ErrorCodes::ANALYZER_FAIL_NAMESPCE_DETECTED,__FILE__,__LINE__));
					return false;
				}
				else
				{
					std::string msg = "El valor '";
					msg += configureProject.namespace_detect + "' no es valido para 'Nombre de espcaio detectado'.";
					core::Error::write(core::Error(msg,ErrorCodes::ANALYZER_FAIL_NAMESPCE_DETECTED,__FILE__,__LINE__));
					return false;
				}
			}
			delete dt;
			return true;
		}
		else
		{
			/*std::string msg = "";
			msg = msg + " MySQL Server Error No. : '";
			msg = msg + std::to_string(mysql_errno((MYSQL*)connector->getConnection()));
			msg = msg + "' ";
			msg = msg + mysql_error((MYSQL*)connector->getConnection());
			core::Error::write(core::Error(msg,ErrorCodes::ANALYZER_FAIL,__FILE__,__LINE__));*/
			return false;
		}	
		return true;
	}
}
}
}
