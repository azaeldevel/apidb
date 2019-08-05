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
#include <toolkit/common/common.hpp>

#include "driver.hpp"
#include "../common.hpp"
#include "mysql-reader-c++/analyzer.hpp"

namespace octetos
{
namespace apidb
{
        Driver::~Driver()
        {
                if(analyzer != NULL)
                {
                        delete analyzer;
                        analyzer = NULL;
                }
                if(connector != NULL)
                {
                        connector->close();
                        delete connector;
                        connector = NULL;
                }
        }
        const Analyzer&  Driver::getAnalyzer() const
        {
                return *analyzer;
        }
        Driver::Driver(const ConfigureProject& config) : configureProject(config)
	{		
		if(configureProject.inputLenguaje == apidb::InputLenguajes::MySQL)
		{
			connector = new octetos::toolkit::clientdb::mysql::Connector();
			analyzer = new mysql::Analyzer(configureProject,connector);		
			try
			{
				bool flag = connector->connect(config.conectordb);
				if(!flag)
				{
					delete connector;
					delete analyzer;
					connector = NULL;
					analyzer = NULL;
				}
			}
			catch(octetos::toolkit::clientdb::SQLException ex)
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
		return configureProject.outputLenguaje;
	}
	
	bool Driver::driving(bool log)
	{
		if(connector == NULL) 
                {
                        std::cout<<"El conector es NULL." << std::endl;
                        return false;
                }

		if(analyze(log))
		{
			if(generate(log))
                        {                
                                return true;
                        }
                        else
                        {
                                std::cout<<"Fallo la etapa de generacion" << std::endl;                
                        }
		}
		else
                {
                        std::cout<<"Fallo la etapa de analisis." << std::endl;                
                }
		
		return false;
	}
	
	bool Driver::generate(bool log)
	{				
		if((configureProject.builDirectory.empty()) | (configureProject.builDirectory.compare(".") == 0))
		{
			
		}
		else
		{
			std::string direct = configureProject.builDirectory;
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
			flagCPP = cpp.generate(log);
			
			//std::cout<<"apidb::generators::CMake cmake(*analyzer);..."<<std::endl;
                        
			apidb::generators::CMake cmake(*analyzer,configureProject);			
			flagCMAKE = cmake.generate(log);
			
			///std::cout<<"if(flagCPP && flagCMAKE)..."<<std::endl;
			if(flagCPP && flagCMAKE)
			{
				if(log)analyzer->getOutputMessage() << "Generacion completada." <<std::endl;				
				return true;				
			}
			else
			{
				analyzer->getOutputMessage() << "Fallo."<<std::endl;
				return false;
			}
		}
		else
		{
			return false;
		}
	}
	
	bool Driver::analyze(bool log)
	{
		if(log)analyzer->getOutputMessage() << "Analisis de codigo..." << std::endl;
		if(log)analyzer->getOutputMessage() << "\tLenguaje de entrada: " << getInputLenguajeString(configureProject.inputLenguaje) << std::endl;
		
		/*apidb::mysql::Analyzer* analyzer = NULL;
		//std::cout<<"Step 1."<<std::endl;
		if(this->analyzer->getInputLenguaje() == apidb::InputLenguajes::MySQL)
		{
			analyzer = (apidb::mysql::Analyzer*)(this->analyzer);
		}
		else
		{
                        std::cout << "El lenguaje '" << getInputLenguajeString(configureProject.inputLenguaje) << "' no tiene soporte aun." << std::endl;
			return false;
		}*/
		//std::cout<<"Step 2."<<std::endl;
		if(analyzer->analyze(log)) //reading tables
                {
                        
                }  
                else
                {
                        std::cout<<"Faill reading table."<<std::endl;
                        return false;
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
}

