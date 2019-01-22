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
 
#include <list>
#include <vector>
#include <iostream>
#include <fstream>
#include <map>


#include "analyzer.hpp"
#include "generator.hpp"


namespace apidb
{
namespace generators
{ 
	Generator::Generator(const ConfigureProject& config) : configureProject(config)
	{
		
	}
	
	CMake::CMake(apidb::Analyzer& d,const ConfigureProject& config): analyzer(d),configureProject(config), apidb::generators::Generator(config)
	{
	}
	
	CMake::~CMake()
	{
		
	}
	
	bool CMake::generate()
	{
		std::string namefile = "CMakeLists.txt";
		if((analyzer.getDirectoryProject().empty()) | (analyzer.getDirectoryProject().compare(".") == 0))
		{
			cmakelists.open(namefile);
		}
		else
		{
			cmakelists.open(analyzer.getDirectoryProject() + "/" + namefile);
		}
			
		//CMakeLists.txt
		analyzer.getOutputMessage() << "Generando archivos de gestor de projecto... " << std::endl;
		analyzer.getOutputMessage() << "\tTipo de Gestor: " << getOutputLenguajeString() << std::endl;
		
		cmakelists<<"CMAKE_MINIMUM_REQUIRED(VERSION ";
		cmakelists<<"3";
		cmakelists<<".";
		cmakelists<<"0";
		cmakelists<<")"<<std::endl;
		
		cmakelists<<"PROJECT(";
		cmakelists<<configureProject.getName();
		cmakelists<<" VERSION ";
		cmakelists<<configureProject.getVersion().getMajor();
		cmakelists<<".";
		cmakelists<<configureProject.getVersion().getMinor();
		cmakelists<<".";
		cmakelists<<configureProject.getVersion().getPatch();
		cmakelists<<".";
		cmakelists<<"0 ";
		if(analyzer.getOutputLenguaje() == apidb::OutputLenguajes::CPP)
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
		cmakelists<<std::endl;
		cmakelists<<"SET(" << configureProject.name << "_VERSION_STAGE \"alpha\")"<<std::endl;
		cmakelists<<"CONFIGURE_FILE(\"${PROJECT_SOURCE_DIR}/config.h.in\" \"${PROJECT_SOURCE_DIR}/config.h\")"<<std::endl;
		cmakelists<<std::endl;
		cmakelists<<"FIND_PACKAGE(MySQL REQUIRED PATHS ${PROJECT_SOURCE_DIR}/cmake.modules/)"<<std::endl;
		cmakelists<<"IF(MySQL_FOUND)"<<std::endl;
			cmakelists<<"INCLUDE_DIRECTORIES(${MYSQL_INCLUDE_DIR})"<<std::endl;
		cmakelists<<"ENDIF()"<<std::endl;
		cmakelists<<"FIND_PACKAGE(toolkit-common-c++ REQUIRED PATHS ${PROJECT_SOURCE_DIR}/cmake.modules/)"<<std::endl;
		cmakelists<<"IF(TOOLKIT_COMMON_FOUND)"<<std::endl;
			cmakelists<<"INCLUDE_DIRECTORIES(${TOOLKIT_COMMON_INCLUDE_DIR})"<<std::endl;
		cmakelists<<"ENDIF()"<<std::endl;
		cmakelists<<"FIND_PACKAGE(toolkit-clientdb-c++ REQUIRED PATHS ${PROJECT_SOURCE_DIR}/cmake.modules/)"<<std::endl;
		cmakelists<<"IF(TOOLKIT_CLIENTDB_FOUND)"<<std::endl;
			cmakelists<<"INCLUDE_DIRECTORIES(${TOOLKIT_CLIENTDB_INCLUDE_DIR})"<<std::endl;
		cmakelists<<"ENDIF()"<<std::endl;
		cmakelists<<std::endl;
		cmakelists<<"ADD_EXECUTABLE(developing "<< configureProject.name <<".cpp developing.cpp)"<<std::endl;
		cmakelists<<"TARGET_LINK_LIBRARIES(developing ${TOOLKIT_CLIENTDB_LIBRARY} ${TOOLKIT_COMMON_LIBRARY} ${MYSQL_LIBRARY})"<<std::endl;
		cmakelists<<"ADD_LIBRARY("<< configureProject.name <<" STATIC "<< configureProject.name <<".cpp )"<<std::endl;
		cmakelists<<"TARGET_LINK_LIBRARIES("<< configureProject.name <<" ${MYSQL_LIBRARY} ${TOOLKIT_COMMON_LIBRARY}  ${TOOLKIT_CLIENDB_LIBRARY} )"<<std::endl;
		cmakelists.close();
		analyzer.getOutputMessage()<<"\tArchivo de gestion de projecto: " << namefile <<std::endl;
		
		//std::cout<<"Creating cmake.modules..."<<std::endl;
		//cmake.modules
		if((analyzer.getDirectoryProject().empty()) | (analyzer.getDirectoryProject().compare(".") == 0))
		{
			std::ifstream ifile("cmake.modules");
			if (!ifile) 
			{
				system("mkdir cmake.modules");
			}
		}
		else
		{
			std::string direct = analyzer.getDirectoryProject() + "/cmake.modules";
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
		namefile = "toolkit-common-c++Config.cmake";
		if((analyzer.getDirectoryProject().empty()) | (analyzer.getDirectoryProject().compare(".") == 0))
		{
			toolkitcommonconifg.open(namefile);
		}
		else
		{
			toolkitcommonconifg.open(analyzer.getDirectoryProject() + "/cmake.modules/" + namefile);
		}
		
		toolkitcommonconifg<<"IF (TOOLKIT_COMMON_INCLUDE_DIR)"<<std::endl;
		  toolkitcommonconifg<<"SET(TOOLKIT_COMMON_FIND_QUIETLY TRUE)"<<std::endl;
		toolkitcommonconifg<<"ENDIF (TOOLKIT_COMMON_INCLUDE_DIR)"<<std::endl;

		toolkitcommonconifg<<"FIND_PATH(TOOLKIT_COMMON_INCLUDE_DIR common.hpp"<<std::endl;
		  toolkitcommonconifg<<"/usr/local/include/toolkit"<<std::endl;
		  toolkitcommonconifg<<"/usr/include/toolkit"<<std::endl;
		toolkitcommonconifg<<")"<<std::endl;

		toolkitcommonconifg<<"SET(TOOLKIT_COMMON_NAMES toolkit-common-c++)"<<std::endl;
		toolkitcommonconifg<<"FIND_LIBRARY(TOOLKIT_COMMON_LIBRARY"<<std::endl;
		  toolkitcommonconifg<<"NAMES ${TOOLKIT_COMMON_NAMES}"<<std::endl;
		  toolkitcommonconifg<<"PATHS /usr/lib/toolkit /usr/local/lib/toolkit /usr/lib/x86_64-linux-gnu/toolkit"<<std::endl;
		  toolkitcommonconifg<<"PATH_SUFFIXES toolkit-common-c++"<<std::endl;
		toolkitcommonconifg<<")"<<std::endl;

		toolkitcommonconifg<<"IF (TOOLKIT_COMMON_INCLUDE_DIR AND TOOLKIT_COMMON_LIBRARY)"<<std::endl;
		  toolkitcommonconifg<<"SET(TOOLKIT_COMMON_FOUND TRUE)"<<std::endl;
		  toolkitcommonconifg<<"SET( TOOLKIT_COMMON_LIBRARIES ${TOOLKIT_COMMON_LIBRARY} )"<<std::endl;
		toolkitcommonconifg<<"ELSE (TOOLKIT_COMMON_INCLUDE_DIR AND TOOLKIT_COMMON_LIBRARY)"<<std::endl;
		  toolkitcommonconifg<<"SET(TOOLKIT_COMMON_FOUND FALSE)"<<std::endl;
		  toolkitcommonconifg<<"SET( TOOLKIT_COMMON_LIBRARIES )"<<std::endl;
		toolkitcommonconifg<<"ENDIF (TOOLKIT_COMMON_INCLUDE_DIR AND TOOLKIT_COMMON_LIBRARY)"<<std::endl;

		toolkitcommonconifg<<"IF (TOOLKIT_COMMON_FOUND)"<<std::endl;
		  toolkitcommonconifg<<"IF (NOT TOOLKIT_COMMON_FIND_QUIETLY)"<<std::endl;
			toolkitcommonconifg<<"MESSAGE(STATUS \"Found toolkit-common: ${TOOLKIT_COMMON_LIBRARY}\")"<<std::endl;
		  toolkitcommonconifg<<"ENDIF (NOT TOOLKIT_COMMON_FIND_QUIETLY)"<<std::endl;
		toolkitcommonconifg<<"ELSE (TOOLKIT_COMMON_FOUND)"<<std::endl;
		  toolkitcommonconifg<<"IF (TOOLKIT_COMMON_FIND_REQUIRED)"<<std::endl;
			toolkitcommonconifg<<"MESSAGE(STATUS \"Looked for toolkit-common libraries named ${TOOLKIT_COMMON_NAMES}.\")"<<std::endl;
			toolkitcommonconifg<<"MESSAGE(FATAL_ERROR \"Could NOT find toolkit-common library\")"<<std::endl;
		  toolkitcommonconifg<<"ENDIF (TOOLKIT_COMMON_FIND_REQUIRED)"<<std::endl;
		toolkitcommonconifg<<"ENDIF (TOOLKIT_COMMON_FOUND)"<<std::endl;

		toolkitcommonconifg<<"MARK_AS_ADVANCED("<<std::endl;
		  toolkitcommonconifg<<"TOOLKIT_COMMON_LIBRARY"<<std::endl;
		  toolkitcommonconifg<<"TOOLKIT_COMMON_INCLUDE_DIR"<<std::endl;
		  toolkitcommonconifg<<")"<<std::endl;
		toolkitcommonconifg.close();
		
		//std::cout<<"Creating toolkit-clientdbConfig.cmake..."<<std::endl;
		namefile = "toolkit-clientdb-c++Config.cmake";
		if((analyzer.getDirectoryProject().empty()) | (analyzer.getDirectoryProject().compare(".") == 0))
		{
			toolkitclientdbConfig.open(namefile);
		}
		else
		{
			toolkitclientdbConfig.open(analyzer.getDirectoryProject() + "/cmake.modules/" + namefile);
		}		
		
		toolkitclientdbConfig<<"IF (TOOLKIT_CLIENTDB_INCLUDE_DIR)"<<std::endl;
		  toolkitclientdbConfig<<"SET(TOOLKIT_CLIENTDB_FIND_QUIETLY TRUE)"<<std::endl;
		toolkitclientdbConfig<<"ENDIF (TOOLKIT_CLIENTDB_INCLUDE_DIR)"<<std::endl;

		toolkitclientdbConfig<<"FIND_PATH(TOOLKIT_CLIENTDB_INCLUDE_DIR clientdb.hpp"<<std::endl;
		  toolkitclientdbConfig<<"/usr/local/include/toolkit"<<std::endl;
		  toolkitclientdbConfig<<"/usr/include/toolkit"<<std::endl;
		toolkitclientdbConfig<<")"<<std::endl;

		toolkitclientdbConfig<<"SET(TOOLKIT_CLIENTDB_NAMES toolkit-clientdb-c++)"<<std::endl;
		toolkitclientdbConfig<<"FIND_LIBRARY(TOOLKIT_CLIENTDB_LIBRARY"<<std::endl;
		  toolkitclientdbConfig<<"NAMES ${TOOLKIT_CLIENTDB_NAMES}"<<std::endl;
		  toolkitclientdbConfig<<"PATHS /usr/lib/toolkit /usr/local/lib/toolkit /usr/lib/x86_64-linux-gnu/toolkit"<<std::endl;
		  toolkitclientdbConfig<<"PATH_SUFFIXES toolkit-clientdb-c++"<<std::endl;
		toolkitclientdbConfig<<")"<<std::endl;

		toolkitclientdbConfig<<"IF (TOOLKIT_CLIENTDB_INCLUDE_DIR AND TOOLKIT_CLIENTDB_LIBRARY)"<<std::endl;
		  toolkitclientdbConfig<<"SET(TOOLKIT_CLIENTDB_FOUND TRUE)"<<std::endl;
		  toolkitclientdbConfig<<"SET( TOOLKIT_CLIENTDB_LIBRARIES ${TOOLKIT_CLIENTDB_LIBRARY} )"<<std::endl;
		toolkitclientdbConfig<<"ELSE (TOOLKIT_CLIENTDB_INCLUDE_DIR AND TOOLKIT_CLIENTDB_LIBRARY)"<<std::endl;
		  toolkitclientdbConfig<<"SET(TOOLKIT_CLIENTDB_FOUND FALSE)"<<std::endl;
		  toolkitclientdbConfig<<"SET( TOOLKIT_CLIENTDB_LIBRARIES )"<<std::endl;
		toolkitclientdbConfig<<"ENDIF (TOOLKIT_CLIENTDB_INCLUDE_DIR AND TOOLKIT_CLIENTDB_LIBRARY)"<<std::endl;

		toolkitclientdbConfig<<"IF (TOOLKIT_CLIENTDB_FOUND)"<<std::endl;
		  toolkitclientdbConfig<<"IF (NOT TOOLKIT_CLIENTDB_FIND_QUIETLY)"<<std::endl;
			toolkitclientdbConfig<<"MESSAGE(STATUS \"Found toolkit-cliendb: ${TOOLKIT_CLIENTDB_LIBRARY}\")"<<std::endl;
		  toolkitclientdbConfig<<"ENDIF (NOT TOOLKIT_CLIENTDB_FIND_QUIETLY)"<<std::endl;
		toolkitclientdbConfig<<"ELSE (TOOLKIT_CLIENTDB_FOUND)"<<std::endl;
		  toolkitclientdbConfig<<"IF (TOOLKIT_CLIENTDB_FIND_REQUIRED)"<<std::endl;
			toolkitclientdbConfig<<"MESSAGE(STATUS \"Looked for toolkit-client libraries named ${TOOLKIT_CLIENTDB_NAMES}.\")"<<std::endl;
			toolkitclientdbConfig<<"MESSAGE(FATAL_ERROR \"Could NOT find toolkit-client library\")"<<std::endl;
		  toolkitclientdbConfig<<"ENDIF (TOOLKIT_CLIENTDB_FIND_REQUIRED)"<<std::endl;
		toolkitclientdbConfig<<"ENDIF (TOOLKIT_CLIENTDB_FOUND)"<<std::endl;

		toolkitclientdbConfig<<"MARK_AS_ADVANCED("<<std::endl;
		  toolkitclientdbConfig<<"TOOLKIT_CLIENTDB_LIBRARY"<<std::endl;
		  toolkitclientdbConfig<<"TOOLKIT_CLIENTDB_INCLUDE_DIR"<<std::endl;
		  toolkitclientdbConfig<<")"<<std::endl;
		toolkitclientdbConfig.close();
		
		//std::cout<<"Creating MySQLConfig.cmake..."<<std::endl;
		namefile = "MySQLConfig.cmake";
		if((analyzer.getDirectoryProject().empty()) | (analyzer.getDirectoryProject().compare(".") == 0))
		{
			toolkitcommonconifg.open(namefile);
		}
		else
		{
			toolkitcommonconifg.open(analyzer.getDirectoryProject() + "/cmake.modules/" + namefile);
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
		if((analyzer.getDirectoryProject().empty()) | (analyzer.getDirectoryProject().compare(".") == 0))
		{
			config.open(namefile);
		}
		else
		{
			config.open(analyzer.getDirectoryProject() + "/" + namefile);
		}
		config<<"#define VERSION_MAJOR @apidb_VERSION_MAJOR@"<<std::endl;
		config<<"#define VERSION_MINOR @apidb_VERSION_MINOR@"<<std::endl;
		config<<"#define VERSION_PATCH @apidb_VERSION_PATCH@"<<std::endl;
		config<<"#define VERSION_STAGE toolkit::Version::Stage::@apidb_VERSION_STAGE@"<<std::endl;
		config<<"#define PAKAGENAME \"@PROJECT_NAME@\""<<std::endl;
		config.close();		
		//analyzer->getOutputMessage()<<"\tArchivo de configuración de projecto: " << namefile <<std::endl;
		
		//std::cout<<"Creating developing.cpp..."<<std::endl;
		namefile = "developing.cpp";
        bool preexits;
        if (FILE *file = fopen(namefile.c_str(), "r")) 
        {
            fclose(file);
            preexits = true;
        }
        else
        {
            preexits = false;
        }
		if((analyzer.getDirectoryProject().empty()) | (analyzer.getDirectoryProject().compare(".") == 0))
		{
			if(preexits) developing.open(namefile);
		}
		else
		{
			if(preexits) developing.open(analyzer.getDirectoryProject() + "/" + namefile);
		}
        if(preexits)
        {
            developing<<"#include \"" << configureProject.name << ".hpp\""<<std::endl;
            developing<<std::endl;
            developing<<"#include <iostream>"<<std::endl;
            developing<<"#include <list>"<<std::endl;
            developing<<std::endl;
            developing<<"int main()"<<std::endl;
            developing<<"{"<<std::endl;
            developing<<"return 0;"<<std::endl;
            developing<<"}"<<std::endl;
        }
		//analyzer->getOutputMessage()<<"\tArchivo de develping phase: " << namefile <<std::endl;
		//std::cout<<"return..."<<std::endl;
		return true;
	}

	std::string Generator::getOutputLenguajeString()const
	{
		switch(configureProject.outputLenguaje)
		{
			case OutputLenguajes::CMAKE:		
				return "CMake";
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
	
	CPP::CPP(apidb::Analyzer& d,const ConfigureProject& config) : analyzer(d), apidb::generators::Generator(config)
	{
		//outputLenguaje = d.getOutputLenguaje();
		writeResults = new std::ofstream[2];
		if((d.getDirectoryProject().empty()) | (d.getDirectoryProject().compare(".") == 0)) 
		{
			projectH = d.getNameProject() + ".hpp";
			writeResults[0].open(projectH);
			projectCPP = d.getNameProject() + ".cpp";
			writeResults[1].open(projectCPP);
		}
		else
		{
			projectH = d.getNameProject() + ".hpp";
			projectCPP = d.getNameProject() + ".cpp";
			writeResults[0].open(d.getDirectoryProject() + "/" + projectH);
			writeResults[1].open(d.getDirectoryProject() + "/" + projectCPP);
		}
	}    
	bool CPP::generate()
	{
		analyzer.getOutputMessage() << "Generando archivos de codigo fuente... " << std::endl;
		analyzer.getOutputMessage() << "\tLenguaje resultado: " << getOutputLenguajeString() << std::endl;
		//includes in header file
		std::string headers = "";
		bool stringFlag = false;
		const apidb::symbols::Tables& tables = analyzer.getListTable();
		for(symbols::Table* table: tables)
		{
            for (auto const& [key, attr] : *table)
			{
				if(attr->outType.compare("std::string")==0 && stringFlag == false)
				{
					getHeaderOutput()<< "#include <string>" <<std::endl;
					stringFlag = true;
				}
			}
		}
			
			
		//inlcudes in source file
        getSourceOutput()<< "#include \"" <<getHeaderName() <<"\""<<std::endl<<std::endl; 
        getSourceOutput()<< "#include <mysql/my_global.h>"<<std::endl;
        getSourceOutput()<< "#include <mysql/mysql.h>"<<std::endl;
		getHeaderOutput()<< "#include <clientdb.hpp>"<<std::endl<<std::endl;
			
		//writing code				
		createSpaceH(getHeaderOutput());  
		createSpaceCPP(getSourceOutput()); 
          
        return true;    
    }
    

	
}
}
