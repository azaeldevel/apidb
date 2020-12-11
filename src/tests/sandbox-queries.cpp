
#include <iostream>
#include <muposys.hpp>

int main(int argc, char **argv)
{
    std::cout << "Comenzando consultas..\n";
    
    octetos::db::mariadb::Connector connector; 
    bool flag = false;  
	flag = connector.connect(muposys::conector);
    if(flag)
    {
        printf("SQL Server version: %s\n", connector.getVerionServer().toString().c_str());
    }
    else
    {
        return EXIT_FAILURE;
    }
    
    
    std::vector<muposys::Entities*>* le = muposys::Entities::select(connector," id > 0 ");
    for(muposys::Entities* e : *le)
    {
        std::cout << "Ente(" << e->getID() << ")\n";
    }
    delete le;
    
    
    
    return EXIT_SUCCESS;
}
