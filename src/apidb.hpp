#ifndef APIDB_HPP
#define APIDB_HPP

#include "clientdb.hpp"
#include <list>
#include <vector>

namespace apidb
{
	toolkit::Version getPakageVersion();
	const char* getPakageName();
	
	namespace internal
	{
		struct Table: public clientdb::Rows
		{
            struct Attribute 
            {
                std::string type;
                std::string name;
                bool required;   
            };
			std::string table_name;
            std::list<Attribute*> attributes;        
            
            virtual void import(void*);
		};
		
		class RowsShowTables: public clientdb::Rows, public std::list<Table*>
		{
		public:
			virtual void import(void* row);
		};		
	}
	
	class Driver
	{
	public:		
		virtual bool parse() = 0;
		virtual std::vector<std::string> listAttrib(std::string table) = 0;
		virtual std::string getTypeAttrib(std::string attrib) = 0;
	};
	
	class MySQLDriver: public Driver
	{
	public:
		const internal::RowsShowTables* getListTable();
		virtual bool parse();
		virtual std::vector<std::string> listAttrib(std::string table);
		virtual std::string getTypeAttrib(std::string attrib);
		MySQLDriver();
	private:
		clientdb::Connector* connector;
		internal::RowsShowTables* rows;
	};
	
	class Generator
	{
	public:
		virtual void generate(const Driver& driver) = 0;
	};
	
	class CPPGenerator: public Generator
	{
		virtual void generate(const Driver& driver);
	};	
}

#endif
