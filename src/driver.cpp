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
 *  author: Azael Reyes
 * */

#include <exception>
#include <string.h>
#include <stdio.h>
#include <exception>


#include "apidb.hpp"
#include "common.hpp"


namespace apidb
{
    Driver::Driver(const ConfigureProject& config) : configureProject(config)
	{		
		if(configureProject.inputLenguaje == apidb::InputLenguajes::MySQL_Server)
		{
			connector = new toolkit::clientdb::Connector();
			analyzer = new mysql::Analyzer(configureProject);		
			try
			{
				bool flag = connector->connect(config.conectordb);
				if(!flag)
				{
					delete connector;
					delete analyzer;
					connector = NULL;
					analyzer = NULL;
					//throw toolkit::Exception(msg);
				}
			}
			catch(toolkit::clientdb::SQLException ex)
			{
				
				analyzer->getErrorMessage() <<"Fallo la conexion a DB : "<< ex.what() <<std::endl;
			}
		}
		else
		{
			analyzer->getErrorMessage() <<"Lenguaje de entrada desconocido."<<std::endl;
		}	
	}
	

	
	
	OutputLenguajes Driver::getOutputLenguaje() const
	{
		return generator->getOutputLenguaje();
	}
	
	bool Driver::driving()
	{
		if(connector == NULL) return false;

		if(analyze())
		{
			if(generate()) return true;
		}
		
		return false;
	}
	
	bool Driver::generate()
	{				
		if((analyzer->getDirectoryProject().empty()) | (analyzer->getDirectoryProject().compare(".") == 0))
		{
			
		}
		else
		{
			std::string direct = analyzer->getDirectoryProject();
			std::ifstream ifile(direct);
			if (!ifile) 
			{
				std::string cmd = "mkdir ";
				cmd = cmd + direct;
				system(cmd.c_str());
			}			
		}
		
		if(configureProject.outputLenguaje == apidb::OutputLenguajes::CPP)
		{
			bool flagCPP,flagCMAKE;
			
			//std::cout<<"apidb::generators::CPP cpp(*analyzer);..."<<std::endl;
			apidb::generators::CPP cpp(*analyzer,configureProject);
			generator = &cpp;
			flagCPP = cpp.generate();
			
			//std::cout<<"apidb::generators::CMake cmake(*analyzer);..."<<std::endl;
			apidb::generators::CMake cmake(*analyzer,configureProject);			
			flagCMAKE = cmake.generate();
			
			///std::cout<<"if(flagCPP && flagCMAKE)..."<<std::endl;
			if(flagCPP && flagCMAKE)
			{
				analyzer->getOutputMessage() <<"Completado."<<std::endl;				
				return true;				
			}
			else
			{
				analyzer->getOutputMessage() <<"Fallo."<<std::endl;
				return false;
			}
		}
		else
		{
			return false;
		}
	}
	
	bool Driver::analyze()
	{
		analyzer->getOutputMessage() << "Analisis de codigo..." << std::endl;
		analyzer->getOutputMessage() << "\tLenguaje de entrada: " << analyzer->getInputLenguajeString() << std::endl;
		
		apidb::mysql::Analyzer* analyzer = NULL;
		if(this->analyzer->getInputLenguaje() == apidb::InputLenguajes::MySQL_Server)
		{
			analyzer = (apidb::mysql::Analyzer*)(this->analyzer);
		}
		else
		{
			return false;
		}
		
		//analyzer->setPramsLenguajes(configureProject->inputLenguaje,configureProject->outputLenguaje);
		
		if(analyzer->listing(*connector)) //reading tables
        	{
            		for(symbols::Table* table: analyzer->getListTable()) //reading attrubtes by table
            		{
				analyzer->getOutputMessage() << "\tCreating basic simbols for " << table->name  << "." << std::endl;
                		//simbolos basicos 
                		if(!table->basicSymbols(*connector))
                		{
					//std::cerr<<"Faill on basicSymbols"<<std::endl;
					return false;
				}				
				//foreign key's
				if(!table->fillKeyType(*connector,analyzer->getListTable()))
                		{
					//std::cerr<<"Faill on fillKeyType"<<std::endl;
					return false;
				}//parsing imput types
				//std::cout<<"\t"<<table->name<<std::endl;
				for(symbols::Symbol* attribute: *table)
				{
					//std::cout<<"\t"<<attribute->inType<<std::endl;
					attribute->outType = analyzer->parse(attribute->inType);
				}				
			}	
			analyzer->getListTable().reorder();
        	}  
          
	return true;
	}
		
	/*Driver::Driver(const std::string& name,const std::string& directory,const toolkit::clientdb::Datconection& datconection,InputLenguajes inputLenguaje, OutputLenguajes outputLenguaje,toolkit::Version version)
	{		
		this->inputLenguaje = inputLenguaje;
		this->outputLenguaje = outputLenguaje;
		if(this->inputLenguaje == apidb::InputLenguajes::MySQL_Server)
		{
			this->datconection = new toolkit::clientdb::DatconectionMySQL((toolkit::clientdb::DatconectionMySQL&)datconection);
			connector = new toolkit::clientdb::Connector();
			analyzer = new mysql::Analyzer(inputLenguaje,outputLenguaje);		
			try
			{
				bool flag = connector->connect(datconection);
				if(flag)
				{
					analyzer->setPramsProject(name,directory);
					this->name = name;
					this->directory = directory;
					this->version = version;
				}
			}
			catch(toolkit::clientdb::SQLException ex)
			{
				analyzer->getErrorMessage() <<"Fallo la conexion el servidor de datos el cual respondio; "<<std::endl;
			}
		}
		else
		{
			analyzer->getErrorMessage() <<"Lenguaje de entrada desconocido."<<std::endl;
		}	
	}*/
} 

