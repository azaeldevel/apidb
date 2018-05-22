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
#include "toolkit.hpp"

#include <iostream>
#include <mysql/my_global.h>
#include <mysql/mysql.h>

namespace apidb
{
	bool internal::Table::fillKeyType(toolkit::clientdb::Connector& connect,const internal::Tables& tables)
	{
		std::string fks = "SELECT k.COLUMN_NAME, k.REFERENCED_TABLE_NAME FROM information_schema.TABLE_CONSTRAINTS i  LEFT JOIN information_schema.KEY_COLUMN_USAGE k ON i.CONSTRAINT_NAME = k.CONSTRAINT_NAME WHERE i.CONSTRAINT_TYPE = 'FOREIGN KEY' AND i.TABLE_SCHEMA =";
		fks += "'" ;
		fks += ((toolkit::clientdb::DatconectionMySQL&)(connect.getDatconection())).getDatabase();
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
				for(Symbol* attribute: *this) 
				{
					if(attribute->name.compare(row[0]) == 0)
					{						
						attribute->classReferenced = tables.search(row[1]);//returna null o un puntero valido.		
						if(attribute->keyType == internal::Symbol::KeyType::UNIQUE && attribute->classReferenced != NULL)		
						{
							attribute->keyType = internal::Symbol::KeyType::FOREIGN_UNIQUE;//usada como llave
						}
					}
				}
			}			
			mysql_free_result(result);
			return true;	
		}		
		return true;
	}
	
    bool internal::Table::basicSymbols(toolkit::clientdb::Connector& connect)
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
				attrribute->get = "get";attrribute->get += row[0];attrribute->get += "()";
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
				if(keyType.size() == 0)//si esta vacio el campo no es key
				{
					attrribute->keyType = internal::Symbol::KeyType::NOKEY;
					if(!setkey)	key = NULL;
				}
				else if(attrribute->required && (keyType.compare("PRI") == 0) && (extra.compare("auto_increment") == 0))//primary key
				{
					attrribute->keyType = internal::Symbol::KeyType::PRIMARY;
					key = attrribute;
					setkey = true;
				}
				else if(attrribute->required && (keyType.compare("PRI") == 0))//unique constraing
				{
					attrribute->keyType = internal::Symbol::KeyType::PRIMARY;
					key = attrribute;
					setkey = true;
				}
				else if(attrribute->required && (keyType.compare("UNI") == 0))//unique constraing
				{
					attrribute->keyType = internal::Symbol::KeyType::UNIQUE;
					key = attrribute;
					setkey = true;
				}
				else
				{
					attrribute->keyType = internal::Symbol::KeyType::NOKEY;
					if(!setkey)	key = NULL;
				}
				
							
				
				push_back(attrribute);
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
    
    
	bool internal::Tables::listing(toolkit::clientdb::Connector& connect)
	{
		if(connect.query("SHOW TABLES")) 
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
