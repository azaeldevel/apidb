
#include <octetos/db/clientdb-maria.hh>


#include "../apidb.hpp"
#include "../common.hpp"

int main(int argc, char **argv)
{
    //std::cout << "Step 1\n";
    octetos::db::maria::Datconnect mariaSource("localhost",3306,"muposys-0.1-alpha","muposys","123456");
    octetos::db::maria::Connector connector; 
    bool flag = false;  
	flag = connector.connect(mariaSource);
    if(flag)
    {
        //printf("SQL Server version: %s\n", connector.getVerionServer().toString().c_str());
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
    configProject.outputLenguaje = octetos::apidb::OutputLenguajes::JAVA;
    configProject.packing = octetos::apidb::PackingLenguajes::OnlyCode;
    configProject.compiled = octetos::apidb::Compiled::STATIC;
	configProject.writeDatconnect = "conector";
    configProject.saveConfig("muposys");
    /*
    octetos::apidb::ConfigureProject configProject;
    configProject.readConfig("/home/azael/develop/octetos/muposys/C++/apidb/muposys.apidb");
    configProject.builDirectory = "muposys-apidb";
    configProject.packing = octetos::apidb::PackingLenguajes::CMake;
    */
    
    //std::cout << "Step 3\n";
    octetos::apidb::Driver driver(configProject);

    //std::cout << "Step 4\n";
    octetos::apidb::Tracer tracer(0);
    //std::cout << "Step 5\n";
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
    //std::cout << "Step 6\n";
    
    return EXIT_SUCCESS;
}
