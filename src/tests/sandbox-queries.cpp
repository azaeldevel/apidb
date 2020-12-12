
#include <iostream>
#include <fstream>
#include <muposys.hpp>
#include <mysql/mysql.h>
#include <stdio.h>
#include <stdlib.h>


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
    std::cout << "---------------\n";
    
    MYSQL *con = mysql_init(NULL);
    if (con == NULL)
    {
        fprintf(stderr, "mysql_init() failed\n");
        exit(1);
    }
    if (mysql_real_connect(con, muposys::conector.getHost().c_str(), muposys::conector.getUser().c_str(), muposys::conector.getPassword().c_str(),
            muposys::conector.getDatabase().c_str(), 0, NULL, 0) == NULL)
    {
        //finish_with_error(con);
    }
    clock_t begindirect = clock();
    MYSQL_RES *result;
    int num_fields;
    MYSQL_ROW row;
    int id;
    for(int i = 0; i < 1000; i++)
    {
        if (mysql_query(con, "SELECT id FROM Entities"))
        {
            //finish_with_error(con);
        }
        result = mysql_store_result(con);
        if (result == NULL)
        {
            //finish_with_error(con);
        }
        num_fields = mysql_num_fields(result);
        while ((row = mysql_fetch_row(result)))
        {
            id = atoi(row[0]);
            //printf("Ente(%i)\n", id);
        }
        mysql_free_result(result);
    }
    clock_t enddirect = clock();
    double time_spentdirect = (double)(enddirect - begindirect) / CLOCKS_PER_SEC;
    
    std::cout << "---------------\n";
    
    std::vector<muposys::Entities*>* le;
    clock_t beginapi = clock();
    for(int i = 0; i < 1000; i++)
    {
        le = muposys::Entities::select(connector," id > 0 ");
        for(muposys::Entities* e : *le)
        {
            //std::cout << "Ente(" << e->getID() << ")\n";
        }
        delete le;
    }
    clock_t endapi = clock();  
    double time_spentapi = (double)(endapi - beginapi) / CLOCKS_PER_SEC;
        
    std::cout << "---------------\n";
    
    
    std::cout << "API time : " << time_spentapi << "\n";
    std::cout << "Direct time : " << time_spentdirect << "\n";
    double r = time_spentapi/time_spentdirect;
    std::cout << "Razon : " << r << "\n";
    
    
    
    
    
    std::ofstream stadistica;
    stadistica.open ("stadistica.csv",std::ios::app);
    stadistica << time_spentapi << "," << time_spentdirect  << ",1" << "\n";
    stadistica.flush();
    stadistica.close();
    
    return EXIT_SUCCESS;
}
