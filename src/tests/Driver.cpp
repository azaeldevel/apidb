
#include "defines.hpp"

#include "../apidb.hpp"
#include "../common.hpp"

int main(int argc, char **argv)
{
    //std::cout << "Step 1\n";
    bool display = false;
    
    for(int i = 1; i < argc; i++)
    {
        if(strcmp("--display",argv[i]) == 0)
        {
            display = true;
        }
        else if(strcmp("--help",argv[i]) == 0)
        {
            std::cout << "--display\t Despiega el log de contruccion de projecto.";
        }
        else
        {
            std::cerr << "Opcion desconocida '" << argv[i] << "\n";
        }
    }
    
    octetos::db::mariadb::Connector connector; 
    bool flag = false;  
	flag = connector.connect(mariaSource);
    if(flag)
    {
        printf("SQL Server version: %s\n", connector.getVerionServer().toString().c_str());
    }
    else
    {
        std::cerr << "Fallo en laconexion \n";
        return EXIT_FAILURE;
    }
    //std::cout << "Step 2\n";
    
    
    octetos::apidb::ConfigureProject configProject;
    configProject.name = "muposys";
    configProject.builDirectory = "muposys-nlst";
	configProject.setInputs(octetos::apidb::InputLenguajes::MariaDB,mariaSource);    
	octetos::core::Semver version;
	version.setNumbers(0,1,0);
    version.setPrerelease("alpha");    
    configProject.versionResult = version;
    configProject.outputLenguaje = octetos::apidb::OutputLenguajes::CPP;	
    configProject.packing = octetos::apidb::PackingLenguajes::CMake;
    configProject.compiled = octetos::apidb::Compiled::STATIC;
	configProject.writeDatconnect = "conector";
    
    //std::cout << "Step 3\n";
    octetos::apidb::Driver driver(configProject);

    //std::cout << "Step 4\n";
    octetos::apidb::Tracer tracer(0);
    //std::cout << "Step 5\n";
    bool ret = false;
    try
    {
        if(display)
        {
            ret = driver.driving(&tracer);
        }
        else
        {
            ret = driver.driving(NULL);
        }        
    }
    catch(const std::exception& ex)
    {
        std::cerr << "Error : " << ex.what() << "\n";
    }
    if(ret)
    {
        
    }
    //std::cout << "Step 6\n";
    
    return EXIT_SUCCESS;
} 
