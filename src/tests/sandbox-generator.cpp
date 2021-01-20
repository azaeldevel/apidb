
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
	octetos::core::Semver version;
	version.setNumbers(0,1,0);
    version.setPrerelease("alpha"); 
    
    
    octetos::apidb::ConfigureProject configProjectCpp;
    configProjectCpp.name = "muposys";    
	configProjectCpp.setInputs(octetos::apidb::InputLenguajes::MariaDB,mariaSource);     
    configProjectCpp.versionResult = version;
	configProjectCpp.writeDatconnect = "conector";
    configProjectCpp.builDirectory = "muposys";
    configProjectCpp.space = "muposys::db";
    configProjectCpp.outputLenguaje = octetos::apidb::OutputLenguajes::CPP;
    configProjectCpp.packing = octetos::apidb::PackingLenguajes::CMake;
    configProjectCpp.compiled = octetos::apidb::Compiled::STATIC;  
    
    /*PHP
    configProject.outputLenguaje = octetos::apidb::OutputLenguajes::PHP;
    configProject.packing = octetos::apidb::PackingLenguajes::OnlyCode;
    configProject.compiled = octetos::apidb::Compiled::NoCompile;
    */
    
    //configProject.builDirectory = "/home/azael/develop/octetos/muposys/java/muposys-db/src/main/java/octetos/muposys/db";
    //configProject.space = "octetos.muposys.db";
    
    //
    /*
    octetos::apidb::ConfigureProject configProject;
    configProject.readConfig("/home/azael/develop/octetos/muposys/C++/apidb/muposys.apidb");
    configProject.builDirectory = "muposys-apidb";
    configProject.packing = octetos::apidb::PackingLenguajes::CMake;
    */
    
    //configProject.saveConfig("muposys.apidb");
    
    //octetos::apidb::ConfigureProject configProject2;
    //configProject2.readConfig("muposys.apidb");
    
    //std::cout << "Step 3\n";
    octetos::apidb::Driver driver(configProjectCpp);

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
    
    
    octetos::apidb::ConfigureProject configProjectJava;
    configProjectCpp.name = "muposys";    
	configProjectCpp.setInputs(octetos::apidb::InputLenguajes::MariaDB,mariaSource);   
    configProjectCpp.versionResult = version;
	configProjectCpp.writeDatconnect = "conector";
    configProjectCpp.builDirectory = "muposys";
    configProjectCpp.space = "octetos.muposys.db";
    configProjectCpp.outputLenguaje = octetos::apidb::OutputLenguajes::JAVA;
    configProjectCpp.packing = octetos::apidb::PackingLenguajes::NoPack;
    configProjectCpp.compiled = octetos::apidb::Compiled::NoCompile; 
    octetos::apidb::Driver driverJava(configProjectCpp);
    octetos::apidb::Tracer tracerJava(0);
    try
    {
        driverJava.driving(&tracerJava);
    }
    catch(const std::exception& ex)
    {
        std::cerr << "Error : " << ex.what() << "\n";
    }
    
    
    octetos::apidb::ConfigureProject configProjectPHP;
    configProjectPHP.name = "muposys";    
	configProjectPHP.setInputs(octetos::apidb::InputLenguajes::MariaDB,mariaSource);   
    configProjectPHP.versionResult = version;
	configProjectPHP.writeDatconnect = "conector";
    configProjectPHP.builDirectory = "muposys";
    //configProjectPHP.space = "octetos.muposys.db";
    configProjectPHP.outputLenguaje = octetos::apidb::OutputLenguajes::PHP;
    configProjectPHP.packing = octetos::apidb::PackingLenguajes::NoPack;
    configProjectPHP.compiled = octetos::apidb::Compiled::NoCompile; 
    octetos::apidb::Driver driverPHP(configProjectPHP);
    octetos::apidb::Tracer tracerPHP(0);
    try
    {
        driverPHP.driving(&tracerPHP);
    }
    catch(const std::exception& ex)
    {
        std::cerr << "Error : " << ex.what() << "\n";
    }
    
    return EXIT_SUCCESS;
}
