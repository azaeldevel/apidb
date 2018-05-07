
#include "apidb.hpp" 

#include <iostream>

int main()
{
	
	apidb::MySQLDriver driver;
	if(driver.listTables())
	{
		if(driver.getListTable()->size() == 0)
		{
			std::cout<< "no hay elemtos" <<std::endl;
		}
		else if(driver.getListTable()->size() == 1)
		{
			std::cout<< "Hay " <<driver.getListTable()->size() <<" elemento"<<std::endl;
		}	
		else if(driver.getListTable()->size() > 1)
		{
			std::cout<< "Hay " <<driver.getListTable()->size() <<" elementos"<<std::endl;
		}		
	}
	
	return 0;	
}
