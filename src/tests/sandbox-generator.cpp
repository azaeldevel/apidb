
#include <octetos/db/clientdb-maria.hh>
#include <time.h>

#include "../apidb.hpp"
#include "../common.hpp"

double testTimeProc()
{
    clock_t begin = clock();

    /* here, do your time-consuming job */

    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    
    return time_spent;
}
int main(int argc, char **argv)
{

    octetos::db::mariadb::Datconnect mariaSource("localhost",3306,"muposys-0.1-alpha","muposys","123456");
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
    
    
    octetos::apidb::ConfigureProject configProject_nls;
    configProject_nls.name = "muposys";
    configProject_nls.builDirectory = "muposys-nlst";
	configProject_nls.setInputs(octetos::apidb::InputLenguajes::MariaDB,mariaSource);    
	octetos::core::Semver version;
	version.setNumbers(0,1,0);
    version.setPrerelease("alpha");    
    configProject_nls.versionResult = version;
    configProject_nls.outputLenguaje = octetos::apidb::OutputLenguajes::CPP;	
    configProject_nls.packing = octetos::apidb::PackingLenguajes::CMake;
    configProject_nls.compiled = octetos::apidb::Compiled::STATIC;
	configProject_nls.writeDatconnect = "conector";
    
    octetos::apidb::Driver driver(configProject_nls);

    octetos::apidb::Tracer tracer(0);
    //std::cout << "testBuild: Step 3\n";
    if(!driver.driving(&tracer))
    {
        
    }
    
    return EXIT_SUCCESS;
}
