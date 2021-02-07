
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
        printf("SQL Server version: %s\n", connector.getVerionServer().toString().c_str());
    }
    else
    {
        std::cerr << "Fallo en laconexion \n";
        return EXIT_FAILURE;
    }
    time_t t;
    srand((unsigned) time(&t));
    int r = rand() % 100000;
    std::string userstr,name,userstr2,name2,userstr3,name3,projectName,rqname1,rqname2,rqname3;
    
    
    userstr = "user-";
    userstr += std::to_string(r);
    name = "name-";
    name += std::to_string(r);    
    muposys::db::Users user;
    if(user.insert(connector,userstr,name))
    {
        std::cout << "insert 1: " << name << " \n";
    }
    else
    {
        std::cerr << "Fallo en insert" << name << " \n";
        return EXIT_FAILURE;
    }    
    octetos::core::MD5sum md5(std::to_string(user.getPerson().getEnte().getID()));
    if(user.getPerson().getEnte().upMd5sum(connector,md5))
    {
        std::cout << "update id=" << user.getPerson().getEnte().getID() << ", md5=" << (const std::string&)md5 << " \n";
    }
    
    
    r = rand() % 100000;
    userstr2 = "user-";
    userstr2 += std::to_string(r);
    name2 = "name-";
    name2 += std::to_string(r);    
    muposys::db::Users user2;
    if(user2.insert(connector,userstr2,name2))
    {
        //std::cerr << "insert 2: " << name2 << " \n";
    }
    else
    {
        std::cerr << "Fallo en insert" << name2 << " \n";
        return EXIT_FAILURE;
    }
    
    r = rand() % 100000;
    userstr3 = "user-";
    userstr3 += std::to_string(r);
    name3 = "name-";
    name3 += std::to_string(r);    
    muposys::db::Users user3;
    
    if(user3.insert(connector,userstr3,name3))
    {
        std::cout << "insert 3: " << name3 << " \n";
    }
    else
    {
        std::cerr << "Fallo en insert" << name3 << " \n";
        return EXIT_FAILURE;
    }
    if(user3.remove(connector))
    {
        std::cout << "delete : " << name3 << " \n";
    }
    else
    {
        std::cerr << "Fallo en delete" << name3 << " \n";
        return EXIT_FAILURE;
    }
    
    std::vector<muposys::db::Users*>* userList = muposys::db::Users::select(connector,"person != 0",10,'D');
    for(muposys::db::Users* u : *userList)
    {
        std::cout << "user id=" << u->getPerson().getEnte().getID() << "\n";
    }    
    
    
    
    
    r = rand() % 100000;
    projectName = "proj-";
    projectName += std::to_string(r);    
    muposys::db::SoftwareProjects proj;
    if(proj.insert(connector,projectName))
    {
        std::cout << "insert : " << projectName << " - " << proj.getEnteValue() << " \n";
    }
    else
    {
        std::cerr << "Fallo en insert" << projectName << " \n";
        return EXIT_FAILURE;
    }
    std::string where = "project = ";
    where += std::to_string(proj.getEnteValue());
    std::vector<muposys::db::SoftwareRQs*>* rqsList = muposys::db::SoftwareRQs::select(connector,where,3,'D');
    int lastRQNumber = 1;
    if(rqsList->size() > 0)
    {
        muposys::db::SoftwareRQs* rqlast = rqsList->at(0);
        if(rqlast->downNumber(connector))
        {
            lastRQNumber = rqlast->getNumber();
        }
        else
        {
            std::cerr << "Fallo la descarga de lastRQNumber en '" << projectName << "'\n";
            return EXIT_FAILURE;
        }
    }
    
    r = rand() % 100000;
    rqname1 = "rq-";
    rqname1 += std::to_string(r);    
    muposys::db::SoftwareRQs rq1;
    if(rq1.insert(connector,proj,lastRQNumber++,rqname1,1))
    {
        std::cout << "insert : " << rqname1 << " \n";
    }
    else
    {
        std::cerr << "Fallo en insert" << rqname1 << " \n";
        return EXIT_FAILURE;
    }  
    //std::cout << "Step 1\n";
    
    r = rand() % 100000;
    rqname2 = "rq-";
    rqname2 += std::to_string(r);    
    muposys::db::SoftwareRQs rq2;
    if(rq2.insert(connector,proj,lastRQNumber++,rqname2,1))
    {
        std::cout << "insert : " << rqname2 << " \n";
    }
    else
    {
        std::cerr << "Fallo en insert" << rqname2 << " \n";
        return EXIT_FAILURE;
    }
    
    r = rand() % 100000;
    rqname3 = "rq-";
    rqname3 += std::to_string(r);    
    muposys::db::SoftwareRQs rq3;
    if(rq3.insert(connector,proj,lastRQNumber++,rqname3,1))
    {
        std::cout << "insert : " << rqname3 << " \n";
    }
    else
    {
        std::cerr << "Fallo en insert" << rqname3 << " \n";
        return EXIT_FAILURE;
    }
    if(rq3.downName(connector))
    {
        std::cout << "descarga de (name): " << rqname3 << " \n";
    }
    else
    {
        std::cerr << "Fallo descarga (name) " << rqname3 << " \n";
        return EXIT_FAILURE;
    }    
    if(rq3.downPriority(connector))
    {
        std::cout << "descarga de (priority): " << rqname3 << " \n";
    }
    else
    {
        std::cerr << "Fallo descarga (priority) " << rqname3 << " \n";
        return EXIT_FAILURE;
    }
    std::cout << "In project:" << rq3.getProjectValue() << ", RQ number:" << rq3.getNumber() << ", Name: " << rq3.getName() << "\n";  
    if(rqname3.compare(rq3.getName()) == 0)
    {
        std::cout << "Comprobacion de descarga para " << rqname3 << " \n";
    }
    else
    {
        std::cerr << "Fallo comprobacion descarga " << rqname3 << " \n";
        return EXIT_FAILURE;
    }
    
    std::string stritem1,catalog1,brief1;
    r = rand() % 100000;
    catalog1 = "ctl-";
    catalog1 += std::to_string(r);
    stritem1 = "item-";
    stritem1 += std::to_string(r);  
    brief1 = "brief-";
    brief1 += std::to_string(r);
    muposys::db::Catalog item1;
    if(item1.insert(connector,catalog1,stritem1,brief1))
    {
        std::cout << "insert item: " << stritem1 << " \n";
    }
    else
    {
        std::cerr << "Fallo en insert" << stritem1 << " \n";
        return EXIT_FAILURE;
    }
    
    //std::cout << "Step 2\n";
    connector.commit();
    //std::cout << "Step 3\n";
    /*for(muposys::db::Users* u : *userList)
    {
        delete u;        
    }*/
    delete userList;
    /*for(muposys::db::SoftwareRQs* rq : *rqsList)
    {
        delete rq;        
    }*/
    delete rqsList;
    //std::cout << "Step 4\n";
    connector.close();    
    //std::cout << "Step 5\n";
    return EXIT_SUCCESS;
}
