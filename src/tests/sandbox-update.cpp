
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
    
    octetos::db::maria::Connector connector; 
    bool flag = false;  
	flag = connector.connect(muposys::db::conector);
    if(flag)
    {
        printf("SQL Server version: %s\n", connector.getVerionServer().toString().c_str());
    }
    else
    {
        return EXIT_FAILURE;
    }
    std::cout << "---------------\n";
    
    MYSQL *con = mysql_init(NULL);
    if (con == NULL)
    {
        fprintf(stderr, "mysql_init() failed\n");
        exit(1);
    }
    if (mysql_real_connect(con, muposys::db::conector.getHost().c_str(), muposys::db::conector.getUser().c_str(), muposys::db::conector.getPassword().c_str(),
            muposys::db::conector.getDatabase().c_str(), 0, NULL, 0) == NULL)
    {
        //finish_with_error(con);
    }
    
    std::vector<muposys::db::Entities*>* le;    
    //for(int i = 0; i < 1000; i++)
    {
        //connector.begin();
        le = muposys::db::Entities::select(connector," id > 0 ");
        for(muposys::db::Entities* e : *le)
        {
            //std::cout << "Ente(" << e->getID() << ")\n";
            octetos::core::MD5sum md5(std::to_string(e->getID()));
            if(e->updateMd5sum(connector,md5))
            {
                std::cout << "Actualizado " << e->getID() << "\n";
            }
            else
            {
                std::cout << "Fallo actualizacion " << e->getID() << "\n";
            }
        }
        delete le;
        connector.commit();
    }
    
    
    
    
    return EXIT_SUCCESS;
}
