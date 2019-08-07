
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
        class Analyzer;
        namespace mysql
        {
                class Analyzer;
        }
        /*enum ErrorScope
        {
                PACKAGE,
                Class_ConfigureProject,
                Class_ConfigureProject_ReadFile,
        };*/
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
	class BuildException : public toolkit::Error
	{
	public:
                /**
                 * \brief Requerida por toolkit::Error
                 * */
		virtual ~BuildException() throw();
                /**
                 * \brief Requerida por std::exception
                 * */
		virtual const char* what() const throw();
                /**
                 * \brief Requerida por toolkit::Error
                 * */
		BuildException(const std::string &description) throw();
        //Exception()throw();
	private:
		//std::string description;
	};
    
        /**
         * \private Table y elmentos de la tabla de simbolos
         * */
	namespace symbols
	{
		struct Table;
		struct Space;
		
                /**
                 * \private 
                 * */
                struct cmp_str
                {
                bool operator()(char const *a, char const *b) const
                {
                        return std::strcmp(a, b) < 0;
                }
                };
        
		/**
		 * \brief Informacion sobre cada symbolo
                 * \private
		 * */
		struct Symbol 
		{
                        friend class Table;
                        friend class mysql::Analyzer;
                        
			/**
                         * \private
                         * */
			enum KeyType
			{
				NOKEY,
				PRIMARY,
				UNIQUE,
				FOREIGN_UNIQUE,
			};
			
					
			/**
                         * \private Este miembro sera convertido en privado en v2
                         * */
                        std::string inType;      
			/**
                         * \private Este miembro sera convertido en privado en v2
                         * */
                        std::string name;   
			/**
                         * \private Este miembro sera convertido en privado en v2
                         * */
                        std::string get;       
			/**
                         * \private Este miembro sera convertido en privado en v2
                         * */
			std::string upperName;       
			/**
                         * \private Este miembro sera convertido en privado en v2
                         * */
			bool required;             
			/**
                         * \private Este miembro sera convertido en privado en v2
                         * */
                        KeyType keyType;      
			/**
                         * \private Este miembro sera convertido en privado en v2
                         * */
                        Table* classReferenced;  
			/**
                         * \private Este miembro sera convertido en privado en v2
                         * */
                        Table* classParent;    
			/**
                         * \private Este miembro sera convertido en privado en v2
                         * */
                        std::string outType;         
			/**
                         * \private Este miembro sera convertido en privado en v2
                         * */
                        Symbol* symbolReferenced;   
                        
                        
			/**
                         * \brief Indica si el compo es un llave primaria
                         * */
                        bool isPrimaryKey();    
			/**
                         * \brief Indica si el compo es un llave foranea
                         * */
                        bool isForeignKey();    
			/**
                         * \brief Indica si el compo es auto incremento
                         * */
                        bool isAutoIncrement();
                            
			/**
                         * \brief Inicializa los valores
                         * */
                        Symbol();    
			/**
                         * \brief El ID de simbolo.
                         * */
                        int getID()const;
                
                        /**
                         * \brief Retorna el nombre del campo.
                         * */
                        const std::string& getName()const;
                        /**
                         * \brief Retorna el nombre del campo decorado con mayusculas.
                         * */
                        const std::string& getUpperName()const;
                        /**
                         * \brief Retorna el nombre del metodo get para el campo.
                         * */
                        const std::string& getGet()const;
                        /**
                         * \brief Retorna el tipo de dato correspodiente a el lenguaje resultado.
                         * \details Por ejemplo, generará int si en la base de datos el campo es entero y el lenguaje resultado es C++ o C.
                         * */
                        const std::string& getOutType()const;
                        /**
                         * \brief Tabla referenciada por el campo(En el caso de las llaves foraneas).
                         * */
                        const Table* getClassReferenced()const;
                        /**
                         * \brief Tabla contenedora del campo
                         * */
                        const Table* getClassParent()const;
                        /**
                         * \brief Campo referenciado por este campo(En el caso de las llaves foraneas).
                         * */
                        const Symbol* getSymbolReferenced()const;
                        
                private:
			static int counter;
			int id;	
                        bool isPK;
                        bool isFK;
                        bool isAutoInc;
		};
		
                /**
                 * \private
                 * */
		struct Key : public std::vector<Symbol*>
		{
		};
		/**
		 * \brief Simbolos por alcance(tabla en SQL) 
                 * \private
		 **/
		struct Table : public std::map<const char*,Symbol*,cmp_str>
		{
                        friend class octetos::apidb::Analyzer;
                        friend class octetos::apidb::mysql::Analyzer;
                        
                //private:
                        /**
                         * \brief Nombre de la tabla
                         * \private Este miembro sera convertido en privado en v2
                         * */
			std::string name;
                        /**
                         * \private Este miembro sera convertido en privado en v2
                         * */
                        std::string upperName;
                        /**
                         * \private Este miembro sera convertido en privado en v2
                         * */
                        Key key;
                        /**
                         * \private Este miembro sera convertido en privado en v2
                         * */
                        std::list<Symbol*> required;//ademas de porner en true su abtributo se agrega a esta lista    
                        /**
                         * \private Este miembro sera convertido en privado en v2
                         * */
                        std::string space;
                        /**
                         * \private Este miembro sera convertido en privado en v2
                         * */
                        std::string fullname;
                        
                        /**
                         * \brief Busca todos lo campos de la tabla actual y construlle la tabla de simbolos
                         * */
			bool basicSymbols(octetos::toolkit::clientdb::Connector& connect);
                        /**
                         * \brief Busca los campo que son foraneos y completa la informacion de la tabla de simbolos.
                         * */
                        bool fillKeyType(octetos::toolkit::clientdb::Connector& connect, std::map<const char*,symbols::Space*,symbols::cmp_str>& tables);
                        /**
                         * \brief Cuanta la cantidad de hay hacia tabla.
                         * */
                //public:
                        short getCountRefereces() const; 
                        //std::list<Symbol*>::iterator search(const std::string&);
                        /**
                         * \brief Retorna el nombre de la tabla.
                         * */
                        /**
                         * \brief Simplemete crea el obejto con valores limpios
                         * */
                        Table(const std::string& );
                        /**
                         * \brief Libera la memoroa del Objeto.
                         * */
                        ~Table();
                        const std::string& getName()const;                          
                        const std::string& getUpperName()const;           
                        const std::list<Symbol*>& getRequired()const;           
                        const std::string& getSpace()const;           
                        const std::string& getFullName()const;   
                        const Key& getKey()const;
		private:
			short countRef;
		};
		
		/**
		 * \brief Conjunto de tablas
                 * \details No representa un spacio realmente sino que umula uno en base a ciertos creterio en el nombrambramiento de las tablas, esto para poder organizar mejor el codigo. En MySQL por ejemplo, Se permite poner el caracter punto en el nombre de la tabla, y este es el criterio usado para MySQL, APIDB crea un nuevo anidamiento de espacio cada vez que encuentre un punto en el nombre de la tabla.
                 * \private
		 * */
		struct Space : public std::list<Table*>
		{
		public:
                        /**
                         * \brief Limpa el objeto
                         * */
			~Space();	
                        /**
                         * \brief Busca una tabla por su nombre y la retorna en cals de encontrarla
                         * */
                        Table* search(const std::string&); 
                        /**
                         * \brief Busca una tabla por su nombre y la retorna en cals de encontrarla
                         * */
			std::list<Table*>::iterator find(const std::string& tableName);        
			/**
                         * \deprecated Funcion inestable se removera en v2.
                         * */
			short getMaxCountRef();
                        /**
                         * \brief Retorna el nombre del espacio.
                         * */
                        const std::string& getName()const;  
                        /**
                         * \brief Crea el objeto con su nombre.
                         * */
                        Space(const std::string name);
                        
                //private:                        
                        /**
                         * \brief Nombre del espacio.
                         * \private Este campo sera convertir en privado en v2. Use getName en su lugar.
                         * */
                        std::string name;     
		};
                /**
                 * \brief es confusi usar tables aveces pasa sesapercivida la 's', sin embargo se mantiene crea este alias para mantener compatibilidad.
                 * */
                typedef Space Tables;
                
                /**
                 * \private
                 * */
                std::string getSpaceName(std::string fullname);
                /**
                 * \private
                 * */
                std::string getSpacePatch(std::string fullname);
                /**
                 * \private
                 * */
                short getSpaceLevel(std::string fullname);
	}
    
        /**
         * \brief Implementa el modelo de mensajes basodo en la politica de toolkit::ActivityProgress.
         * */
        class Tracer : toolkit::ActivityProgress
        {
        public:
                /**
                 * \brief Requerida por toolkit::ActivityProgress
                 * */
                Tracer(int activities);
                /**
                 * \brief Requerida por toolkit::ActivityProgress
                 * */
                virtual void add(const std::string&);
                /**
                 * \brief Requerida por toolkit::ActivityProgress
                 * */
                virtual void add(const toolkit::Error&);
                /**
                 * \brief Requerida por toolkit::ActivityProgress
                 * */
                virtual void add(const toolkit::Confirmation&);
                /**
                 * \brief Requerida por toolkit::ActivityProgress
                 * */
                virtual void add(const toolkit::Warning&);
        };
}
}
#endif
