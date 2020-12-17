
#include <octetos/db/clientdb-maria.hh>
#include <octetos/core/MD5sum.hh>
#include <muposys.hpp>
#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>

#include "../apidb.hpp"
#include "../common.hpp"

int main(int argc, char **argv)
{
    std::cout << "Step 1\n";
    octetos::db::mariadb::Datconnect mariaSource("localhost",3306,"muposys-0.1-alpha","muposys","123456");
    mariaSource.setAutocommit(false);
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
    
    int randNumber;
    /* initialize random seed: */
    srand (time(NULL));
    /* generate secret number between 1 and 10: */
    randNumber = rand() % 10000 + 1;
    
    muposys::Users user;
    std::string pname = "person";
    pname += std::to_string(randNumber);
    std::string uname = "user";
    uname += std::to_string(randNumber);
    try
    {
        if(user.insert(connector,pname,uname))
        {
            std::cout << "Data inserted\n";
        }
        else
        {
            std::cerr << "Data not inserted\n";
            return EXIT_FAILURE;
        }
    }
    catch(const std::exception& ex)
    {
        std::cerr << "Exception : " << ex.what() << "\n";
    }
    connector.commit();
    /*muposys::Entities ente;
    ente.insert(connector);
    octetos::core::MD5sum md5(std::to_string(ente.getID()));
    ente.updateMd5sum(connector,md5);
    connector.commit();
    
    muposys::Person person;*/
    
    
    return EXIT_SUCCESS;
}
