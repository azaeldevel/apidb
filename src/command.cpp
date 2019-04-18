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

#include "apidb.hpp"

#include <iostream>
#include <list>
#include <regex>
#include <string>
#include <cstdlib>

int main(int argc, char *argv[])
{
	if(argc > 1)
	{
		if(strcmp(argv[0],"-v") || strcmp(argv[0],"--version"))
		{
			std::cout<<"Version: " << apidb::getPakageVersion().toString()<<std::endl;
			return EXIT_SUCCESS;
		}
		else
		{
			std::cout<<"Opción desconocida "<<std::endl;
			return EXIT_FAILURE;
		}
	}

	std::cout<<"Nombre de projecto:";
	std::string name;
	std::cin>>name;
		
	std::cout<<"Directorio de proyecto:";
	std::string dir;
	std::cin>>dir;
	
	
	//verificar si exist un projecto en el directorio.
	std::string strProject;	
	if(dir.compare(".") == 0 | dir.empty())
	{
		strProject = "apidb";
	}
	else
	{
		strProject = dir + "/" +"apidb";
	}
	std::ifstream fin(strProject);
	if(fin) 
	{
		std::cout<<"Cargando '" << strProject << "' ..." <<std::endl;
		apidb::ConfigureProject config(strProject);
        	apidb::Driver driver(config);
		if(!driver.driving())
		{
			std::cerr<<"Fallo la configuracion."<<std::endl;
			return EXIT_FAILURE;
		}
		else
		{
			std::cout<<"'"<<strProject<<"' cargado."<<std::endl;
		}
		return EXIT_SUCCESS;
	}
	
	std::cout<<"Tipo de Servidor de DB:";
	std::string db;
	std::cin>>db;
	std::regex dbOption("[M|m][Y|y][S|s][Q|q][L|l]");
	if(!regex_match(db,dbOption))
	{
		std::cout<<"\tTipo de servidor desconocido."<<std::endl;
	}
	
	
	std::cout<<"Servidor:";
	std::string server;
	std::cin>>server;
	std::regex serverOption("[0-9]{1,3}.[0-9]{1,3}.[0-9]{1,3}.[0-9]{1,3}");
	if(!regex_match(server,serverOption))
	{
		std::cout<<"\tServidor desconocido."<<std::endl;
	}
	
	std::cout<<"Puerto de servidor:";
	int puerto;
	std::cin >> puerto;
	
	
	std::cout<<"Nombre de Base de Datos:";
	std::string dbName;
	std::cin >> dbName;
	
	std::cout<<"Usuario:";
	std::string dbUser;
	std::cin >>  dbUser;
	
	std::cout<<"Conatraseña:";
	std::string dbPW;
	std::cin >>  dbPW;
	
	
	toolkit::clientdb::mysql::Datconnect mysqlConnector(server,puerto,dbName,dbUser,dbPW); 
	toolkit::clientdb::mysql::Connector connector;    
    bool flagServer = connector.connect(&mysqlConnector);
    if(flagServer)
    {
		std::cout<<"\tConexion a servidor completada."<<std::endl;
		connector.close();
	}
	else
	{
		std::cout<<"\tFallo conexion a servidor."<<std::endl;
	}
	
	toolkit::Version version(0,1,0,toolkit::Version::Stage::alpha);
	apidb::ConfigureProject config;
    	config.name = name;
    	config.directory = dir;
    	config.conectordb = &mysqlConnector;
    	config.version = version;
    	config.inputLenguaje = apidb::InputLenguajes::MySQL;
    	config.outputLenguaje = apidb::OutputLenguajes::CPP;
	apidb::Driver driver(config);
	if(!driver.driving())
    {
        std::cerr<<mysqlConnector.toString()<<" - is bat "<<std::endl;
        return -1;
    }  
    
repeatconfigCh:
	std::cout<< "Archivo de configuracion(s/n):";
	std::string configCh;
	std::cin >> configCh;
	if(configCh.compare("N") == 0 | configCh.compare("n") == 0)
	{
		;
	}
	else if(configCh.compare("S") == 0 | configCh.compare("s") == 0)
	{
		std::string fn = dir + "/apidb";
		config.saveConfig();
		std::cout<<"\tArchivo de configuracion:"<<fn<<std::endl;		
	}
	else if(configCh.compare("Q") == 0 | configCh.compare("q") == 0)
	{
		//solo continua con el programam para evitar el bucle		
	}
	else
	{
		std::cout<<"\tIndique S o N:"<<std::endl;
	}
    
	return EXIT_SUCCESS;	
}
