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
		if(log)analyzer.getOutput().add( "Generando archivos de gestor de projecto... \n");
		std::string msg1 = "\tTipo de Gestor: " ;
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
		cmakelists<<".";
		cmakelists<<"0 ";
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
		cmakelists<<"FIND_PACKAGE(MySQL REQUIRED PATHS ${PROJECT_SOURCE_DIR}/cmake.modules/)"<<std::endl;
		cmakelists<<"IF(MySQL_FOUND)"<<std::endl;
			cmakelists<<"INCLUDE_DIRECTORIES(${MYSQL_INCLUDE_DIR})"<<std::endl;
		cmakelists<<"ENDIF()"<<std::endl;
		cmakelists<<"FIND_PACKAGE(octeos-toolkit-common-c++ REQUIRED PATHS ${PROJECT_SOURCE_DIR}/cmake.modules/)"<<std::endl;
		cmakelists<<"IF(OCTETOS_TOOLKIT_COMMON_CPP_FOUND)"<<std::endl;
                cmakelists<<"INCLUDE_DIRECTORIES(${OCTETOS_TOOLKIT_COMMON_CPP_INCLUDE_DIR})"<<std::endl;
		cmakelists<<"ENDIF()"<<std::endl;
		cmakelists<<"FIND_PACKAGE(octetos-toolkit-clientdb-myc++ REQUIRED PATHS ${PROJECT_SOURCE_DIR}/cmake.modules/)"<<std::endl;
		cmakelists<<"IF(OCTETOS_TOOLKIT_CLIENTDB_MYCPP_FOUND)"<<std::endl;
                cmakelists<<"INCLUDE_DIRECTORIES(${OCTETOS_TOOLKIT_CLIENTDB_MYCPP_INCLUDE_DIR})"<<std::endl;
		cmakelists<<"ENDIF()"<<std::endl;
		cmakelists<<std::endl;
                if(!configureProject.executable_target.empty())//la adicion de un ejecutable es opcional
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
                        cmakelists<<"TARGET_LINK_LIBRARIES(" << configureProject.executable_target << "  ${OCTETOS_TOOLKIT_CLIENTDB_MYCPP_LIBRARIES} ${OCTETOS_TOOLKIT_COMMON_CPP_LIBRARIES} ${MYSQL_LIBRARIES})"<<std::endl;
                }
		cmakelists<<"ADD_LIBRARY("<< configureProject.builDirectory;
                if(configureProject.compiled == apidb::Compiled::SHARED)
                {
                        cmakelists << " SHARED ";
                }
                else if(configureProject.compiled == apidb::Compiled::STATIC)
                {
                         cmakelists  << " STATIC ";
                }                
                cmakelists << configureProject.name <<".cpp )"<<std::endl;
		cmakelists<<"TARGET_LINK_LIBRARIES("<< configureProject.name <<" ${MYSQL_LIBRARIES} ${OCTETOS_TOOLKIT_COMMON_LIBRARIES}  ${OCTETOS_TOOLKIT_CLIENTDB_MYCPP_LIBRARIES} )"<<std::endl;
		cmakelists.close();
                std::string msg2 = "\tArchivo de gestion de projecto: '";
                msg2 += namefile + "'\n";
		if(log)analyzer.getOutput().add( msg2);
		
		//std::cout<<"Creating cmake.modules..."<<std::endl;
		//cmake.modules
		if((configureProject.builDirectory.empty()) | (configureProject.builDirectory.compare(".") == 0))
		{
			std::ifstream ifile("cmake.modules");
			if (!ifile) 
			{
				system("mkdir cmake.modules");
			}
		}
		else
		{
			std::string direct = configureProject.builDirectory + "/cmake.modules";
			std::ifstream ifile(direct);
			if (!ifile) 
			{
				std::string cmd = "mkdir  ";
				//system((cmd + analyzer.getDirectoryProject()).c_str());
				cmd = cmd + direct;				
				system(cmd.c_str());
			}			
		}
	
		//std::cout<<"Creating toolkit-commonConfig.cmake..."<<std::endl;
		namefile = "octeos-toolkit-common-c++Config.cmake";
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
		toolkitcommonconifg<<"IF(OCTETOS_TOOLKIT_COMMON_CPP_INCLUDE_DIR)"<<std::endl;
		  toolkitcommonconifg<<"SET(OCTETOS_TOOLKIT_COMMON_CPP_FIND_QUIETLY TRUE)"<<std::endl;
		toolkitcommonconifg<<"ENDIF (OCTETOS_TOOLKIT_COMMON_CPP_INCLUDE_DIR)"<<std::endl;

		toolkitcommonconifg<<"FIND_PATH(OCTETOS_TOOLKIT_COMMON_CPP_INCLUDE_DIR toolkit/common/common.hpp"<<std::endl;
		  toolkitcommonconifg<<"/usr/local/include/octetos"<<std::endl;
		  toolkitcommonconifg<<"/usr/include/octetos"<<std::endl;
		toolkitcommonconifg<<")"<<std::endl;

		toolkitcommonconifg<<"SET(OCTETOS_TOOLKIT_COMMON_CPP_NAMES octetos-toolkit-common-c++)"<<std::endl;
		toolkitcommonconifg<<"FIND_LIBRARY(OCTETOS_TOOLKIT_COMMON_CPP_LIBRARY"<<std::endl;
		  toolkitcommonconifg<<"NAMES ${OCTETOS_TOOLKIT_COMMON_CPP_NAMES}"<<std::endl;
		  toolkitcommonconifg<<"PATHS /usr/lib/octetos/toolkit/common /usr/local/lib/octetos/toolkit/common /usr/lib/x86_64-linux-gnu/octetos/toolkit/common"<<std::endl;
		  toolkitcommonconifg<<"PATH_SUFFIXES octetos-toolkit-common-c++"<<std::endl;
		toolkitcommonconifg<<")"<<std::endl;

		toolkitcommonconifg<<"IF (OCTETOS_TOOLKIT_COMMON_CPP_INCLUDE_DIR AND OCTETOS_TOOLKIT_COMMON_CPP_LIBRARY)"<<std::endl;
		  toolkitcommonconifg<<"SET(OCTETOS_TOOLKIT_COMMON_CPP_FOUND TRUE)"<<std::endl;
		  toolkitcommonconifg<<"SET(OCTETOS_TOOLKIT_COMMON_CPP_LIBRARIES ${OCTETOS_TOOLKIT_COMMON_CPP_LIBRARY} )"<<std::endl;
		toolkitcommonconifg<<"ELSE (OCTETOS_TOOLKIT_COMMON_CPP_INCLUDE_DIR AND OCTETOS_TOOLKIT_COMMON_CPP_LIBRARY)"<<std::endl;
		  toolkitcommonconifg<<"SET(OCTETOS_TOOLKIT_COMMON_CPP_FOUND FALSE)"<<std::endl;
		  toolkitcommonconifg<<"SET( OCTETOS_TOOLKIT_COMMON_CPP_LIBRARIES )"<<std::endl;
		toolkitcommonconifg<<"ENDIF (OCTETOS_TOOLKIT_COMMON_CPP_INCLUDE_DIR AND OCTETOS_TOOLKIT_COMMON_CPP_LIBRARY)"<<std::endl;

		toolkitcommonconifg<<"IF (OCTETOS_TOOLKIT_COMMON_CPP_FOUND)"<<std::endl;
		  toolkitcommonconifg<<"IF (NOT OCTETOS_TOOLKIT_COMMON_CPP_FIND_QUIETLY)"<<std::endl;
			toolkitcommonconifg<<"MESSAGE(STATUS \"Found toolkit-common: ${OCTETOS_TOOLKIT_COMMON_CPP_LIBRARY}\")"<<std::endl;
		  toolkitcommonconifg<<"ENDIF (NOT OCTETOS_TOOLKIT_COMMON_CPP_FIND_QUIETLY)"<<std::endl;
		toolkitcommonconifg<<"ELSE (OCTETOS_TOOLKIT_COMMON_CPP_FOUND)"<<std::endl;
		  toolkitcommonconifg<<"IF (OCTETOS_TOOLKIT_COMMON_CPP_FIND_REQUIRED)"<<std::endl;
			toolkitcommonconifg<<"MESSAGE(STATUS \"Looked for toolkit-common libraries named ${OCTETOS_TOOLKIT_COMMON_CPP_NAMES}.\")"<<std::endl;
			toolkitcommonconifg<<"MESSAGE(FATAL_ERROR \"Could NOT find toolkit-common library\")"<<std::endl;
		  toolkitcommonconifg<<"ENDIF (OCTETOS_TOOLKIT_COMMON_CPP_FIND_REQUIRED)"<<std::endl;
		toolkitcommonconifg<<"ENDIF (OCTETOS_TOOLKIT_COMMON_CPP_FOUND)"<<std::endl;

		toolkitcommonconifg<<"MARK_AS_ADVANCED("<<std::endl;
		  toolkitcommonconifg<<"OCTETOS_TOOLKIT_COMMON_CPP_LIBRARIES"<<std::endl;
		  toolkitcommonconifg<<"OCTETOS_TOOLKIT_COMMON_CPP_INCLUDE_DIR"<<std::endl;
		  toolkitcommonconifg<<")"<<std::endl;
		toolkitcommonconifg.close();
                
                if(configureProject.inputLenguaje  == InputLenguajes::MySQL)
                {
		//std::cout<<"Creating toolkit-clientdbConfig.cmake..."<<std::endl;
		namefile = "octetos-toolkit-clientdb-myc++Config.cmake";
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
		
		toolkitclientdbConfig<<"IF (OCTETOS_TOOLKIT_CLIENTDB_MYCPP_INCLUDE_DIR)"<<std::endl;
		  toolkitclientdbConfig<<"SET(OCTETOS_TOOLKIT_CLIENTDB_MYCPP_FIND_QUIETLY TRUE)"<<std::endl;
		toolkitclientdbConfig<<"ENDIF (OCTETOS_TOOLKIT_CLIENTDB_MYCPP_INCLUDE_DIR)"<<std::endl;

		toolkitclientdbConfig<<"FIND_PATH(OCTETOS_TOOLKIT_CLIENTDB_MYCPP_INCLUDE_DIR clientdb.hpp"<<std::endl;
		  toolkitclientdbConfig<<"/usr/local/include/octetos/toolkit/clientdb"<<std::endl;
		  toolkitclientdbConfig<<"/usr/include/octetos/toolkit/clientdb"<<std::endl;
		toolkitclientdbConfig<<")"<<std::endl;

		toolkitclientdbConfig<<"SET(OCTETOS_TOOLKIT_CLIENTDB_MYCPP_NAMES octetos-toolkit-clientdb-myc++)"<<std::endl;
		toolkitclientdbConfig<<"FIND_LIBRARY(OCTETOS_TOOLKIT_CLIENTDB_MYCPP_LIBRARY"<<std::endl;
		  toolkitclientdbConfig<<"NAMES ${OCTETOS_TOOLKIT_CLIENTDB_MYCPP_NAMES}"<<std::endl;
		  toolkitclientdbConfig<<"PATHS /usr/lib/octetos/toolkit/clientdb /usr/local/lib/octetos/toolkit/clientdb /usr/lib/x86_64-linux-gnu/octetos/toolkit/clientdb"<<std::endl;
		  toolkitclientdbConfig<<"PATH_SUFFIXES octetos-toolkit-clientdb-myc++"<<std::endl;
		toolkitclientdbConfig<<")"<<std::endl;

		toolkitclientdbConfig<<"IF (OCTETOS_TOOLKIT_CLIENTDB_MYCPP_INCLUDE_DIR AND OCTETOS_TOOLKIT_CLIENTDB_MYCPP_LIBRARY)"<<std::endl;
		  toolkitclientdbConfig<<"SET(OCTETOS_TOOLKIT_CLIENTDB_MYCPP_FOUND TRUE)"<<std::endl;
		  toolkitclientdbConfig<<"SET(OCTETOS_TOOLKIT_CLIENTDB_MYCPP_LIBRARIES ${OCTETOS_TOOLKIT_CLIENTDB_MYCPP_LIBRARY} )"<<std::endl;
		toolkitclientdbConfig<<"ELSE (OCTETOS_TOOLKIT_CLIENTDB_MYCPP_INCLUDE_DIR AND OCTETOS_TOOLKIT_CLIENTDB_MYCPP_LIBRARY)"<<std::endl;
		  toolkitclientdbConfig<<"SET(OCTETOS_TOOLKIT_CLIENTDB_MYCPP_FOUND FALSE)"<<std::endl;
		  toolkitclientdbConfig<<"SET(OCTETOS_TOOLKIT_CLIENTDB_MYCPP_LIBRARIES )"<<std::endl;
		toolkitclientdbConfig<<"ENDIF (OCTETOS_TOOLKIT_CLIENTDB_MYCPP_INCLUDE_DIR AND OCTETOS_TOOLKIT_CLIENTDB_MYCPP_LIBRARY)"<<std::endl;

		toolkitclientdbConfig<<"IF (OCTETOS_TOOLKIT_CLIENTDB_MYCPP_FOUND)"<<std::endl;
		  toolkitclientdbConfig<<"IF (NOT OCTETOS_TOOLKIT_CLIENTDB_MYCPP_FIND_QUIETLY)"<<std::endl;
			toolkitclientdbConfig<<"MESSAGE(STATUS \"Found toolkit-cliendb: ${OCTETOS_TOOLKIT_CLIENTDB_MYCPP_LIBRARY}\")"<<std::endl;
		  toolkitclientdbConfig<<"ENDIF (NOT OCTETOS_TOOLKIT_CLIENTDB_MYCPP_FIND_QUIETLY)"<<std::endl;
		toolkitclientdbConfig<<"ELSE (OCTETOS_TOOLKIT_CLIENTDB_MYCPP_FOUND)"<<std::endl;
		  toolkitclientdbConfig<<"IF (OCTETOS_TOOLKIT_CLIENTDB_MYCPP_FIND_REQUIRED)"<<std::endl;
			toolkitclientdbConfig<<"MESSAGE(STATUS \"Looked for toolkit-client libraries named ${OCTETOS_TOOLKIT_CLIENTDB_MYCPP_NAMES}.\")"<<std::endl;
			toolkitclientdbConfig<<"MESSAGE(FATAL_ERROR \"Could NOT find toolkit-client library\")"<<std::endl;
		  toolkitclientdbConfig<<"ENDIF (OCTETOS_TOOLKIT_CLIENTDB_MYCPP_FIND_REQUIRED)"<<std::endl;
		toolkitclientdbConfig<<"ENDIF (OCTETOS_TOOLKIT_CLIENTDB_MYCPP_FOUND)"<<std::endl;

		toolkitclientdbConfig<<"MARK_AS_ADVANCED("<<std::endl;
		  toolkitclientdbConfig<<"OCTETOS_TOOLKIT_CLIENTDB_MYCPP_LIBRARY"<<std::endl;
		  toolkitclientdbConfig<<"OCTETOS_TOOLKIT_CLIENTDB_MYCPP_INCLUDE_DIR"<<std::endl;
		  toolkitclientdbConfig<<")"<<std::endl;
		toolkitclientdbConfig.close();
                }                
                else
                {
                        std::cerr << "No hay soporte para este lenguaje en cmake" << std::endl;
                        return false;
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
			projectH = configureProject.builDirectory + ".hpp";
			projectCPP = configureProject.builDirectory + ".cpp";
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
		getHeaderOutput()<< "#include <string>" <<std::endl;
                
		//inlcudes in source file
                getSourceOutput()<< "#include \"" <<getHeaderName() <<"\""<<std::endl<<std::endl;
                getSourceOutput()<< "#include <mysql/mysql.h>"<<std::endl;
		getHeaderOutput()<< "#include <clientdb-mysql.hpp>"<<std::endl<<std::endl;
			
		
		//writing code				
		createH(getHeaderOutput(),log,getSymbolsTable());  
		//createCPP(getSourceOutput(),log,analyzer.getListTableConst()); 
          
        return true;    
    }
    
}
}
}
