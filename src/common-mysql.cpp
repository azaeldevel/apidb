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
#include <mysql/mysql.h>
#include <string>

#include "apidb.hpp"
#include "common.hpp"
#include "Errors.hpp"


namespace octetos
{
namespace apidb
{
    /**
    * Rellena los campos 'classReferenced' y 'symbolReferenced' de la tabla
    */
    bool symbols::Table::fillKeyType(octetos::toolkit::clientdb::mysql::Connector& connect,const SymbolsTable& symbolsTable)
	{
        /**
        * Lista las relaciones de llaves foraneas para la tabla actual
        */
		std::string fks = "SELECT k.COLUMN_NAME, k.REFERENCED_TABLE_NAME, k.REFERENCED_COLUMN_NAME FROM information_schema.TABLE_CONSTRAINTS i,information_schema.KEY_COLUMN_USAGE k WHERE i.CONSTRAINT_NAME = k.CONSTRAINT_NAME  AND i.CONSTRAINT_TYPE = 'FOREIGN KEY' AND i.TABLE_SCHEMA =k.TABLE_SCHEMA AND i.TABLE_NAME = "; 
        fks += "'";
		fks += fullname;
        fks += "' AND i.CONSTRAINT_SCHEMA =  '" ;
		fks += ((octetos::toolkit::clientdb::Datconnect*)(connect.getDatconection()))->getDatabase();
		fks += "'";
		//std::cout<<fks<<std::endl;
        octetos::toolkit::clientdb::Datresult* dt = connect.query(fks.c_str());
        if(dt != NULL)
        {                      
            MYSQL_ROW row;
            while ((row = mysql_fetch_row((MYSQL_RES*)(dt->getResult()))))
            {
				std::cout<<"Buscando tabla '" << row[1] << "'" << std::endl;
				Table* tableRef = symbolsTable.findTable(row[1]);
				if(tableRef != NULL)
				{
					std::cout<<"Se encontró tabla '" << tableRef->getName() << "'" << std::endl;
				}
			}	
		}
                
		delete dt;                
		return true;
    }
	
        bool symbols::Table::basicSymbols(octetos::toolkit::clientdb::mysql::Connector& connect)
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
                octetos::toolkit::clientdb::Datresult* dt = connect.query(str.c_str());
		if(dt != NULL) 
		{
			//std::cout<<str<<std::endl;
			//MYSQL_RES *result = mysql_store_result((MYSQL*)connect.getServerConnector());
			MYSQL_ROW row;
			bool setkey = false;
			while ((row = mysql_fetch_row((MYSQL_RES*)(dt->getResult()))))
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
			delete dt;//mysql_free_result(result);
			return true;			
		}
		else
		{
			std::cout<<"Faill on basicSymbols  : "<< str <<std::endl;
			delete dt;//mysql_free_result(result);
			return false;
		}
	}
    
    
	bool Analyzer::listing()
	{
		std::string db = connector->getDatconection()->getDatabase();
		std::string str = "SELECT TABLE_NAME FROM information_schema.tables WHERE TABLE_SCHEMA = '";
		str = str + db + "' and TABLE_TYPE = 'BASE TABLE' ORDER BY TABLE_NAME ASC";
		octetos::toolkit::clientdb::Datresult* dt = connector->query(str.c_str());   
		if(dt != NULL) 
		{
			MYSQL_ROW row;
			while ((row = mysql_fetch_row((MYSQL_RES*)(dt->getResult()))))
			{
				symbols::Table* prw = new symbols::Table(symbols::getFirstName(row[0]));
				std::string upper = row[0];
				upper[0] = toupper(upper[0]);
				prw->upperName = upper;
				prw->space = symbols::getSpacePatch(row[0]);
				prw->fullname = row[0];
				if(symbols::getSpaceLevel(prw->fullname) == 0)
				{//si no esta anidada en un espacio.
					symbols::SymbolsTable::iterator it = symbolsTable.find("");
					if(it != symbolsTable.end())
					{
						//std::cout<<"Table: " << row[0] << std::endl;
						symbols::Space* space = (symbols::Space*)it->second;
						std::pair<const char*, symbols::ISpace*> newInser(prw->fullname.c_str(),prw);
						space->insert(newInser);
					}
					else
					{
						toolkit::Error::write(toolkit::Error("No se encontró el espacio global.",ErrorCodes::ANALYZER_FAIL,__FILE__,__LINE__));
						return false;
					}
				}
			}
			delete dt;
			return true;
		}
		else
		{
			std::string msg = "";
			msg = msg + " MySQL Server Error No. : '";
			msg = msg + std::to_string(mysql_errno((MYSQL*)connector->getServerConnector()));
			msg = msg + "' ";
			msg = msg + mysql_error((MYSQL*)connector->getServerConnector());
			toolkit::Error::write(toolkit::Error(msg,ErrorCodes::ANALYZER_FAIL,__FILE__,__LINE__));
			return false;
		}	
	}
}
}
