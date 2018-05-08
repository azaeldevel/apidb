/**
 * 
 *  This file is part of apidb.
 * 
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.

 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.

 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * 
 * */

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
		virtual bool read() = 0;
	};
	
	class MySQLDriver: public Driver
	{
	public:
		const internal::RowsShowTables* getListTable();
		virtual bool read();
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
