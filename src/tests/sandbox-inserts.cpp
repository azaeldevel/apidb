
#include <iostream>
#include <fstream>
#include <muposys.hpp>
#include <mysql/mysql.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <octetos/core/MD5sum.hh>

int main(int argc, char **argv)
{
    std::cout << "Comenzando consultas..\n";
    
    octetos::db::maria::Datconnect mariaSource("localhost",3306,"muposys-0.1-alpha","muposys","123456");
    octetos::db::maria::Connector connector; 
    bool flag = false;  
	flag = connector.connect(mariaSource);
    if(flag)
    {
        //printf("SQL Server version: %s\n", connector.getVerionServer().toString().c_str());
    }
    else
    {
        std::cerr << "Fallo en laconexion \n";
        return EXIT_FAILURE;
    }
    time_t t;
    srand((unsigned) time(&t));
    int r = rand() % 10000;
    std::string userstr,name;
    
    userstr = "user-";
    userstr += std::to_string(r);
    name = "name-";
    name += std::to_string(r);
    
    muposys::db::Users user;
    if(user.insert(connector,userstr,name))
    {
        std::cerr << "insert : " << name << " \n";
    }
    else
    {
        std::cerr << "Fallo en insert" << name << " \n";
        return EXIT_FAILURE;
    }
    
    connector.commit();
    
    return EXIT_SUCCESS;
}
