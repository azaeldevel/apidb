
#if defined WINDOWS_MINGW
    #include <clientdb-maria.hh>
#else
    #include <octetos/db/clientdb-maria.hh>
#endif

#include "dynamic.hh"


void destroyConector(octetos::db::Connector* obj)
{
    delete (octetos::db::mariadb::Datconnect*)obj;
}

octetos::db::Connector* createConnector()
{
    return new octetos::db::mariadb::Connector();
}

/*octetos::db::Datconnect* createDatconnect(const std::string& host, unsigned int port,const std::string& database,const std::string& usuario,const std::string& password)
{
    return new octetos::db::mysql::Datconnect(host,port,database,usuario,password);
}*/


octetos::db::Datconnect* createDatconnect()
{
    return new octetos::db::mariadb::Datconnect();
}
void destroyDatconnect(octetos::db::Datconnect* dat)
{
    delete (octetos::db::mariadb::Datconnect*)dat;
}
