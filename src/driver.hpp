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
#include "analyzer.hpp"
#include "generator.hpp"


namespace apidb
{
	class BuildException : public std::exception
    {
    public:
        //exception () throw();
        //exception (const exception&) throw();
        //exception& operator= (const exception&) throw();
        virtual ~BuildException() throw();
        virtual const char* what() const throw();
        BuildException(const std::string &description) throw();
        //Exception()throw();
	private:
        std::string description;
    };
    
	namespace internal
	{
		class Table;
		class Tables;
		
		/**
		 * Informacion sobre cada symbolo
		 * */
		struct Symbol 
		{
			enum KeyType
			{
				PRIMARY,
				UNIQUE,
				FOREIGN_UNIQUE,
				NOKEY
			};
					
			//in input lenguaje
            std::string inType;
            //in input lenguaje
            std::string name;                
            //in out lenguaje
            std::string get;
			bool required;
            KeyType keyType;
            const Table* classReferenced;
            const Table* classParent;
            std::string outType; 
            //bool forInsert; 
            
            Symbol();
            int getID()const;
		private:
			static int counter;
			int id;	
		};
		
		/**
		 * Simbolos por alcance(tabla en SQL) 
		 **/
		struct Table : public std::list<Symbol*>
		{
			std::string name;
            Symbol* key;
            std::list<Symbol*> required;//ademas de porner en true su abtributo se agrega a esta lista
            
            ~Table();
			bool basicSymbols(toolkit::clientdb::Connector& connect);
            bool fillKeyType(toolkit::clientdb::Connector& connect,const Tables& tables); 
		};
		
		/**
		 * Conjunto de tablas
		 * */
		struct Tables: public std::list<Table*>
		{
			~Tables();	
            const Table* search(const std::string&)const;           
			bool listing(toolkit::clientdb::Connector& connect);
		};
	}
	
	
       
	class Driver
	{
	public:
		virtual bool generate();
		virtual bool analyze();
				
		std::string getOutputLenguajeString()const;		
		std::string getInputLenguajeString()const;
		Generator::OutputLenguajes getOutputLenguaje()const;		
		Analyzer::InputLenguajes getInputLenguaje()const;
		
		std::ostream& getOutputMessage();
		std::ostream& getErrorMessage();
				
		const std::string& getHeaderName() const;
		std::ofstream& getSourceOutput();
		std::ofstream& getHeaderOutput();
		
		const std::string& getNameProject();
		void setPramsProject(const std::string& name,const std::string& directory);
		const internal::Tables& getListTable() const;
        
        Driver();
        Driver(Analyzer::InputLenguajes, Generator::OutputLenguajes);
		//Driver(const std::string& name,const std::string& directory,const toolkit::clientdb::Datconection& datconection,InputLenguajes inputLenguaje, OutputLenguajes outputLenguaje);
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
		internal::Tables symbolsTables;
	private:
		void parse_helper( std::istream &stream );
		apidb::parser::MySQL  *parser  = nullptr;
		apidb::scanner::MySQL *scanner = nullptr;
		toolkit::clientdb::Connector* connector;
		//flags
		//std::string outputLenguaje;
		//std::string inputLenguaje;
		Analyzer::InputLenguajes inputLenguaje;
		Generator::OutputLenguajes outputLenguaje;
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
