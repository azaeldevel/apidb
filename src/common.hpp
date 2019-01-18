
#ifndef APIDB_COMMON_HPP
#define APIDB_COMMON_HPP
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
 
 

#include <string>
#include <list>
#include <vector>
#include <map>
#include <clientdb.hpp>
#include <libxml/xmlreader.h>
#include <iostream>
#include <cstring>

namespace apidb
{
	enum InputLenguajes
	{
		MySQL_Server,
		//MySQL_Script,
		//PostgresSQL
	};
	
	enum OutputLenguajes
	{
		C,
		CPP,
		//JAVA,
		//CSHARP,
		//PERL,
		//PYTHON,
		CMAKE		
	};
	
	enum MVC
	{
		NO,
		//X,
		GTK2,
		GTK3
		//WIN32
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
    

	namespace symbols
	{
		struct Table;
		struct Tables;
		
        struct cmp_str
        {
            bool operator()(char const *a, char const *b) const
            {
                return std::strcmp(a, b) < 0;
            }
        };
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
			std::string upperName;
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
		
		struct Key : public std::vector<Symbol*>
		{
			/**
			 * return true si los campos son iguales o es inico, falso en otro caso.
			 * */
			bool isHomogeneous();
			
			/**
			 * 	para las llaves homegenesa retorna el valor de outType, si no lo son returna una cadaena vacia.
			 * */
			std::string getOutType();
		};
		/**
		 * Simbolos por alcance(tabla en SQL) 
		 **/
		struct Table : public std::map<const char*,Symbol*,cmp_str>
		{
			std::string name;
            Symbol* key;
            std::list<Symbol*> required;//ademas de porner en true su abtributo se agrega a esta lista    
            
            Table();
            ~Table();
			bool basicSymbols(toolkit::clientdb::connectors::Connector& connect);
            bool fillKeyType(toolkit::clientdb::connectors::Connector& connect,Tables& tables);
            short getCountRefereces()const; 
            //std::list<Symbol*>::iterator search(const std::string&);
            
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
			std::list<Table*>::iterator find(const std::string& tableName);       
			bool listing(toolkit::clientdb::connectors::Connector& connect);
			short getMaxCountRef();
            bool reorder();
            
        private:
			int floatup();
		};
	}
    class ConfigureProject
    {
    private:
        bool processNode(xmlTextReaderPtr);
        bool getProjectNodes(xmlTextReaderPtr);
        
    public:        
        class Parameters : public std::vector<const char*>
        {
            
        };
        class Function : public std::vector<const Parameters*>
        {
        private:
            std::string name;
        public:
            const std::string& getName() const;
            Function(const std::string&);
            Function();
        };
        class Table : public std::map<const char*, const Function*>
        {
        private:
            std::string name;
        public:
            const std::string& getName() const;
            Table(const std::string&);
            Table();
        };
        
        std::string name; 
        std::string directory;
        toolkit::Version version;
        toolkit::clientdb::datasourcies::MySQL* conectordb;
		InputLenguajes inputLenguaje;
		OutputLenguajes outputLenguaje;
		MVC mvc;
        std::vector<Table> downloads;
		
        ConfigureProject(std::string filename);
        ConfigureProject();
        bool saveConfig();
        bool checkXML(xmlTextReaderPtr);
		
        const std::string& getName()const;
        const std::string& getDirectory()const;
        const toolkit::Version& getVersion()const;
        const toolkit::clientdb::datasourcies::MySQL& getConector()const;
    };
}

#endif
