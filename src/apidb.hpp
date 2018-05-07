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
		struct RowShowTables
		{
			std::string table_name;
		};
		
		class RowsShowTables: public clientdb::Rows, public std::list<RowShowTables*>
		{
		public:
			virtual void import(void* row);
		};		
	}
	
	class Driver
	{
	public:		
		virtual bool listTables() = 0;
		virtual std::vector<std::string> listAttrib(std::string table) = 0;
		virtual std::string getTypeAttrib(std::string attrib) = 0;
	};
	
	class MySQLDriver: public Driver
	{
	public:
		const internal::RowsShowTables* getListTable();
		virtual bool listTables();
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
