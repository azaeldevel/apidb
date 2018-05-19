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
	
	class CPPGenerator: public Generator
	{
    public:
		virtual bool generate(apidb::Driver& driver);
    private:
        void createSpaceH(apidb::Driver& driver,std::ofstream& file);
        void createClassH(apidb::Driver& driver,const apidb::internal::Table&,std::ofstream&,const std::string&);
        void createClassPrivateH(std::ofstream&);
        void createClassAttributesH(apidb::Driver& driver,const apidb::internal::Table&,std::ofstream&);
        void createClassPublicH(std::ofstream&);
        void createClassMethodesH(apidb::Driver& driver,const apidb::internal::Table&,std::ofstream&);
        void createSpaceCPP(apidb::Driver& driver,std::ofstream& file);
        void createClassCPP(apidb::Driver& driver,const apidb::internal::Table&,std::ofstream&,const std::string&);
        void createClassPrivateCPP(std::ofstream&);
        void createClassAttributesCPP(apidb::Driver& driver,const apidb::internal::Table&,std::ofstream&);
        void createClassPublicCPP(std::ofstream&);
        void createClassMethodesCPP(apidb::Driver& driver,const apidb::internal::Table&,std::ofstream&);
        void writeDefaultContructorH(apidb::Driver& driver,const apidb::internal::Table&,std::ofstream&);
        void writeDefaultContructorC(apidb::Driver& driver,const apidb::internal::Table&,std::ofstream&);
	};	
    
	class CG: public apidb::Driver
	{
	public:
		virtual bool analyze();
		virtual bool generate();
		CG(const std::string& name,const std::string& directory,const toolkit::clientdb::Datconection& datconection);
	private:
		toolkit::clientdb::Connector* connector;
	};
}

#endif
