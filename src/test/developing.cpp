#include <stdio.h>
#include <iostream>
#include <stdlib.h>     /* srand, rand */
#include <time.h> 
#include <string>

#include "nmp.hpp"

int main(int argc, char **argv)
{
    toolkit::clientdb::DatconectionMySQL mysqlConnector("192.168.0.101",3306,"business.alpha","root","k3yL0c41");  
    toolkit::clientdb::Connector connector; 
    bool flag = false;  
    try
    {
		flag = connector.connect(mysqlConnector);
	}
	catch(toolkit::clientdb::SQLException ex)
	{
		std::cerr<<ex.what()<< std::endl;
	}
    if(flag)
    {
        printf("SQL Server version: %s\n", connector.serverDescription());
    }
    else
    {
        std::cerr<<"Fallo la conexion el servidor."<< std::endl;
    }
    
    srand (time(NULL));
	int random = rand() % 10000 + 1;
    
    nmp::Persons* person1 = new nmp::Persons();
    std::string n1 = "n1-";
    n1 += std::to_string(random);
    if(person1->insert(connector))
    {
		std::cout << "Inserted "<< n1 << std::endl;
	}
	else
	{
		std::cerr << "Fail "<< n1 << std::endl;
	}	
    if(person1->updaten1(connector,n1))
    {
		std::cout << "Ipdate sussces "<< n1 << std::endl;
	}
	else
	{
		std::cerr << "Update Fail "<< n1 << std::endl;
	}
    
    if(connector.commit())
    {
		std::cout << "Commit done " << std::endl;
	}
	else
	{
		std::cerr << "Commit fail"<< std::endl;
	}
	
    /*nmp::Persons* person2 = new nmp::Persons();
    if(person2->selectRandom(connector))
    {
		std::cout << "Select Random "<< person2->toString() << std::endl;
	}
	else
	{
		std::cerr << "Fail Slected random "<< n1 << std::endl;
	}*/    
	
	std::string db = ((toolkit::clientdb::DatconectionMySQL&)(connector.getDatconection())).getDatabase();
	std::cout<<db<<std::endl;
}
