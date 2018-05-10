
/**
 * 
 *  This file is part of apidb.
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

#include "apidb.hpp"
#include "toolkit.hpp"

#include <iostream>

namespace apidb
{
    void CPPGenerator::createClassMethodes(apidb::Driver& driver,const apidb::internal::Table* table,std::ofstream& ofile)
    {
    }
    void CPPGenerator::createClassAttributes(apidb::Driver& driver,const apidb::internal::Table* table,std::ofstream& ofile)
    {
        for(internal::Table::Attribute* attr : table->attributes)
        {
			if(driver.getOutputLenguaje().compare("C++") == 0)
			{
				ofile << attr->cpp_type <<" "<<attr->name <<";"<<std::endl;
			}
			else if(driver.getOutputLenguaje().compare("C") == 0)
			{
				ofile <<attr->c_type<<" "<<attr->name <<";"<<std::endl;
			}
			else
			{
				driver.message("OutputLenguaje is unknow.");
			}
             
        }        
    }
    void CPPGenerator::createSpace(apidb::Driver& driver,std::ofstream& file)
    {
        file <<"namespace "<<driver.getNameProject()<<std::endl;
        file <<"{"<<std::endl;
        const internal::RowsShowTables* tables = driver.getListTable();
        for (apidb::internal::Table* n : *tables) 
        {
            createClass(driver,n,file,n->table_name);       
        }
        file <<"}"<<std::endl;
    }
    void CPPGenerator::createClassPublic(std::ofstream& file)
    {
        file << "public:" <<std::endl;
    }
    void CPPGenerator::createClassPrivate(std::ofstream& file)
    {
        file << "private:" <<std::endl;
    }
    void CPPGenerator::createClass(apidb::Driver& driver,const apidb::internal::Table* cl,std::ofstream& file,const std::string& nameClass)
    {
        file <<"class "<<nameClass<<std::endl;
        file <<"{"<<std::endl;
        createClassPublic(file);
        createClassAttributes(driver,cl,file);
        createClassPrivate(file);
        createClassMethodes(driver,cl,file);
        file <<"};"<<std::endl;
    }
    
    bool CPPGenerator::generate(apidb::Driver& driver)
    {
        if(driver.getNameProject().length() > 0)
        {
            createSpace(driver,driver.getHeaderOutput());
        }
        else
        {
            const apidb::internal::RowsShowTables* tables = driver.getListTable();
            for (apidb::internal::Table* n : *tables) 
            {
                createClass(driver,n,driver.getHeaderOutput(),n->table_name);       
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
		rows = new apidb::internal::RowsShowTables();
		if(connector->query("SHOW TABLES",*rows)) //reading tables
        {
            for(internal::Table* n: *rows) //reading attrubtes by table
            {
                std::string str = "DESCRIBE ";
                str += n->table_name;   
                if(!connector->query(str.c_str(),*n))
                {
                    return false;                
                }
            }
        }  
        
		for(internal::Table* table: *rows) 
		{
			for(internal::Table::Attribute* attribute: table->attributes)
			{
				//std::cout<<"Parsing "<<attribute->type<<std::endl;
				attribute->cpp_type = parse(attribute->type);
			}
		}
		
		/*std::ofstream outFile;
		outFile.open ("out.txt");
		apidb::CPPGenerator cpp;
		std::string space = "nmq";
		cpp.generate(driver,outFile,space);
		outFile.flush();*/
        
		return false;
	}
		
	MySQLDriver::MySQLDriver()
	{
		toolkit::clientdb::DatconectionMySQL mysqlConnector;
		mysqlConnector.host = "192.168.0.101";
		mysqlConnector.database = "business.alpha";
		mysqlConnector.usuario = "root";
		mysqlConnector.password = "k3yL0c41";
		mysqlConnector.port = 3306;    
		
		connector = new toolkit::clientdb::Connector();
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
