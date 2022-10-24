
#ifndef OCTETOS_APIDB_INPUTS_HH
#define OCTETOS_APIDB_INPUTS_HH

#ifdef LINUX_ARCH 
    #ifdef APIDB_MYSQL
        #include <octetos/db/clientdb-mysql.hh>
    #endif
    #ifdef APIDB_POSTGRESQL
        #include <octetos/db/clientdb-postgresql.hh>
    #endif
    #ifdef APIDB_MARIADB
        #include <octetos/db/clientdb-maria.hh>
    #endif
	#include <octetos/db/clientdb.hh>
#else
    #error "Unknow Platform"
#endif

#include <vector>


namespace oct::api
{


struct DataSource
{
	virtual const char* engine_name()const = 0;        
};
struct DataSourceMaria : public DataSource
{
	const char* host;
	const char* user;
	const char* passwd;
	const char* db;
	unsigned int port;
	const char* socket;
	unsigned long flags;


	virtual const char* engine_name()const;  

};
struct DataSourcePostgreSQL : public DataSource
{
	virtual const char* engine_name()const;        
};

struct Field
{
	enum Out_Type
	{
		UNKNOW,
		INT,
		SHORT,
		LONG,
		CHAR,
		UINT,
		USHORT,
		ULONG,
		FLOAT,
		DOUBLE,
		CSTRING,
	};
	
	const char* name;
	const char* in_str;
	const char* out_str;
	Out_Type out_type;

	bool isPK;
	bool isFK;
};
struct Table : public std::vector<Field>
{
        const char* name;
};
struct Space : public std::vector<Table>
{
        const char* name;
};

class Input
{
public:
        virtual void read(const char* space) = 0;
        
protected:
        std::vector<Space> space;
};
/**
* \brief SQL estandar
* */
class InputSQL: public Input
{
public:
        InputSQL(octetos::db::Connector&);
        InputSQL(const octetos::db::Datconnect&);
        InputSQL(const char* server,unsigned int port, const char* user,const char* password);        
        virtual ~InputSQL();
        virtual void read(const char* space);
        virtual void listing_tables(const char* space);
        
        static const char* schema_name;
        
protected:
        octetos::db::Connector* conn;
        bool conn_free;
        
};
/**
* \brief Es un analizador compatible en lo posible con MariaDB/MySQL, su funcion es servir como base para los analizadores especificos correspondientes.
* */
class InputMM: public InputSQL
{
public:
        InputMM(octetos::db::Connector&);
        InputMM(const octetos::db::Datconnect&);
        InputMM(const char* server,unsigned int port, const char* user,const char* password);
        virtual void read(const char* space);

};
class InputMySQL: public InputMM
{
public:
        InputMySQL(octetos::db::Connector&);
        virtual void read(const char* space);
};
class InputMaria: public InputMM
{
public:
        InputMaria(octetos::db::Connector&);
        InputMaria(const octetos::db::Datconnect&);
        InputMaria(const char* server,unsigned int port, const char* user,const char* password);
        virtual void read(const char* space);
};


}

#endif