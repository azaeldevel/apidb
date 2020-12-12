
#include <octetos/db/clientdb-maria.hh>

#include "../apidb.hpp"
#include "../common.hpp"


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
        std::cerr << "Fallo en la conexion\n";
        return EXIT_FAILURE;
    }    
    
    
    octetos::apidb::ConfigureProject configProject;
	try
	{
		configProject.readConfig("/home/azael/develop/octetos/muposys/C++/apidb/muposys.apidb");
	}
	catch(octetos::core::Error& e)
	{
		std::cerr << e.what() << "\n";
        return;		
	}
    
    octetos::apidb::Driver driver(configProject);

    octetos::apidb::Tracer tracer(0);
    //std::cout << "testBuild: Step 3\n";
    if(!driver.driving(&tracer))
    {
        std::cerr << "Fallo en la contruccion.\n";
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}
