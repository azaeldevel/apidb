
#include "apidb.hpp" 

#include <iostream>
#include <list>

int main()
{	
    //std::cout<<"Creating driver..."<<std::endl;
	apidb::MySQLDriver driver;
    //std::cout<<"Listing..."<<std::endl;
	if(driver.parse())
	{
        //std::cout<<"Listing done."<<std::endl;
		if(driver.getListTable()->size() == 0)
		{            
			std::cout<< "no hay elemtos" <<std::endl;
		}
		else if(driver.getListTable()->size() == 1)
		{
			std::cout<< "\nHay " <<driver.getListTable()->size() <<" elemento."<<std::endl;
            for (apidb::internal::Table* n : *(driver.getListTable())) 
            {
                std::cout<<"" << n->table_name <<std::endl;
                for(apidb::internal::Table::Attribute* m : n->attributes)
                {
                    std::cout<<"  " << m->name<<std::endl;
                }
            }
		}	
		else if(driver.getListTable()->size() > 1)
		{
			std::cout<< "\nHay " <<driver.getListTable()->size() <<" elementos"<<std::endl;
            for (apidb::internal::Table* n : *(driver.getListTable())) 
            {
                std::cout<<" "<< n->table_name <<std::endl;
                for(apidb::internal::Table::Attribute* m : n->attributes)
                {
                    std::cout<<"  "<< m->name<<std::endl;
                }
            }
		}		
	}
	else
    {
        std::cerr<<"Not parsin phase"<<std::endl;
    }
	
	
	
	return 0;	
}
