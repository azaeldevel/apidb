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
 *  author: Azael Reyes
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
		JAVA,
		CSHARP,
		PERL,
		PYTHON,
		CMAKE		
	};
		
	
	class BuildException : public std::exception
	{
	public:
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
            Table* classReferenced;
            Table* classParent;
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
            
            Table();
            ~Table();
			bool basicSymbols(toolkit::clientdb::Connector& connect);
            bool fillKeyType(toolkit::clientdb::Connector& connect,Tables& tables);
            short getCountRefereces()const; 
            
		private:
			short countRef;
		};
		
		/**
		 * Conjunto de tablas
		 * */
		struct Tables: public std::list<Table*>
		{
		public:
			~Tables();	
            Table* search(const std::string&);           
			bool listing(toolkit::clientdb::Connector& connect);
			
		private:			
			short max();
		};
	}
		
	class Analyzer
	{
	public:
		virtual std::ostream& getOutputMessage() = 0;
		virtual std::ostream& getErrorMessage() = 0;
		virtual std::string parse(const std::string& line) = 0;
		
		internal::Tables& getListTable();
			
		std::string getInputLenguajeString()const;
		
		const std::string& getNameProject();
		const std::string& getDirectoryProject();
		void setPramsProject(const std::string& name,const std::string& directory);
		void setPramsLenguajes(InputLenguajes inputLenguaje, OutputLenguajes outputLenguaje);
				
		InputLenguajes getInputLenguaje() const;
		OutputLenguajes getOutputLenguaje() const;
		
    protected:
		internal::Tables symbolsTables;		
		toolkit::clientdb::Connector* connector;
		std::ostream* outputMessages;//out stream
		std::ostream* errorMessages;//out stream
		std::string nameProject;
		std::string directoryProject;
		//flags
		InputLenguajes inputLenguaje;
		OutputLenguajes outputLenguaje;
	};


namespace mysql
{
	class Analyzer : public apidb::Analyzer
	{
	public:
		bool listing(toolkit::clientdb::Connector& connect);
		
		virtual std::ostream& getOutputMessage();
		virtual std::ostream& getErrorMessage();
		
        Analyzer();
        Analyzer(InputLenguajes, OutputLenguajes);		
        virtual ~Analyzer();
        
        /**
         * Parse desde una std::string
         **/
		virtual std::string parse(const std::string& line);
		/** 
		* parse - parse from a file
		* @param filename - valid string with input file
		*/
		void parse(const char * const filename);
		/** 
		* parse - parse from a c++ input stream
		* @param is - std::istream&, valid input stream
		*/
		void parse(std::istream &iss);
		
		void message(const std::string&);		
		std::ostream& print(std::ostream &stream);
		
		//Don't use, is temporal: usada por parse para retorna sui resultado
		std::string oneLine;
	private:
		void parse_helper(std::istream &stream);
		Parser  *parser  = nullptr;
		Scanner *scanner = nullptr;   
	};
}
}
#endif /* END __MCDRIVER_HPP__ */
