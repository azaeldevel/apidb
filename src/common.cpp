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

#include <cctype>
#include <fstream>
#include <cassert>
#include <sstream> 
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
#include <string.h>
#include <stdio.h>
#include <iostream>
#include <map>


#include "common.hpp"

namespace apidb
{
    /*std::list<apidb::symbols::Symbol*>::iterator apidb::symbols::Table::search(const std::string& name)
    {			
        auto it = find(name.c_str());
        return it;
        
    }*/
    const std::string& ConfigureProject::Table::getName() const
    {
        return name;        
    }
    ConfigureProject::Table::Table(const std::string& name)
    {
        this->name = name;
    }
    ConfigureProject::Table::Table()
    {
        
    }
    
    const ConfigureProject::Parameters& ConfigureProject::Function::getParameters() const 
    {
        return *header;
    }
    void ConfigureProject::Function::setHeader(const Parameters* header)
    {
        this->header = header;
    }
    ConfigureProject::Function::Skeleton ConfigureProject::Function::getSkeleton() const
    {
    	return skeleton;
    }
    const std::string& ConfigureProject::Function::getName() const
    {
        return name;
    }
    ConfigureProject::Function::Function(const std::string& name,Skeleton skeleton)
    {
        this->name = name;
        this->skeleton = skeleton;
    }
    ConfigureProject::Function::Function()
    {
        
    }
            
	bool ConfigureProject::checkXML(xmlTextReaderPtr reader)
	{
		/*const xmlChar *name;
        name = xmlTextReaderConstName(reader);
        if(strcmp((const char*)name,"project") != 0)
        {
        	return false;
        }
        
        xmlTextReaderRead(reader);
        xmlTextReaderRead(reader);
        name = xmlTextReaderConstName(reader);
        if(strcmp((const char*)name,"name") != 0)
        {
        	return false;
        }*/
        
        
        
        return true;
	}
	
	bool ConfigureProject::saveConfig()
	{
		xmlDocPtr doc  = xmlNewDoc((const xmlChar *)"1.0");
		xmlNodePtr root_node = xmlNewNode(NULL, (const xmlChar *)"project");
		xmlDocSetRootElement(doc, root_node);
		
		xmlNewChild(root_node, NULL, (const xmlChar *)"name", (const xmlChar *)name.c_str());
		//xmlNewChild(root_node, NULL, (const xmlChar *)"directory", (const xmlChar *)directory.c_str());
				
		xmlNodePtr version_node = xmlNewChild(root_node, NULL, (const xmlChar *)"version", NULL);
		xmlNewChild(version_node, NULL, (const xmlChar *)"major", (const xmlChar *)std::to_string(version.getMajor()).c_str());
		xmlNewChild(version_node, NULL, (const xmlChar *)"minor", (const xmlChar *)std::to_string(version.getMinor()).c_str());
		xmlNewChild(version_node, NULL, (const xmlChar *)"patch", (const xmlChar *)std::to_string(version.getPatch()).c_str());
		//xmlNewChild(version_node, NULL, (const xmlChar *)"stage", (const xmlChar *)version.stage );
		
		xmlNodePtr db_node = xmlNewChild(root_node, NULL, (const xmlChar *)"ConectorDB", NULL);
		if(inputLenguaje == apidb::InputLenguajes::MySQL_Server)
		{
			//toolkit::clientdb::DatconectionMySQL* dat = (toolkit::clientdb::DatconectionMySQL*)connector;
			xmlNewChild(db_node, NULL, (const xmlChar *)"host", (const xmlChar *)conectordb->getHost().c_str());
			xmlNewChild(db_node, NULL, (const xmlChar *)"port", (const xmlChar *)std::to_string(conectordb->getPort()).c_str());
			xmlNewChild(db_node, NULL, (const xmlChar *)"nameDB", (const xmlChar *)conectordb->getDatabase().c_str());
			xmlNewChild(db_node, NULL, (const xmlChar *)"user", (const xmlChar *)conectordb->getUser().c_str());
			xmlNewChild(db_node, NULL, (const xmlChar *)"pw", (const xmlChar *)conectordb->getPassword().c_str());
		}
		else
        {
            return false;
        }
		
        std::string nmFile = "";
		if((directory.empty()) || (directory.compare(".") == 0))
		{
			nmFile = "apidb";
		}
		else
        {
            std::ifstream ifile(directory);
            if (ifile) 
            {            
                nmFile = directory + "/apidb";
            }
            else
            {            
                return false;
            }
        }
        
		int ret = xmlSaveFormatFileEnc(nmFile.c_str(), doc, "UTF-8", 1);	
		xmlFreeDoc(doc);
		xmlCleanupParser();
		if( ret == -1) return false;	
		return true;
	}

	
    const toolkit::clientdb::mysql::Datasource& ConfigureProject::getConector() const
    {
        return *conectordb;    
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
        short major = 0;
        if(strcmp((const char*)name,"#text") == 0)
        {  
            major = atoi((const char*)xmlTextReaderConstValue(reader));
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
        short minor = 0;
        if(strcmp((const char*)name,"#text") == 0)
        {
            minor = atoi((const char*)xmlTextReaderConstValue(reader));
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
        short patch = 0;
        if(strcmp((const char*)name,"#text") == 0)
        {
            patch = atoi((const char*)xmlTextReaderConstValue(reader));
        }
        
        this->version.set(major,minor,patch,toolkit::Version::Stage::alpha);
        
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
        std::string host = "";
        if(strcmp((const char*)name,"#text") == 0)
        {
            host = (const char*)xmlTextReaderConstValue(reader);
        }
        
        xmlTextReaderRead(reader);
        xmlTextReaderRead(reader);
        xmlTextReaderRead(reader);
        name = xmlTextReaderConstName(reader);
        if(strcmp((const char*)name,"port") == 0)
        {
            xmlTextReaderRead(reader);
        }
        else
        {
            return true;
        }
        name = xmlTextReaderConstName(reader);
        int port = 0;
        if(strcmp((const char*)name,"#text") == 0)
        {
            port = atoi((const char*)xmlTextReaderConstValue(reader));
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
        std::string database = "";
        if(strcmp((const char*)name,"#text") == 0)
        {
            database = (const char*)xmlTextReaderConstValue(reader);
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
        std::string user = "";
        if(strcmp((const char*)name,"#text") == 0)
        {
            user = (const char*)xmlTextReaderConstValue(reader);
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
        std::string password = "";
        if(strcmp((const char*)name,"#text") == 0)
        {
            password = (const char*)xmlTextReaderConstValue(reader);
        }
        
        conectordb = new toolkit::clientdb::mysql::Datasource(host,port,database,user,password);
        
        return true;
    }


    bool ConfigureProject::processNode(xmlTextReaderPtr reader) 
    {
        const xmlChar *name = xmlTextReaderConstName(reader);

        if(xmlTextReaderNodeType(reader) == 1) //es d apertura?
        {        	
            if(checkXML(reader))
            {
                //std::cout<<(const char*)name<<std::endl;
                getProjectNodes(reader);
            }
            else
            {
            	std::cerr << "El XML no cumple con el formato requerido(el roden de los nodos es importante)" << std::endl;
            	return false;
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
        conectordb = NULL;
    }
    
    ConfigureProject::ConfigureProject(const ConfigureProject& configProy)
    {
        this->name = configProy.name;
        this->directory = configProy.directory;
        this->version = configProy.version;
        this->conectordb = configProy.conectordb;
		this->inputLenguaje = configProy.inputLenguaje;
		this->outputLenguaje = configProy.outputLenguaje;
		this->mvc = configProy.mvc;
        this->downloads = configProy.downloads;
        this->selects = configProy.selects;
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
			
        bool Symbol::isPrimaryKey()
        {
            return isPK;
        }
        bool Symbol::isForeignKey()
        {
            return isFK;
        }
        bool Symbol::isAutoIncrement()
        {
            return isAutoInc;
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
            isPK = false;
            isFK = false;
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
			//key = NULL;
		}
		
		Table::~Table()
		{
            for (auto const& [key, symbol] : *this)
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
		std::list<Table*>::iterator Tables::find(const std::string& tableName)
		{
			std::list<Table*>::iterator actual = begin();
			std::list<Table*>::iterator last = end();
			
			while (actual != last) 
			{
				if ((*actual)->name.compare(tableName) == 0) return actual;
				++actual;
			}
			return last;
		}		
	}
}
