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

namespace apidb
{
	const internal::RowsShowTables* MySQLDriver::getListTable()
	{
		return rows;
	}
	
	bool MySQLDriver::read()
	{
		rows = new internal::RowsShowTables();
		if(connector->query("SHOW TABLES",*rows)) 
        {
            for(internal::Table* n: *rows)
            {
                std::string str = "DESCRIBE ";
                str += n->table_name;   
                if(!connector->query(str.c_str(),*n))
                {
                    return false;                
                }
            }
            return true;
        }
        else
        {
            return false;
        }  
        
		return false;
	}
		
	MySQLDriver::MySQLDriver()
	{
		clientdb::DatconectionMySQL mysqlConnector;
		mysqlConnector.host = "192.168.0.101";
		mysqlConnector.database = "business.alpha";
		mysqlConnector.usuario = "root";
		mysqlConnector.password = "k3yL0c41";
		mysqlConnector.port = 3306;    
		
		connector = new clientdb::Connector();
		bool flag = connector->connect(mysqlConnector);
		if(flag)
		{
			printf("SQL Server version: %s\n", connector->serverDescription());
		}
		else
		{
			std::cerr<<"Fallo la conexion el servidor de datos el cual respondio; "<<std::endl;
		}
	}
} 
