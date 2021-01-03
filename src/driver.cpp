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
#if defined WINDOWS_MINGW
    #include <Message.hh>
#else
    #include <octetos/core/Message.hh>
#endif
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
    	if(configureProject.getDatconnection() == NULL)
    	{
    		std::string msg = "No se ha asignado la informacionde conexio.";
            throw core::Exception(msg,__FILE__,__LINE__);
    	}

        void* handle = configureProject.getLibraryHandle();
      	if(handle == NULL)
		{
            std::string msg = "Falla al intentar cargar la funcion de libreia 'destroyAnalyzer'";
            throw core::Exception(msg,__FILE__,__LINE__);
		}

        createAnalyzer = (apidb::Analyzer* (*)(const octetos::apidb::ConfigureProject*,octetos::db::Connector*,octetos::core::ActivityProgress*))dlsym(handle, "createAnalyzer");
        destroyAnalyzer = (void (*)(octetos::apidb::Analyzer*))dlsym(handle, "destroyAnalyzer");

        if(createAnalyzer == NULL)
        {
            std::string msgErr ="No se pudo cargar la funcion createAnalyzer :\n" ;
            msgErr = msgErr + "\t" + dlerror();
            throw core::Exception(msgErr,__FILE__,__LINE__);
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
			throw core::Exception(msg,__FILE__,__LINE__);
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
			if(analyzer == NULL)
			{
				std::string msg = "No se ha asignado el analizador '";
				throw core::Exception(msg,__FILE__,__LINE__);
			}
			symbols::Space* global = analyzer->symbolsTable.findSpace(configureProject.name);
			if(global == NULL)
			{
				std::string msg = "No se encontro el espacio Global '";
				msg += configureProject.name + "'";
				throw core::Exception(msg,__FILE__,__LINE__);
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
					throw core::Exception("Fallo durante la resolución de nombres de tabla.",__FILE__,__LINE__);
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
        if(analyzer)
        {
            destroyAnalyzer(analyzer);
        }
        if(connector)
        {
            connector->close();
            configureProject.deleteConnector(connector);
        }
    }
    const Analyzer&  Driver::getAnalyzer() const
    {
        return *analyzer;
    }
	Driver::Driver(ConfigureProject& config) : configureProject(config)
	{
		analyzer = NULL;
        destroyAnalyzer = NULL;
        createAnalyzer = NULL;
        
        if(!loadLibrary())
        {
            throw core::Exception("Fallo la carga(dinamica) de componete de basi de datos solicitado.");
        }
        
        if(config.build())
        {
			connector = config.newConnector();
            bool flag = connector->connect(*(config.getDatconnection()));
            if(!flag)
            {
                delete connector;
                connector = NULL;
            }
        }
        
        if(!createAnalyzer)
        {
            throw core::Exception("No se ha creado el constructor de anlizador",__FILE__,__LINE__);
        }
        if(!destroyAnalyzer)
        {
            throw core::Exception("No se ha creado el destructor de anlizador",__FILE__,__LINE__);
        }
        analyzer = createAnalyzer(&configureProject,connector,NULL);
        if(!analyzer)
        {
            throw core::Exception("Fallo al crean el anlizador",__FILE__,__LINE__);
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
        //std::cout << "Driver::driving : Step 3\n";
		return false;
	}

	bool Driver::generate(core::ActivityProgress* progress)
	{
        
        //std::cout << "Driver::generate : Step 1 \n";
        
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
        
        //std::cout << "Driver::generate : Step 2\n";
        
		if(configureProject.outputLenguaje == apidb::OutputLenguajes::CPP)
		{
            //std::cout << "Driver::generate : Step 2.1\n";
			apidb::generators::CPP cpp(*analyzer,configureProject);
            //std::cout << "Driver::generate : Step 2.2\n";
			if(!cpp.generate(true)) progress->add("Fallo la generacion de codigo fuente.\n");
            //std::cout << "Driver::generate : Step 2.3\n";
			//std::cout<<"apidb::generators::CMake cmake(*analyzer);..."<<std::endl;
            if(configureProject.packing == PackingLenguajes::CMake)
            {
                apidb::generators::CMake cmake(*analyzer,configureProject);
                if(!cmake.generate(true)) progress->add("Fallo la generacion de empaquetado.\n");;
            }
		}
		else if(configureProject.outputLenguaje == apidb::OutputLenguajes::JAVA)
		{
            //std::cout << "Driver::generate : Step 2.1\n";
			apidb::generators::Java java(*analyzer,configureProject);
            //std::cout << "Driver::generate : Step 2.2\n";
			if(!java.generate(true)) progress->add("Fallo la generacion de codigo fuente.\n");
            if(configureProject.packing == PackingLenguajes::Maven)
            {
                apidb::generators::Maven maven(*analyzer,configureProject);
                if(!maven.generate(true)) progress->add("Fallo la generacion de empaquetado.\n");
            }
            //std::cout << "Driver::generate : Step 2.3\n";
		}
		else
		{
			return false;
		}
        //std::cout << "Driver::generate : Step 3\n";
        //if(progress != NULL) progress->add("Fallo la generacion.\n");
        
        return true; 
    }


	bool Driver::analyze(core::ActivityProgress* progress)
	{
        //std::cout << "Driver::analyze : Step 1\n";

        //std::cout << "Driver::analyze : Step 2\n";
        if(connector == NULL)
        {
            throw core::Exception("No se ha asignado a conector.");
        }
        if(createAnalyzer == NULL)
        {
            throw core::Exception("No se ha asignado a metodo contructor de driver.");
        }
        
        //std::cout << "Driver::analyze : Step 3\n";

        analyzer = createAnalyzer(&configureProject,connector,progress);

        //std::cout << "Driver::analyze : Step 4\n";
        if(progress != NULL)
        {
            core::Confirmation conf1("\n\tAnalisis de Base de Datos..");
            progress->add(conf1);

            std::string msg ="\n\tLenguaje de entrada : " ;
            msg+= getInputLenguaje(configureProject.getInputLenguaje()) + "\n";
            core::Confirmation conf2(msg);
            progress->add(conf2);
        }
        
		bool flagAnalyzer = false;
        //std::cout << "Driver::analyze : Step 5\n";
        flagAnalyzer = analyzer->analyze(progress);
        //std::cout << "Driver::analyze : Step 6\n";
		if(flagAnalyzer == false) //reading tables
		{
            if(progress != NULL)
            {
                std::string msgErr = "\tFallo al leer durante la fase de analisis.";
                throw core::Exception(msgErr,__FILE__,__LINE__);
            }
            
            return false;
		}
        
        //std::cout << "Driver::analyze : Step 7\n";
		return true;
	}

}
}

