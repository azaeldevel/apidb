/**
 * 
 *  This file is part of apidb.
 *  APIDB do Make easy to connect your Database
 *  Copyright (C) 2018  Azael Reyes
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

#include <exception>
#include <string.h>
#include <stdio.h>
#include <exception>
#include <toolkit/common/common.hpp>

#include "driver.hpp"
#include "../common.hpp"
#include "mysql-reader-c++/analyzer.hpp"
#include "Errors.hpp"


namespace octetos
{
namespace apidb
{
	bool Driver::getFiledsName(std::list<std::string>& retList,const std::string& table)const
	{
		
		symbols::Space* global = analyzer->symbolsTable.findSpace(configureProject.name);
		if(global == NULL)
		{
			std::string msg = "No se encontro el espacio Global '";
			msg += configureProject.name + "'";
			toolkit::Error::write(toolkit::Error(msg,ErrorCodes::GENERATOR_FAIL,__FILE__,__LINE__));
			return false;
		}
		
		
		
		symbols::Table* tb = global->findTable(table);
		if(tb != NULL)
		{
			for(symbols::Table::iterator it = tb->begin(); it != tb->end(); it++)
			{
				symbols::Symbol* symbol = it->second;
				retList.push_back(symbol->getName());
			}
		}
		else			
		{
			return false;
		}			
		
		return true;
	}
	bool Driver::getTablesName(std::list<std::string>& ret, symbols::Space* actualspace)const
	{
		symbols::Space* space;		
		if(actualspace ==  NULL)
		{
			symbols::Space* global = analyzer->symbolsTable.findSpace(configureProject.name);
			if(global == NULL)
			{
				std::string msg = "No se encontro el espacio Global '";
				msg += configureProject.name + "'";
				toolkit::Error::write(toolkit::Error(msg,ErrorCodes::GENERATOR_FAIL,__FILE__,__LINE__));
				return false;
			}
			space = global;
		}
		else
		{
			space = actualspace;
		}		
		for(symbols::SymbolsTable::iterator it = space->begin(); it != space->end(); it++)
		{
			symbols::ISpace* ispace = it->second;
			if(ispace->what() == symbols::SpaceType::SPACE)
			{
				if(getTablesName(ret,(symbols::Space*)ispace) == false) 
				{
					toolkit::Error::write(toolkit::Error("Fallo durante la resolución de nombres de tabla.",ErrorCodes::GENERATOR_FAIL,__FILE__,__LINE__));
					return false;
				}
			}
			else if(ispace->what() == symbols::SpaceType::TABLE)
			{
				symbols::Table* tb = (symbols::Table*)ispace;
				ret.push_back(tb->getFullName());
			}
		}
		return true;
	}
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
                analyzer = NULL;
                
		//std::cout <<"Iniciando contruccion." <<std::endl;
		if(configureProject.inputLenguaje == apidb::InputLenguajes::MySQL)
		{
                        //std::cout <<"Creando conector." <<std::endl;
			connector = new octetos::toolkit::clientdb::mysql::Connector();
			try
			{
				bool flag = connector->connect(config.conectordb);
				if(!flag)
				{
					delete connector;
					connector = NULL;
				}
			}
			catch(octetos::toolkit::clientdb::SQLException ex)
			{				
				std::cout <<"Fallo la conexion a DB : "<< ex.what() <<std::endl;
			}
		}
		else
		{
			std::cout <<"Lenguaje de entrada desconocido."<<std::endl;
		}	
		//std::cout <<"Objeto contruido" <<std::endl;
	}
		
	OutputLenguajes Driver::getOutputLenguaje() const
	{
		return configureProject.outputLenguaje;
	}
	
	bool Driver::driving(toolkit::ActivityProgress* progress)
	{
		if(connector == NULL) 
		{
			std::cout<<"El conector es NULL." << std::endl;
			return false;
		}

		if(analyze(progress))
		{
			if(generate(progress))
			{                
				return true;
			}
			else
			{
				std::cout<<"Fallo la etapa de generacion" << std::endl;  
				return false;
			}
		}
		else
		{
			std::cout<<"Fallo la etapa de analisis." << std::endl;   
			return false;
		}
		
		return false;
	}
	/*bool Driver::driving(bool log)
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
	}*/
	
	bool Driver::generate(toolkit::ActivityProgress* progress)
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
			if(progress != NULL)flagCPP = cpp.generate(true);	
                        else flagCPP = cpp.generate(false);	
			//std::cout<<"apidb::generators::CMake cmake(*analyzer);..."<<std::endl;                        
		}
		else
		{
			return false;
		}
		
                if(configureProject.packing == PackingLenguajes::CMake)
                {
			apidb::generators::CMake cmake(*analyzer,configureProject);			
			if(progress != NULL)flagCMAKE = cmake.generate(true);
                        else flagCMAKE = cmake.generate(false);
                }
                else
                {
                        return false;
                }
			
                ///std::cout<<"if(flagCPP && flagCMAKE)..."<<std::endl;
                if(flagCPP && flagCMAKE)
                {
                        std::string msg1 =  "Generacion completada.\n" ;
                        toolkit::Confirmation conf1(msg1);
                        if(progress != NULL) progress->add(conf1);	
                        return true;				
                }
                else
                {
                        BuildException fail("Fallo.");
                        analyzer->getOutput().add(fail);
                        return false;
                }
	}
	
	/*bool Driver::generate(bool log)
	{		
                if(ENABLE_DEVEL_WARNING)
                {
                        std::cout <<"\u001b[31;1m" << "\nAdvertencia: Driver::generate(bool log) esta marcada como obsoleta yserá removida a apartir de v2.\n Use Driver::analyze(toolkit::ActivityProgress* progress) en su lugar.\n" << "\u001b[0m";
                }
                
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
                        if(log)analyzer->getOutput().add("Generacion completada.\n");				
                        return true;				
                }
                else
                {
                        BuildException fail2("Fallo.");
                        analyzer->getOutput().add(fail2);
                        return false;
                }
	}*/
	
	bool Driver::analyze(toolkit::ActivityProgress* progress)
	{
                if(configureProject.inputLenguaje == apidb::InputLenguajes::MySQL)
                {
			if(analyzer != NULL)
                        {
                                delete analyzer;
                                analyzer = NULL;
                                analyzer = new mysql::Analyzer(configureProject,connector,progress);		
                        }
                        else
                        {
                                analyzer = new mysql::Analyzer(configureProject,connector,progress);
                        }
                }
                
                if(progress != NULL)
                {
                        toolkit::Confirmation conf1("\n\tAnalisis de codigo...");
                        progress->add(conf1);
                        std::string msg ="\n\tLenguaje de entrada: " ;
                        msg+= getInputLenguaje(configureProject.inputLenguaje) + "\n";
                        toolkit::Confirmation conf2(msg);
                        progress->add(conf2);
                }
		
		if(analyzer->analyze(progress) == false) //reading tables
		{
			if(toolkit::Error::check())
			{
				if(progress != NULL)progress->add(toolkit::Error::get());
				return false;
			}
			else
			{           
				if(progress != NULL)
				{
					std::string msgErr ="\tFallo al leer durante la fase de analisis." ;
					toolkit::Error err(msgErr,toolkit::Error::ERROR_UNKNOW,__FILE__,__LINE__);
					progress->add(err);
					return false;
				}
			}
		}
		
		return true;
	}
	
} 
}

