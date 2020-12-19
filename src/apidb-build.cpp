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

int main(int argc, char *argv[])
{
    std::string copyright = "APIDB  Copyright (C) 2018  Azael Reyes \nThis program comes with ABSOLUTELY NO WARRANTY.\n    Contacto: azael.devel@gmail.com";
        
    const char* file = NULL;
    const char* dir = NULL;
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
            }
            else
            {
                std::cerr<<"Deve especificar la ruta del archivo.\n";
                return EXIT_FAILURE;
            }
        }
        else if(strcmp(argv[i],"-o") == 0 || strcmp(argv[i],"--out-build") == 0)
        {
            dir = argv[i]; 
        }
        else
        {
            std::cerr<<"Opcion desconocida '" << argv[i] << "'.\n";
        }
	}
	
	if(file == NULL)
    {
        std::cerr<<"Indique el archivo de projecto mediente la opcion '-p'.\n";
        return EXIT_FAILURE;
    }
    
		std::cout<<"Cargando '" << file << "' ..." <<std::endl;
		octetos::apidb::ConfigureProject config;
		try
		{
			config.readConfig(file);
		}
        catch(const std::exception& e)
        {
			std::cerr<<"Fallo la lectura del archivo."<< e.what() <<std::endl;
			return EXIT_FAILURE;                        
        }
        if(dir != NULL)
        {
            config.builDirectory = dir;
            std::cout<<"Generando en '" << config.builDirectory << "' ..." <<std::endl;
        }
        //verificar si exite el archivo de projecto
        FILE *tmpfile = fopen(file, "r");
        if (tmpfile) 
        {
            fclose(tmpfile);
        } 
        else 
        {
            std::cerr<<"No existe el archivo : " << file  <<std::endl;
			return EXIT_FAILURE;
        } 
    octetos::apidb::Driver driver(config);
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
    if(retDriver)
    {
        std::cerr<<"Fallo desconocido.";
        return EXIT_FAILURE;
    }
	
	return EXIT_SUCCESS;	
}
