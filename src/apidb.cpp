
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
    void CPPGenerator::createClassMethodesCPP(apidb::Driver& driver,const apidb::internal::Table* table,std::ofstream& ofile)
    {
        for(internal::Table::Attribute* attr : table->attributes)
        {
			if(driver.getOutputLenguaje().compare("C++") == 0)
			{
				if(attr->classReferenced.empty())
				{
					if((attr->cpp_type.compare("int") == 0) | (attr->cpp_type.compare("float") == 0) | (attr->cpp_type.compare("double") == 0))
					{
						ofile << attr->cpp_type <<" ";	
					}
					else
					{
						ofile << "const " << attr->cpp_type <<"& ";
					}
				}
				else
				{			
					ofile << "const " << attr->classReferenced <<"& ";	
				}
				ofile << table->name <<"::get" << attr->name << "()const"<< std::endl;
				ofile << "{"<<std::endl;
				if(attr->classReferenced.empty())
				{
					ofile << "return " <<attr->name << ";"<<std::endl;
				}
				else
				{
					ofile << "return *" <<attr->name << ";"<<std::endl;
				}
				ofile << "}"<<std::endl;
			}
			else
			{
				driver.message("OutputLenguaje is unknow.");
			}     
        }
    }
    
    void CPPGenerator::createSpaceCPP(apidb::Driver& driver,std::ofstream& file)
    {
        file <<"namespace "<<driver.getNameProject()<<std::endl;
        file <<"{"<<std::endl;
        const internal::RowsShowTables* tables = driver.getListTable();
        for (apidb::internal::Table* n : *tables) 
        {
            createClassCPP(driver,n,file,n->name);       
        }
        file <<"}"<<std::endl;
    }
	void CPPGenerator::createClassCPP(apidb::Driver& driver,const apidb::internal::Table* cl,std::ofstream& file,const std::string& nameClass)
    {
        createClassMethodesCPP(driver,cl,file);
    }
    
    void CPPGenerator::createClassMethodesH(apidb::Driver& driver,const apidb::internal::Table* table,std::ofstream& ofile)
    {
        for(internal::Table::Attribute* attr : table->attributes)
        {
			if(driver.getOutputLenguaje().compare("C++") == 0)
			{
				if((attr->cpp_type.compare("int") == 0) | (attr->cpp_type.compare("float") == 0) | (attr->cpp_type.compare("double") == 0))
				{
					if(attr->classReferenced.empty())
					{
						ofile << attr->cpp_type << " ";						
					}
					else
					{
						ofile <<  "const " << attr->classReferenced << "& ";
					}
				}
				else
				{
					ofile << "const " << attr->cpp_type <<"& ";
				}
				ofile << "get" << attr->name << "() const;"<< std::endl;
			}
			else
			{
				driver.message("OutputLenguaje is unknow.");
			}             
        }   
    }
    void CPPGenerator::createClassAttributesH(apidb::Driver& driver,const apidb::internal::Table* table,std::ofstream& ofile)
    {
        for(internal::Table::Attribute* attr : table->attributes)
        {
			if(driver.getOutputLenguaje().compare("C++") == 0)
			{
				if(attr->classReferenced.empty())
				{
					ofile << attr->cpp_type <<" "<<attr->name <<";"<<std::endl;
				}
				else
				{
					ofile << attr->classReferenced <<"* "<<attr->name <<";"<<std::endl;
				}				
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
    void CPPGenerator::createSpaceH(apidb::Driver& driver,std::ofstream& file)
    {
        file <<"namespace "<<driver.getNameProject()<<std::endl;
        file <<"{"<<std::endl;
        const internal::RowsShowTables* tables = driver.getListTable();
        for (apidb::internal::Table* table : *tables) 
        {
			file <<"class " <<table->name << ";"<<std::endl;
		}
		file<<std::endl;
        for (apidb::internal::Table* n : *tables) 
        {
            createClassH(driver,n,file,n->name);       
        }
        file <<"}"<<std::endl;
    }
    void CPPGenerator::createClassPublicH(std::ofstream& file)
    {
        file << "public:" <<std::endl;
    }
    void CPPGenerator::createClassPrivateH(std::ofstream& file)
    {
        file << "private:" <<std::endl;
    }
    void CPPGenerator::createClassH(apidb::Driver& driver,const apidb::internal::Table* cl,std::ofstream& file,const std::string& nameClass)
    {
        file <<"class "<<nameClass<<std::endl;
        file <<"{"<<std::endl;
        createClassPrivateH(file);
        createClassAttributesH(driver,cl,file);
        createClassPublicH(file);
        createClassMethodesH(driver,cl,file);
        file <<"};"<<std::endl;
    }
    
    bool CPPGenerator::generate(apidb::Driver& driver)
    {		
		
		//includes in header file
        std::string headers = "";
        bool stringFlag = false;
        const apidb::internal::RowsShowTables* tables = driver.getListTable();
		for(internal::Table* table: *tables)
		{
			for(internal::Table::Attribute* attr : table->attributes)
			{
				if(attr->cpp_type.compare("std::string")==0 && stringFlag == false)
				{
					driver.getHeaderOutput()<< "#include <string>" <<std::endl;
					stringFlag = true;
				}					
			}
		}
			
			
		//inlcudes in source file
        driver.getSourceOutput()<< "#include \"" <<driver.getHeaderName() <<"\""<<std::endl<<std::endl; 
			
		//writing code
		if(!driver.getNameProject().empty())
		{			
			createSpaceH(driver,driver.getHeaderOutput());    
			createSpaceCPP(driver,driver.getSourceOutput()); 
		}   
		else
		{
			for (apidb::internal::Table* n : *tables) 
			{
				for(internal::Table* table: *tables)
				{
					//declaracion adelantada
					driver.getHeaderOutput() <<"class " <<table->name << ";"<<std::endl;
				}
				createClassH(driver,n,driver.getHeaderOutput(),n->name);  
				createClassCPP(driver,n,driver.getSourceOutput(),n->name);      
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
		if(rows->listing(*connector)) //reading tables
        {
            for(internal::Table* table: *rows) //reading attrubtes by table
            {
                if(!table->basicSymbols(*connector))
                {
					std::cerr<<"Faill on basicSymbols"<<std::endl;
					return false;
				}
				if(!table->fillKeyType(*connector))
                {
					std::cerr<<"Faill on fillKeyType"<<std::endl;
					return false;
				}
				
				//parsing imput types
				for(internal::Table::Attribute* attribute: table->attributes)
				{
					attribute->cpp_type = parse(attribute->type);
				}
            }            
        }  
        		        
		return true;
	}
		
	MySQLDriver::MySQLDriver(const std::string& name,const std::string& directory)
	{
		toolkit::clientdb::DatconectionMySQL mysqlConnector("192.168.0.101",3306,"business.alpha","root","k3yL0c41");     
		
		connector = new toolkit::clientdb::Connector();
		bool flag = connector->connect(mysqlConnector);
		if(!flag)
		{
			std::cerr<<"Fallo la conexion el servidor de datos el cual respondio; "<<std::endl;
		}
		setPramsProject(name,directory);
	}
} 
