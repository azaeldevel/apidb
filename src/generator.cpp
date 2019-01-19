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
	void CPP::writeSelectsCPP(const apidb::symbols::Table& table, std::ofstream& ofile,const ConfigureProject& config)
    {
        std::string strmsg = "APIDB requiere que la tabla '";
        strmsg += table.name;
        strmsg += "' tenga llave primaria para continuar";
        if(table.key == NULL) throw BuildException(strmsg);
        
        ofile << "\tstd::vector<"  << table.name << "*>* " << table.name << "::select(toolkit::clientdb::connectors::Connector& connector, const std::string& where)"<<std::endl;
        ofile << "\t{" <<std::endl;
        ofile << "\t\tstd::string sqlString = \"SELECT ";
        {
            ofile << table.key->name;
            ofile << " FROM " << table.name << " WHERE \" + where ;"<< std::endl;
            ofile << "\t\tif(connector.query(sqlString))"  << std::endl;
            ofile << "\t\t{" << std::endl;
            ofile << "\t\t\tMYSQL_RES *result = mysql_store_result((MYSQL*)connector.getServerConnector());" << std::endl;
            ofile << "\t\t\tif (result == NULL)"  << std::endl;
            ofile << "\t\t\t{"  << std::endl;
            ofile << "\t\t\t\t//throw toolkit::clientdb::SQLException(\"La descarga de los datos fallo con la consulta '\" + sqlString + \"'\");"<< std::endl;
            ofile << "\t\t\t\t return NULL;"<< std::endl;
            ofile << "\t\t\t}"  << std::endl;
            ofile << "\t\t\tMYSQL_ROW row;"<< std::endl;
            ofile << "\t\t\tstd::vector<"<< table.name << "*>* tmpVc = new std::vector<" << table.name << "*>;" << std::endl;
            ofile << "\t\t\twhile((row = mysql_fetch_row(result))) " << std::endl;
            ofile << "\t\t\t{"<< std::endl;
            ofile << "\t\t\t\t"<< table.name << "* tmp = NULL;" << std::endl;
            auto fl = table.find(table.key->name.c_str());
            if(fl != table.end())
            {
                if((*fl).second->outType.compare("int") == 0)
                {
                    ofile << "\t\t\t\ttmp = new " << table.name << "(std::stoi(row[0]))" << ";" << std::endl ;
                    ofile << "\t\t\t\ttmpVc->push_back(tmp);" << std::endl;
                }
                else if((*fl).second->outType.compare("std::string") == 0)
                {
                    ofile << "\t\t\t\ttmp = new " << table.name << "(row[0])" << ";" << std::endl ;
                    ofile << "\t\t\t\ttmpVc->push_back(tmp);" << std::endl;
                }
                else
                {
                     
                }
                
            }
            else
            {
                std::string strmsg = "No se encontro el campo ";
                strmsg = strmsg + "'" + table.key->name + "' en la tabla '" + table.name + "'";
                throw BuildException(strmsg);
            }
            ofile << "\t\t\t}"<< std::endl;
                ofile << "\t\t\treturn tmpVc;" << std::endl;
            ofile << "\t\t}" << std::endl;
            ofile << "\t\treturn NULL;" << std::endl;
        }
        ofile << "\t}" <<std::endl;
        
        std::vector<apidb::ConfigureProject::Table> tbs = config.selects;
        for( auto tb: tbs)//std::vector<Table>
        {
            if(table.name.compare(tb.getName()) != 0) 
            {
                continue;//buscar la configuracion de la tabla correspondiente
            }
            for (auto const& [key, val] : tb)//class Table : public std::map<std::string,Function>
            {
                ofile << "\tstd::vector<" << table.name<< "*>* " << table.name << "::select(toolkit::clientdb::connectors::Connector& connector,";
                
                for(auto func : *val)//class Function : public std::vector<const Parameters*>
                {
                    apidb::ConfigureProject::Parameters::const_iterator itParamEnd = func->end();
                    itParamEnd--;
                    for(const char* param : *func)
                    {
                        auto fl = table.find(param);
                        if(fl != table.end())
                        {
                            ofile << (*fl).second->outType << " ";                            
                        }
                        ofile << param; 
                        if(param != *itParamEnd)
                        {
                            ofile << ",";
                        }
                    }
                }
                ofile << ")"<<std::endl;
                ofile << "\t{"<<std::endl;
                ofile << "\t\tstd::string sqlString = \"SELECT ";
                for(auto func : *val)//class Function : public std::vector<const Parameters*>
                {
                    apidb::ConfigureProject::Parameters::const_iterator itParamEnd = func->end();
                    --itParamEnd;
                    ofile << table.key->name;
                    ofile << " FROM " << table.name << " WHERE \";"<< std::endl;
                    for(const char* param : *func)
                    {
                        auto fl = table.find(param);
                        if(fl != table.end())
                        {
                            if((*fl).second->outType.compare("int") == 0)
                            {
                                ofile << "\t\tsqlString = sqlString + \"" << param << " = \" + \"'\" + std::stoi(" << (*fl).second->name << ") + \"'\"";
                            }
                            else if((*fl).second->outType.compare("std::string") == 0)
                            {
                                ofile << "\t\tsqlString = sqlString + \"" << param << " = \" + \"'\" + " << (*fl).second->name << " + \"'\"";
                            }
                            else
                            {
                                std::string strmsg = "No se encontro el campo ";
                                strmsg = strmsg + "'" + (*fl).second->name + "' en la tabla '" + table.name + "'";
                                throw BuildException(strmsg);                                
                            }
                        }
                        else
                        {
                                std::string strmsg = "No se encontro el campo ";
                                strmsg = strmsg + "'" + param + "' en la tabla '" + table.name + "'";
                                throw BuildException(strmsg);
                        }
                        if(param != *itParamEnd)
                        {
                            ofile << " + \",\";"<< std::endl;
                        }
                    }
                    ofile << ";" << std::endl;
                    ofile << "\t\tif(connector.query(sqlString))"  << std::endl;
                    ofile << "\t\t{" << std::endl;
                    ofile << "\t\t\tMYSQL_RES *result = mysql_store_result((MYSQL*)connector.getServerConnector());" << std::endl;
                    ofile << "\t\t\tif (result == NULL)"  << std::endl;
                    ofile << "\t\t\t{"  << std::endl;
                    ofile << "\t\t\t\t//throw toolkit::clientdb::SQLException(\"La descarga de los datos fallo con la consulta '\" + sqlString + \"'\");"<< std::endl;
                    ofile << "\t\t\t\treturn NULL;"<< std::endl;
                    ofile << "\t\t\t}"  << std::endl;
                    ofile << "\t\t\tstd::vector<"<< table.name << "*>* tmpVc = new std::vector<" << table.name << "*>;" << std::endl;
                    ofile << "\t\t\tMYSQL_ROW row;"<< std::endl;
                    ofile << "\t\t\twhile((row = mysql_fetch_row(result))) "<< std::endl;
                    ofile << "\t\t\t{"<< std::endl;     
                    ofile << "\t\t\t\t"<< table.name << "* tmp = NULL;" << std::endl;               
                    //for(const char* param : *func)
                    {
                        //ofile << param; 
                        //if(param != *itParamEnd)
                        {
                            //ofile << "\t\t\t\t\tthis->" << param << " = (row[i] ? row[i] : NULL);"<< std::endl;
                            //ofile << "\t\t\t\tthis->" << table.key->name << " = ";
                            auto fl = table.find(table.key->name.c_str());
                            if(fl != table.end())
                            {
                                if((*fl).second->outType.compare("int") == 0)
                                {
                                    ofile << "\t\t\t\ttmp = new " << table.name << "(std::stoi(row[0]));" << std::endl ;
                                    ofile << "\t\t\t\ttmpVc->push_back(tmp);" << std::endl;
                                }
                                else if((*fl).second->outType.compare("std::string") == 0)
                                {
                                    ofile << "\t\t\t\ttmp = new " << table.name << "(row[0])" << ";" << std::endl ;
                                    ofile << "\t\t\t\ttmpVc->push_back(tmp);" << std::endl;
                                }
                                else
                                {
                                    
                                }
                            }
                            else
                            {
                                std::string strmsg = "No se encontro el campo ";
                                strmsg = strmsg + "'" + table.key->name + "' en la tabla '" + table.name + "'";
                                throw BuildException(strmsg);
                            }
                        }
                    }
                    ofile << "\t\t\t}"<< std::endl;
                    ofile << "\t\t\treturn tmpVc;"<< std::endl;
                    ofile << "\t\t}" << std::endl;
                    ofile << "\t\telse" << std::endl;
                    ofile << "\t\t{" << std::endl;
                    ofile << "\t\t\treturn NULL;" << std::endl;
                    ofile << "\t\t}" << std::endl;
                }
                ofile << "\t}"<<std::endl;
                
            }
        }
    }
	void CPP::writeSelectsH(const apidb::symbols::Table& table, std::ofstream& ofile,const ConfigureProject& config)
    {        
        std::string strmsg = "APIDB requiere que la tabla '";
        strmsg += table.name;
        strmsg += "' tenga llave primaria para continuar";
        if(table.key == NULL) throw BuildException(strmsg);
        
        ofile << "\t\tstatic std::vector<" << table.name << "*>* select(toolkit::clientdb::connectors::Connector& connector,const std::string& where);"<<std::endl;
        
        std::vector<apidb::ConfigureProject::Table> tbs = config.selects;
        for( auto tb: tbs)//std::vector<Table>
        {
            if(table.name.compare(tb.getName()) != 0) 
            {
                continue;//buscar la configuracion de la tabla correspondiente
            }
            for (auto const& [key, val] : tb)//class Table : public std::map<std::string,Function>
            {
                ofile << "\t\tstatic std::vector<" << table.name << "*>* select(toolkit::clientdb::connectors::Connector& connector,";
                
                for(auto func : *val)//class Function : public std::vector<const Parameters*>
                {
                    apidb::ConfigureProject::Parameters::const_iterator itParamEnd = func->end();
                    itParamEnd--;
                    for(const char* param : *func)
                    {
                        auto fl = table.find(param);
                        if(fl != table.end())
                        {
                            ofile << (*fl).second->outType << " ";                            
                        }
                        ofile << param; 
                        if(param != *itParamEnd)
                        {
                            ofile << ",";
                        }
                    }
                }
                ofile << ");"<<std::endl;
                
            }
        }
    }
	void CPP::writeDownloadsCPP(const apidb::symbols::Table& table, std::ofstream& ofile,const ConfigureProject& config)
    {
        std::string strmsg = "APIDB requiere que la tabla '";
        strmsg += table.name;
        strmsg += "' tenga llave primaria para continuar";
        if(table.key == NULL) throw BuildException(strmsg);
                
        std::vector<apidb::ConfigureProject::Table> tbs = config.downloads;
        for( auto tb: tbs)//std::vector<Table>
        {
            if(table.name.compare(tb.getName()) != 0) 
            {                
                continue;//buscar la configuracion de la tabla correspondiente
            }
            
            for (auto const& [key, val] : tb)//class Table : public std::map<const char*, const Function*>
            {
                ofile << "\tbool " << table.name << "::download_" << key << "(toolkit::clientdb::connectors::Connector& connector)"<<std::endl;
                ofile << "\t{ " << std::endl;
                ofile << "\t\tstd::string sqlString = \"SELECT ";
                for(auto func : *val)//class Function : public std::vector<const Parameters*>
                {
                    apidb::ConfigureProject::Parameters::const_iterator itParamEnd = func->end();
                    itParamEnd--;
                    for(const char* param : *func)
                    {
                        ofile << param; 
                        if(param != *itParamEnd)
                        {
                            ofile << ",";
                        }
                    }                    
                    ofile << " FROM " << table.name << " WHERE " << table.key->name << " = '\" + get" << table.key->upperName << "String() + \"'\";" << std::endl;
                    ofile << "\t\tif(connector.query(sqlString))"  << std::endl;
                    ofile << "\t\t{" << std::endl;
                    ofile << "\t\t\tMYSQL_RES *result = mysql_store_result((MYSQL*)connector.getServerConnector());" << std::endl;
                    ofile << "\t\t\tif (result == NULL)"  << std::endl;
                    ofile << "\t\t\t{"  << std::endl;
                    ofile << "\t\t\t\treturn false;"  << std::endl;
                    ofile << "\t\t\t}"  << std::endl;
                    //ofile << "\t\t\tint num_fields = mysql_num_fields(result);"<< std::endl;
                    ofile << "\t\t\tMYSQL_ROW row;"<< std::endl;
                    ofile << "\t\t\twhile ((row = mysql_fetch_row(result))) "<< std::endl;
                    ofile << "\t\t\t{"<< std::endl;
                    //ofile << "\t\t\t\tfor(int i = 0; i < num_fields; i++)"<< std::endl;
                    ofile << "\t\t\t\t{"<< std::endl;
                    itParamEnd = func->end();
                    int countparam = 0;
                    for(const char* param : *func)
                    {
                        //ofile << param; 
                        if(param != *itParamEnd)
                        {
                            //ofile << "\t\t\t\t\tthis->" << param << " = (row[i] ? row[i] : NULL);"<< std::endl;
                            ofile << "\t\t\t\t\tthis->" << param << " = ";
                            auto fl = table.find(param);
                            if(fl != table.end())
                            {
                                if((*fl).second->classReferenced != NULL)
                                {
                                    ofile << " new " << (*fl).second->classReferenced->name << "(row[" << countparam << "])" << ";" << std::endl ;
                                }
                                else if((*fl).second->outType.compare("int") == 0)
                                {
                                    ofile << " std::stoi(row[" << countparam << "] ? row[" << countparam << "] : 0)" << ";"<< std::endl ;
                                }
                                else if((*fl).second->outType.compare("long") == 0)
                                {
                                    ofile << " std::stol(row[" << countparam << "] ? row[" << countparam << "] : 0)" << ";"<< std::endl ;
                                }
                                else if((*fl).second->outType.compare("std::string") == 0 || (*fl).second->outType.compare("const char*") == 0)
                                {
                                    ofile << " row[" << countparam << "] ? row[" << countparam << "] : \"NULL\"" << ";" << std::endl ;
                                }
                                else
                                {
                                    ofile << " row[" << countparam << "] ? row[" << countparam << "] : \"NULL\"" << ";" << std::endl ;
                                }
                            }
                            else
                            {
                                std::string strmsg = "No se encontro el campo ";
                                strmsg = strmsg + "'" + param + "' en la tabla '" + table.name + "'";
                                throw BuildException(strmsg);
                            }
                        }
                        countparam++;
                    }
                    //ofile << "\t\t\t;"<< std::endl;
                    ofile << "\t\t\t\t}"<< std::endl;
                    ofile << "\t\t\t}"<< std::endl;;
                    ofile << "\t\t\tmysql_free_result(result);" << std::endl;
                    ofile << "\t\t\treturn true;" << std::endl;
                    ofile << "\t\t}" << std::endl;
                    ofile << "\t\telse" << std::endl;
                    ofile << "\t\t{" << std::endl;
                    ofile << "\t\t\treturn false;" << std::endl;
                    ofile << "\t\t}" << std::endl;
                }
                ofile << "\t} " << std::endl;
            }         
        }   
        
    }
    void CPP::writeDownloadsH(const apidb::symbols::Table& table, std::ofstream& ofile,const ConfigureProject& config)
    {
        std::string strmsg = "APIDB requiere que la tabla '";
        strmsg += table.name;
        strmsg += "' tenga llave primaria para continuar";
        if(table.key == NULL) throw BuildException(strmsg);
                
        std::vector<apidb::ConfigureProject::Table> tbs = config.downloads;
        for( auto tb: tbs)//std::vector<Table>
        {
            //std::cout<<"Iterate on '" << tb.getName() << "'" << std::endl;
            if(table.name.compare(tb.getName()) != 0) 
            {          
                continue;//buscar la configuracion de la tabla correspondiente
            }            
            for (auto const& [key, val] : tb)//class Table : public std::map<std::string,Function>
            {
                ofile << "\t\tbool download_" << key << "(toolkit::clientdb::connectors::Connector& connector);"<<std::endl;
            }         
        }
    }
	
    /*void CPP::writeSelectH(const apidb::symbols::Table& table,std::ofstream& ofile)
    {
        std::string strmsg = "APIDB requiere que la tabla '";
        strmsg += table.name;
        strmsg += "' tenga llave primaria para continuar";
        if(table.key == NULL) throw BuildException(strmsg);
        
        if(table.key->outType.compare("int") == 0 || table.key->outType.compare("std::string") == 0 || table.key->outType.compare("const char*") == 0)
        {
            ofile << "\t\tvoid select(" << table.key->outType << ");"<< std::endl;
        }
        else
        {
            strmsg += " y la llave debe ser de tipo entero y no de tipo '" + table.key->outType + "'";
            throw BuildException(strmsg);
        }
    }*/
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
	void CPP::writeInsertH(const apidb::symbols::Table& table,std::ofstream& ofile)
	{
		int countFIelds = 0;
		// creando insert
        ofile << "\t\t"<< "bool ";
        ofile << "insert(toolkit::clientdb::connectors::Connector& connector";
        for(std::list<symbols::Symbol*>::const_iterator i = table.required.begin(); i != table.required.end(); i++)
        {
			if((*i)->keyType == symbols::Symbol::KeyType::PRIMARY) continue;
			countFIelds++;
			if(i != table.required.end())
			{
				ofile << " ,"; //se agrega la coma si hay un segundo parametro
			}
				
			//
			if((*i)->classReferenced == NULL)
			{
				ofile << (*i)->outType << " ";
			}
			else
			{
				ofile << "const " << (*i)->classReferenced->name << "& ";
				//ofile << "const " << (*i)->outType <<"& ";
			}
			ofile << (*i)->name;
		}
        ofile << ");"<<std::endl;
		if(countFIelds == 0) throw BuildException(table.name + " no tiene campo requerido por lo que no se puede generar metodo insert."); 
	}

	void CPP::writeInsertCPP(const apidb::symbols::Table& table,std::ofstream& ofile)	
	{
        std::string strmsg = "APIDB requiere que la tabla '" ;
        strmsg += table.name;
        strmsg += "' tenga llave para continuar con el proceso.";
        if(table.key == NULL) throw BuildException(strmsg);
        
		// Methodo insert
        ofile << "\t"<< "bool ";
        ofile <<table.name<< "::insert(toolkit::clientdb::connectors::Connector& connector";
        for(std::list<symbols::Symbol*>::const_iterator i = table.required.begin(); i != table.required.end(); i++)
        {
            if((*i)->keyType == symbols::Symbol::KeyType::PRIMARY) continue;

            if(i != table.required.end())
            {
                    ofile << ","; //se agrega la coma si hay un segundo parametro
            }
                    
                    //
            if((*i)->classReferenced == NULL)
            {
                    ofile << (*i)->outType << " ";
            }
            else
            {
                    ofile << "const " << (*i)->classReferenced->name << "& ";
                    //ofile << "const " << (*i)->outType <<"& ";
            }
            ofile << (*i)->name;
        }
        ofile << ")"<<std::endl;
        ofile << "\t{"<<std::endl;
        ofile << "\t\t"<<"std::string sqlString = \"\";"<<std::endl;
        ofile << "\t\t"<<"sqlString = sqlString + \"INSERT INTO \" + TABLE_NAME; "<<std::endl;
        ofile << "\t\t"<<"sqlString = sqlString + \"(";
        for(std::list<symbols::Symbol*>::const_iterator i = table.required.begin(); i != table.required.end(); ++i)
        {
            if((*i)->keyType == symbols::Symbol::KeyType::PRIMARY) continue;

			ofile << (*i)->name;
            auto penultimo = table.required.end();
            penultimo--;					
			if(i != penultimo)
			{
				ofile << ","; //se agrega la coma si hay un segundo parametro
			}			
		}
		ofile << ")\";" << std::endl;
		ofile << "\t\tsqlString = sqlString + \" VALUES(\"";
        auto end = table.required.end();
        for(std::list<symbols::Symbol*>::const_iterator i = table.required.begin(); i != end; ++i)
        {
			if((*i)->keyType == symbols::Symbol::KeyType::PRIMARY) continue;
		
            if((*i)->classReferenced != NULL)
            {
                ofile << " + \"'\" + " << (*i)->name << ".get" << (*i)->classReferenced->key->upperName << "String()) + \"'\" ";
            }
            else
            {
                ofile << " + \"'\" + " << (*i)->name << " + \"'\" ";
            }
            auto penultimo = table.required.end();
            penultimo--;
			if(i != penultimo)
			{
				ofile << " + \",\" ";
			}		
		}
		ofile << " + \")\";"<< std::endl;
		//asegurar que tiene key
		if(table.key != NULL)
		{
			ofile << "\t\tthis->"  << table.key->name;
			if((table.key->outType.compare("int") == 0))
			{
				if(table.key->classReferenced == NULL)//si es foreing key
				{
					ofile << " = connector.insert(sqlString);"<< std::endl;		
					ofile << "\t\tif(this->" << table.key->name << " > 0) return true;"<< std::endl;
					ofile << "\t\telse return false;"<< std::endl;				
				}
				else
				{
					ofile << " = new " << table.key->classReferenced->name << "((int)connector.insert(sqlString));"<< std::endl;
					ofile << "\t\tif(this->" << table.key->name << " != NULL) return true;"<< std::endl;
					ofile << "\t\telse return false;"<< std::endl;
				}
			}
			else
			{
				ofile << " = " << table.key->classReferenced->name << "(connector.insert(sqlString));"<< std::endl;
				ofile << "\t\tif(this->" << table.key->name << " != NULL) return true;"<< std::endl;
				ofile << "\t\telse return false;"<< std::endl;
			}			
		}
		else //no tiene key
		{
			throw BuildException("La tabla no tiene llave que es necesaria para el constructor de la clase in writeInsertCPP");
		}
        	ofile << "\t}"<<std::endl;
	}
        
	void CPP::writeKeyContructorH(const apidb::symbols::Table& table,std::ofstream& ofile)
	{
        	//constructor que toma key como parametro
        	if(table.key != NULL)//tiene key
        	{
			if(table.key->outType.compare("int") == 0)
			{
				ofile << "\t\t" <<table.name;
				if(table.key->classReferenced == NULL)
				{
					ofile << "(int id);"<<std::endl;
				}
				else
				{
					ofile << "(const " << table.key->classReferenced->name<< "& obj);"<<std::endl;
				}
			}
			else if(table.key->outType.compare("std::string") == 0)
			{
				ofile << "\t\t" <<table.name;
				ofile << "(std::string id);"<<std::endl;
			}
			else
			{
				throw BuildException("El tipo de dato correspondiente a la llave es inmanejable para este esquema este esquema'" + table.key->outType + "'");
			}
		}
		else
		{
			std::string msg = "La tabla no tiene llave que es necesaria para el constructor de la clase in writeKeyContructorH ";
			msg = msg + table.name;
			throw BuildException(msg);
		}
	}
	void CPP::writeKeyContructorCPP(const apidb::symbols::Table& table ,std::ofstream& ofile)
	{
		//constructor que toma key como parametro
        if(table.key != NULL)//tiene key
        {
			if(table.key->outType.compare("int") == 0)
			{
				ofile << "\t" <<table.name << "::" << table.name;
				if(table.key->classReferenced == NULL)
				{
					ofile << "(int id)"<<std::endl;
				}
				else
				{
					ofile << "(const " << table.key->classReferenced->name << "& obj)"<<std::endl;
				}
			}
			else if(table.key->outType.compare("std::string") == 0)
			{
				ofile << "(std::string id)"<<std::endl;
			}
			else
			{
				throw BuildException("EL tipo de dato correspondiente a la llave es inmanejable para este esquema este esquema");
			}
		}
		else
		{
			throw BuildException("La tabla no tiene llave que es necesaria para el constructor de la clase in writeKeyContructorCPP" );
		}
		ofile << "\t{" <<std::endl;
		if(table.key->outType.compare("int") == 0)
		{
			if(table.key->classReferenced == NULL)
			{
					ofile << "\t\tthis->" << table.key->name << "=id;" <<std::endl;
			}
			else
			{
					ofile << "\t\tthis->" << table.key->name<< " = new " << table.key->classReferenced->name << "(obj);"<<std::endl;
			}
		}
		else
		{
				throw BuildException("EL tipo de dato correspondiente a la llave es inmanejable para este esquema este esquema");
		}
		ofile << "\t}" <<std::endl;
	}
        
	void CPP::writeCopyContructorH(const apidb::symbols::Table& table,std::ofstream& ofile)
	{
		//constructor de copias 
		ofile << "\t\t" <<table.name<<"(const " << table.name <<"&);"<<std::endl;
	}
	void CPP::writeCopyContructorCPP(const apidb::symbols::Table& table,std::ofstream& ofile)
	{
		//constructor de copias 
		ofile << "\t" << table.name << "::" << table.name <<"(const " << table.name <<"& obj)"<<std::endl;
		ofile << "\t{"<<std::endl;
        for (auto const& [key, attr] : table)
		{
			ofile << "\t\tthis->"<< attr->name << " = obj." << attr->name<<";"<<std::endl;
		}
		ofile << "\t}"<<std::endl;
				
	}
	void CPP::writeKeyValueH(const apidb::symbols::Table& table ,std::ofstream& ofile)
	{
        //si la table tiene key
        if(table.key != NULL) 
        {
			ofile << "\t\tstd::string toStringKey()const;" <<std::endl;			
		}
	}
	void CPP::writeKeyValueCPP(const apidb::symbols::Table& table,std::ofstream& ofile)
	{
        if(table.key != NULL)
        {
			ofile << "\tstd::string " <<table.name << "::toStringKey()const" <<std::endl;	
			ofile << "\t{" << std::endl;
			if((table.key->outType.compare("int") == 0))
			{
				if(table.key->classReferenced == NULL)//si es foreing key
				{
					ofile <<"\t\treturn std::to_string(" << table.key->name;
					ofile <<");"<<std::endl;						
				}
				else
				{
					ofile <<"\t\treturn std::to_string(" << table.key->name;
					symbols::Symbol* actual = table.key;
					do
					{
						ofile << "->" << actual->classReferenced->key->get;
						actual = actual->classReferenced->key;						
					}
					while((actual->outType.compare("int") == 0) && (actual->classReferenced != NULL));
					ofile <<");"<<std::endl;
				}
			}
			else if((table.key->outType.compare("std::string") == 0))
			{
				ofile <<"\t\treturn " << table.key->name << ";" <<std::endl;
			}
			else
			{
				ofile <<"\t\treturn std::to_string(" << table.key->name <<");"<<std::endl;
			}
			ofile << "\t}" << std::endl;
		}
	}
        
	void CPP::writeDefaultContructorH(const apidb::symbols::Table& table,std::ofstream& ofile)
	{
		ofile <<"\t\t"<<table.name<<"();"<<std::endl;
	}
	void CPP::writeDefaultContructorCPP(const apidb::symbols::Table& table,std::ofstream& ofile)
    {
		ofile <<"\t"<<table.name<< "::" <<table.name<<"()"<<std::endl;
		ofile <<"\t{"<<std::endl;
		ofile <<"\t}"<<std::endl;
	}
	void CPP::createClassMethodesCPP(const apidb::symbols::Table& table,std::ofstream& ofile,const ConfigureProject& config)
	{
        for (auto const& [key, attr] : table)
        {            
			//gets
			if((attr->outType.compare("char") == 0) | (attr->outType.compare("short") == 0) | (attr->outType.compare("int") == 0) | (attr->outType.compare("long") == 0) | (attr->outType.compare("float") == 0) | (attr->outType.compare("double") == 0))
			{
				if(attr->classReferenced == NULL)//si es foreing key
				{
					ofile <<"\t"<< attr->outType << " ";						
				}
				else
				{
					ofile <<"\t"<< "const " << attr->classReferenced->name << "& ";
				}
			}
			else
			{
				ofile <<"\t" << "const " << attr->outType <<"& ";
			}
				
			ofile << table.name <<"::" << attr->get << " const"<< std::endl;
			ofile << "\t{"<<std::endl;	
			if((attr->outType.compare("char") == 0) | (attr->outType.compare("short") == 0) | (attr->outType.compare("int") == 0) | (attr->outType.compare("long") == 0) | (attr->outType.compare("float") == 0) | (attr->outType.compare("double") == 0))
			{
				if(attr->classReferenced == NULL)//si es foreing key
				{
					ofile <<"\t\treturn "<< attr->name<<";"<< std::endl;						
				}
				else
				{
					ofile <<"\t\treturn *"<< attr->name <<";"<< std::endl;
				}						
			}
			else
			{
				ofile <<"\t\treturn " << attr->name <<";"<< std::endl;
			}								
			ofile << "\t}"<<std::endl;
			
			
			/*if(attr->keyType == symbols::Symbol::KeyType::PRIMARY || attr->keyType == symbols::Symbol::KeyType::FOREIGN_UNIQUE)
			{
				continue;
			}*/	
			
			//getString()		
			ofile << "\tstd::string "<< table.name <<"::get" << attr->upperName << "String() const "<< std::endl;
			ofile << "\t{"<< std::endl;
            
            ofile << "\t\treturn ";
            if(table.key == NULL)
            {
                    std::string strmsg = "APIDB neceista que la tabla '" + table.name + "' tenga llave primaria para poder contunuar.";
                    throw BuildException(strmsg);
            }
            if(attr->classReferenced != NULL)
            {
                ofile <<"get" << table.key->upperName << "String();";   
            }
            else
            {
                if(attr->outType.compare("std::string") == 0 || attr->outType.compare("const char*") == 0)
                {
                    ofile << attr->name << ";";                    
                }
                else
                {
                    ofile <<"std::to_string(" << attr->name << ");";
                } 
            }
            ofile << std::endl;			
			ofile << "\t}"<< std::endl;
            
			//updates
			ofile << "\tbool " << table.name <<"::update" << attr->upperName << "(toolkit::clientdb::connectors::Connector& connector,";
			if((attr->outType.compare("int") == 0) | (attr->outType.compare("long") == 0))
			{
				if(attr->classReferenced == NULL)//si es foreing key
				{
					ofile << attr->outType << " " << attr->name;						
				}
				else
				{
					ofile <<"const "<< attr->classReferenced->name <<"& " << attr->name;
				}						
			}
			else
			{
				ofile << attr->outType << " " << attr->name;
			}
			ofile <<")"<< std::endl;
			ofile << "\t{"<<std::endl;
			ofile << "\t\tstd::string sqlString = \"\";"<<std::endl;
			ofile << "\t\tsqlString = \"UPDATE \" + TABLE_NAME;"<<std::endl;
			ofile << "\t\tsqlString = sqlString + \" SET " << attr->name << " = \" " ;
			if(attr->classReferenced != NULL)
            {
                if((attr->outType.compare("int") == 0) | (attr->outType.compare("long") == 0))
                {
                    ofile << " + \"'\" + " << attr->name << ".get" << attr->classReferenced->key->upperName <<"String() + \"'\";"<<std::endl;
                }
                else if((attr->outType.compare("std::string") == 0 | attr->outType.compare("const char*") == 0))
                {
                    ofile << " \"'\" + " << attr->name << ".get" << attr->classReferenced->key->upperName <<"String() + \"'\";"<<std::endl;	
                }
                else
                {
                    ofile <<"  \"'\" + " << attr->name << ".get" << attr->classReferenced->key->upperName <<"String() + \"'\";"<<std::endl;		
                }
            }
            else if((attr->outType.compare("int") == 0) | (attr->outType.compare("long") == 0))
			{
				ofile << " + \"'\" + std::to_string(" << attr->name <<") + \"'\";"<<std::endl;
			}
			else if((attr->outType.compare("std::string") == 0 | attr->outType.compare("const char*") == 0))
			{
				ofile << " \"'\" + " << attr->name <<" + \"'\";"<<std::endl;	
			}
			else
			{
				ofile <<"  \"'\" +  std::to_string(" << attr->name <<") + \"'\";"<<std::endl;		
			}
			ofile <<"\t\tsqlString = sqlString + \" WHERE " << table.key->name << "  = \" ";			
			if((table.key->outType.compare("int") == 0) | (table.key->outType.compare("long") == 0))
			{
				if(table.key->classReferenced == NULL)//si no es foreing key
				{
					ofile << " + \"'\" + this->get" << table.key->upperName <<"String() + \"'\";"<<std::endl;
				}
				else
				{
					ofile << " + \"'\" + this->get" << table.key->upperName <<"String() + \"'\";"<<std::endl;	
				}
			}
			else
			{
				
			}
			
			ofile <<"\t\treturn connector.query(sqlString);"<<std::endl;
			ofile << "\t}"<<std::endl;						
        }
		
		writeKeyContructorCPP(table,ofile);
		writeCopyContructorCPP(table,ofile);
		writeDefaultContructorCPP(table,ofile);
			
		writeInsertCPP(table,ofile);		
		writeKeyValueCPP(table,ofile);
        writeDownloadsCPP(table,ofile,config);
		//writeSelectCPP(table,ofile);
		writeSelectsCPP(table,ofile,config);
		ofile << std::endl; 
    }
    
    void CPP::createSpaceCPP(std::ofstream& file,const ConfigureProject& config)
    {
		if(configureProject.mvc == apidb::MVC::NO)
		{
			file <<"namespace "<< analyzer.getNameProject() <<std::endl;
			file <<"{"<<std::endl;
		}
		else 
		{
			file <<"namespace "<< analyzer.getNameProject() <<std::endl;
			file <<"{" <<std::endl;
			file <<"namespace  controller" <<std::endl;
			file <<"{" <<std::endl;
		}
        	const symbols::Tables& tables = analyzer.getListTable();
        	for (apidb::symbols::Table* table : tables) 
        	{
            		createClassCPP(*table,file,table->name,config);       
        	}
		if(configureProject.mvc == apidb::MVC::NO)
		{
			file <<"}"<<std::endl;
		}
		else 
		{
			file <<"}" <<std::endl;
			file <<"}" <<std::endl;
		}
    }
	void CPP::createClassCPP(const apidb::symbols::Table& cl,std::ofstream& file,const std::string& nameClass,const ConfigureProject& config)
    {
		file << "\tconst std::string " <<  nameClass << "::TABLE_NAME = \""<<  nameClass << "\";" << std::endl;
		createClassMethodesCPP(cl,file,config);        
		file<< std::endl<< std::endl;
    }
    
	void CPP::createClassMethodesH(const apidb::symbols::Table& table,std::ofstream& ofile,const ConfigureProject& config)
    {
		std::string insertMethode = "";
        for (auto const& [key, attr] : table)
        {
			//get
			if((attr->outType.compare("char") == 0) | (attr->outType.compare("short") == 0) | (attr->outType.compare("int") == 0) | (attr->outType.compare("long") == 0) | (attr->outType.compare("float") == 0) | (attr->outType.compare("double") == 0))
			{
				if(attr->classReferenced == NULL)//si es foreing key
				{
					ofile <<"\t\t"<< attr->outType << " ";						
				}
				else
				{
					ofile <<"\t\t"<< "const " << attr->classReferenced->name << "& ";
				}
			}
			else
			{
				ofile <<"\t\t" << "const " << attr->outType <<"& ";
			}		
			ofile << attr->get << " const;"<< std::endl;
			
			
			/*if(attr->keyType == symbols::Symbol::KeyType::PRIMARY)
			{
				continue;
			}*/
			//getString()			
			ofile << "\t\tstd::string get" << attr->upperName << "String() const;"<< std::endl;		
			//update
			ofile << "\t\tbool " << "update" << attr->upperName << "(toolkit::clientdb::connectors::Connector& connector,";
			if((attr->outType.compare("int") == 0) | (attr->outType.compare("long") == 0))
			{
				if(attr->classReferenced == NULL)//si es foreing key
				{
					ofile << attr->outType << " " << attr->name;						
				}
				else
				{
					ofile <<"const "<< attr->classReferenced->name <<"& ";
				}						
			}
			else
			{
				ofile << attr->outType << " " << attr->name;
			}
			ofile << ");"<< std::endl;					
        }  
                 
		writeKeyContructorH(table,ofile);		
		writeCopyContructorH(table,ofile);
		writeDefaultContructorH(table,ofile);
		ofile << std::endl; 	
		writeInsertH(table,ofile);	
		writeKeyValueH(table,ofile);
		ofile << std::endl; 
        //writeSelectH(table,ofile);		
        writeSelectsH(table,ofile,config);
        writeDownloadsH(table,ofile,config);
        ofile << std::endl;
    }
    
    void CPP::createClassAttributesH(const apidb::symbols::Table& table,std::ofstream& ofile)
    {
        for (auto const& [key, attr] : table)
        {
			//ofile <<"["<<attr->outType<<"]"<<std::endl;
			if(configureProject.outputLenguaje == OutputLenguajes::CPP)
			{
				if((attr->outType.compare("char") == 0) | (attr->outType.compare("short") == 0) | (attr->outType.compare("int") == 0) | (attr->outType.compare("long") == 0) | (attr->outType.compare("float") == 0) | (attr->outType.compare("double") == 0))
				{
					if(attr->classReferenced == NULL)//si es foreing key
					{
						//ofile <<"[1]"<<std::endl;
						ofile <<"\t\t"<< attr->outType << " "<< attr->name<<";"<< std::endl;						
					}
					else
					{
						//ofile <<"[2]"<<std::endl;
						ofile <<"\t\t"<< attr->classReferenced->name << "* "<< attr->name<<";"<< std::endl;
					}
				}
				else
				{
					//ofile <<"[3]"<<std::endl;
					ofile <<"\t\t" << attr->outType <<" "<< attr->name <<";"<< std::endl;
				}
			}
			else
			{
				analyzer.getErrorMessage()<<"OutputLenguaje is unknow.";
			}             
        }        
    }
    
    void CPP::createSpaceH(std::ofstream& file,const ConfigureProject& config)
    {
		if(configureProject.mvc == apidb::MVC::NO)
		{
			file <<"namespace "<< analyzer.getNameProject() <<std::endl;
			file <<"{"<<std::endl;
		}
		else if(configureProject.mvc != apidb::MVC::NO)
		{
			file <<"namespace "<< analyzer.getNameProject() <<std::endl;
			file <<"{" <<std::endl;
			file <<"namespace  controller" <<std::endl;
			file <<"{" <<std::endl;
		}
        	const symbols::Tables& tables = analyzer.getListTable();
        	for (const apidb::symbols::Table* table : tables) 
        	{
			if(table->getCountRefereces() > 0) file << "\tclass " << table->name << ";"<<std::endl;
		}
		file<<std::endl;
        	for (const apidb::symbols::Table* table : tables) 
        	{
			//file <<"Declare Table " << table->name << std::endl;
            		createClassH(*table,file,table->name,config);       
        	}

		if(configureProject.mvc == apidb::MVC::NO)
		{
			file <<"}"<<std::endl;
		}
		else  if(configureProject.mvc != apidb::MVC::NO)
		{
			file <<"}" <<std::endl;
			file <<"}" <<std::endl;
		}
    }
    
    void CPP::createClassPublicH(std::ofstream& file)
    {
        file << "\tpublic:" <<std::endl;
    }
    
    void CPP::createClassPrivateH(std::ofstream& file)
    {
        file << "\tprivate:" <<std::endl;
    }
    void CPP::createClassH(const apidb::symbols::Table& cl,std::ofstream& file,const std::string& nameClass,const ConfigureProject& config)
    {
		//file <<"keyword"<<std::endl;
		analyzer.getOutputMessage() <<"\tHeading class " << cl.name<<std::endl;
        file <<"\tclass "<<nameClass<<std::endl;        
        file <<"\t{"<<std::endl;
        //file <<"private"<<std::endl;
        createClassPrivateH(file);
        file << "\t\tstatic const std::string TABLE_NAME;" <<std::endl;
        //file <<"atributes"<<std::endl;
        createClassAttributesH(cl,file);
        createClassPublicH(file);
        //file <<"methodes"<<std::endl;
        createClassMethodesH(cl,file,config);
        file <<"\t};"<<std::endl;
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
		createSpaceH(getHeaderOutput(),configureProject);  
		createSpaceCPP(getSourceOutput(),configureProject); 
          
        return true;    
    }
    

	
}
}
