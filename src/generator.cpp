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
 
#include <list>
#include <vector>
#include <iostream>
#include <fstream>
#include <map>
#include <sys/stat.h>


#include "analyzer.hpp"
#include "generator.hpp"

namespace octetos
{
namespace apidb
{
namespace generators
{ 
	
	const symbols::SymbolsTable& Generator::getSymbolsTable()const
	{
		return analyzer.symbolsTable;
	}
	
	Generator::Generator(const ConfigureProject& config,apidb::Analyzer& d)  : configureProject(config),analyzer(d)
	{
		
	}
	
	CMake::CMake(apidb::Analyzer& d,const ConfigureProject& config): apidb::generators::Generator(config,d)
	{
                if(config.packing != PackingLenguajes::CMake)
                {
                        std::string msg = "La configuracion indica '" ;
                        msg = msg + getPackingLenguajeString() + "', pero el componente es CMake.";
                        throw BuildException(msg);
                }
	}
	
	CMake::~CMake()
	{
		
	}
	
	bool CMake::generate(bool log)
	{
		std::string namefile = "CMakeLists.txt";
		if((configureProject.builDirectory.empty()) | (configureProject.builDirectory.compare(".") == 0))
		{
			cmakelists.open(namefile);
		}
		else
		{
			cmakelists.open(configureProject.builDirectory+ "/" + namefile);
		}
			
		//CMakeLists.txt
		if(log)analyzer.getOutput().add( "\nGenerando archivos de gestor de projecto... ");
		std::string msg1 = "\n\tTipo de Gestor: " ;
                msg1 += getPackingLenguajeString() + "\n";
		if(log)analyzer.getOutput().add(msg1);
		
		cmakelists<<"CMAKE_MINIMUM_REQUIRED(VERSION ";
		cmakelists<<"3";
		cmakelists<<".";
		cmakelists<<"0";
		cmakelists<<")"<<std::endl;
		
		cmakelists<<"PROJECT(";
		cmakelists<<configureProject.name;
		cmakelists<<" VERSION ";
		cmakelists<<configureProject.versionResult.getMajor();
		cmakelists<<".";
		cmakelists<<configureProject.versionResult.getMinor();
		cmakelists<<".";
		cmakelists<<configureProject.versionResult.getPatch();
        
		if(configureProject.outputLenguaje == apidb::OutputLenguajes::CPP)
		{
			cmakelists<<" LANGUAGES CXX)"<<std::endl;
		}
		else
		{
			throw BuildException("El lenguje de salida es desconocido para el gentor de projecto.");
		}
		
		cmakelists<<"SET(CMAKE_CXX_STANDARD 14)"<<std::endl;
		cmakelists<<"SET(CMAKE_CXX_STANDARD_REQUIRED ON)"<<std::endl;
		cmakelists<<"SET(CMAKE_CXX_EXTENSIONS OFF)"<<std::endl;
                cmakelists<<" SET(CMAKE_BUILD_TYPE Debug)"<<std::endl;
		cmakelists<<std::endl;
		cmakelists<<"SET(" << configureProject.name << "_VERSION_STAGE \"alpha\")"<<std::endl;
		cmakelists<<"CONFIGURE_FILE(\"${PROJECT_SOURCE_DIR}/config.h.in\" \"${PROJECT_SOURCE_DIR}/config.h\")"<<std::endl;
		cmakelists<<std::endl;
        
    cmakelists<<"FIND_PACKAGE(PkgConfig REQUIRED)"<<std::endl;

    cmakelists<<"PKG_CHECK_MODULES(OCTETOS_CORE REQUIRED octetos-core)"<<std::endl;
    cmakelists<<"IF(OCTETOS_CORE_FOUND)"<<std::endl;
    cmakelists<<"INCLUDE_DIRECTORIES(${OCTETOS_CORE_INCLUDE_DIR})"<<std::endl;
    cmakelists<<"ELSE()\n";
    cmakelists<<"MESSAGE(FATAL_ERROR \"Could NOT find Octetos Core library\")\n";
    cmakelists<<"ENDIF()"<<std::endl;
        
    
    cmakelists<<"PKG_CHECK_MODULES(OCTETOS_DB_POSTGRESQL REQUIRED octetos-db-postgresql)"<<std::endl;
    cmakelists<<"IF(OCTETOS_DB_POSTGRESQL_FOUND)"<<std::endl;
    cmakelists<<"INCLUDE_DIRECTORIES(${OCTETOS_DB_POSTGRESQL_INCLUDE_DIR})"<<std::endl;
    cmakelists<<"ELSE()\n";
    cmakelists<<"MESSAGE(FATAL_ERROR \"Could NOT find Octetos DB library\")\n";
    cmakelists<<"ENDIF()"<<std::endl;
    
    if(configureProject.getInputLenguaje()  == InputLenguajes::MySQL)
    {
		cmakelists<<"PKG_CHECK_MODULES(OCTETOS_DB_MYSQL REQUIRED octetos-db-mysql)"<<std::endl;
		cmakelists<<"IF(OCTETOS_DB_MYSQL_FOUND)"<<std::endl;
		cmakelists<<"INCLUDE_DIRECTORIES(${OCTETOS_DB_MYSQL_INCLUDE_DIR})"<<std::endl;
        cmakelists<<"ELSE(OCTETOS_CORE_FOUND)\n";
        cmakelists<<"MESSAGE(FATAL_ERROR \"Could NOT find Octetos DB MySQL library\")\n";
		cmakelists<<"ENDIF()"<<std::endl;
    }
    else if(configureProject.getInputLenguaje()  == InputLenguajes::MariaDB)
    {
		cmakelists<<"PKG_CHECK_MODULES(MARIADB REQUIRED libmariadb)"<<std::endl;
		cmakelists<<"IF(MARIADB_FOUND)"<<std::endl;
		cmakelists<<"INCLUDE_DIRECTORIES(${MARIADB_INCLUDE_DIR})"<<std::endl;
		cmakelists<<"ENDIF()"<<std::endl;
    }
    else if(configureProject.getInputLenguaje()  == InputLenguajes::PostgreSQL)
    {
		cmakelists<<"PKG_CHECK_MODULES(LIBPQ libpq)"<<std::endl;
		cmakelists<<"IF(NOT LIBPQ_FOUND)"<<std::endl;
		cmakelists<<"PKG_CHECK_MODULES(LIBPQ REQUIRED libpqxx)"<<std::endl;
		cmakelists<<"ENDIF()"<<std::endl;
		cmakelists<<"IF(LIBPQ_FOUND)"<<std::endl;
		cmakelists<<"INCLUDE_DIRECTORIES(${LIBPQ_INCLUDE_DIR})"<<std::endl;
		cmakelists<<"ELSE()"<<std::endl;
         cmakelists<<"MESSAGE(FATAL_ERROR \"Could NOT find libpq or libpqxx library\")\n";
		cmakelists<<"ENDIF()"<<std::endl;
    }
    else
    {
        std::cerr << "No hay soporte para este lenguaje en cmake" << std::endl;
    }
		
		cmakelists<<std::endl;
		if(!configureProject.executable_target.empty() and configureProject.executable_target.compare("¿?") != 0)//la adicion de un ejecutable es opcional
		{
			cmakelists<<"ADD_EXECUTABLE(" << configureProject.executable_target << "  "<< configureProject.name; 
			if(configureProject.outputLenguaje == OutputLenguajes::CPP)
			{
				cmakelists <<".cpp "; 
			}
			cmakelists<<configureProject.executable_target;
			if(configureProject.outputLenguaje == OutputLenguajes::CPP)
			{
				cmakelists <<".cpp "; 
			}
			cmakelists <<")"<<std::endl;
            if(configureProject.getInputLenguaje()  == InputLenguajes::MySQL)
            {
                cmakelists<<"TARGET_LINK_LIBRARIES(" << configureProject.executable_target << "  ${OCTETOS_DB_MYSQL_LIBRARIES} ${OCTETOS_CORE_LIBRARIES} ${OCTETOS_CORE_LIBRARY} ${MYSQL_LIBRARIES})"<<std::endl;
            }
            else if(configureProject.getInputLenguaje()  == InputLenguajes::MariaDB)
            {
                cmakelists<<"TARGET_LINK_LIBRARIES(" << configureProject.executable_target << "  ${OCTETOS_DB_MARIA_LIBRARIES} ${OCTETOS_CORE_LIBRARIES} ${OCTETOS_CORE_LIBRARY} ${LIBMARIADB_LIBRARIES})"<<std::endl;
            }
            else if(configureProject.getInputLenguaje()  == InputLenguajes::PostgreSQL)
            {
                cmakelists<<"TARGET_LINK_LIBRARIES(" << configureProject.executable_target << "  ${OCTETOS_DB_POSTGRESQL_LIBRARIES} ${OCTETOS_CORE_LIBRARIES} ${LIBPQ_LIBRARIES})"<<std::endl;
            }
		}
		cmakelists<<"ADD_LIBRARY(${PROJECT_NAME} ";
        if(configureProject.compiled == apidb::Compiled::SHARED)
        {
            cmakelists << " SHARED ";
        }
        else if(configureProject.compiled == apidb::Compiled::STATIC)
        {
            cmakelists  << " STATIC ";
        }                
        cmakelists << "${PROJECT_NAME}.cpp )"<<std::endl;
        if(configureProject.getInputLenguaje()  == InputLenguajes::MySQL)
        {
            cmakelists<<"TARGET_LINK_LIBRARIES(${PROJECT_NAME}  ${OCTETOS_DB_MYSQL_LIBRARIES} ${OCTETOS_CORE_LIBRARIES} ${MYSQL_LIBRARIES})"<<std::endl;
        }
        else if(configureProject.getInputLenguaje()  == InputLenguajes::MariaDB)
        {
            cmakelists<<"TARGET_LINK_LIBRARIES(${PROJECT_NAME}   ${OCTETOS_DB_MARIA_LIBRARIES} ${OCTETOS_CORE_LIBRARIES} ${LIBMARIADB_LIBRARIES})"<<std::endl;
        }
        else if(configureProject.getInputLenguaje()  == InputLenguajes::PostgreSQL)
        {
            cmakelists<<"TARGET_LINK_LIBRARIES(${PROJECT_NAME}   ${OCTETOS_DB_POSTGRESQL_LIBRARIES} ${OCTETOS_CORE_LIBRARIES} ${LIBPQ_LIBRARIES})"<<std::endl;
        }
        cmakelists<<"if(APIDBLIB)\n";
        cmakelists<<"set(APIDBLIB ${PROJECT_NAME} PARENT_SCOPE)\n";
        cmakelists<<"endif()\n";
		cmakelists.close();
                std::string msg2 = "\tArchivo de gestion de projecto: '";
                msg2 += namefile + "'\n";
		if(log)analyzer.getOutput().add( msg2);
		
		//std::cout<<"Creating cmake.modules..."<<std::endl;
		//cmake.modules
		if(configureProject.builDirectory.empty() | (configureProject.builDirectory.compare(".") == 0))
		{
			std::string str = "cmake.modules";			
			mkdir(str.c_str(),S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
		}
		else
		{
			std::string direct = configureProject.builDirectory + "/cmake.modules";
			mkdir(direct.c_str(),S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
		}
		
	
		//std::cout<<"Creating toolkit-commonConfig.cmake..."<<std::endl;
		namefile = "octetos-coreConfig.cmake";
		if((configureProject.builDirectory.empty()) | (configureProject.builDirectory.compare(".") == 0))
		{
                        std::string file = "cmake.modules/";
                        file += namefile;
			toolkitcommonconifg.open(file);
		}
		else
		{
			toolkitcommonconifg.open(configureProject.builDirectory + "/cmake.modules/" + namefile);
		}
		toolkitcommonconifg<<"IF(OCTETOS_CORE_INCLUDE_DIR)"<<std::endl;
		  toolkitcommonconifg<<"SET(OCTETOS_CORE_FIND_QUIETLY TRUE)"<<std::endl;
		toolkitcommonconifg<<"ENDIF (OCTETOS_CORE_INCLUDE_DIR)"<<std::endl;

		toolkitcommonconifg<<"FIND_PATH(OCTETOS_CORE_INCLUDE_DIR core/common.hh"<<std::endl;
		  toolkitcommonconifg<<"/include/octetos"<<std::endl;
		  toolkitcommonconifg<<"/usr/include/octetos"<<std::endl;
		  toolkitcommonconifg<<"/usr/local/include/octetos"<<std::endl;
		toolkitcommonconifg<<")"<<std::endl;

		toolkitcommonconifg<<"FIND_LIBRARY(OCTETOS_CORE_LIBRARY"<< std::endl;
		  toolkitcommonconifg<<"NAMES liboctetos-core.so" << std::endl;
		  toolkitcommonconifg<<"PATHS  /lib /lib64 /lib32 /usr/lib /usr/local/lib/ /usr/lib/octetos/core /usr/local/lib/octetos/core /usr/lib/x86_64-linux-gnu/octetos/core" << std::endl;
		  toolkitcommonconifg<<"PATH_SUFFIXES octetos-core"<< std::endl;
		toolkitcommonconifg<<")"<<std::endl;

		toolkitcommonconifg<<"FIND_LIBRARY(OCTETOS_SEMVER_100_LIBRARY"<< std::endl;
		  toolkitcommonconifg<<"NAMES liboctetos-semver-100.so" << std::endl;
		  toolkitcommonconifg<<"PATHS  /lib /lib64 /lib32 /usr/lib /usr/local/lib/ /usr/lib/octetos/core /usr/local/lib/octetos/core /usr/lib/x86_64-linux-gnu/octetos/core" << std::endl;
		  toolkitcommonconifg<<"PATH_SUFFIXES octetos-semver-100"<< std::endl;
		toolkitcommonconifg<<")"<<std::endl;

		toolkitcommonconifg<<"IF (OCTETOS_CORE_INCLUDE_DIR AND OCTETOS_CORE_LIBRARY)" << std::endl;
		  toolkitcommonconifg<<"SET(OCTETOS_CORE_FOUND TRUE)" << std::endl;
		  toolkitcommonconifg<<"SET(OCTETOS_CORE_LIBRARIES ${OCTETOS_SEMVER_100_LIBRARY}  ${OCTETOS_CORE_LIBRARY} )" << std::endl;
		toolkitcommonconifg<<"ELSE (OCTETOS_CORE_INCLUDE_DIR AND OCTETOS_CORE_LIBRARY)" << std::endl;
		  toolkitcommonconifg<<"SET(OCTETOS_CORE_FOUND FALSE)" << std::endl;
		  toolkitcommonconifg<<"SET( OCTETOS_CORE_LIBRARIES )" << std::endl;
		toolkitcommonconifg<<"ENDIF (OCTETOS_CORE_INCLUDE_DIR AND OCTETOS_CORE_LIBRARY)" << std::endl;

		toolkitcommonconifg<<"IF (OCTETOS_CORE_FOUND)" << std::endl;
		  toolkitcommonconifg<<"IF (NOT OCTETOS_CORE_FIND_QUIETLY)" << std::endl;
			toolkitcommonconifg<<"MESSAGE(STATUS \"Found Octetos Core: ${OCTETOS_CORE_LIBRARY}\")" << std::endl;
		  toolkitcommonconifg<<"ENDIF (NOT OCTETOS_CORE_FIND_QUIETLY)" << std::endl;
		toolkitcommonconifg<<"ELSE (OCTETOS_CORE_FOUND)" << std::endl;
		  toolkitcommonconifg<<"IF (OCTETOS_CORE_FIND_REQUIRED)"<<std::endl;
			toolkitcommonconifg<<"MESSAGE(STATUS \"Looked for Octetos Core libraries named ${OCTETOS_CORE_NAMES}.\")" << std::endl;
			toolkitcommonconifg<<"MESSAGE(FATAL_ERROR \"Could NOT find Octetos Core library\")" << std::endl;
		  toolkitcommonconifg<<"ENDIF (OCTETOS_CORE_FIND_REQUIRED)" << std::endl;
		toolkitcommonconifg<<"ENDIF (OCTETOS_CORE_FOUND)" << std::endl;

		toolkitcommonconifg<<"MARK_AS_ADVANCED("<<std::endl;
		  toolkitcommonconifg<<"OCTETOS_CORE_LIBRARIES"<<std::endl;
		  toolkitcommonconifg<<"OCTETOS_CORE_INCLUDE_DIR"<<std::endl;
		  toolkitcommonconifg<<")"<<std::endl;
		toolkitcommonconifg.close();
                
    if(configureProject.getInputLenguaje()  == InputLenguajes::MySQL)
    {
		//std::cout<<"Creating toolkit-clientdbConfig.cmake..."<<std::endl;
		namefile = "octetos-db-mysqlConfig.cmake";
		if((configureProject.builDirectory.empty()) | (configureProject.builDirectory.compare(".") == 0))
		{
                        std::string file = "cmake.modules/";
                        file += namefile;
			toolkitclientdbConfig.open(file);
		}
		else
		{
			toolkitclientdbConfig.open(configureProject.builDirectory + "/cmake.modules/" + namefile);
		}	
		
		toolkitclientdbConfig<<"IF (OCTETOS_DB_MYSQL_INCLUDE_DIR)"<<std::endl;
		  toolkitclientdbConfig<<"SET(OCTETOS_DB_MYSQL_FIND_QUIETLY TRUE)"<<std::endl;
		toolkitclientdbConfig<<"ENDIF (OCTETOS_DB_MYSQL_INCLUDE_DIR)"<<std::endl;

		toolkitclientdbConfig<<"FIND_PATH(OCTETOS_DB_MYSQL_INCLUDE_DIR db/clientdb-mysql.hh"<<std::endl;
		  toolkitclientdbConfig<<"/include/octetos"<<std::endl;
		  toolkitclientdbConfig<<"/usr/include/octetos"<<std::endl;
		  toolkitclientdbConfig<<"/usr/local/include/octetos"<<std::endl;
		toolkitclientdbConfig<<")"<<std::endl;

		toolkitclientdbConfig<<"FIND_LIBRARY(OCTETOS_DB_LIBRARY"<<std::endl;
		  toolkitclientdbConfig<<"NAMES liboctetos-db.so"<<std::endl;
		  toolkitclientdbConfig<<"PATHS /lib /lib64 /lib32 /usr/lib /usr/local/lib/ /usr/lib/octetos/core /usr/local/lib/octetos/core /usr/lib/x86_64-linux-gnu/octetos/core"<<std::endl;
		  toolkitclientdbConfig<<"PATH_SUFFIXES octetos-db"<<std::endl;
		toolkitclientdbConfig<<")"<<std::endl;


		toolkitclientdbConfig<<"FIND_LIBRARY(OCTETOS_DB_MYSQL_LIBRARY"<<std::endl;
		  toolkitclientdbConfig<<"NAMES liboctetos-db-mysql.so"<<std::endl;
		  toolkitclientdbConfig<<"PATHS /lib /lib64 /lib32 /usr/lib /usr/local/lib/ /usr/lib/octetos/core /usr/local/lib/octetos/core /usr/lib/x86_64-linux-gnu/octetos/core"<<std::endl;
		  toolkitclientdbConfig<<"PATH_SUFFIXES octetos-db-mysql"<<std::endl;
		toolkitclientdbConfig<<")"<<std::endl;

		toolkitclientdbConfig<<"IF (OCTETOS_DB_MYSQL_INCLUDE_DIR AND OCTETOS_DB_MYSQL_LIBRARY)"<<std::endl;
		  toolkitclientdbConfig<<"SET(OCTETOS_DB_MYSQL_FOUND TRUE)"<<std::endl;
		  toolkitclientdbConfig<<"SET(OCTETOS_DB_MYSQL_LIBRARIES ${OCTETOS_DB_LIBRARY} ${OCTETOS_DB_MYSQL_LIBRARY} )"<<std::endl;
		toolkitclientdbConfig<<"ELSE (OCTETOS_DB_MYSQL_INCLUDE_DIR AND OCTETOS_DB_MYSQL_LIBRARY)"<<std::endl;
		  toolkitclientdbConfig<<"SET(OCTETOS_DB_MYSQL_FOUND FALSE)"<<std::endl;
		  toolkitclientdbConfig<<"SET(OCTETOS_DB_MYSQL_LIBRARIES )"<<std::endl;
		toolkitclientdbConfig<<"ENDIF (OCTETOS_DB_MYSQL_INCLUDE_DIR AND OCTETOS_DB_MYSQL_LIBRARY)"<<std::endl;

		toolkitclientdbConfig<<"IF (OCTETOS_DB_MYSQL_FOUND)"<<std::endl;
		  toolkitclientdbConfig<<"IF (NOT OCTETOS_DB_MYSQL_FIND_QUIETLY)"<<std::endl;
			toolkitclientdbConfig<<"MESSAGE(STATUS \"Found Octetos MySQL C++: ${OCTETOS_DB_MYSQL_LIBRARY}\")"<<std::endl;
		  toolkitclientdbConfig<<"ENDIF (NOT OCTETOS_DB_MYSQL_FIND_QUIETLY)"<<std::endl;
		toolkitclientdbConfig<<"ELSE (OCTETOS_DB_MYSQ_FOUND)"<<std::endl;
		  toolkitclientdbConfig<<"IF (OCTETOS_DB_MYSQL_FIND_REQUIRED)"<<std::endl;
			toolkitclientdbConfig<<"MESSAGE(STATUS \"Looked for Octetos MySQL C++ libraries named ${OCTETOS_DB_MYSQL_NAMES}.\")"<<std::endl;
			toolkitclientdbConfig<<"MESSAGE(FATAL_ERROR \"Could NOT find Octetos MySQL C++ library\")"<<std::endl;
		  toolkitclientdbConfig<<"ENDIF (OCTETOS_DB_MYSQL_FIND_REQUIRED)"<<std::endl;
		toolkitclientdbConfig<<"ENDIF (OCTETOS_DB_MYSQL_FOUND)"<<std::endl;

		toolkitclientdbConfig<<"MARK_AS_ADVANCED("<<std::endl;
		  toolkitclientdbConfig<<"OCTETOS_DB_MYSQL_LIBRARY"<<std::endl;
		  toolkitclientdbConfig<<"OCTETOS_DB_MYSQL_INCLUDE_DIR"<<std::endl;
		  toolkitclientdbConfig<<")"<<std::endl;
		toolkitclientdbConfig.close();
    }
    else
    {
        //ya no se construllen modules de coinfiguracion, se usa pkconfig en su lugar
    }
		//std::cout<<"Creating MySQLConfig.cmake..."<<std::endl;
		namefile = "MySQLConfig.cmake";
		if((configureProject.builDirectory.empty()) | (configureProject.builDirectory.compare(".") == 0))
		{
                        std::string file = "cmake.modules/";
                        file += namefile;
			toolkitcommonconifg.open(file);
		}
		else
		{
			toolkitcommonconifg.open(configureProject.builDirectory + "/cmake.modules/" + namefile);
		}
		
		toolkitcommonconifg<<"IF (MYSQL_INCLUDE_DIR)"<<std::endl;
		  toolkitcommonconifg<<"SET(MYSQL_FIND_QUIETLY TRUE)"<<std::endl;
		toolkitcommonconifg<<"ENDIF (MYSQL_INCLUDE_DIR)"<<std::endl;

		toolkitcommonconifg<<"FIND_PATH(MYSQL_INCLUDE_DIR mysql.h"<<std::endl;
		  toolkitcommonconifg<<"/usr/local/include/mysql"<<std::endl;
		  toolkitcommonconifg<<"/usr/include/mysql"<<std::endl;
		toolkitcommonconifg<<")"<<std::endl;


		toolkitcommonconifg<<"SET(MYSQL_NAMES mysqlclient mysqlclient_r)"<<std::endl;
		toolkitcommonconifg<<"FIND_LIBRARY(MYSQL_LIBRARY"<<std::endl;
		  toolkitcommonconifg<<"NAMES ${MYSQL_NAMES}"<<std::endl;
		  toolkitcommonconifg<<"PATHS /usr/lib /usr/local/lib /usr/lib/x86_64-linux-gnu"<<std::endl;
		  toolkitcommonconifg<<"PATH_SUFFIXES mysql"<<std::endl;
		toolkitcommonconifg<<")"<<std::endl;

		toolkitcommonconifg<<"IF (MYSQL_INCLUDE_DIR AND MYSQL_LIBRARY)"<<std::endl;
		  toolkitcommonconifg<<"SET(MYSQL_FOUND TRUE)"<<std::endl;
		  toolkitcommonconifg<<"SET( MYSQL_LIBRARIES ${MYSQL_LIBRARY} )"<<std::endl;
		toolkitcommonconifg<<"ELSE (MYSQL_INCLUDE_DIR AND MYSQL_LIBRARY)"<<std::endl;
		  toolkitcommonconifg<<"SET(MYSQL_FOUND FALSE)"<<std::endl;
		  toolkitcommonconifg<<"SET(MYSQL_LIBRARIES )"<<std::endl;
		toolkitcommonconifg<<"ENDIF (MYSQL_INCLUDE_DIR AND MYSQL_LIBRARY)"<<std::endl;

		toolkitcommonconifg<<"IF (MYSQL_FOUND)"<<std::endl;
		  toolkitcommonconifg<<"IF (NOT MYSQL_FIND_QUIETLY)"<<std::endl;
			toolkitcommonconifg<<"MESSAGE(STATUS \"Found MySQL: ${MYSQL_LIBRARY}\")"<<std::endl;
		  toolkitcommonconifg<<"ENDIF (NOT MYSQL_FIND_QUIETLY)"<<std::endl;
		toolkitcommonconifg<<"ELSE (MYSQL_FOUND)"<<std::endl;
		  toolkitcommonconifg<<"IF (MYSQL_FIND_REQUIRED)"<<std::endl;
			toolkitcommonconifg<<"MESSAGE(STATUS \"Looked for mysqlclient libraries named ${MYSQL_NAMES}.\")"<<std::endl;
			toolkitcommonconifg<<"MESSAGE(FATAL_ERROR \"Could NOT find mysqlclient library\")"<<std::endl;
		  toolkitcommonconifg<<"ENDIF (MYSQL_FIND_REQUIRED)"<<std::endl;
		toolkitcommonconifg<<"ENDIF (MYSQL_FOUND)"<<std::endl;

		toolkitcommonconifg<<"MARK_AS_ADVANCED("<<std::endl;
		  toolkitcommonconifg<<"MYSQL_LIBRARY"<<std::endl;
		  toolkitcommonconifg<<"MYSQL_INCLUDE_DIR"<<std::endl;
		  toolkitcommonconifg<<")"<<std::endl;
		toolkitcommonconifg.close();

		//std::cout<<"Creating config.h.in..."<<std::endl;
		namefile = "config.h.in";
		if((configureProject.builDirectory.empty()) | (configureProject.builDirectory.compare(".") == 0))
		{
			config.open(namefile);
		}
		else
		{
			config.open(configureProject.builDirectory + "/" + namefile);
		}
		config<<"#define VERSION_MAJOR @apidb_VERSION_MAJOR@"<<std::endl;
		config<<"#define VERSION_MINOR @apidb_VERSION_MINOR@"<<std::endl;
		config<<"#define VERSION_PATCH @apidb_VERSION_PATCH@"<<std::endl;
		config<<"#define VERSION_STAGE toolkit::Version::Stage::@apidb_VERSION_STAGE@"<<std::endl;
		config<<"#define PAKAGENAME \"@PROJECT_NAME@\""<<std::endl;
		config.close();		
		//analyzer->getOutputMessage()<<"\tArchivo de configuración de projecto: " << namefile <<std::endl;
				
                
		//analyzer->getOutputMessage()<<"\tArchivo de develping phase: " << namefile <<std::endl;
		//std::cout<<"return..."<<std::endl;
		return true;
	}
	
        std::string Generator::getPackingLenguajeString()const
	{
		switch(configureProject.packing)
		{
			case PackingLenguajes::CMake:		
				return "CMake";
			default:
				return "Unknow";
		}
	}
		
	PackingLenguajes Generator::getPackingLenguaje()const
	{
		return configureProject.packing;
	}
	
	std::string Generator::getOutputLenguajeString()const
	{
		switch(configureProject.outputLenguaje)
		{
			case OutputLenguajes::CPP:
				return "C++";
			default:
				return "Unknow";
		}
	}
		
	OutputLenguajes Generator::getOutputLenguaje()const
	{
		return configureProject.outputLenguaje;
	}
	CPP::~CPP()
	{
		delete[] writeResults;
	}
	const std::string& CPP::getHeaderName() const
	{
		return projectH;
	}
	std::ofstream& CPP::getSourceOutput()
	{
		return writeResults[1];
	}
	std::ofstream& CPP::getHeaderOutput()
	{
		return writeResults[0];
	}
	
	CPP::CPP(apidb::Analyzer& d,const ConfigureProject& config) : apidb::generators::Generator(config,d)
	{                
        if(config.outputLenguaje != OutputLenguajes::CPP)
        {
            std::string msg = "La configuracion indica '" ;
            msg = msg + getOutputLenguajeString()+ "', pero el componente es CMake.";
            throw BuildException(msg);
        }
                
		//outputLenguaje = d.getOutputLenguaje();
		writeResults = new std::ofstream[2];
		if((configureProject.builDirectory.empty()) | (configureProject.builDirectory.compare(".") == 0)) 
		{
			projectH = configureProject.name + ".hpp";
			writeResults[0].open(projectH);
			projectCPP = configureProject.name + ".cpp";
			writeResults[1].open(projectCPP);
		}
		else
		{
			projectH = configureProject.name + ".hpp";
			projectCPP = configureProject.name + ".cpp";
			writeResults[0].open(configureProject.builDirectory + "/" + projectH);
			writeResults[1].open(configureProject.builDirectory + "/" + projectCPP);
		}
	}    
	bool CPP::generate(bool log)
	{
		if(log)analyzer.getOutput().add("Generando archivos de codigo fuente... \n");
		std::string msg1 = "\tLenguaje resultado: " ;
		msg1 += getOutputLenguajeString() ;
		if(log)analyzer.getOutput().add(msg1);;
		//includes in header file
		std::string headers = "";     
        if(configureProject.getInputLenguaje() == InputLenguajes::MySQL)
        {
            getHeaderOutput()<< "#include <octetos/db/clientdb-mysql.hh>"<<std::endl<<std::endl;
            getSourceOutput()<< "#include <mysql/mysql.h>"<<std::endl;
        }
        else if(configureProject.getInputLenguaje() == InputLenguajes::MariaDB)
        {
            getHeaderOutput()<< "#include <octetos/db/clientdb-maria.hh>"<<std::endl<<std::endl;
        }
        else if(configureProject.getInputLenguaje() == InputLenguajes::PostgreSQL)
        {            
            getHeaderOutput()<< "#include <octetos/db/clientdb-postgresql.hh>"<<std::endl<<std::endl;
        }
        else
        {
            std::string msg = "Lenguaje no soportado " ;
            throw BuildException(msg);
            return false;
        }
		getHeaderOutput()<< "#include <string>" <<std::endl;        
		//inlcudes in source file
		getSourceOutput()<< "#include \"" << getHeaderName() <<"\""<<std::endl<<std::endl;
			
		
		//writing code				
		if(createH(getHeaderOutput(),log,getSymbolsTable()) == false)  
		{
			return false;
		}
		if(createCPP(getSourceOutput(),log,getSymbolsTable()) == false)
		{
			return false;
		}
          
        return true;    
    }
    
}
}
}
