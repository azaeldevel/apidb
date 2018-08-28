
#ifndef APIDB_COMMON_HPP
#define APIDB_COMMON_HPP


#include <string>
#include <list>
#include <clientdb.hpp>
#include <libxml/xmlreader.h>
#include <iostream>

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
    

	namespace symbols
	{
		struct Table;
		struct Tables;
		
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
	

    class ConfigureProject
    {
    private:
        bool processNode(xmlTextReaderPtr);
        bool getProjectNodes(xmlTextReaderPtr);
        
    public:    
        std::string name; 
        std::string directory;
        toolkit::Version version;
        toolkit::clientdb::DatconectionMySQL conectordb;
		InputLenguajes inputLenguaje;
		OutputLenguajes outputLenguaje;	
        ConfigureProject(std::string filename);
        ConfigureProject();
        const std::string& getName()const;
        const std::string& getDirectory()const;
        const toolkit::Version& getVersion()const;
        const toolkit::clientdb::DatconectionMySQL& getConector()const;
        bool saveConfig();
    };
}

#endif
