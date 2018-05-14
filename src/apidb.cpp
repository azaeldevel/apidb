
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
		std::string attrib = "";
		std::string values = "";
		std::string id = "";
		std::string insertMethode = "";
		
        for(internal::Table::Symbol* attr : table->attributes)
        {
			if(driver.getOutputLenguaje() == Driver::OutputLenguajes::CPP)
			{
				if(attr->classReferenced == NULL)
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
					ofile << "const " << attr->classReferenced->name <<"& ";	
				}
				ofile << table->name <<"::get" << attr->name << "()const"<< std::endl;
				ofile << "{"<<std::endl;
				if(attr->classReferenced == NULL)
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
			
			//buscando el campo llave
			if(attr->keyType == internal::Table::Symbol::KeyType::PRIMARY)
			{
				id = attr->name;
			}
			
			//parametros para insert metodo		
			if(attr->forInsert)            
			{
				if(!insertMethode.empty())//si hay texto en la variable
				{
					attrib += ",";
					values += " std::string(\",\") + ";
					insertMethode += ",";//entonmces agregar coma, ya que se va a gregar otro parametro
				}
				if((attr->cpp_type.compare("char") == 0) | (attr->cpp_type.compare("short") == 0) | (attr->cpp_type.compare("int") == 0) | (attr->cpp_type.compare("long") == 0) | (attr->cpp_type.compare("float") == 0) | (attr->cpp_type.compare("double") == 0))
				{
					if(attr->classReferenced == NULL)
					{
						attrib += attr->name;
						
						values += attr->name;
						
						insertMethode += attr->cpp_type;						
					}
					else
					{
						attrib += attr->name;
						
						if(attr->classReferenced->key != NULL) values += attr->classReferenced->key->get;
						
						insertMethode += "const ";
						insertMethode += attr->classReferenced->name;
						insertMethode += "& ";
						insertMethode += attr->name;
					}
				}
				else
				{
					attrib += attr->name;
					
					if(attr->classReferenced->key != NULL) values += attr->classReferenced->key->get;
						
					insertMethode += "const ";
					insertMethode += attr->cpp_type; 
					insertMethode += "& ";
					insertMethode += attr->name;
				}
			}   
        }
        //como no se encontro llave primaria se aceptara un campo con restrccion de unicidad.
        for(internal::Table::Symbol* attr : table->attributes)
        {
			if(id.empty())
			{
				if(attr->keyType == internal::Table::Symbol::KeyType::UNIQUE)
				{
					id = attr->name;
					break; 
				}
			}
		}
        ofile << "bool " << table->name << "::insert(" << insertMethode <<")"<<std::endl;
        ofile << "{"<<std::endl;
        ofile << "std::string str = \"\";"<<std::endl;
        ofile << "str += \"INSERT INTO \";"<<std::endl;
        ofile << "str += TABLE_NAME;"<<std::endl;
        ofile << "str += \"(\""<<";"<<std::endl;
        ofile << "str += \""<< attrib<<"\";"<<std::endl;
        ofile << "str += \") VALUES(\";"<<std::endl;
        ofile << "str += \""<< values <<"\";"<<std::endl;
        //ofile << "str +=\")\""<<";"<<std::endl;
        if(table->key != NULL)
        {			
			ofile << "this->" <<table->key->name << " = new " << table->key->classParent->name << "(";
			ofile << "connector.insert(str));"<< std::endl;
			ofile << "return (" << table->key->name << "->get" <<"> 0);"<<std::endl;	
		}
		else
		{
			ofile << "return connector.query(str);"<< std::endl;
		}
        ofile << "}"<<std::endl;
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
		file << "const std::string " <<  nameClass << "::TABLE_NAME = \""<<  nameClass << "\";" << std::endl;
        createClassMethodesCPP(driver,cl,file);        
        file<< std::endl<< std::endl;
    }
    
    void CPPGenerator::createClassMethodesH(apidb::Driver& driver,const apidb::internal::Table* table,std::ofstream& ofile)
    {
		std::string insertMethode = "";
        for(internal::Table::Symbol* attr : table->attributes)
        {
			if(driver.getOutputLenguaje() == Driver::OutputLenguajes::CPP)
			{
				if((attr->cpp_type.compare("char") == 0) | (attr->cpp_type.compare("short") == 0) | (attr->cpp_type.compare("int") == 0) | (attr->cpp_type.compare("long") == 0) | (attr->cpp_type.compare("float") == 0) | (attr->cpp_type.compare("double") == 0))
				{
					if(attr->classReferenced == NULL)
					{
						ofile << attr->cpp_type << " ";						
					}
					else
					{
						ofile <<  "const " << attr->classReferenced->name << "& ";
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
			
			//parametros para insert metodo
			if(attr->forInsert)            
			{
				if(!insertMethode.empty())//si hay texto en la variable
				{
					insertMethode += ",";//entonmces agregar coma, ya que se va a gregar otro parametro
				}
				if((attr->cpp_type.compare("char") == 0) | (attr->cpp_type.compare("short") == 0) | (attr->cpp_type.compare("int") == 0) | (attr->cpp_type.compare("long") == 0) | (attr->cpp_type.compare("float") == 0) | (attr->cpp_type.compare("double") == 0))
				{
					if(attr->classReferenced == NULL)
					{
						insertMethode += attr->cpp_type;						
					}
					else
					{
						insertMethode +=  "const ";
						insertMethode +=  attr->classReferenced->name;
						insertMethode +=  "& ";
					}
				}
				else
				{
					insertMethode += "const ";
					insertMethode += attr->cpp_type; 
					insertMethode += "& ";
				}
			}
        }   
        ofile << "bool " << "insert(" << insertMethode <<");"<<std::endl;
    }
    void CPPGenerator::createClassAttributesH(apidb::Driver& driver,const apidb::internal::Table* table,std::ofstream& ofile)
    {
        for(internal::Table::Symbol* attr : table->attributes)
        {
			if(driver.getOutputLenguaje() == Driver::OutputLenguajes::CPP)
			{
				if(attr->classReferenced == NULL)
				{
					ofile << attr->cpp_type <<" "<<attr->name <<";"<<std::endl;
				}
				else
				{
					ofile << attr->classReferenced->name <<"* "<<attr->name <<";"<<std::endl;
				}				
			}
			else if(driver.getOutputLenguaje() == Driver::OutputLenguajes::C)
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
        file << "static const std::string TABLE_NAME;" <<std::endl;
        createClassAttributesH(driver,cl,file);
        createClassPublicH(file);
        createClassMethodesH(driver,cl,file);
        file <<"};"<<std::endl;
    }
    
    bool CPPGenerator::generate(apidb::Driver& driver)
    {		
		driver.getOutputMessage() << "Generando codigo... " << std::endl;
		driver.getOutputMessage() << "\tLenguaje resultado: " << driver.getOutputLenguajeString() << std::endl;
		//includes in header file
        std::string headers = "";
        bool stringFlag = false;
        const apidb::internal::RowsShowTables* tables = driver.getListTable();
		for(internal::Table* table: *tables)
		{
			driver.getOutputMessage() << "\tCoding " << table->name << "." << std::endl;
			for(internal::Table::Symbol* attr : table->attributes)
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
	
	
	
	
	
	
	
	
	
	
	
	bool CG::generate()
	{
		apidb::CPPGenerator cpp;
		cpp.generate(*this);
		return false;
	}
	
	bool CG::analyze()
	{
		getOutputMessage() << "Analisis de codigo..." << std::endl;
		getOutputMessage() << "\tLenguaje de entrada: " << getInputLenguajeString() << std::endl;
		rows = new apidb::internal::RowsShowTables();
		if(rows->listing(*connector)) //reading tables
        {
            for(internal::Table* table: *rows) //reading attrubtes by table
            {
				getOutputMessage() << "\tCreating simbols for " << table->name  << "." << std::endl;
                if(!table->basicSymbols(*connector))
                {
					//std::cerr<<"Faill on basicSymbols"<<std::endl;
					return false;
				}
				if(!table->fillKeyType(*connector,*rows))
                {
					//std::cerr<<"Faill on fillKeyType"<<std::endl;
					return false;
				}
				
				//parsing imput types
				for(internal::Table::Symbol* attribute: table->attributes)
				{
					attribute->cpp_type = parse(attribute->sqlType);
				}
            }            
        }  
        		        
		return true;
	}
		
	CG::CG(const std::string& name,const std::string& directory,const toolkit::clientdb::Datconection& datconection)
	{		
		connector = new toolkit::clientdb::Connector();
		try
		{
			bool flag = connector->connect(datconection);
			if(flag)
			{
				setPramsProject(name,directory);				
			}
		}
		catch(toolkit::clientdb::SQLException ex)
		{
			getErrorMessage() <<"Fallo la conexion el servidor de datos el cual respondio; "<<std::endl;
		}
	}
} 
