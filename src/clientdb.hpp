#ifndef clientdb_hpp
#define clientdb_hpp

#include "toolkit.hpp"

namespace clientdb
{
	toolkit::Version getPakageVersion();
	const char* getPakageName();	
	typedef unsigned int ID;
    
    struct Datconection    
    {        
        
    };
    
    struct DatconectionMySQL : public Datconection
    {
        const char *host;
        const char *usuario;
        const char *password;
        const char *database;
        unsigned int port;
        const char *unix_socket;
        unsigned long client_flag;

        int last_errono;
        const char * last_errmsg;
    };
	
	class Rows
	{
	public:
		virtual void import(void* row) = 0;
	};
    
    class Connector
    {
    private:
        void* serverConnector;
        Datconection* datconection;
    public:
        Connector();
        Connector(DatconectionMySQL& connector);
        bool connect(DatconectionMySQL& connector);
        const char* serverDescription();
        bool query(const std::string&);
        bool query(const std::string&,Rows&);
        ID insert(const std::string&);
        bool commit();
        bool rollback();
        void* getServerConnector();
        const Datconection* getDatconection() const;  
    };    
}

#endif
