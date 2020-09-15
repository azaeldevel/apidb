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
#if defined LINUX_GENTOO
    #include <mariadb/mysql.h>
#elif defined LINUX_ARCH
    #include <mysql/mysql.h>
#else
    #error "Plataforma desconocida."
#endif
#include <string>
#include <octetos/db/clientdb-maria.hh>

#include "apidb.hpp"
#include "common-mariadb.hpp"
#include "Errors.hpp"
#include "analyzer.hpp"


namespace octetos
{
namespace apidb
{
    symbols::TableMariaDB::TableMariaDB(const std::string& s) : Table(s)
    {
        
        
    }
    /**
    * Rellena los campos 'classReferenced' y 'symbolReferenced' de la tabla
    */
    bool symbols::TableMariaDB::fillKeyType(octetos::db::Connector& connect,const SymbolsTable& symbolsTable)
	{
        /**
        * Lista las relaciones de llaves foraneas para la tabla actual
        */
		std::string fks = "SELECT k.COLUMN_NAME, k.REFERENCED_TABLE_NAME, k.REFERENCED_COLUMN_NAME FROM information_schema.TABLE_CONSTRAINTS i,information_schema.KEY_COLUMN_USAGE k WHERE i.CONSTRAINT_NAME = k.CONSTRAINT_NAME  AND i.CONSTRAINT_TYPE = 'FOREIGN KEY' AND i.TABLE_SCHEMA =k.TABLE_SCHEMA AND i.TABLE_NAME = "; 
        fks += "'";
		fks += fullname;
        fks += "' AND i.CONSTRAINT_SCHEMA =  '" ;
		fks += ((octetos::db::Datconnect*)(connect.getDatconection()))->getDatabase();
		fks += "'";
		//std::cout<<fks<<std::endl;
		//std::cout<< "In table: " <<fullname<<std::endl;
        octetos::db::mariadb::Datresult dt;
        bool flag = connect.execute(fks,dt);
        if(flag)
        {                      
			symbols::SymbolsTable::const_iterator itGlobal = symbolsTable.find(symbolsTable.getConfigureProject().name.c_str());
			symbols::Space* global = ((symbols::Space*)itGlobal->second);
            MYSQL_ROW row;
            while ((row = mysql_fetch_row((MYSQL_RES*)(dt.getResult()))))
            {
				//std::cout<<"Buscando tabla '" << row[1] << "' symbols::Table::fillKeyType Find" << std::endl;			
				symbols::Table* table = global->findTable(row[1]);
				//std::cout<<"Buscando tabla '" << row[1] << "' symbols::Table::fillKeyType return" << std::endl;	
				if(table == NULL)
				{
					std::string msg = "No se encontro la tabla '";
					msg += row[1];
					msg += "'";
					core::Error::write(core::Error(msg,ErrorCodes::ANALYZER_FAIL,__FILE__,__LINE__));
					return false;
				}
				//std::cout<<"Se encontró tabla '" << table->getName() << "'" << std::endl;
				Symbol* referenceSymbol = ((Table*)table)->findSymbol(row[2]);
				if(referenceSymbol == NULL)
				{
					std::string msg = "No se encontro el campo '";
					msg += row[2];
					msg += "'";
					core::Error::write(core::Error(msg,ErrorCodes::ANALYZER_FAIL,__FILE__,__LINE__));
					return false;
				}
				//std::cout<<"Se encontró campo '" << referenceSymbol->getName() << "'" << std::endl;
				Symbol* targetSymbol = findSymbol(row[0]);
				if(targetSymbol == NULL)
				{
					std::string msg = "No se encontro el campo '";
					msg += row[0];
					msg += "'";
					core::Error::write(core::Error(msg,ErrorCodes::ANALYZER_FAIL,__FILE__,__LINE__));
					return false;
				}
				//std::cout<< targetSymbol->getName() << "-->" << referenceTable->getName()  << ":" << referenceSymbol->getName() << "'" << std::endl;
			}	
		}
		else
		{
			//std::cout << "No retorno resultado la consulta" << std::endl;
		}
        
		return true;
    }
	
	
    bool symbols::TableMariaDB::basicSymbols(octetos::db::Connector& connect)
    {
        std::string str = "DESCRIBE ";
        if(space.compare("") != 0)
        {
            str += "`" + space + "." + name + "`";
        }
        else
        {
            str += name;
        }
        octetos::db::mariadb::Datresult dt;
        bool flag = connect.execute(str,dt);
		if(flag) 
		{
			//std::cout<<str<<std::endl;
			//MYSQL_RES *result = mysql_store_result((MYSQL*)connect.getServerConnector());
			MYSQL_ROW row;
			bool setkey = false;
			while ((row = mysql_fetch_row((MYSQL_RES*)(dt.getResult()))))
			{
				Symbol* attrribute = new Symbol();
				attrribute->classParent = this;
				attrribute->name = row[0];
                //std::cout<<attrribute->name<<std::endl;
				std::string strName = attrribute->name;
				if(strName.compare("id") == 0)
				{
					strName = "ID";
				}
				else
				{
					strName[0] = toupper(strName[0]);	
				}
				attrribute->get = "get"; attrribute->get += strName; attrribute->get += "()";
				attrribute->upperName = strName;
				attrribute->inType = row[1];
				std::string requiered = row[2];
				if(requiered.compare("NO") == 0)//NULL permited in DB?
				{
					attrribute->required = true;
				}
				else if(requiered.compare("YES") == 0)//NULL permited in DB?
				{
					attrribute->required = false;
				}
				//std::string keyType = row[3];
				if(strcmp(row[3],"PRI") == 0)
                {
                    attrribute->keyType = Symbol::KeyType::PRIMARY;
                }
                else if(strcmp(row[3],"MUL") == 0)
                {
                    attrribute->keyType = Symbol::KeyType::FOREIGN_UNIQUE;
                }
				std::string extra = row[5];

				if(attrribute->required && attrribute->keyType == Symbol::KeyType::PRIMARY && extra.compare("auto_increment") == 0)//primary key
				{
					key.push_back(attrribute);
					attrribute->isPK = true;//attrribute->keyType = symbols::Symbol::KeyType::PRIMARY;
                    attrribute->isAutoInc = true;
				}
				else if(attrribute->required && attrribute->keyType == Symbol::KeyType::PRIMARY)//unique constraing
				{
					key.push_back(attrribute);
					attrribute->isPK = true;//attrribute->keyType = symbols::Symbol::KeyType::PRIMARY;
                    attrribute->isAutoInc = false;
				}
				
                                insert(std::make_pair(attrribute->name.c_str(),attrribute));
				if(attrribute->required)
				{
					required.push_back(attrribute);//si attrribute->required tambie se agrega a un lista especial
				}
				//std::cout<<"Termitade with:" << attrribute->name << "(" << attrribute->upperName << ")" <<std::endl;
			}
			
			return true;			
		}
		else
		{
			std::cout<<"Faill on basicSymbols  : "<< str <<std::endl;
            
			return false;
		}
	}
}
}
