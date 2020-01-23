
#include <octetos/db/clientdb-mysql.hh>

#include "dynamic.hh"


void destroyConector(octetos::db::Connector* obj)
{
    delete (octetos::db::mysql::Datconnect*)obj;
}

octetos::db::Connector* createConnector()
{
    return new octetos::db::mysql::Connector();
} 

/*octetos::db::Datconnect* createDatconnect(const std::string& host, unsigned int port,const std::string& database,const std::string& usuario,const std::string& password)
{
    return new octetos::db::mysql::Datconnect(host,port,database,usuario,password);
}*/


octetos::db::Datconnect* createDatconnect()
{
    return new octetos::db::mysql::Datconnect();
}
void destroyDatconnect(octetos::db::Datconnect* dat)
{
    delete (octetos::db::mysql::Datconnect*)dat;
}
