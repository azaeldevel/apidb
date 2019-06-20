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

	apidb::ConfigureProject config;
        config.name = "sysapp";
        config.directory = "apidb";
        config.conectordb = &mysqlSource;
        config.version = version;
        config.inputLenguaje = apidb::InputLenguajes::MySQL;
        config.outputLenguaje = apidb::OutputLenguajes::CPP;	
        config.packing = apidb::PackingLenguajes::CMake;
        config.compiled = apidb::Compiled::SHARED;
	config.mvc = apidb::MVC::NO;
        //config.keyMode = apidb::KeyModel::BY_MODEL_DB;
        apidb::ConfigureProject::Table tbP("Persons");
        apidb::ConfigureProject::Function dwFullName("fullname",apidb::ConfigureProject::Function::DOWNLOAD);
        apidb::ConfigureProject::Parameters params_FullName;
        params_FullName.push_back("name1");
        params_FullName.push_back("name2");
        params_FullName.push_back("name3");
        params_FullName.push_back("name4");
        dwFullName.setHeader(&params_FullName);
        tbP.insert(std::make_pair(dwFullName.getName().c_str(), &dwFullName));
        apidb::ConfigureProject::Function dwShortName("shortname",apidb::ConfigureProject::Function::DOWNLOAD);
        apidb::ConfigureProject::Parameters params_ShortName;
        params_ShortName.push_back("name1");
        params_ShortName.push_back("name2");
        dwShortName.setHeader(&params_ShortName);
        tbP.insert(std::make_pair(dwShortName.getName().c_str(), &dwShortName));
        config.downloads.push_back(tbP);
        //apidb::ConfigureProject::Table table1("table1");
        //apidb::ConfigureProject::Function byPerson("byPerson",apidb::ConfigureProject::Function::SELECT);    
        // apidb::ConfigureProject::Parameters params_byPerson;
        //params_byPerson.push_back("dataSel");
        //params_byPerson.push_back("person");
        //byPerson.setHeader(&params_byPerson);
        //table1.insert(std::make_pair(byPerson.getName().c_str(), &byPerson));
        config.selects.push_back(tbP);
        //config.selects.push_back(table1);
    
        apidb::Driver driver(config);	
	if(!driver.driving())
	{
		std::cerr<<mysqlSource.toString()<<" - es incorrecta."<<std::endl;
                return EXIT_FAILURE;
	}	
        if(!config.saveConfig())
        {
		std::cerr<<"Fail creation prject."<<std::endl;
		return EXIT_FAILURE;		
	}
	/*
        std::cout<<"Configuracion previa: " << config.getConector().toString()<<std::endl;
	std::cout<<"Version previa: " << config.getVersion().toString()<<std::endl;
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
	std::cout<<"Configuracion posterior: " << config2.getConector().toString()<<std::endl;
	std::cout<<"Version posterior: " << config2.getVersion().toString()<<std::endl;
        */
	
	return EXIT_SUCCESS;	
}
