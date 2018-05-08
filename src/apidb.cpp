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
    void CPPGenerator::createClassMethodes(const internal::Table* table,std::ofstream& ofile)
    {
    }
    void CPPGenerator::createClassAttributes(const internal::Table* table,std::ofstream& ofile)
    {
        for(internal::Table::Attribute* attr : table->attributes)
        {
            ofile <<" CLASS "<<attr->name<<std::endl;
        }        
    }
    void CPPGenerator::createSpace(const Driver& driver,std::ofstream& file,const std::string& space)
    {
        file <<"namespace "<<space<<std::endl;
        file <<"{"<<std::endl;
        const internal::RowsShowTables* tables = driver.getListTable();
        for (apidb::internal::Table* n : *tables) 
        {
            createClass(n,file,n->table_name);       
        }
        file <<"}"<<std::endl;
    }
    void CPPGenerator::createClassPublic(std::ofstream& file)
    {
        file << "public:" <<std::endl;
        file <<std::endl;        
    }
    void CPPGenerator::createClassPrivate(std::ofstream& file)
    {
        file << "private:" <<std::endl;
        file <<std::endl;        
    }
    void CPPGenerator::createClass(const internal::Table* cl,std::ofstream& file,const std::string& nameClass)
    {
        file <<"class "<<nameClass<<std::endl;
        file <<"{"<<std::endl;
        createClassPublic(file);
        createClassAttributes(cl,file);
        createClassPrivate(file);
        createClassMethodes(cl,file);
        file <<"};"<<std::endl;
    }
    
    bool CPPGenerator::generate(const Driver& driver,std::ofstream& fout,const std::string& space)
    {
        
        if(space.length() > 0)
        {
            createSpace(driver,fout,space);
        }
        else
        {
            const internal::RowsShowTables* tables = driver.getListTable();
            for (apidb::internal::Table* n : *tables) 
            {
                createClass(n,fout,n->table_name);       
            }
        }
        return true;    
    }
    
	const internal::RowsShowTables* Driver::getListTable() const
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
