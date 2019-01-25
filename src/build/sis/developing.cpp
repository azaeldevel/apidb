#include <stdio.h>
#include <iostream>
#include <stdlib.h>     /* srand, rand */
#include <time.h> 
#include <string>

#include "sis.hpp"

int main(int argc, char **argv)
{	
	toolkit::clientdb::datasourcies::MySQL mysqlConnector("192.168.0.101",3306,"sis","develop","123456");  
    toolkit::clientdb::connectors::MySQL connector; 
    bool flag = false;  
    try
    {
		flag = connector.connect(mysqlConnector);
	}
	catch(toolkit::clientdb::SQLException& ex)
	{
		std::cerr<<ex.what()<< std::endl;
	}
    if(flag)
    {
        printf("SQL Server version: %s\n", connector.serverDescription());
    }
    else
    {
        printf("Fallo la conexion el servidor.\n");
    }
    
    srand (time(NULL));
	int random = rand() % 10000 + 1;
    
    sis::Persons person1;
    std::string n1 = "n1-";
    n1 += std::to_string(random);
    if(person1.insert(connector,n1))
    {
		std::cout << "Inserted "<< n1 << std::endl;
	}
	else
	{
		std::cerr << "Fail "<< n1 << std::endl;
	}
    
    if(connector.commit())
    {
		std::cout << "Commit done " << std::endl;
	}
	else
	{
		std::cerr << "Commit fail"<< std::endl;
	}
	
	if(person1.download_shortname(connector))
    {
        std::cout << ""<< person1.getN1() << " " << person1.getAp() << std::endl;
    }
    else
    {
        std::cout << "Fallo la descarga de person1" << std::endl;
    }
    
    static std::vector<sis::Persons*>* lst = sis::Persons::select(connector,"n1 like 'n1-%8'");
    for(auto p : *lst)
    {
        if(p->download_shortname(connector))
        {
            std::cout << ""<< p->getN1() << " " << p->getAp() << std::endl;
        }
    }
    
    
    
    return EXIT_SUCCESS;

}

