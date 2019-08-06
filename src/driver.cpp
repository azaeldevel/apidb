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
				//analyzer->getErrorMessage() <<"Fallo la conexion a DB : "<< ex.what() <<std::endl;
			}
		}
		else
		{
			//analyzer->getErrorMessage() <<"Lenguaje de entrada desconocido."<<std::endl;
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
		
		
                bool flagCPP,flagCMAKE;
		if(configureProject.outputLenguaje == apidb::OutputLenguajes::CPP)
		{
			
			//std::cout<<"apidb::generators::CPP cpp(*analyzer);..."<<std::endl;
			apidb::generators::CPP cpp(*analyzer,configureProject);
			flagCPP = cpp.generate(log);			
			//std::cout<<"apidb::generators::CMake cmake(*analyzer);..."<<std::endl;                        
		}
		else
		{
			return false;
		}
		
                if(configureProject.packing == PackingLenguajes::CMake)
                {
			apidb::generators::CMake cmake(*analyzer,configureProject);			
			flagCMAKE = cmake.generate(log);
                }
                else
                {
                        return false;
                }
			
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
	
	bool Driver::analyze(bool log)
	{
                if(configureProject.inputLenguaje == apidb::InputLenguajes::MySQL)
                {
			analyzer = new mysql::Analyzer(configureProject,connector);		
                }
                
		if(log)analyzer->getOutputMessage() << "Analisis de codigo..." << std::endl;
		if(log)analyzer->getOutputMessage() << "\tLenguaje de entrada: " << getInputLenguajeString(configureProject.inputLenguaje) << std::endl;
		
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
} 
}

