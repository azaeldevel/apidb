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
#include <vector>
#include <string>
#include <stdio.h>
#include <string.h>
#include <boost/algorithm/string.hpp>


#include "common.hpp"
namespace octetos
{
namespace apidb
{
        void Tracer::add(const std::string&)
        {
                
        }
        void Tracer::add(const toolkit::Error&)
        {
                
        }
        void Tracer::add(const toolkit::Confirmation&)
        {
                
        }
        void Tracer::add(const toolkit::Warning&)
        {
                
        }
        Tracer::Tracer(int a) : toolkit::ActivityProgress(a)
        {
                
        }
        
        
	BuildException::~BuildException() throw()
	{
		;
	}
	const char* BuildException::what() const throw()
	{
		return toolkit::Error::what();
	}
        BuildException::BuildException(const std::string &description) throw() : toolkit::Error(description,toolkit::Error::ERROR_UNKNOW)
        {
	}
               
        	
	std::string getInputLenguajeString(InputLenguajes inputLenguaje )
	{
		switch(inputLenguaje)
		{
			case InputLenguajes::MySQL:
				return "Servidor MySQL";
			default:
				return "Unknow";
		}
	}
	
    
    
        namespace symbols
	{
                /**
                 * \private
                 * */
                short  getSpaceLevel(std::string fullname)
                {
                        short count = 0;
                        std::vector<std::string> comps;
                        std::string str;
                        boost::split(comps, fullname, boost::is_any_of( "." ) );

                        if(strcmp(fullname.c_str(),""))
                        {
                                return 0;
                        }
                        for(int i = 0; i <= comps.size() ; i++)
                        {
                                count++;
                        }
                        return count;
                }
                
                /**
                 * \private
                 * */
                std::string getSpacePatch(std::string fullname)
                {
                        std::vector<std::string> comps;
                        std::string str;
                        boost::split(comps, fullname, boost::is_any_of( "." ) );
                        if(comps.size() == 1) 
                        {
                                return "";
                        }
                        else if(comps.size() > 1) 
                        {
                                for(int i = 0; i < comps.size() ; i++)
                                {
                                        if( i != 0) str += "." ;
                                        str += comps[i];
                                        if(i == (comps.size()-2))
                                        {
                                                break;
                                        }
                                }
                        }
                        return str;
                }
                
                /**
                 * \private
                 * */
                std::string getSpaceName(std::string fullname)
                {                        
                        std::vector<std::string> comps;
                        boost::split( comps, fullname, boost::is_any_of( "." ) );
                        if(comps.size() == 1) 
                        {
                                return fullname;
                        }
                        else if(comps.size() > 1) 
                        {
                                return comps.back();
                        }
                        
                        return "";
                }
        
        
                 
                const Table* Symbol::getClassReferenced()const
                {
                        return classReferenced;
                }
                const Table* Symbol::getClassParent()const
                {
                        return classParent;
                }
                const Symbol* Symbol::getSymbolReferenced()const
                {
                        return symbolReferenced;
                }
                const std::string& Symbol::getOutType()const
                {
                        return outType;
                }
                const std::string& Symbol::getName()const
                {
                        return name;
                }
                const std::string& Symbol::getUpperName()const
                {
                        return upperName;
                }
                const std::string& Symbol::getGet()const
                {
                        return get;
                }
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
		
                Space::Space(const std::string name)
                {
                        this->name = name;
                }
		short Space::getMaxCountRef()
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
				
                const std::string& Table::getUpperName()const
                {
                        return upperName;
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
                        for(std::map<const char*,Symbol*,cmp_str>::iterator it= begin();  it != end(); it++)
			{
				delete it->second;
			}	
			clear();
		}
		const std::string& Table::getName()const
                {
                        return name;
                }
		
		
		            
               const std::string& Space::getName()const
                {
                        return name;
                }
		
		
		Space::~Space()
		{
			for (Table* table : *this)
			{
				delete table;
			}
			clear();
		}
		
		Table* Space::search(const std::string& tableName)
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
		std::list<Table*>::iterator Space::find(const std::string& tableName)
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
}
