
#include <octetos/db/clientdb-postgresql.hh>

#include "dynamic.hh"


void destroyConector(octetos::db::Connector* obj)
{
    delete (octetos::db::postgresql::Datconnect*)obj;
}

octetos::db::Connector* createConnector()
{
    octetos::db::postgresql::Connector* conn = new octetos::db::postgresql::Connector();
    return conn;
} 

/*octetos::db::Datconnect* createDatconnect(const std::string& host, unsigned int port,const std::string& database,const std::string& usuario,const std::string& password)
{
    return new octetos::db::postgresql::Datconnect(host,port,database,usuario,password);
}*/


octetos::db::Datconnect* createDatconnect()
{
    return new octetos::db::postgresql::Datconnect();
}
void createDatconnect(octetos::db::Datconnect* dat)
{
    delete (octetos::db::postgresql::Datconnect*)dat;
}
