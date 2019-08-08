
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
        /**
         * \brief Determina si los mensajes de Warning(dirigidos a los desarrolladores) seran emitidos
         * */
        static bool ENABLE_DEVEL_WARNING = true;
        /**
         * \brief Determina si los mensajes de Warning(dirigidos a los usuarios) seran emitidos
         * */
        static bool ENABLE_USER_WARNING = true;
        
        class Analyzer;
        namespace mysql
        {
                class Analyzer;
        }
        
        typedef octetos::toolkit::clientdb::Datconnect::ServerType InputLenguajes;
	std::string getInputLenguaje(InputLenguajes);	
        InputLenguajes getInputLenguaje(const std::string&);	
        
        /**
         * \brief Identifica los gentores de paquetes disponibles.
         * */
        enum PackingLenguajes
        {
                NoPack,
                CMake
        };
        std::string getPackingLenguajes(PackingLenguajes);
        PackingLenguajes getPackingLenguajes(const std::string&);
        /**
         * \brief Identicia los posibles lenguajes del codigo generado.
         * */
	enum OutputLenguajes
	{
                NoLang,
                C,
		CPP
		//JAVA,
		//CSHARP,
		//PERL,
		//PYTHON
	};
        std::string getOutputLenguajes(OutputLenguajes);
        OutputLenguajes getOutputLenguajes(const std::string&);
        
        /**
         * \brief Distinge entre librerias estaticas y dinamicas para generar.
         * */
        enum Compiled
        {
                NoCompile,
                STATIC,
                SHARED
        };
        std::string getCompiled(Compiled);
        Compiled getCompiled(const std::string&);
        
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
         * \brief Estructura de datos para construir la tabla de simbolos
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
		 * */
		class Symbol 
		{
                        friend class Analyzer;
                        friend class mysql::Analyzer;
                        friend class Table;
                        
                public:
			/**
                         * \brief Identifica el tipo de llave.
                         * */
			enum KeyType
			{
				NOKEY,
				PRIMARY,
				UNIQUE,
				FOREIGN_UNIQUE,
			};
                private:
			/**
                         * \brief Tipo de dato en Bd para el campo
                         * */
                        std::string inType;      
			/**
                         * \brief Nombre del campo en BD
                         * */
                        std::string name;   
			/**
                         * \brief Nombre del metodo get para leer el campo
                         * */
                        std::string get;       
			/**
                         * \brief Nombre del campo decorado con mayusculas
                         * */
			std::string upperName;       
			/**
                         * \brief Determina si es campo es requierido
                         * */
			bool required;             
			/**
                         * \brief Tipo de llave
                         * */
                        KeyType keyType;      
			/**
                         * \brief Puntero hacia la tabla referido por el campo en el caso de llaves foraneas.
                         * */
                        Table* classReferenced;  
			/**
                         * \brief Clase contenedora del campo.
                         * */
                        Table* classParent;    
			/**
                         * \brief Tipo de dato en el lenguaje resultado
                         * */
                        std::string outType;         
			/**
                         * \brief Puntero al campo referido en el caso de campo foraneos.
                         * */
                        Symbol* symbolReferenced;   
                        
			static int counter;
			int id;	
                        bool isPK;
                        bool isFK;
                        bool isAutoInc;
                        
                public:			
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
                        /**
                         * \brief retorna la string correspodiente al tipo de dato en la BD.
                         * */
                        const std::string& getInType()const;
		};
		
                /**
                 * \brief Conjunto de campos que forma una llave, cuan la llave es compuesta se hace una entrada por cada campo relacionado.
                 * */
		class Key : public std::vector<Symbol*>
		{
		};
		/**
		 * \brief Simbolos por alcance(tabla en SQL) 
		 **/
		class Table : public std::map<const char*,Symbol*,cmp_str>
		{
                        friend class octetos::apidb::Analyzer;
                        friend class octetos::apidb::mysql::Analyzer;
                        /**
                         * \brief Nombre de la tabla
                         * */
			std::string name;
                        /**
                         * \brief Nombre de la tabla decorado con mayusculas
                         * */
                        std::string upperName;
                        /**
                         * \brief LLave de la tabla
                         * */
                        Key key;
                        /**
                         * Lista los campos requerido(para uso de insert)
                         * */
                        std::list<Symbol*> required;//ademas de porner en true su abtributo se agrega a esta lista    
                        /**
                         * \brief Nombre del spacio si lo tiene
                         * */
                        std::string space;
                        /**
                         * \brief Nombre completa de la tabla
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

			short countRef;
                public:
                        short getCountRefereces() const; 
                        /**
                         * \brief Simplemete crea el obejto con valores limpios
                         * */
                        Table(const std::string& );
                        /**
                         * \brief Libera la memoroa del Objeto.
                         * */
                        ~Table();
                        /**
                         * \brief Retorna el nombre de la tabla.
                         * */
                        const std::string& getName()const;                          
                        const std::string& getUpperName()const;           
                        const std::list<Symbol*>& getRequired()const;           
                        const std::string& getSpace()const;           
                        const std::string& getFullName()const;   
                        const Key& getKey()const;
		};
		
		/**
		 * \brief Conjunto de tablas
                 * \details No representa un spacio realmente sino que umula uno en base a ciertos creterio en el nombrambramiento de las tablas, esto para poder organizar mejor el codigo. En MySQL por ejemplo, Se permite poner el caracter punto en el nombre de la tabla, y este es el criterio usado para MySQL, APIDB crea un nuevo anidamiento de espacio cada vez que encuentre un punto en el nombre de la tabla.
		 * */
		class Space : public std::list<Table*>
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
			//short getMaxCountRef();
                        /**
                         * \brief Retorna el nombre del espacio.
                         * */
                        const std::string& getName()const;  
                        /**
                         * \brief Crea el objeto con su nombre.
                         * */
                        Space(const std::string name);
                        
                private:                        
                        /**
                         * \brief Nombre del espacio.
                         * */
                        std::string name;     
		};
                
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
        class Tracer : public toolkit::ActivityProgress
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
