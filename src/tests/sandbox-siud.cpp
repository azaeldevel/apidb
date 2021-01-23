
#include <iostream>
#include <fstream>
#include <muposys.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <octetos/core/MD5sum.hh>
#include <vector>


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
    std::string userstr,name,userstr2,name2;
    
    userstr = "user-";
    userstr += std::to_string(r);
    name = "name-";
    name += std::to_string(r);    
    muposys::db::Users user;
    if(user.insert(connector,userstr,name))
    {
        std::cout << "insert : " << name << " \n";
    }
    else
    {
        std::cerr << "Fallo en insert" << name << " \n";
        return EXIT_FAILURE;
    }
    
    octetos::core::MD5sum md5(std::to_string(user.getPerson().getEnte().getID()));
    if(user.getPerson().getEnte().upMd5sum(connector,md5))
    {
        std::cout << "update id=" << user.getPerson().getEnte().getID() << ", name=" << (const std::string&)md5 << " \n";
    }
    r = rand() % 10000;
    userstr2 = "user-";
    userstr2 += std::to_string(r);
    name2 = "name-";
    name2 += std::to_string(r);    
    muposys::db::Users user2;
    if(user2.insert(connector,userstr2,name2))
    {
        std::cerr << "insert : " << name2 << " \n";
    }
    else
    {
        std::cerr << "Fallo en insert" << name2 << " \n";
        return EXIT_FAILURE;
    }
    
    connector.commit();
    
    std::vector<muposys::db::Users*>* userList = muposys::db::Users::select(connector,"person != 0",10);
    for(muposys::db::Users* u : *userList)
    {
        std::cout << "user id=" << u->getPerson().getEnte().getID() << "\n";
    }
    
    
    return EXIT_SUCCESS;
}
