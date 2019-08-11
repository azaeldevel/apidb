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
        
        const char* file;
        const char* dir;
	if(argc > 0)
	{
		if(strcmp(argv[1],"-v") ==0 || strcmp(argv[1],"--version") == 0)
		{
			std::cout<<"Version: " << octetos::apidb::getPakageVersion().toString()<<std::endl;
                        std::cout<<std::endl;
                        std::cout<< copyright<<std::endl;
                        
			return EXIT_SUCCESS;
		}
		else if((strcmp(argv[1],"-p") ==0 || strcmp(argv[1],"--project-file") == 0) && (strcmp(argv[3],"-o") ==0 || strcmp(argv[3],"--out-build") == 0) )
                {
                        file = argv[2];
                        dir = argv[4];
                }
                else
		{
			std::cerr<<"Opciónes desconocida "<<std::endl;
			return EXIT_FAILURE;
		}
	}
	else
        {
                std::cerr<<"Deve indicar el archivo de prjecto y el directorio de generacion, use las opciones -p 'file' -o 'dir'."<<std::endl;
                return EXIT_FAILURE;
        }
		
	std::ifstream fin(file);
	if(fin) 
	{
		std::cout<<"Cargando '" << file << "' ..." <<std::endl;
		octetos::apidb::ConfigureProject config;
                config.setBuildDirectory(dir);
                if(config.readConfig(file))
                {
			std::cerr<<"Fallo la lectura del archivo."<<std::endl;
			return EXIT_FAILURE;                        
                }
        	octetos::apidb::Driver driver(config);
                octetos::apidb::Tracer tr(0);
		if(!driver.driving(&tr))
		{
			std::cerr<<"Fallo la generacion."<<std::endl;
			return EXIT_FAILURE;
		}
		else
		{
			std::cout<<"Proyecto generado en '"<<dir<<"' exitosamente."<<std::endl;
                        return EXIT_SUCCESS;
		}
	}
	
	return EXIT_SUCCESS;	
}
