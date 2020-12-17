
#include <octetos/db/clientdb-maria.hh>


#include "../apidb.hpp"
#include "../common.hpp"

int main(int argc, char **argv)
{
    std::cout << "Step 1\n";
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
    std::cout << "Step 2\n";
    
    
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
    
    std::cout << "Step 3\n";
    octetos::apidb::Driver driver(configProject_nls);

    std::cout << "Step 4\n";
    octetos::apidb::Tracer tracer(0);
    std::cout << "Step 5\n";
    bool ret = false;
    try
    {
        ret = driver.driving(&tracer);
    }
    catch(const std::exception& ex)
    {
        std::cerr << "Error : " << ex.what() << "\n";
    }
    if(ret)
    {
        
    }
    std::cout << "Step 6\n";
    
    return EXIT_SUCCESS;
}
