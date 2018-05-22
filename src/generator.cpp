#include <iostream>
#include <cstdlib>
#include <cstring>

#include "driver.hpp"


int main( const int argc, const char **argv )
{
	apidb::Driver driver;	
	std::string str = "VARCHAR(25)";
	std::cout<<driver.parse(str)<<std::endl;
	str = "INT(10)";
	std::cout<<driver.parse(str)<<std::endl;
	str = "int(10)";
	std::cout<<driver.parse(str)<<std::endl;
	str = "varchar(25)";
	std::cout<<driver.parse(str)<<std::endl;	
	
	return( EXIT_SUCCESS );
}
