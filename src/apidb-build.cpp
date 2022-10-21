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

#include "apidb.hpp"

#include <iostream>
#include <list>
#include <regex>
#include <string>
#include <cstdlib>
#include "common.hpp"


int main(int argc, char *argv[])
{
    std::string copyright = "APIDB  Copyright (C) 2018  Azael Reyes \nThis program comes with ABSOLUTELY NO WARRANTY.\n    Contacto: azael.devel@gmail.com";
        
    const char* file = NULL;
    const char* dir = NULL;
    bool open_project = false, open_create = false;
    octetos::apidb::ConfigureProject* config = new octetos::apidb::ConfigureProject;
    octetos::db::Datconnect* datconn = NULL;
    octetos::db::Connector* conn = NULL;
	for(int i = 1; i < argc; i++)
	{
		if(strcmp(argv[i],"-v") == 0 || strcmp(argv[i],"--version") == 0)
		{
			std::cout<<"Version: " << octetos::apidb::getPakageVersion().toString()<<std::endl;
            std::cout<<std::endl;
            std::cout<< copyright<<std::endl;                        
			return EXIT_SUCCESS;
		}
		else if(strcmp(argv[i],"-p") == 0 || strcmp(argv[i],"--project-file") == 0)
        {
            //std::cout<<"Detectando opcion -p\n";
            if(argc >= i + 1)
            {
                i++;
                //std::cout<<"Detectando valor -p = " << argv[i] << "\n";                
                file = argv[i]; 
                /*FILE *tmpfile = fopen(file, "r");
                if (tmpfile) 
                {
                    fclose(tmpfile);
                } 
                else 
                {
                    std::cerr<<"No existe el archivo : " << file  <<std::endl;
                    return EXIT_FAILURE;
                }*/
                try
                {
                    config->readConfig(file);
                }
                catch(const std::exception& e)
                {
                    std::cerr<<"Fallo la lectura del archivo."<< e.what() <<std::endl;
                    return EXIT_FAILURE;                        
                }
            }
            else
            {
                std::cerr<<"Deve especificar la ruta del archivo.\n";
                return EXIT_FAILURE;
            }
        }
        else if(strcmp(argv[i],"-o") == 0 || strcmp(argv[i],"--out-build") == 0)
        {
            if(argc >= i + 1)
            {
                i++;
                //std::cout<<"Detectando valor -p = " << argv[i] << "\n";                
                dir = argv[i]; 
            }
            else
            {
                std::cerr<<"Deve especificar la ruta del archivo.\n";
                return EXIT_FAILURE;
            }
        }
		else if(strcmp(argv[i],"-c") == 0 || strcmp(argv[i],"--create-project") == 0)
        {           
                open_project = true;
                std::cout << "Captura de parámetros de conexión\n";
                
                std::cout << "\tTipo de Base de Datos (MariaDB,MySQL,):\n";
                std::cout << "\t\t0 : Desconocida\n";
                std::cout << "\t\t1 : MySQL\n";
                std::cout << "\t\t2 : PostgreSQL\n";
                std::cout << "\t\t3 : MariaDB\n";
                std::cout << "\tSelección : ";
                int dbtype = 0;
                std::cin >> dbtype;
                std::cout << "\tHost : ";
                std::string dbhost;
                std::cin >> dbhost;
                std::cout << "\tNombre : ";
                std::string dbname;
                std::cin >> dbname;
                std::cout << "\tPuerto : ";
                int dbport;
                std::cin >> dbport;
                std::cout << "\tUsuario : ";
                std::string dbuser;
                std::cin >> dbuser;
                std::cout << "\tContrseña : ";
                std::string dbpassw;
                std::cin >> dbpassw;
                                
                datconn = octetos::apidb::create_dc((octetos::apidb::InputLenguajes)dbtype);
                datconn->set(dbhost,dbport,dbname,dbuser,dbpassw);
                std::cout << "\tprobando conexion .. ";
                conn = octetos::apidb::create_c((octetos::apidb::InputLenguajes)dbtype);
                if(conn->connect(*datconn)) 
                {
                    std::cout << "completada.\n";
                }
                else
                {
                    std::cout << "fallida\n";
                }
                
                std::cout << "\tNombre del projecto: ";
                std::cin >> config->name;
                
        }
        else
        {
            std::cerr<<"Opcion desconocida '" << argv[i] << "'.\n";
        }
	}
        
    if(dir != NULL)
    {
        config->builDirectory = dir;
        std::cout<<"Generando en '" << config->builDirectory << "' ..." <<std::endl;
    }
    
    octetos::apidb::Driver driver(*config);
    octetos::apidb::Tracer tr(0);
    bool retDriver;
    try
    {
        retDriver = driver.driving(&tr);
    }
    catch(const std::exception& e)
    {
        std::cerr<<"Fallo la lectura del archivo."<< e.what() <<std::endl;
        return EXIT_FAILURE;
    }
    if(not retDriver)
    {
        std::cerr<<"Fallo desconocido.";
        return EXIT_FAILURE;
    }
    delete config;
    if(datconn) delete datconn;
    if(conn) delete conn;
	
	return EXIT_SUCCESS;	
}
