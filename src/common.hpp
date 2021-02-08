/**
 *
 *  This file is part of apidb.
 *  APIDB do Make easy to connect your Database
 *  Copyright (C) 2018  Azael Reyes
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

#ifndef APIDB_COMMON_HPP
#define APIDB_COMMON_HPP

#include <string>
#include <list>
#include <vector>
#include <map>
#if defined WINDOWS_MINGW
    #include <clientdb.hh>
    #include <Message.hh>
#elif defined LINUX && defined CODEBLOCKS
    #include <clientdb.hh>
    #include <Message.hh>
#else
    #include <octetos/db/clientdb.hh>
    #include <octetos/core/Message.hh>
#endif
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
        namespace postgresql
        {
                class Analyzer;
        }
        namespace mariadb
        {
                class Analyzer;
        }
        class ConfigureProject;

    /*bool createDatconnect(const std::string& host, unsigned int port,const std::string& database,const std::string& usuario,const std::string& password, octetos::db::Datconnect**);
    bool createDatconnect(octetos::db::Datconnect**);
    bool createConnector(octetos::db::Connector**);*/

    typedef octetos::db::Driver InputLenguajes;
    std::string getInputLenguaje(InputLenguajes);
    InputLenguajes getInputLenguaje(const std::string&);

        /**
         * \brief Identifica los gentores de paquetes disponibles.
         * */
        enum PackingLenguajes
        {
            NoPack,
            CMake,
            OnlyCode,
            Maven,
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
            CPP,
            JAVA,
            PHP,
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
	class BuildException : public core::Exception
	{
	public:
                /**
                 * \brief Requerida por toolkit::Error
                 * */
		virtual ~BuildException() throw();
                /**
                 * \brief Requerida por std::exception
                 * */
		//virtual const char* what() const throw();
                /**
                 * \brief Requerida por toolkit::Error
                 * */
		BuildException(const std::string &description) throw();
        BuildException(const std::string &description,const char* filename,int lineNumber) throw();
        //Exception()throw();
	private:
		//std::string description;
	};

    /**
    * \brief Estructura de datos para construir la tabla de simbolos
    **/
	namespace symbols
	{
		class Table;
		class Space;
		class SymbolsTable;

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

        enum SpaceType
        {
            TABLE,
            SPACE
        };
		class ISpace
		{
		public:
			virtual SpaceType what()const = 0;
			virtual ~ISpace(){};
		};

		/**
		 * \brief Informacion sobre cada symbolo
		 * */
		class Symbol
		{
        friend class Analyzer;
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
            //private:
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
            /**
             * 
             **/
            

			static int counter;
			int id;
            bool isPK;
            bool isFK;
            bool isAutoInc;
        public:
			bool isRequiered() const;
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
            * \brief Inicializa los valores
            * */
            ~Symbol();
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
        friend class Analyzer;
        friend class Table;
        public:
            const std::string& getName()const;
            void setName(const std::string&);
        private:
            std::string name;
		};

		/**
		 * \brief Simbolos por alcance(tabla en SQL)
		 **/
		class Table : public ISpace, public std::map<const char*,Symbol*,cmp_str>
		{
        public:
            friend class octetos::apidb::Analyzer;
            //friend class octetos::apidb::mysql::Analyzer;
            //friend class octetos::apidb::postgresql::Analyzer;
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
            virtual bool basicSymbols(octetos::db::Connector& connect) = 0;

            /**
            * \brief Busca los campo que son foraneos y completa la informacion de la tabla de simbolos.
            * */
			virtual bool fillKeyType(octetos::db::Connector& connect, const SymbolsTable&) = 0;
            
            virtual std::string primaryName(octetos::db::Connector& connect) const = 0;

			short countRef;
            /**
            * \brief La cantidad de rencias hechas a esta tabla en BD.
            * */
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
            /**
            * \brief Retorna una string con el nombre de la tabla decorado con mayusculas
            * */
            const std::string& getUpperName()const;
            /**
            * \brief Retorna una lista de los campos requerido en insert(NOT NULL)
            * */
            const std::list<Symbol*>& getRequired()const;
            /**
            * \brief En desarrollo
            * */
            const std::string& getSpace()const;
            /**
            * \brief En desarrollo
            * */
            const std::string& getFullName()const;
            /**
            * \brief Retorna una refencias a la llave de la tabla.
            * */
			const Key& getKey()const;
			virtual SpaceType what()const;
			Symbol* findSymbol(const std::string&);
			//virtual ISpace* searh(const std::string&);
		};

		/**
        * \brief Conjunto de tablas
        * \details No representa un spacio realmente sino que umula uno en base a ciertos creterio en el nombrambramiento de las tablas, esto para poder organizar mejor el codigo. En MySQL por ejemplo, Se permite poner el caracter punto en el nombre de la tabla, y este es el criterio usado para MySQL, APIDB crea un nuevo anidamiento de espacio cada vez que encuentre un punto en el nombre de la tabla.
        * */
		class Space : public ISpace, public std::map<const char*,symbols::ISpace*,symbols::cmp_str>
		{
		public:
            /**
            * \brief Limpa el objeto
            * */
			~Space();
            /**
            * \brief Busca una tabla por su nombre y la retorna en cals de encontrarla
            * */
            //Table* search(const std::string&);
            /**
            * \brief Busca una tabla por su nombre y la retorna en cals de encontrarla
            * */
			//std::list<Table*>::iterator find(const std::string& tableName);
			//short getMaxCountRef();
            /**
            * \brief Retorna el nombre del espacio.
            * */
            //const std::string& getMiddleName()const;
            const std::string& getName()const;
            //const std::string& getFullName()const;
            /**
            * \brief Crea el objeto con su nombre.
            * */
            Space(const std::string& middleName);

            virtual SpaceType what()const;
            //virtual ISpace* searh(const std::string&);
            Table* findTable(const std::string&);
            Space* findSpace(const std::string&);

            /**
            *
            *
            * \param name indica el nombre relativo a apartir del espacio actual
            * */
            Space* addSpace(const std::string& name);
            /**
            *
            * */
            Table* addTable(symbols::Table* table);
            private:
            /**
            * \brief Nombre del espacion
            * */
            std::string name;
            short level;
		};

        /**
        * \private
        **/
        std::string getFirstName(const std::string&fullname);
        /**
        * \private
        * */
        std::string getSpacePatch(const std::string& fullname);
        /**
        * \private
        * */
        short getSpaceLevel(const std::string& fullname);
        std::string getDeepChilName(const std::string& fullname);
        std::string getChilFullName(const std::string& fullname);
        std::string getExcludeChilName(const std::string& fullname);

        bool hasChild(const std::string& fullname);
        std::string getTopName(const std::string& fullname);

        /**
        *\brief Contiene las estructura completa de la tabla de symbolos
        **/
        class SymbolsTable : public std::map<const char*,symbols::ISpace*,symbols::cmp_str>
        {
        private:
            const ConfigureProject* configureProject;

        public:
            /**
            *\brief Libera memoria
            **/
            SymbolsTable(const ConfigureProject&);
            ~SymbolsTable();
            Table* findTable(const std::string&)const;
            Space* addSpace(const std::string&);
            Table* addTable(const std::string&);
            Space* findSpace(const std::string&);
            //ISpace* search(const std::string&)const;
            const ConfigureProject& getConfigureProject()const;
        };
    }

    /**
    * \brief Implementa el modelo de mensajes basodo en la politica de toolkit::ActivityProgress.
    * */
    class Tracer : public core::ActivityProgress
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
        virtual void add(const core::Error&);
        /**
        * \brief Requerida por toolkit::ActivityProgress
        * */
        virtual void add(const core::Exception&);
        /**
        * \brief Requerida por toolkit::ActivityProgress
        * */
        virtual void add(const core::Confirmation&);
        /**
        * \brief Requerida por toolkit::ActivityProgress
        * */
        virtual void add(const core::Warning&);
    };
}
}
#endif
