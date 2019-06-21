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
	}
    
    if(connector.commit())
    {
		std::cout << "Commit done " << std::endl;
	}
	else
	{
		std::cerr << "Commit fail"<< std::endl;
	}
	
	/*if(person1.download_shortname(connector))
    {
        std::cout << ""<< person1.getN1() << " " << person1.getAp() << std::endl;
    }
    else
    {
        std::cout << "Fallo la descarga de person1" << std::endl;
    }
    
    static std::vector<sysapp::Person*>* lst = sysapp::Person::select(connector,"n1 like 'n1-%8'");
    if(lst != NULL)
    {
		for(auto p : *lst)
		{
		    if(p->download_shortname(connector))
		    {
		        std::cout << ""<< p->getN1() << " " << p->getAp() << std::endl;
		    }
		}
    }*/
    
    /*sis::Persons* person2 = new sis::Persons();
    if(person2->select(connector,"n1-4182","ap-4182"))
    {
		std::cout << "Select Random "<< person2->toString() << std::endl;
	}
	else
	{
		std::cerr << "Fail Slected random "<< n1 << std::endl;
	} */
	
	//std::string db = ((toolkit::clientdb::DatconectionMySQL&)(connector.getDatconection())).getDatabase();
	//std::cout<<db<<std::endl;
	
	connector.close();
	
	return 0;
}

