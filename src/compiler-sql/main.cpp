#include <iostream>
#include <cstdlib>
#include <cstring>

#include "driver.hpp"
#include <sstream> 

int 
main( const int argc, const char **argv )
{
	apibd::Driver driver;
	std::istringstream text("VARCHAR(25)");
	driver.parse(text);
	std::cout<<std::endl;
	
	return( EXIT_SUCCESS );
}
