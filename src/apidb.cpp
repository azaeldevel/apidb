
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
        //getting tables
		rows = new internal::RowsShowTables();
		if(connector->query("SHOW TABLES",*rows)) 
        {
            //std::cout << "Decribing tables..."<<std::endl;
            //getting atributes by tables
            //for(std::list<internal::Table*>::iterator it=rows->begin(); it != rows->end(); ++it) 
            for(internal::Table* n: *rows)
            {
                std::string str = "DESCRIBE ";
                str += n->table_name;   
                //std::cout << str <<std::endl;
                if(!connector->query(str.c_str(),*n))
                {
                    return false;                
                }
                //std::cout<<std::endl;
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
		toolkit::Message flag = connector->connect(mysqlConnector);
		if(flag.isPass())
		{
			printf("SQL Server version: %s\n", connector->serverDescription());
		}
		else
		{
			std::cerr<<"Fallo la conexion el servidor de datos el cual respondio; "<<std::endl;
		}
		
		
	}
} 
