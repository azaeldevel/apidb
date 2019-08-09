#include "sysapp.hpp"
#include <iostream>


int main(int argc, char **argv)
{	
	octetos::toolkit::clientdb::mysql::Datconnect mysqlConnector("192.168.0.101",3306,"sysapp.alpha","develop","123456");  
    octetos::toolkit::clientdb::mysql::Connector connector; 
    bool flag = false;  
    try
    {
		flag = connector.connect(&mysqlConnector);
	}
	catch(octetos::toolkit::clientdb::SQLException& ex)
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
    
    sysapp::Persons person1;
    std::string n1 = "n1-";
    n1 += std::to_string(random);
    std::string ap = "ap-";
    ap += std::to_string(random);
    if(person1.insert(connector,n1,ap))
    {
		std::cout << "Inserted "<< n1 << " " << ap << " de Mexico." << std::endl;
	}
	else
	{
		std::cerr << "Fail "<< n1 << std::endl;
		return EXIT_FAILURE;
	}
    
    if(connector.commit())
    {
		std::cout << "Commit done " << std::endl;
	}
	else
	{
		std::cerr << "Commit fail"<< std::endl;
		return EXIT_FAILURE;
	}
	
	if(person1.download_shortname(connector))
    {
        std::cout << ""<< person1.getName1() << " " << person1.getName3() << std::endl;
    }
    else
    {
        std::cout << "Fallo la descarga de person1" << std::endl;
		return EXIT_FAILURE;
    }
    
    static std::vector<sysapp::Persons*>* lst = sysapp::Persons::select(connector,"name1 like 'n1-%8'");
    if(lst != NULL)
    {
		for(auto p : *lst)
		{
		    if(p->download_shortname(connector))
		    {
		        std::cout << ""<< p->getName1() << " " << p->getName3() << std::endl;
		    }
		}
    }
    
	
	//std::string db = ((toolkit::clientdb::DatconectionMySQL&)(connector.getDatconection())).getDatabase();
	//std::cout<<db<<std::endl;
	
	connector.close();
	
	return EXIT_SUCCESS;
}

