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

#include <iostream>
#include <mysql/my_global.h>
#include <mysql/mysql.h>

#include "driver.hpp"

namespace apidb
{
	bool internal::Table::fillKeyType(toolkit::clientdb::Connector& connect)
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
				for(Attribute* attribute: attributes) 
				{
					if(attribute->name.compare(row[0]) == 0)
					{
						attribute->classReferenced = row[1];
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
			while((row = mysql_fetch_row(result)))
			{
				Attribute* attrribute = new Attribute();
				
				attrribute->name = row[0];
				attrribute->type = row[1];
				std::string requiered = row[2];
				if(requiered.compare("NO") == 0)
				{
					attrribute->required = true;
				}
				else if(requiered.compare("YES") == 0)
				{
					attrribute->required = false;
				}
				std::string keyType = row[3];
				std::string extra = row[5];
				if((keyType.compare("PRI") == 0) && (extra.compare("auto_increment") == 0))
				{
					attrribute->keyType = internal::Table::Attribute::KeyType::PRIMARY;
				}
				else if(((keyType.compare("PRI") == 0) && (extra.compare("auto_increment") != 0)))//unique constraing with key primary
				{
					attrribute->keyType = internal::Table::Attribute::KeyType::UNIQUE;
				}
				else if(keyType.compare("UNI") == 0)
				{
					attrribute->keyType = internal::Table::Attribute::KeyType::UNIQUE;
				}
				
				if((keyType.compare("PRI") == 0) && (extra.compare("auto_increment") == 0))
				{
					attrribute->forInsert = false;
				}
				else
				{
					if(attrribute->required)
					{
						attrribute->forInsert = true;
					}
				}				
				
				attributes.push_back(attrribute);
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
    
    
	bool internal::RowsShowTables::listing(toolkit::clientdb::Connector& connect)
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
