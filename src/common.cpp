#include <cctype>
#include <fstream>
#include <cassert>
#include <sstream> 
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
#include <string.h>
#include <stdio.h>

#include "common.hpp"

namespace apidb
{

    const toolkit::clientdb::DatconectionMySQL& ConfigureProject::getConector() const
    {
        return conectordb;    
    }
    const toolkit::Version& ConfigureProject::getVersion()const
    {
        return version;
    }
    const std::string& ConfigureProject::getName()const
    {
        return name;
        
    }
    const std::string& ConfigureProject::getDirectory()const
    {
        return directory;    
    }
        
    bool ConfigureProject::getProjectNodes(xmlTextReaderPtr reader)
    {
        xmlTextReaderRead(reader);
        xmlTextReaderRead(reader);
        xmlTextReaderRead(reader);
        const xmlChar *name = xmlTextReaderConstName(reader);
        if(strcmp((const char*)name,"#text") == 0)
        {
            //std::cout<<"Find text."<<std::endl;   
            this->name = (const char*)xmlTextReaderConstValue(reader);
        }
            
        xmlTextReaderRead(reader);
        xmlTextReaderRead(reader);
        xmlTextReaderRead(reader);
        xmlTextReaderRead(reader);
        name = xmlTextReaderConstName(reader);  
        if(strcmp((const char*)name,"#text") == 0)
        {
            this->directory = (const char*)xmlTextReaderConstValue(reader);
        }
        
        
        xmlTextReaderRead(reader);
        xmlTextReaderRead(reader);
        xmlTextReaderRead(reader);
        xmlTextReaderRead(reader);
        xmlTextReaderRead(reader);
        name = xmlTextReaderConstName(reader);
        if(strcmp((const char*)name,"major") == 0)
        {
            xmlTextReaderRead(reader);
        }
        else
        {
            return true;
        }
        name = xmlTextReaderConstName(reader);
        if(strcmp((const char*)name,"#text") == 0)
        {  
            this->version.major = atoi((const char*)xmlTextReaderConstValue(reader));
        }
        

        xmlTextReaderRead(reader);
        xmlTextReaderRead(reader);
        xmlTextReaderRead(reader);
        name = xmlTextReaderConstName(reader);    
        if(strcmp((const char*)name,"minor") == 0)
        {
            xmlTextReaderRead(reader);
        }
        else
        {
            return true;
        }  
        name = xmlTextReaderConstName(reader);
        if(strcmp((const char*)name,"#text") == 0)
        {
            this->version.minor = atoi((const char*)xmlTextReaderConstValue(reader));
        }
        

        xmlTextReaderRead(reader);
        xmlTextReaderRead(reader);
        xmlTextReaderRead(reader);
        xmlTextReaderRead(reader);
        xmlTextReaderRead(reader);
        name = xmlTextReaderConstName(reader);
        if(strcmp((const char*)name,"patch") == 0)
        {
            xmlTextReaderRead(reader);
        }
        else
        {
            return true;
        }
        name = xmlTextReaderConstName(reader);
        if(strcmp((const char*)name,"#text") == 0)
        {
            this->version.patch = atoi((const char*)xmlTextReaderConstValue(reader));
        }
        
        this->version.stage = toolkit::Version::Stage::alpha;
        
        xmlTextReaderRead(reader);
        xmlTextReaderRead(reader);
        xmlTextReaderRead(reader);
        xmlTextReaderRead(reader);
        xmlTextReaderRead(reader);
        name = xmlTextReaderConstName(reader);
        if(strcmp((const char*)name,"host") == 0)
        {
            xmlTextReaderRead(reader);
        }
        else
        {
            return true;
        }
        name = xmlTextReaderConstName(reader);
        if(strcmp((const char*)name,"#text") == 0)
        {
            this->conectordb.host = (const char*)xmlTextReaderConstValue(reader);
        }
        
        xmlTextReaderRead(reader);
        xmlTextReaderRead(reader);
        xmlTextReaderRead(reader);
        name = xmlTextReaderConstName(reader);
        if(strcmp((const char*)name,"puerto") == 0)
        {
            xmlTextReaderRead(reader);
        }
        else
        {
            return true;
        }
        name = xmlTextReaderConstName(reader);
        if(strcmp((const char*)name,"#text") == 0)
        {
            this->conectordb.port = atoi((const char*)xmlTextReaderConstValue(reader));
        }
        
        xmlTextReaderRead(reader);
        xmlTextReaderRead(reader);
        xmlTextReaderRead(reader);
        name = xmlTextReaderConstName(reader);
        if(strcmp((const char*)name,"nameDB") == 0)
        {
            xmlTextReaderRead(reader);
        }
        else
        {
            return true;
        }
        name = xmlTextReaderConstName(reader);
        if(strcmp((const char*)name,"#text") == 0)
        {
            this->conectordb.database = (const char*)xmlTextReaderConstValue(reader);
        }
        
        xmlTextReaderRead(reader);
        xmlTextReaderRead(reader);
        xmlTextReaderRead(reader);
        name = xmlTextReaderConstName(reader);
        if(strcmp((const char*)name,"user") == 0)
        {
            xmlTextReaderRead(reader);
        }
        else
        {
            return true;
        }
        name = xmlTextReaderConstName(reader);
        if(strcmp((const char*)name,"#text") == 0)
        {
            this->conectordb.usuario = (const char*)xmlTextReaderConstValue(reader);
        }
        
        xmlTextReaderRead(reader);
        xmlTextReaderRead(reader);
        xmlTextReaderRead(reader);
        name = xmlTextReaderConstName(reader);
        if(strcmp((const char*)name,"pw") == 0)
        {
            xmlTextReaderRead(reader);
        }
        else
        {
            return true;
        }
        name = xmlTextReaderConstName(reader);
        if(strcmp((const char*)name,"#text") == 0)
        {
            this->conectordb.password = (const char*)xmlTextReaderConstValue(reader);
        }
        return true;
    }


    bool ConfigureProject::processNode(xmlTextReaderPtr reader) 
    {
        const xmlChar *name = xmlTextReaderConstName(reader);

        if( xmlTextReaderNodeType(reader) == 1) //es d apertura?
        {
            if(strcmp((const char*)name,"project") == 0)
            {
                //std::cout<<(const char*)name<<std::endl;
                getProjectNodes(reader);
            }
        }
        else if( xmlTextReaderNodeType(reader) == 15) //es d cerradura?
        {
            //stack.pop_front();
        }
        
        return true;
    }

    ConfigureProject::ConfigureProject()
    {
        ;
    }
    
    ConfigureProject::ConfigureProject(std::string filename)
    {    
        xmlTextReaderPtr reader;
        int ret;
        inputLenguaje = apidb::InputLenguajes::MySQL_Server;
        outputLenguaje = apidb::OutputLenguajes::CPP;	
        reader = xmlReaderForFile(filename.c_str(), NULL, 0);
        if (reader != NULL) 
        {
            ret = xmlTextReaderRead(reader);               
            if (!processNode(reader)) 
            {
                fprintf(stderr, "%s : failed to parse\n", filename.c_str());
            }
            xmlFreeTextReader(reader);
        }
        else 
        {
            fprintf(stderr, "Unable to open %s\n", filename.c_str());
        }
    }
    
    namespace symbols
	{
		std::string Key::getOutType()
		{
			if(isHomogeneous()) return (*begin())->outType;	
			return "";
		}
		bool Key::isHomogeneous()
		{
			if(size() == 1) return true;
			
			std::vector<Symbol*>::iterator firts = begin();
			std::vector<Symbol*>::iterator actual = begin();
			std::vector<Symbol*>::iterator last  = end();		
			
			while (actual != last) 
			{
				if((*actual)->outType.compare((*firts)->outType) != 0)
				{
					return false;
				}
				actual++;
			}
			return true;
		}
		
        void Tables::reorder()
		{
			short max = getMaxCountRef();
			short i = max;
			std::cout<< "Max number: " << max <<std::endl;
			std::cout<< "Size this: " << size() <<std::endl;
			std::list<Table*> tmp(*this);//copy all datas
			std::cout<< "Size tmp: " << tmp.size() <<std::endl;
			clear();
			std::cout<< "Size this: " << size() <<std::endl;
			std::list<Table*>::iterator actual;
			std::list<Table*>::iterator tmpactual;
			std::list<Table*>::iterator last;
			for(short i = max; i > 0; i--) 
			{
				std::cout<< "for(short i = max; i > 0; i++)" <<std::endl;			
				actual = tmp.begin();
				tmpactual = tmp.begin();
				last = tmp.end();
				while(actual != last)
				{
					if(((*actual)->getCountRefereces()) == i || ((*actual)->getCountRefereces()) == 0)
					{
						std::cout<< i << "-esimo number " <<std::endl;
						push_back(*actual);						
						tmp.erase(actual);
					}	
					actual = tmp.begin();
					tmpactual = tmp.begin();
					last = tmp.end();		
				}
			}
			std::cout<< "Count in this: " << size() <<std::endl;
		}
		
        int Symbol::getID()const
        {
            return id;
        }
        
        Symbol::Symbol()
        {
            counter++;
            id = counter;
            classReferenced = NULL;
            classParent = NULL;
            keyType = NOKEY;
            id = 0;
        }
		int Symbol::counter = 0;	
		
		short Tables::getMaxCountRef()
		{
			std::list<Table*>::iterator actual = begin();
			std::list<Table*>::iterator last = end();
			short m = 0;
			
			while (actual != last) 
			{
				if(((*actual)->getCountRefereces()) > m)
				{
					m = (*actual)->getCountRefereces();					
				}	
				actual++;
			}
			
			return m;
		}
				
		short Table::getCountRefereces()const 
		{
			return countRef;
		}
		
		Table::Table()
		{
			countRef = 0;
			key = NULL;
		}
		
		Table::~Table()
		{
			for (Symbol* symbol : *this)
			{
				delete symbol;
			}	
			clear();
		}
		
		Tables::~Tables()
		{
			for (Table* table : *this)
			{
				delete table;
			}
			clear();
		}
		
		Table* Tables::search(const std::string& tableName)
		{
			std::list<Table*>::iterator actual = begin();
			std::list<Table*>::iterator last = end();
			
			while (actual != last) 
			{
				if ((*actual)->name.compare(tableName) == 0) return (symbols::Table*)(*actual);
				++actual;
			}
			return NULL;
		}		
	}
}
