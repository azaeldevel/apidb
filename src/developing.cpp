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


int main()
{
	toolkit::clientdb::DatconectionMySQL mysqlConnector("192.168.0.101",3306,"business.alpha","develop","123456"); 
	toolkit::Version version;
	version.set(0,1,0,toolkit::Version::Stage::alpha); 

	apidb::ConfigureProject config;
    config.name = "nmp";
    config.directory = "nmp";
    config.conectordb = mysqlConnector;
    config.version = version;
    config.inputLenguaje = apidb::InputLenguajes::MySQL_Server;
    config.outputLenguaje = apidb::OutputLenguajes::CPP;	
    apidb::Driver driver(config);	
	if(!driver.driving())
	{
		std::cerr<<mysqlConnector.toString()<<" - es incorrecta."<<std::endl;
        return EXIT_FAILURE;
	}
	
	//std::cout<<"----"<<std::endl;
	
    if(!config.saveConfig())
    {
        std::cerr<<"Fail creation prject."<<std::endl;
        return EXIT_FAILURE;		
	}
		
	apidb::ConfigureProject config2("nmp/apidb");
    config2.directory = "nmp2";
    apidb::Driver driver2(config2);
    if(!driver2.driving())
    {
        std::cerr<<config2.getConector().toString()<<" - is bat "<<std::endl;
        return EXIT_FAILURE;
    }
	if(!config2.saveConfig())
    {
        std::cerr<<"Fail creation prject."<<std::endl;
        return EXIT_FAILURE;		
	}
    
    
	return EXIT_SUCCESS;	
}
