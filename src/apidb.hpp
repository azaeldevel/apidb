/**
 * 
 *  This file is part of apidb.
 * 
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * 
 * */

#ifndef APIDB_HPP
#define APIDB_HPP

#include <clientdb.hpp>
#include <list>
#include <vector>
#include <iostream>
#include <fstream>

#include "generator-sql/driver.hpp"

namespace apidb
{
	toolkit::Version getPakageVersion();
	const char* getPakageName();
        
	class Generator
	{
	public:
		virtual bool generate(const apidb::Driver& driver,std::ofstream&,const std::string& space) = 0;
	};
    
	class MySQLDriver: public apidb::Driver
	{
	public:
		virtual bool read();
		MySQLDriver();
	private:
		toolkit::clientdb::Connector* connector;
	};
		
	class CPPGenerator: public Generator
	{
    public:
		virtual bool generate(const apidb::Driver& driver,std::ofstream&,const std::string& space);
    private:
        void createSpace(const apidb::Driver& driver,std::ofstream& file,const std::string& space);
        void createClass(const apidb::internal::Table*,std::ofstream&,const std::string&);
        void createClassPrivate(std::ofstream&);
        void createClassAttributes(const apidb::internal::Table*,std::ofstream&);
        void createClassPublic(std::ofstream&);
        void createClassMethodes(const apidb::internal::Table*,std::ofstream&);
	};	
}

#endif
