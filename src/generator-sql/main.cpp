#include <iostream>
#include <cstdlib>
#include <cstring>

#include "driver.hpp"


int main( const int argc, const char **argv )
{
	apibd::Driver driver;	
	std::string str = "VARCHAR(25)";
	std::cout<<driver.parse(str)<<std::endl;
	str = "INT";
	std::cout<<driver.parse(str)<<std::endl;	
	
	return( EXIT_SUCCESS );
}
