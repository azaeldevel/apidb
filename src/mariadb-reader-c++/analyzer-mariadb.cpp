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

#include <iostream>
#include <mariadb/mysql.h>
#include <string>
#include <octetos/db/clientdb-maria.hh>
#include "analyzer.hpp"


extern "C" octetos::apidb::mariadb::Analyzer* createAnalyzer(const octetos::apidb::ConfigureProject* config,octetos::db::Connector* connector,octetos::core::ActivityProgress* progress)
{
    return new octetos::apidb::mariadb::Analyzer(*config,connector,progress);
}

extern "C" void destroyAnalyzer(octetos::apidb::mariadb::Analyzer* object)
{
    delete object;
}


namespace octetos
{
namespace apidb
{
namespace mariadb
{
	bool Analyzer::listing()
	{
		std::string db = connector->getDatconection()->getDatabase();
		std::string str = "SELECT TABLE_NAME FROM information_schema.tables WHERE TABLE_SCHEMA = '";
		str = str + db + "' and TABLE_TYPE = 'BASE TABLE' ORDER BY TABLE_NAME ASC";
		octetos::db::mariadb::Datresult dt;
        bool flag = connector->execute(str,dt);
		//std::cout << str  <<std::endl;
		if(flag) 
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
			MYSQL_ROW row;
			while ((row = mysql_fetch_row((MYSQL_RES*)(dt.getResult()))))
			{
				symbols::Table* prw = new symbols::Table(symbols::getFirstName(row[0]));
				std::string upper = row[0];
				upper[0] = toupper(upper[0]);
				prw->upperName = upper;
				prw->space = symbols::getSpacePatch(row[0]);
				prw->fullname = row[0];
				int level = symbols::getSpaceLevel(prw->fullname);
				//std::cout << "Presesando : "<< level  << " - " << prw->fullname << std::endl;
				if(level == 0)
				{
					spaceGlobal->addTable(prw);
				}
				else if(level > 0 and configureProject.namespace_detect.compare("emulate") == 0)
				{
					//std::cout << "\nNested Tabla : " << prw->fullname << std::endl;
					std::string spacePath = symbols::getSpacePatch(row[0]);
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
			
			return true;
		}
		else
		{
			std::string msg = "";
			msg = msg + " MariaDB Server Error No. : '";
			msg = msg + std::to_string(mysql_errno((MYSQL*)connector->getConnection()));
			msg = msg + "' ";
			msg = msg + mysql_error((MYSQL*)connector->getConnection());
			core::Error::write(core::Error(msg,ErrorCodes::ANALYZER_FAIL,__FILE__,__LINE__));
			return false;
		}	
		return true;
	}
}
}
}
