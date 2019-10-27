
#include <iostream>
#include <stdio.h>
#include <string.h>

#include "../apidb.hpp"
#include "../Errors.hpp"
#include <toolkit/common/common.hpp>
#include "../common.hpp"

int main(int argc, char **argv)
{
    std::string user,host,database,password;
    int port;
    
    std::cout << "Host: \n";
    std::cin >> host;
    std::cout << "Usuario: \n";
    std::cin >> user;
    std::cout << "Contraseña: \n";
    std::cin >> password;
    std::cout << "Puerto: \n";
    std::cin >> port;
    std::cout << "Base de datos: \n";
    std::cin >> database;
    
    octetos::toolkit::clientdb::mysql::Datconnect datConect(host,port,database,user,password);     
	octetos::toolkit::Version version;
	version.setNumbers(0,1,0);
	version.setStage(octetos::toolkit::Version::Stage::alpha);
	octetos::apidb::ConfigureProject configProject;
	configProject.name = "sysapp";
	configProject.builDirectory  = "apidb";
	configProject.conectordb = &datConect;//esta variable esta dispobli hasta que se genera el proyecto
	configProject.versionResult = version;
	configProject.inputLenguaje = octetos::apidb::InputLenguajes::MySQL;
	configProject.outputLenguaje = octetos::apidb::OutputLenguajes::CPP;	
	configProject.packing = octetos::apidb::PackingLenguajes::CMake;
	configProject.compiled = octetos::apidb::Compiled::STATIC;    
	configProject.executable_target  = "capture_v3";
	configProject.namespace_detect = "emulate";
	configProject.writeDatconnect = "datConect";
    if(configProject.saveConfig("sysapp"))
	{
        std::cout << "Creacion de proyecto: completada.";
        return EXIT_SUCCESS;
	}
	else
	{
        std::cout << "Creacion de proyecto: fallò.";
        return EXIT_FAILURE;
	}	
}
