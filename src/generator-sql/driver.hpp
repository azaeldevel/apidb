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

#ifndef __MCDRIVER_HPP__
#define __MCDRIVER_HPP__ 1

#include <string>
#include <cstddef>
#include <istream>
#include <clientdb.hpp>
#include <list>
#include <iostream>


#include "scanner.hpp"
#include "parser.tab.hh"


namespace apidb
{
	namespace internal
	{
		struct Table: public toolkit::clientdb::Rows
		{
            struct Attribute 
            {
                std::string type;
                std::string name;
                bool required; 
                std::string c_type;
                std::string cpp_type;
            };
			std::string table_name;
            std::list<Attribute*> attributes;        
            
            virtual void import(void*);
		};
		
		class RowsShowTables: public toolkit::clientdb::Rows, public std::list<Table*>
		{
		public:
			virtual void import(void* row);
		};
	}
	
	class Driver
	{
	public:
		virtual bool read();
		const internal::RowsShowTables* getListTable() const;
        
        Driver();

        virtual ~Driver();
        std::string parse(const std::string& line);
	   /** 
		* parse - parse from a file
		* @param filename - valid string with input file
		*/
		void parse( const char * const filename );
		/** 
		* parse - parse from a c++ input stream
		* @param is - std::istream&, valid input stream
		*/
		void parse( std::istream &iss );
		
		void message(const std::string&);
		
		std::ostream& print(std::ostream &stream);
		const std::string& getOutputLenguaje();
		std::string oneLine;//to get the retur from parser
		std::ostream* oss;//out stream
    protected:
		internal::RowsShowTables* rows;
	private:
	   void parse_helper( std::istream &stream );
	   apidb::Parser  *parser  = nullptr;
	   apidb::Scanner *scanner = nullptr;
	   //flags
	   std::string outputLenguaje;
	};

} /* end namespace MC */
#endif /* END __MCDRIVER_HPP__ */
