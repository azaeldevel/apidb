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
#include <iostream>
#include <fstream>

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
		const internal::RowsShowTables* getListTable() const;
    protected:
		internal::RowsShowTables* rows;
	};
    
	class Generator
	{
	public:
		virtual bool generate(const Driver& driver,std::ofstream&,const std::string& space) = 0;
	};
    
	class MySQLDriver: public Driver
	{
	public:
		virtual bool read();
		MySQLDriver();
	private:
		clientdb::Connector* connector;
	};
		
	class CPPGenerator: public Generator
	{
    public:
		virtual bool generate(const Driver& driver,std::ofstream&,const std::string& space);
    private:
        void createSpace(const Driver& driver,std::ofstream& file,const std::string& space);
        void createClass(const internal::Table*,std::ofstream&,const std::string&);
        void createClassPrivate(std::ofstream&);
        void createClassAttributes(const internal::Table*,std::ofstream&);
        void createClassPublic(std::ofstream&);
        void createClassMethodes(const internal::Table*,std::ofstream&);
	};	
}

#endif
