/**
 * 
 *  This file is part of apidb.
 *  APIDB do Make easy to connect your Database
 *  Copyright (C) 2018  Azael Reyes
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
#if DISTRO_GENTOO
    #include <libpq-fe.h>
#endif
#if DISTRO_ARCHLINUX
    #include <libpq-fe.h>
#endif
#include <cstring>
#include <cstdio>
#include <iostream>
#include <string>
#include <octetos/db/clientdb-postgresql.hh>

#include "analyzer.hpp"
#include "../common-postgresql.hpp"

extern "C" octetos::apidb::postgresql::Analyzer* createAnalyzer(const octetos::apidb::ConfigureProject* config,octetos::db::Connector* connector,octetos::core::ActivityProgress* progress)
{
    return new octetos::apidb::postgresql::Analyzer(*config,connector,progress);
}

extern "C" void destroyAnalyzer(octetos::apidb::postgresql::Analyzer* object)
{
    delete object;
}


namespace octetos
{
namespace apidb
{
namespace postgresql
{
	bool Analyzer::listing()
	{
        //std::cout << "Analyzer::listing : Step 1\n"  <<std::endl;
		std::string db = connector->getDatconection()->getDatabase();
		std::string str = "SELECT * FROM pg_catalog.pg_tables WHERE schemaname != 'pg_catalog' AND schemaname != 'information_schema' ";
        //std::cout << str << "\n";
		octetos::db::postgresql::Datresult dt;
        bool flag = connector->execute(str,dt);
        //std::cout << "Analyzer::listing : Step 2\n"  <<std::endl;
		if(flag) 
		{
            //std::cout << "Analyzer::listing : Step 2.1\n"  <<std::endl;
			symbols::SymbolsTable::iterator itGlobal = symbolsTable.find(configureProject.name.c_str());
			if(itGlobal == symbolsTable.end())
			{
				core::Error::write(core::Error("No se encontró Espacion Global",ErrorCodes::ANALYZER_FAIL,__FILE__,__LINE__));
				return false;
			}
            //std::cout << "Analyzer::listing : Step 2.2\n"  <<std::endl;
			symbols::Space* spaceGlobal = (symbols::Space*)(itGlobal->second);
			spaceGlobal->clear();
			if(spaceGlobal == NULL)
			{
				core::Error::write(core::Error("No se encontró Espacion Global",ErrorCodes::ANALYZER_FAIL,__FILE__,__LINE__));
				return false;
			}
            //std::cout << "Analyzer::listing : Step 2.3\n"  <<std::endl;
			//std::cout << str << "\n";
			while (dt.nextRow())
			{
                //std::cout << "Analyzer::listing : Step 2.3.1 : Tabla encontrada : " << dt.getString(1) << "\n";
				symbols::Table* prw = new symbols::TablePostgreSQL(dt.getString(1));
				std::string upper = dt.getString(1);
				upper[0] = std::toupper(dt.getString(1).c_str()[0]);
				prw->upperName = upper;
				prw->space = symbols::getSpacePatch(dt.getString(1));
				prw->fullname = dt.getString(1);
				int level = symbols::getSpaceLevel(prw->fullname);
				//std::cout << "Presesando : "<< level  << " - " << prw->fullname << std::endl;
				if(level == 0)
				{
					spaceGlobal->addTable(prw);
				}
				else if(level > 0 and configureProject.namespace_detect.compare("emulate") == 0)
				{
					//std::cout << "\nNested Tabla : " << prw->fullname << std::endl;
					std::string spacePath = symbols::getSpacePatch(dt.getString(1));
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
            //std::cout << "Analyzer::listing : Step 2.4\n"  <<std::endl;
			return true;
		}
		else
		{
			std::string msg = "";
			msg = msg + " PostgreSQL Server Error : '";
			msg = msg + PQerrorMessage((PGconn *)connector->getConnection());
			msg = msg + "' ";
			core::Error::write(core::Error(msg,ErrorCodes::ANALYZER_FAIL,__FILE__,__LINE__));
			return false;
		}		
        //std::cout << "Analyzer::listing : Step 3\n"  <<std::endl;
		return true;
	}
}
}
}
