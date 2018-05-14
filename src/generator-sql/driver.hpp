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
		class RowsShowTables;
		
		class Table
		{
		public:
            struct Symbol 
            {
				enum KeyType
				{
					PRIMARY,
					UNIQUE,
					NOTKEY
				};
				
                std::string sqlType;
                std::string name;
                std::string get;
                bool required; 
                KeyType keyType;
                const Table* classReferenced;
                const Table* classParent;
                std::string c_type;
                std::string cpp_type;
                std::string java_type; 
                bool forInsert;               
            };
		public:
			std::string name;
            std::list<Symbol*> attributes;
            Symbol* key;
			bool basicSymbols(toolkit::clientdb::Connector& connect);
            bool fillKeyType(toolkit::clientdb::Connector& connect,const RowsShowTables& tables); 
		};
		
		class RowsShowTables: public std::list<Table*>
		{
		public:		
            const Table* search(const std::string&)const;           
			bool listing(toolkit::clientdb::Connector& connect);
		};
	}
	
	
	class Generator
	{
	public:
		virtual bool generate(apidb::Driver& driver) = 0;
	};
       
	class Driver
	{
	public:
		enum InputLenguajes
		{
			MySQL_Server,
			MySQL_Script,
			PostgresSQL
		};
		enum OutputLenguajes
		{
			C,
			CPP,
			Java,
			CSHARP,
			Perl,
			Python
		};
	public:
		virtual bool generate();
		virtual bool analyze();
				
		std::string getOutputLenguajeString()const;		
		std::string getInputLenguajeString()const;
		OutputLenguajes getOutputLenguaje()const;		
		InputLenguajes getInputLenguaje()const;
		
		std::ostream& getOutputMessage();
		std::ostream& getErrorMessage();
				
		const std::string& getHeaderName() const;
		std::ofstream& getSourceOutput();
		std::ofstream& getHeaderOutput();
		
		const std::string& getNameProject();
		void setPramsProject(const std::string& name,const std::string& directory);
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
		
		std::string oneLine;//to get the retur from parser
    protected:
		internal::RowsShowTables* rows;
	private:
		void parse_helper( std::istream &stream );
		apidb::Parser  *parser  = nullptr;
		apidb::Scanner *scanner = nullptr;
		//flags
		//std::string outputLenguaje;
		//std::string inputLenguaje;
		InputLenguajes inputLenguaje;
		OutputLenguajes outputLenguaje;
		std::ostream* outputMessages;//out stream
		std::ostream* errorMessages;//out stream
		std::ofstream* writeResults;//erreglo de writeoutput files
		std::string nameProject;
		std::string projectH;
		std::string projectCPP;
		std::string directoryProject;
	   
	};

}
#endif /* END __MCDRIVER_HPP__ */
