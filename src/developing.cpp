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


int main()
{
	octetos::toolkit::clientdb::mysql::Datconnect mysqlSource("192.168.0.101",3306,"sysapp.alpha","develop","123456"); 
	octetos::toolkit::Version version;
	version.setNumbers(0,1,0);
        version.setStage(octetos::toolkit::Version::Stage::alpha);
        
	octetos::apidb::ConfigureProject config;
        config.name = "sysapp";
        config.directory = "apidb";
        config.conectordb = &mysqlSource;
        config.version = version;
        config.inputLenguaje = octetos::apidb::InputLenguajes::MySQL;
        config.outputLenguaje = octetos::apidb::OutputLenguajes::CPP;	
        config.packing = octetos::apidb::PackingLenguajes::CMake;
        config.compiled = octetos::apidb::Compiled::STATIC;
	config.mvc = octetos::apidb::MVC::NO;
        //config.keyMode = apidb::KeyModel::BY_MODEL_DB;
        octetos::apidb::ConfigureProject::Table tbP("Persons");
        octetos::apidb::ConfigureProject::Function dwFullName("fullname",octetos::apidb::ConfigureProject::Function::DOWNLOAD);
        octetos::apidb::ConfigureProject::Parameters params_FullName;
        params_FullName.push_back("name1");
        params_FullName.push_back("name2");
        params_FullName.push_back("name3");
        params_FullName.push_back("name4");
        dwFullName.setHeader(&params_FullName);
        tbP.insert(std::make_pair(dwFullName.getName().c_str(), &dwFullName));
        octetos::apidb::ConfigureProject::Function dwShortName("shortname",octetos::apidb::ConfigureProject::Function::DOWNLOAD);
        octetos::apidb::ConfigureProject::Parameters params_ShortName;
        params_ShortName.push_back("name1");
        params_ShortName.push_back("name3");
        dwShortName.setHeader(&params_ShortName);
        tbP.insert(std::make_pair(dwShortName.getName().c_str(), &dwShortName));
        config.downloads.push_back(tbP);
        config.selects.push_back(tbP);
        octetos::apidb::ConfigureProject::Table tbUsers("Users");
        octetos::apidb::ConfigureProject::Function byUsername("byUsername",octetos::apidb::ConfigureProject::Function::SELECT);    
        octetos::apidb::ConfigureProject::Parameters params_byPerson;
        params_byPerson.push_back("username");
        params_byPerson.push_back("person");
        byUsername.setHeader(&params_byPerson);
        tbUsers.insert(std::make_pair(byUsername.getName().c_str(), &byUsername));
        config.selects.push_back(tbUsers);
    
        octetos::apidb::Driver driver(config);	
	if(!driver.driving())
	{
		std::cerr<<mysqlSource.toString()<<" - es incorrecta."<<std::endl;
                return EXIT_FAILURE;
	}	
        if(!config.saveConfig())
        {
		std::cerr<<"Fail on create project."<<std::endl;
		return EXIT_FAILURE;		
	}
	
	
        //std::cout<<"Configuracion previa: " << config.getConector().toString()<<std::endl;
	//std::cout<<"Version previa: " << config.getVersion().toString()<<std::endl;
	/*apidb::ConfigureProject config2("apidb/apidb");
    	config2.directory = "nmp";
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
	std::cout<<"Configuracion posterior: " << config2.getConector().toString()<<std::endl;
	std::cout<<"Version posterior: " << config2.getVersion().toString()<<std::endl;       
	*/
        
	return EXIT_SUCCESS;	
}
