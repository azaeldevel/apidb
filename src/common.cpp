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
            

	
	
    const octetos::toolkit::clientdb::mysql::Datconnect& ConfigureProject::getConector() const
    {
        return *conectordb;    
    }
    const octetos::toolkit::Version& ConfigureProject::getVersion()const
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
        inputLenguaje = apidb::InputLenguajes::MySQL;
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
                        symbolReferenced = NULL;
                        inType = "";
                        outType = "";
                        get = "";
                        upperName = "";
                        keyType = NOKEY;
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
