#ifndef APIDB_HPP
#define APIDB_HPP

#include "toolkit.hpp"

namespace apidb
{
	toolkit::Version getPakageVersion();
	const char* getPakageName();
	
	class Driver
	{
	public:		
		virtual void listTables() = 0;
		virtual void listAttrib(std::string) = 0;
	};
	
	class MySQLDriver: public Driver
	{
	public:
		virtual void listTables();
		virtual void listAttrib(std::string);
		MySQLDriver();
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
