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
    
    std::cout << "Step 1.\n";
    
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
    
    muposys::Ente ente1;
    if(not ente1.insert(connector))
    {
		std::cerr << "Fail on insert ente.\n";
		return EXIT_FAILURE;
    }
    
    muposys::Person person1;
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
	
	std::cout << "Step 2.\n";
	
    if(verbose) std::cout << "Listando los que tiene 8 con 5 registro maximo." << std::endl;
    std::vector<muposys::Person*>* lstPerson = muposys::Person::select(connector,"name1 like 'n1-%8%'",5,'D');
    if(lstPerson)
    {
      for(auto p : *lstPerson)
      {
            p->downName1(connector);
            p->downName3(connector);
            if(verbose) std::cout << p->getName1() << " " << p->getName3() << std::endl;
      }
      for(auto p : *lstPerson)
      {
            delete p;
      }
      delete lstPerson;
    }
    
    std::vector<muposys::User*>* lstUser = muposys::User::select(connector,"person > 0",5,'D');
    
	std::random_device generator;
  	std::uniform_int_distribution<int> randInt(1,INT_MAX);
    
    std::cout << "Step 3.\n";
    
    std::vector<muposys::Permission*>* permsslst = muposys::Permission::select(connector,"",0);
    if(permsslst)
    {
      for(auto p : *permsslst)
      {
        p->downName(connector);
        p->downBrief(connector);
        if(verbose) std::cout << p->getName() << " | " << p->getBrief() << std::endl;
      }
      /*for(auto p : *permsslst)
      {
        delete p;
      }
      delete permsslst;*/
    }
    
    if(verbose)  std::cout << "Adding Permissions.\n";
    muposys::Permission permss;
    int randNumber = randInt(generator);
    std::string name_perss = "permss-" + std::to_string(randNumber);
    std::string brief_perss = "Prueba de muposys " + std::to_string(randNumber);
    muposys::Ente ente2;
    if(not ente2.insert(connector))
    {
		std::cerr << "Fail on insert ente.\n";
		return EXIT_FAILURE;
    }
    muposys::UserManagement um2;
    if(not um2.insert(connector,ente2))
    {
		std::cerr << "Fail on insert ente.\n";
		return EXIT_FAILURE;
    }
    if(not permss.insert(connector,um2,name_perss,brief_perss))
    {
		std::cerr << "Fail on insert permision.\n";
		return EXIT_FAILURE;        
    }
		
      std::cout << "Step 4.\n";
        
    randNumber = randInt(generator);    
    muposys::Ente ente3;
    if(not ente3.insert(connector))
    {
		std::cerr << "Fail on insert ente.\n";
		return EXIT_FAILURE;
    }    
    if(verbose)  std::cout << "Adding Person.\n";
    muposys::Person person;
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
    
    if(verbose)  std::cout << "Adding User.\n";
    muposys::Ente ente_user;
    std::string name_user = "user-" + std::to_string(randNumber);
    if(not ente_user.insert(connector))
    {
		std::cerr << "Fail on insert ente.\n";
		return EXIT_FAILURE;
    }
    muposys::UserManagement um;
    if(not um.insert(connector,ente_user))
    {
		std::cerr << "Fail on insert ente.\n";
		return EXIT_FAILURE;
    }
    muposys::User user;
    randNumber = randInt(generator);
    std::cout << "Person : " << person.getEnte().getID() << "\n";
    name_user = "user-" + std::to_string(randNumber);
    if(not user.insert(connector,um,person,name_user))
    {
		std::cerr << "Fail on insert ente.\n";
		return EXIT_FAILURE;        
    }
    
    std::cout << "Step 5.\n";
  
    if(verbose)  std::cout << "Adding UserPermission.\n";
    muposys::Ente ente_up;
    if(not ente_up.insert(connector))
    {
		std::cerr << "Fail on insert ente.\n";
		return EXIT_FAILURE;
    }
    muposys::UserPermission usr_permss;
    if(not usr_permss.insert(connector,user,permss))
    {
		std::cerr << "Fail on insert person.\n";
		return EXIT_FAILURE;
    }
    
    if(verbose)  std::cout << "Adding Catalog.\n";
    randNumber = randInt(generator);
    muposys::Ente ente_catalog;
    if(not ente_catalog.insert(connector))
    {
		std::cerr << "Fail on insert ente.\n";
		return EXIT_FAILURE;
    }
    muposys::Catalog catalog1;
    std::string catalog_name = "catalog-" + std::to_string(randNumber);
    if(not catalog1.insert(connector,ente_catalog,catalog_name))
    {
		std::cerr << "Fail on insert catalog.\n";
		return EXIT_FAILURE;      
    }
    
    if(verbose) std::cout << "Adding CatalogItem.\n";
    randNumber = randInt(generator);
    muposys::Ente ente_cataloging;
    if(not ente_cataloging.insert(connector))
    {
		std::cerr << "Fail on insert ente.\n";
		return EXIT_FAILURE;
    }
    muposys::CatalogItem catItems1;
    std::string item_number = "item-" + std::to_string(randNumber);
    std::string item_name = "name-" + std::to_string(randNumber);
    if(not catItems1.insert(connector,ente_cataloging,catalog1,item_number,item_name))
    {
		std::cerr << "Fail on insert catalog.\n";
		return EXIT_FAILURE;      
    }
    
    if(verbose)  std::cout << "Adding CatalogItem.\n";
    randNumber = randInt(generator);
    muposys::Ente ente_cataloging2;
    if(not ente_cataloging2.insert(connector))
    {
		std::cerr << "Fail on insert ente.\n";
		return EXIT_FAILURE;
    }
    muposys::CatalogItem catItems2;
    std::string item_number2 = "item-" + std::to_string(randNumber);
    std::string item_name2 = "name-" + std::to_string(randNumber);
    if(not catItems2.insert(connector,ente_cataloging2,catalog1,item_number2,item_name2))
    {
		std::cerr << "Fail on insert catalog.\n";
		return EXIT_FAILURE;      
    }
        
    if(verbose) std::cout << "Adding Stock.\n";
    if(verbose) std::cout << "Step 1\n";
    randNumber = randInt(generator);
    if(verbose) std::cout << "Step 2\n";
    muposys::Ente ente_stock1;
    if(verbose) std::cout << "Step 3\n";
	if(not ente_stock1.insert(connector))
    {
		std::cerr << "Fail on insert Stock.id.\n";
		return EXIT_FAILURE;            
    }
    if(verbose) std::cout << "Step 4\n";
	muposys::Stock stock1;
    if(verbose) std::cout << "Step 5\n";
	if(not stock1.insert(connector,ente_stock1))
    {
		std::cerr << "Fail on insert Stock.\n";
		return EXIT_FAILURE;
    }
    if(verbose) std::cout << "Step 6\n";
    std::string stock1_name = "stock-" + std::to_string(randNumber);
    if(verbose) std::cout << "Step 7\n";
    std::string stock1_label = "Stock " + std::to_string(randNumber);
    if(verbose) std::cout << "Step 8\n";
	if(not stock1.upBrief(connector,stock1_name))
    {
		std::cerr << "Fail on update Stock.name.\n";
		return EXIT_FAILURE;       
    }
    if(verbose) std::cout << "Step 9\n";
    if(verbose) std::cout << "Step 10\n";
    
    if(verbose) std::cout << "Adding Stock.\n";
    randNumber = randInt(generator);
    muposys::Ente ente_stock2;
	if(not ente_stock2.insert(connector))
    {
		std::cerr << "Fail on insert Stock.id.\n";
		return EXIT_FAILURE;            
    }
	muposys::Stock stock2;
	if(not stock2.insert(connector,ente_stock2))
    {
		std::cerr << "Fail on insert Stock.\n";
		return EXIT_FAILURE;
    }
    std::string stock2_name = "stock-" + std::to_string(randNumber);
    std::string stock2_label = "Stock " + std::to_string(randNumber);
	if(not stock2.upBrief(connector,stock2_name))
    {
		std::cerr << "Fail on update Stock.name.\n";
		return EXIT_FAILURE;       
    }
    
    if(verbose) std::cout << "Adding Stocking.\n";
    randNumber = randInt(generator);
    muposys::Ente ente_stocking1;
	if(not ente_stocking1.insert(connector))
    {
		std::cerr << "Fail on update Stocking.id.\n";
		return EXIT_FAILURE;      
    }
	/*muposys::Stocking stoking1;
	if(not stoking1.insert(connector,ente_stocking1,stock1,catItems1,3))
    {
		std::cerr << "Fail on update Stocking.\n";
		return EXIT_FAILURE;      
    }
    
    if(verbose) std::cout << "Adding Stocking.\n";
    randNumber = randInt(generator);
    muposys::Ente ente_stocking2;
	if(not ente_stocking2.insert(connector))
    {
		std::cerr << "Fail on update Stocking.id.\n";
		return EXIT_FAILURE;      
    }
	muposys::Stocking stoking2;
	if(not stoking2.insert(connector,ente_stocking2,stock2,catItems2,1))
    {
		std::cerr << "Fail on update Stocking.\n";
		return EXIT_FAILURE;      
    }
    
    if(verbose) std::cout << "Adding Stocking_Production.\n";
    if(verbose) std::cout << "Step 1\n";
    muposys::Stocking_Production stoking_prod1s;
    if(verbose) std::cout << "Step 2\n";
	if(not stoking_prod1s.insert(connector,stoking1))
    {
		std::cerr << "Fail on update Stocking_Production.\n";
		return EXIT_FAILURE;       
    }
    if(verbose) std::cout << "Step 3\n";
    stoking_prod1s.getItem().downItem(connector);
    stoking1.downItem(connector);
    if(verbose)
    {
      std::cout << "catItems1.id " << catItems1.getID().getID() << "\n";
      
      std::cout << "stoking1.item " << stoking1.getItem().getID().getID() << "\n";
      
      std::cout << "stoking_prod1s.item - Stocking* : " << &stoking_prod1s.getItem() << "\n";
      std::cout << "stoking_prod1s.item.item - CatalogItem* : " << &stoking_prod1s.getItem().getItem() << "\n";
      std::cout << "stoking_prod1s.item.item - Ente* : " << &stoking_prod1s.getItem().getItem().getID() << "\n";
      std::cout << "stoking_prod1s.item.item.id : " << stoking_prod1s.getItem().getItem().getID().getID() << "\n";
      
    }
    if(stoking1.getItem().getID().getID() != stoking_prod1s.getItem().getItem().getID().getID())
    {
		std::cerr << "Fail on gettting ids.\n";
		return EXIT_FAILURE;        
    }*/
    
    //if()
    
    
    std::cout << "Ending operations..\n";
    connector.commit();
	connector.close();
	
	return EXIT_SUCCESS;
}

