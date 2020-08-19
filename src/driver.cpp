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
#include <octetos/core/Message.hh>
#include <dlfcn.h>

#include "driver.hpp"
//#include "mysql-reader-c++/analyzer.hpp"
//#include "analyzer-postgresql.hpp"
#include "Errors.hpp"


namespace octetos
{
namespace apidb
{
    bool Driver::loadLibrary()
    {
        void* handle = configureProject.getLibraryHandle();
        if(handle == NULL)
        {
			std::string msg = "Falla al intetar cargar la funcion de libreia 'destroyAnalyzer'";
			core::Error::write(core::Error(msg,ErrorCodes::DRIVER_FAIL ,__FILE__,__LINE__));
			return false;
        }
        destroy = (void (*)(octetos::apidb::Analyzer*))dlsym(handle, "destroyAnalyzer");
            
        create = (apidb::Analyzer* (*)(const octetos::apidb::ConfigureProject*,octetos::db::Connector*,octetos::core::ActivityProgress*))dlsym(handle, "createAnalyzer");
        if(create == NULL)
        {
            std::string msgErr ="No se pudo cargar la funcion createAnalyzer :\n" ;
            msgErr = msgErr + "\t" + dlerror();
            core::Error err(msgErr,ErrorCodes::DRIVER_FAIL,__FILE__,__LINE__);            
            core::Error::write(err);
            return false;
        }
        
        return true;
    }
	bool Driver::getFiledsName(std::list<std::string>& retList,const std::string& table)const
	{
		
		symbols::Space* global = analyzer->symbolsTable.findSpace(configureProject.name);
		if(global == NULL)
		{
			std::string msg = "No se encontro el espacio Global '";
			msg += configureProject.name + "'";
			core::Error::write(core::Error(msg,ErrorCodes::DRIVER_FAIL,__FILE__,__LINE__));
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
				core::Error::write(core::Error(msg,ErrorCodes::GENERATOR_FAIL,__FILE__,__LINE__));
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
					core::Error::write(core::Error("Fallo durante la resolución de nombres de tabla.",ErrorCodes::GENERATOR_FAIL,__FILE__,__LINE__));
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
        if(analyzer and destroy)
        {
            destroy(analyzer);
        }
        if(connector)
        {
            connector->close();
            delete connector;
        }
        //if(!handle) dlclose(handle); 
    }
    const Analyzer&  Driver::getAnalyzer() const
    {
        return *analyzer;
    }
	Driver::Driver(const ConfigureProject& config) : configureProject(config)
	{ 
        if(!loadLibrary())
        {
            return;
        }
        
		analyzer = NULL;
        
		connector = config.newConnector();
        
        try
        {
            bool flag = connector->connect(*(config.getDatconnection()));
            if(!flag)
            {
                delete connector;
                connector = NULL;
            }
        }
        catch(octetos::db::SQLException ex)
        {
            //std::cout <<"Fallo la conexion a DB : "<< ex.what() <<std::endl;
            core::Error err(ex.what(), ErrorCodes::ANALYZER_FAIL,__FILE__,__LINE__);
            core::Error::write(err);
        }                    
		
	}
		
	OutputLenguajes Driver::getOutputLenguaje() const
	{
		return configureProject.outputLenguaje;
	}
	
	bool Driver::driving(core::ActivityProgress* progress)
	{
        //std::cout << "Driver::driving : Step 1\n";
		if(connector == NULL) 
		{
			//std::cout<<"El conector es NULL." << std::endl;
			return false;
		}
        //std::cout << "Driver::driving : Step 2\n";        
		if(analyze(progress))
		{
			if(generate(progress))
			{                
				return true;
			}
			else
			{
				return false;
			}
		}
		else
		{
			//std::cout<<"Fallo la etapa de analisis." << std::endl;   
			return false;
		}		
        std::cout << "Driver::driving : Step 3\n";
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
	
	bool Driver::generate(core::ActivityProgress* progress)
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
                        core::Confirmation conf1(msg1);
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
	
	bool Driver::analyze(core::ActivityProgress* progress)
	{
        //std::cout << "Driver::analyze : Step 1\n";

        
        //std::cout << "Driver::analyze : Step 2\n";
        
        

        analyzer = create(&configureProject,connector,progress);
        
        //std::cout << "Driver::analyze : Step 3\n";        
        if(progress != NULL)
        {
            core::Confirmation conf1("\n\tAnalisis de Base de Datos..");
            progress->add(conf1);
            
            std::string msg ="\n\tLenguaje de entrada: " ;
            msg+= getInputLenguaje(configureProject.getInputLenguaje()) + "\n";
            core::Confirmation conf2(msg);
            progress->add(conf2);
        }
        //std::cout << "Driver::analyze : Step 4\n";
		bool flagAnalyzer = false;
		try
		{
			flagAnalyzer = analyzer->analyze(progress);
		}
		catch(db::SQLException e)
		{
			core::Error err(e.what(),ErrorCodes::ANALYZER_FAIL,__FILE__,__LINE__);
			core::Error::write(err);
			return false;
		}
        //std::cout << "Driver::analyze : Step 5\n";
		if(flagAnalyzer == false) //reading tables
		{
			if(core::Error::check())
			{
				if(progress != NULL)progress->add(core::Error::get());
				return false;
			}
			else
			{           
				if(progress != NULL)
				{
					std::string msgErr ="\tFallo al leer durante la fase de analisis." ;
					core::Error err(msgErr,core::Error::ERROR_UNKNOW,__FILE__,__LINE__);
					progress->add(err);
				}
                return false;
			}
		}
		
        //std::cout << "Driver::analyze : Step 6\n";
		return true;
	}
	
} 
}

