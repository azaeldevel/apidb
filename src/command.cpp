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

int main()
{
	std::cout<<"Servidor de Base de Datos:";
	std::string db;
	std::cin>>db;
	std::regex dbOption("[M|m][Y|y][S|s][Q|q][L|l]");
	if(!regex_match(db,dbOption))
	{
		std::cout<<"\tTipo de servidor desconocido."<<std::endl;
	}
	
	
	std::cout<<"Nombre de servidor:";
	std::string server;
	std::cin>>server;
	std::regex serverOption("[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}");
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
	
	
	
	toolkit::clientdb::DatconectionMySQL mysqlConnector(server,puerto,dbName,dbUser,dbPW); 
	toolkit::clientdb::Connector connector;    
    bool flagServer = connector.connect(mysqlConnector);
    if(flagServer)
    {
		std::cout<<"\tConexion a servidor completada."<<std::endl;
		connector.close();
	}
	else
	{
		std::cout<<"\tFallo conexion a servidor completada."<<std::endl;
	}
	return 0;	
}
