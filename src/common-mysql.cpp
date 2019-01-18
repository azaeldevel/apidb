/**
 * 
 *  This file is part of apidb.
 * 
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.

 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.

 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * 
 * */

#include "apidb.hpp"
#include "common.hpp"

#include <iostream>
#include <mysql/my_global.h>
#include <mysql/mysql.h>

namespace apidb
{
	bool symbols::Table::fillKeyType(toolkit::clientdb::connectors::Connector& connect, symbols::Tables& tables)
	{
		std::string fks = "SELECT k.COLUMN_NAME, k.REFERENCED_TABLE_NAME FROM information_schema.TABLE_CONSTRAINTS i  LEFT JOIN information_schema.KEY_COLUMN_USAGE k ON i.CONSTRAINT_NAME = k.CONSTRAINT_NAME WHERE i.CONSTRAINT_TYPE = 'FOREIGN KEY' AND i.TABLE_SCHEMA =";
		fks += "'" ;
		fks += ((toolkit::clientdb::datasourcies::Datasource&)(connect.getDatconection())).getDatabase();
		fks += "'";
		fks += " AND i.TABLE_NAME = '";
		fks += name;
		fks += "'";
		//std::cout<<fks<<std::endl;
		if(connect.query(fks.c_str()))
		{
			MYSQL_RES *result = mysql_store_result((MYSQL*)connect.getServerConnector());
			MYSQL_ROW row;
			while((row = mysql_fetch_row(result)))
			{
                for (auto const& [key, attribute] : *this) 
				{
					if(attribute->name.compare(row[0]) == 0)
					{
						attribute->classReferenced = tables.search(row[1]);//returna null o un puntero valido.
						if(attribute->classReferenced != NULL)
						{
							attribute->classReferenced->countRef++;//contando la cantiad de veces que es referida la clase
						}		
						if(attribute->classReferenced != NULL && attribute->keyType == symbols::Symbol::KeyType::UNIQUE)	
						{
							//std::cout<< "FOREIGN UNIQUE : " << attribute->classReferenced->name << "." << attribute->name <<std::endl;
							attribute->keyType = symbols::Symbol::KeyType::FOREIGN_UNIQUE;//usada como llave
						}
					}
				}
			}
			mysql_free_result(result);
			return true;	
		}
		return true;
	}
	
    bool symbols::Table::basicSymbols(toolkit::clientdb::connectors::Connector& connect)
    {
		std::string str = "DESCRIBE ";
		str += name;
		if(connect.query(str)) 
		{
			//std::cout<<str<<std::endl;
			MYSQL_RES *result = mysql_store_result((MYSQL*)connect.getServerConnector());
			MYSQL_ROW row;
			bool setkey = false;
			while((row = mysql_fetch_row(result)))
			{
				Symbol* attrribute = new Symbol();
				attrribute->classParent = this;
				attrribute->name = row[0];
				std::string strName = attrribute->name;
				if(strName.compare("id") == 0)
				{
					strName = "ID";
				}
				else
				{
					strName[0] = toupper(strName[0]);	
				}
				attrribute->get = "get";attrribute->get += strName;attrribute->get += "()";
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
				std::string keyType = row[3];
				std::string extra = row[5];
				if(keyType.size() == 0)//si esta vacio (row[3]) el campo no es key
				{
					attrribute->keyType = symbols::Symbol::KeyType::NOKEY;
				}
				else if(attrribute->required && keyType.compare("PRI") == 0 && extra.compare("auto_increment") == 0)//primary key
				{
					if(key != NULL)
					{
						throw new BuildException("No hay soporto para llave compuesta.");
					}
					key = attrribute;
					attrribute->keyType = symbols::Symbol::KeyType::PRIMARY;
				}
				else if(attrribute->required && (keyType.compare("PRI") == 0))//unique constraing
				{
					if(key != NULL)
					{
						throw new BuildException("No hay soporto para llave compuesta.");
					}
					key = attrribute;
					attrribute->keyType = symbols::Symbol::KeyType::PRIMARY;
				}
				else
				{
					attrribute->keyType = symbols::Symbol::KeyType::NOKEY;				
				}
				
				if(attrribute->required && keyType.compare("UNI") == 0)//unique constraing
				{
					attrribute->keyType = symbols::Symbol::KeyType::UNIQUE;
				}				
                insert(std::make_pair(attrribute->name.c_str(),attrribute));
				if(attrribute->required)
				{
					required.push_back(attrribute);//si attrribute->required tambie se agrega a un lista especial
				}
			}
			mysql_free_result(result);
			return true;			
		}
		else
		{
			std::cerr<<"Faill on basicSymbols  : "<< str <<std::endl;
			return false;
		}
    }
    
    
	bool symbols::Tables::listing(toolkit::clientdb::connectors::Connector& connect)
	{
		std::string db;
		switch(connect.getDatconection().getServerType())
		{
            case toolkit::clientdb::datasourcies::Datasource::ServerType::MySQL:
				db = connect.getDatconection().getDatabase();
				break;
			default:
			return false;
		}
		//std::cout<< "db:" << db <<std::endl;
		if(connect.query("SELECT TABLE_NAME FROM information_schema.tables WHERE TABLE_SCHEMA = '"  + db + "' and TABLE_TYPE = 'BASE TABLE'")) 
		{
			MYSQL_RES *result = mysql_store_result((MYSQL*)connect.getServerConnector());
			MYSQL_ROW row;
			while((row = mysql_fetch_row(result)))
			{		
				Table* prw = new Table();
				prw->name = row[0];
				push_back(prw);
			}
			mysql_free_result(result);
			return true;
		}
		return false;	
	}
}
