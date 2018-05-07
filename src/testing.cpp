
#include "apidb.hpp" 

#include <iostream>
#include <list>

int main()
{	
    std::cout<<"Creating driver..."<<std::endl;
	apidb::MySQLDriver driver;
    std::cout<<"Listing..."<<std::endl;
	if(driver.listTables())
	{
        std::cout<<"Listing done."<<std::endl;
		if(driver.getListTable()->size() == 0)
		{            
			std::cout<< "no hay elemtos" <<std::endl;
		}
		else if(driver.getListTable()->size() == 1)
		{
			std::cout<< "Hay " <<driver.getListTable()->size() <<" elemento."<<std::endl;
            for (apidb::internal::RowShowTables* n : *(driver.getListTable())) 
            {
                std::cout<<"\t" << n->table_name <<std::endl;
            }
		}	
		else if(driver.getListTable()->size() > 1)
		{
			std::cout<< "Hay " <<driver.getListTable()->size() <<" elementos"<<std::endl;
            for (apidb::internal::RowShowTables* n : *(driver.getListTable())) 
            {
                std::cout<<"\t" << n->table_name <<std::endl;
            }
		}		
	}
	
	return 0;	
}
