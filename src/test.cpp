
#include <iostream>
#include <cstdlib>
#include <cstring>

#include "analyzer.hpp"

int main(int argc, const char **argv)
{
	apidb::mysql::Analyzer driver;	
	std::string str = "VARCHAR(25)";
	std::cout<<driver.parse(str)<<std::endl;
	str = "int(10)";
	std::cout<<driver.parse(str)<<std::endl;
	str = "set('PO','FXML','FPDF')";
	std::cout<<driver.parse(str)<<std::endl;
	str = "INT(10)";
	std::cout<<driver.parse(str)<<std::endl;
	str = "varchar(25)";	
	std::cout<<driver.parse(str)<<std::endl;
	return(EXIT_SUCCESS);
}
