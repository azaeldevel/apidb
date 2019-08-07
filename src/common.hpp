
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
#include <toolkit/clientdb/clientdb-mysql.hpp>
#include <libxml/xmlreader.h>
#include <iostream>
#include <cstring>

namespace octetos
{
namespace apidb
{

        enum ErrorScope
        {
                PACKAGE,
                Class_ConfigureProject,
                Class_ConfigureProject_ReadFile,
        };
	/*enum InputLenguajes
	{
		MySQL_Server,
		//MySQL_Script,
		//PostgresSQL
	};*/
        typedef octetos::toolkit::clientdb::Datconnect::ServerType InputLenguajes;
	std::string getInputLenguajeString(InputLenguajes);	
        
        /**
         * \brief Identifica los gentores de paquetes disponibles.
         * */
        enum PackingLenguajes
        {
                CMake = 1
        };
        /**
         * \brief Identicia los posibles lenguajes del codigo generado.
         * */
	enum OutputLenguajes
	{
		CPP=1,
                C
		//JAVA,
		//CSHARP,
		//PERL,
		//PYTHON
	};
	/**
         * \deprecated Sera removide en v2 y no es estable ahora. 
         * */
	enum MVC
	{
		NO = 1,
		//X,
		GTK2,
		GTK3
		//WIN32
	};
        /**
         * \brief Distinge entre librerias estaticas y dinamicas para generar.
         * */
        enum Compiled
        {
                STATIC = 1,
                SHARED
        };
        
        /**
         * \brief Clase de manejo de errores.
         **/
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
    
        /**
         * \private Table y elmentos de la tabla de simbolos
         * */
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
		 * \brief Informacion sobre cada symbolo
		 * */
		struct Symbol 
		{
                        friend class Table;
            
			enum KeyType
			{
				NOKEY,
				PRIMARY,
				UNIQUE,
				FOREIGN_UNIQUE,
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
                        Symbol* symbolReferenced;
                        bool isPrimaryKey();
                        bool isForeignKey();
                        bool isAutoIncrement();
            
                        Symbol();
                        int getID()const;
                
                private:
			static int counter;
			int id;	
                        bool isPK;
                        bool isFK;
                        bool isAutoInc;
		};
		
		struct Key : public std::vector<Symbol*>
		{
		};
		/**
		 * \brief Simbolos por alcance(tabla en SQL) 
		 **/
		struct Table : public std::map<const char*,Symbol*,cmp_str>
		{
			std::string name;
                        std::string upperName;
                        Key key;
                        std::list<Symbol*> required;//ademas de porner en true su abtributo se agrega a esta lista    
                        std::string space;
                        std::string fullname;
                        
                        Table();
                        ~Table();
			bool basicSymbols(octetos::toolkit::clientdb::Connector& connect);
                        bool fillKeyType(octetos::toolkit::clientdb::Connector& connect, std::map<const char*,symbols::Tables*,symbols::cmp_str>& tables);
                        short getCountRefereces() const; 
                        //std::list<Symbol*>::iterator search(const std::string&);
            
		private:
			short countRef;
		};
		
		/**
		 * Conjunto de tablas
		 * */
		struct Tables : public std::list<Table*>
		{
		public:
			~Tables();	
                        Table* search(const std::string&); 
			std::list<Table*>::iterator find(const std::string& tableName);
                        std::string name;      
			
			short getMaxCountRef();
                        const std::string& getName();                        
                private:                  
		};
                
                bool listing(octetos::toolkit::clientdb::mysql::Connector& connect, std::map<const char*,symbols::Tables*,symbols::cmp_str>& tables);
                std::string getSpaceName(std::string fullname);
                std::string getSpacePatch(std::string fullname);
                short getSpaceLevel(std::string fullname);
	}
    
  
}
}
#endif
