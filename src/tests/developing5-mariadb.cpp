#include "muposys.hpp"
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <random>
#include <climits>


int main(int argc, char **argv)
{	
    bool verbose = false;
    if(argc >1 )
    {
        if(strcmp(argv[1],"--verbose")==0)
        {
            verbose = true;
        }
    }
        
	octetos::db::maria::Datconnect mariaConnector("localhost",3306,"muposys-0-alpha","muposys","123456");
    octetos::db::maria::Connector connector; 
    bool flag = false;  
	flag = connector.connect(mariaConnector);
    if(flag)
    {
        if(verbose)  printf("SQL Server version: %s\n", connector.getVerionServer().toString().c_str());
    }
    else
    {
        printf("Fallo la conexion el servidor.\n");
        return EXIT_FAILURE;
    }
    
    srand (time(NULL));
	int random = rand() % 10000 + 1;
    
    muposys::Entities ente1;
    if(not ente1.insert(connector))
    {
		std::cerr << "Fail on insert ente.\n";
		return EXIT_FAILURE;
    }
    
    muposys::Persons person1;
    std::string n1 = "n1-";
    n1 += std::to_string(random);
    std::string country = "MEX";
    //ap += std::to_string(random);
    if(person1.insert(connector,ente1,n1))
    {
		if(verbose)   std::cout << "Inserted "<< n1 << " " << country << " de Mexico." << std::endl;
	}
	else
	{
		std::cerr << "Fail "<< n1 << std::endl;
		return EXIT_FAILURE;
	}
    
    if(connector.commit())
    {
		if(verbose)  std::cout << "Commit done " << std::endl;
	}
	else
	{
		std::cerr << "Commit fail"<< std::endl;
		return EXIT_FAILURE;
	}
	
    if(verbose) std::cout << "Listando los que tiene 8 con 5 registro maximo." << std::endl;
    std::vector<muposys::Persons*>* lst = muposys::Persons::select(connector,"name1 like 'n1-%8'",5,'D');
    if(lst != NULL)
    {
        for(auto p : *lst)
        {
            p->downName1(connector);
            if(not p->getName1().empty() and verbose)
            {
                if(verbose)  std::cout << ""<< p->getName1() << " " << p->getName3() << std::endl;
            }
        }
        for(auto p : *lst)
        {
            delete p;
        }
    }
    delete lst;	
    
    std::vector<muposys::Users*>* lstUsers = muposys::Users::select(connector,"person > 0",5,'D');
	
    
	std::random_device generator;
  	std::uniform_int_distribution<int> randInt(1,INT_MAX);
    
    std::vector<muposys::Permissions*>* permsslst = muposys::Permissions::select(connector,"",0);
    if(permsslst != NULL)
    {
        for(auto p : *permsslst)
        {
            if(p->downBrief(connector) and verbose)
            {
                //if(verbose)  std::cout << p->getName() << " " << p->getBrief() << std::endl;
            }
        }
        for(auto p : *permsslst)
        {
            delete p;
        }
    }
    delete permsslst;
    
    muposys::Permissions permss;
    int randNumber = randInt(generator);
    std::string name_perss = "permss-" + std::to_string(randNumber);
    std::string brief_perss = "Prueba de muposys " + std::to_string(randNumber);
    
    muposys::Entities ente2;
    if(not ente2.insert(connector))
    {
		std::cerr << "Fail on insert ente.\n";
		return EXIT_FAILURE;
    }
    
    if(not permss.insert(connector,ente2,name_perss,brief_perss))
    {
		std::cerr << "Fail on insert permision.\n";
		return EXIT_FAILURE;        
    }
		
    randNumber = randInt(generator);
    
    muposys::Entities ente3;
    if(not ente3.insert(connector))
    {
		std::cerr << "Fail on insert ente.\n";
		return EXIT_FAILURE;
    }
		
    muposys::Persons person;
    std::string name_person = "person-" + std::to_string(randNumber);
        
    if(not person.insert(connector,ente3,name_person))
    {
		std::cerr << "Fail on insert person.\n";
		return EXIT_FAILURE;
    }
    name_person = "ap-" + std::to_string(randNumber);
    if(not person.upName3(connector,name_person))
    {
		std::cerr << "Fail on insert person.\n";
		return EXIT_FAILURE;
    }		
    
    muposys::Users user;
    randNumber = randInt(generator);
    std::cout << "Person : " << person.getEnte().getID() << "\n";
    std::string name_user = "user-" + std::to_string(randNumber);
    if(not user.insert(connector,person,name_user))
    {
		std::cerr << "Fail on insert ente.\n";
		return EXIT_FAILURE;        
    }
		
    /*muposys::User_Permission usr_permss;
    if(not usr_permss.insert(connector,user,permss))
    {
		std::cerr << "Fail on insert person.\n";
		return EXIT_FAILURE;
    }*/
    
    connector.commit();
	connector.close();
	
	return EXIT_SUCCESS;
}

