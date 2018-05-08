
#include "apidb.hpp"
#include "toolkit.hpp"

#include <iostream>

namespace apidb
{
	const internal::RowsShowTables* MySQLDriver::getListTable()
	{
		return rows;
	}
	
	bool MySQLDriver::parse()
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
	
	std::vector<std::string> MySQLDriver::listAttrib(std::string table)
	{
		
	}
	
	std::string MySQLDriver::getTypeAttrib(std::string attrib)
	{
		
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
