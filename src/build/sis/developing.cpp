#include <stdio.h>
#include <iostream>
#include <stdlib.h>     /* srand, rand */
#include <time.h> 
#include <string>

#include "sis.hpp"

int main(int argc, char **argv)
{	
	toolkit::clientdb::DatconectionMySQL mysqlConnector("192.168.0.101",3306,"sis","develop","123456");  
    toolkit::clientdb::Connector connector; 
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
	
    /*sis::Persons* person2 = new sis::Persons();
    if(person2->selectRandom(connector))
    {
		std::cout << "Select Random "<< person2->toString() << std::endl;
	}
	else
	{
		std::cerr << "Fail Slected random "<< n1 << std::endl;
	}  */  
	
	//std::string db = ((toolkit::clientdb::DatconectionMySQL&)(connector.getDatconection())).getDatabase();
	//std::cout<<db<<std::endl;

}

