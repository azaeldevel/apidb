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
#include "Errors.hpp"

namespace octetos
{
namespace apidb
{
	
	symbols::Table* symbols::SymbolsTable::findTable(const std::string& tablename)const
	{
		int level = symbols::getSpaceLevel(tablename);
		if(level == 0)
		{
			const_iterator globalSpace = find("");
			if(globalSpace == end())
			{
				toolkit::Error::write(toolkit::Error("No se encontró espacio global en la tabla de simbolos.",ErrorCodes::ANALYZER_FAIL,__FILE__,__LINE__));
				return NULL;
			}
			return ((Space*)globalSpace->second)->findTable(tablename);			
		}	
		else
		{
			toolkit::Error::write(toolkit::Error("Sopurte para espacios virtuales en desarrollo.",ErrorCodes::ANALYZER_FAIL,__FILE__,__LINE__));
			return NULL;
		}
	}
        symbols::SymbolsTable::SymbolsTable()
        {
                Space* sapce = new symbols::Space("");
                std::pair<const char*, symbols::ISpace*> newInser("",sapce);
                insert(newInser);//aseguira que sea el primer en ser creado.
                
        }
        symbols::SymbolsTable::~SymbolsTable()
        {
                for(SymbolsTable::iterator it = begin(); it != end(); it++)
                {
                        delete it->second;
                }
                clear();
        }
        
        
        
        
        
        void Tracer::add(const std::string& str)
        {
                std::cout << str ;
        }
        void Tracer::add(const toolkit::Error& e)
        {
                std::cout << e.describe();
        }
        void Tracer::add(const toolkit::Confirmation& c)
        {
                std::cout <<  c.getBrief();
        }
        void Tracer::add(const toolkit::Warning& w)
        {
                std::cout << w.getBrief();
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
               
	std::string getCompiled(Compiled cmpl )
	{
		switch(cmpl)
		{
                        case Compiled::SHARED:
				return "SHARED";
                        case Compiled::STATIC:
				return "STATIC";
			default:
				return "Unknow";
		}
	}
	Compiled getCompiled(const std::string& str)
        {
                if(str.compare("static") == 0 or str.compare("STATIC") == 0)
                {
                        return Compiled::STATIC;
                }
                else if(str.compare("shared") == 0 or str.compare("SHARED") == 0)
                {
                        return Compiled::SHARED;
                }                
                
                return Compiled::NoCompile;
        }
               
	std::string getPackingLenguajes(PackingLenguajes pack )
	{
		switch(pack)
		{
                        case PackingLenguajes::CMake:
				return "CMake";
			default:
				return "Unknow";
		}
	}
	PackingLenguajes getPackingLenguajes(const std::string& str)
        {
                if(str.compare("cmake") == 0 or str.compare("CMake") == 0)
                {
                        return PackingLenguajes::CMake;
                }
                
                return PackingLenguajes::NoPack;
        }
               
	std::string getOutputLenguajes(OutputLenguajes o )
	{
		switch(o)
		{
                        case OutputLenguajes::CPP:
				return "C++";
                        case OutputLenguajes::C:
				return "C";
			default:
				return "Unknow";
		}
	}
	OutputLenguajes getOutputLenguajes(const std::string& str)
        {
                if(str.compare("c++") == 0 or str.compare("C++") == 0)
                {
                        return OutputLenguajes::CPP;
                }
                else if(str.compare("c") == 0 or str.compare("C") == 0)
                {
                        return OutputLenguajes::C;
                }
                
                return OutputLenguajes::NoLang;
        }
        
	std::string getInputLenguaje(InputLenguajes inputLenguaje )
	{
		switch(inputLenguaje)
		{
			case InputLenguajes::MySQL:
				return "MySQL";
			default:
				return "Unknow";
		}
	}
	InputLenguajes getInputLenguaje(const std::string& str)
        {
                if(str.compare("mysql") == 0 or str.compare("MySQL") == 0)
                {
                        return InputLenguajes::MySQL;
                }
                
                return InputLenguajes::Unknow;
        }
    
    
        namespace symbols
	{
                /**
                 * \private
                 * */
                short  getSpaceLevel(std::string fullname)
                {
                        std::vector<std::string> comps;
                        std::string str;
                        boost::split(comps, fullname, boost::is_any_of( "." ) );

                        return comps.size()-1;
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
                std::string getFirstName(std::string fullname)
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
                const std::string& Symbol::getInType()const
                {
                        return inType;
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
        
        
                Symbol::~Symbol()
                {
                        
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
		
        
		Symbol* Table::findSymbol(const std::string& str)
		{
			iterator it = find(str.c_str());
			if(it != end()) return it->second;
			return NULL;
		}
		ISpace* Table::searh(const std::string&)
		{
			
			return NULL;
		}
		SpaceType Table::what()const
		{
			return SpaceType::TABLE; 
		}
		/*short Space::getMaxCountRef()
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
		}*/
                const std::string& Table::getUpperName()const
                {
                        return upperName;
                }
		short Table::getCountRefereces()const 
		{
			return countRef;
		}
		
		Table::Table(const std::string& name)
		{
			countRef = 0;
			this->name = name;
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
                const std::list<Symbol*>& Table::getRequired()const  
                {
                        return required;
                }
                const std::string& Table::getSpace()const           
                {
                        return space;
                }
                const std::string& Table::getFullName()const   
                {
                        return fullname;
                }
		const Key& Table::getKey()const
		{
                        return key;
                }
		
		
		        
		Table* Space::findTable(const std::string& tablename)
		{
			iterator it = find(tablename.c_str());
			if(it != end())
			{
				if(it->second->what() == SpaceType::TABLE)return (Table*)it->second;
			}
			
			return NULL;
		}
		ISpace* Space::searh(const std::string&)
		{
			
			return NULL;
		}
		SpaceType Space::what()const
		{
			return SpaceType::SPACE; 
		}
                Space::Space(const std::string& middle)
                {
                        middleName = middle;
                        if(middleName.empty())
                        {
                                firstName = middleName;
                        }
                        else
                        {
                                
                        }
                }
		const std::string& Space::getFullName()const
		{
                        return fullName;
                }
                const std::string& Space::getFirstName()const
                {
                        return firstName;
                }
                const std::string& Space::getMiddleName()const
                {
                        return middleName;
                }		
		Space::~Space()
		{
			for (iterator it = begin(); it != end(); it++)
			{
				delete it->second;
			}
			clear();
		}
		
		/*Table* Space::search(const std::string& tableName)
		{
			std::list<Table*>::iterator actual = begin();
			std::list<Table*>::iterator last = end();
			
			while (actual != last) 
			{
				if ((*actual)->getName().compare(tableName) == 0) return (symbols::Table*)(*actual);
				++actual;
			}
			return NULL;
		}*/
		/*std::list<Table*>::iterator Space::find(const std::string& tableName)
		{
			std::list<Table*>::iterator actual = begin();
			std::list<Table*>::iterator last = end();
			
			while (actual != last) 
			{
				if ((*actual)->getName().compare(tableName) == 0) return actual;
				++actual;
			}
			return last;
		}*/		
	}
}
}
