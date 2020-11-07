/**
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
#include "ConfigureProject.hpp"

#include "common.hpp"
#include "Errors.hpp"

namespace octetos
{
namespace apidb
{
	symbols::Space* symbols::SymbolsTable::findSpace(const std::string& str)
	{
			//std::cout << "Buscando '" << str << "' en '" << name << "' Space::findSpace" << std::endl;
			if(hasChild(str))
			{
				std::string top = getTopName(str);
				iterator it = find(top.c_str());
				if(it != end())
				{
					//std::cout << "Se encontro '" << top << "' en '" << name << "' Space::findSpace" << std::endl;
					if(it->second->what() == symbols::SpaceType::SPACE)
					{
						return ((Space*)it->second)->findSpace(getChilFullName(str));
					}
					else
					{
						return NULL;
					}
				}
				else
				{
					return NULL;
				}
			}
			else
			{
				std::cout << "Val : " << str << "\n";
				iterator it2 = find(str.c_str());
				if(it2 != end())
				{
					//std::cout << "* Se encontro '" << str << "(" <<  it2->first << ")"<< "' en '" << name  << "'" << std::endl;
					ISpace* ispace = it2->second;
					if(ispace->what() == SpaceType::SPACE)
					{
						if(hasChild(str))
						{
							//std::cout << "1 Space::findSpace Buscando recursivamente '" << str << "' en '" << name << "'" << std::endl;
							return ((Space*)ispace)->findSpace(getChilFullName(str));
						}
						else
						{
							//std::cout << "return " << ((Space*)ispace)->getName() << "  en '" << name << "'" << std::endl;
							return (Space*)ispace;
						}
					}
					return NULL;
				}				
			}
			
			return NULL;
	}
	symbols::Space* symbols::SymbolsTable::addSpace(const std::string& str)
	{
		if(hasChild(str))
		{
			std::string top = getTopName(str);
			//std::cout << "Creando espacio '" << top << "' en 'Global'" << std::endl;
			Space* space = new Space(top);
			std::pair<const char*, symbols::ISpace*> newInser(top.c_str(),space);
			insert(newInser);
			std::string child = getChilFullName(str);
			if(hasChild(child))
			{
				//std::cout << "Creando sub-espacio '" << child << "' en '"  << space->getName() << "'" << std::endl;
				return space->addSpace(child);
			}
			else
			{
				return space;
			}
		}
		else
		{
			//std::cout << "+ Creando espacio '" << str << "' en 'Golbal'" << std::endl;
			Space* space = new Space(str);
			std::pair<const char*, symbols::ISpace*> newInser(str.c_str(),space);
			insert(newInser);
			return space;
		}
	}
	symbols::Table* symbols::SymbolsTable::findTable(const std::string& tablename)const
	{
		int level = symbols::getSpaceLevel(tablename);
		if(level == 0)
		{
			const_iterator globalSpace = find(configureProject->name.c_str());
			if(globalSpace != end())
			{
				std::string child = getChilFullName(tablename);
				return ((Space*)globalSpace->second)->findTable(child);	
			}
			else
			{
				core::Error::write(core::Error("No se encontró espacio global en la tabla de simbolos.",ErrorCodes::ANALYZER_FAIL,__FILE__,__LINE__));
				return NULL;
			}
		}	
		else
		{
			core::Error::write(core::Error("Sopurte para espacios virtuales en desarrollo.",ErrorCodes::ANALYZER_FAIL,__FILE__,__LINE__));
			return NULL;
		}
	}
    symbols::SymbolsTable::SymbolsTable(const ConfigureProject& config):configureProject(&config)
    {
        Space* sapce = new symbols::Space(configureProject->name.c_str());
        std::pair<const char*, symbols::ISpace*> newInser(configureProject->name.c_str(),sapce);
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
    const ConfigureProject& symbols::SymbolsTable::getConfigureProject()const
    {
        return *configureProject;
    }
        

        
        void Tracer::add(const std::string& str)
        {
                std::cout << str ;
        }
        void Tracer::add(const core::Error& e)
        {
                std::cout << e.describe();
        }
        void Tracer::add(const core::Confirmation& c)
        {
                std::cout <<  c.getBrief();
        }
        void Tracer::add(const core::Warning& w)
        {
                std::cout << w.getBrief();
        }
        Tracer::Tracer(int a) : core::ActivityProgress(a)
        {
                
        }
        
        
	BuildException::~BuildException() throw()
	{
		;
	}
	const char* BuildException::what() const throw()
	{
		return core::Error::what();
	}
    BuildException::BuildException(const std::string &description) throw() : core::Error(description,core::Error::ERROR_UNKNOW)
    {
	}
    BuildException::BuildException(const std::string &description,std::string filename,int lineNumber) throw() : core::Error(description,core::Error::ERROR_UNKNOW,filename,lineNumber)
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
			case InputLenguajes::PostgreSQL:
				return "PostgreSQL";
			case InputLenguajes::MariaDB:
				return "MariaDB";
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
        else if(str.compare("mariadb") == 0 or str.compare("MariaDB") == 0)
        {
            return InputLenguajes::MariaDB;
        }
        else if(str.compare("postgresql") == 0 or str.compare("PostgreSQL") == 0)
        {
            return InputLenguajes::PostgreSQL;
        }
                
        return InputLenguajes::Unknow;
    }
    
    
	namespace symbols
	{
                
                /**
                 * \private
                 * */
                short  getSpaceLevel(const std::string& fullname)
                {
                        std::vector<std::string> comps;
                        std::string str;
                        boost::split(comps, fullname, boost::is_any_of( "." ) );

                        return comps.size()-1;
                }
                
                /**
                 * \private
                 * */
                std::string getSpacePatch(const std::string& fullname)
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
		bool hasChild(const std::string&  fullname)
		{                        
			std::vector<std::string> comps;
			boost::split( comps, fullname, boost::is_any_of( "." ) );
			if(comps.size() > 1) 
			{
				return true;
			}
                        
			return false;
		}
		std::string getTopName(const std::string&  fullname)
		{                        
			std::vector<std::string> comps;
			boost::split( comps, fullname, boost::is_any_of( "." ) );
			if(comps.size() > 1) 
			{
				std::vector<std::string>::iterator it = comps.begin();
				return *it;
			}
                        
			return "";
		}
		/*std::string getDeepChilName(const std::string&  fullname)
		{                        
			std::vector<std::string> comps;
			boost::split( comps, fullname, boost::is_any_of( "." ) );
			if(comps.size() > 0) 
			{
				return comps.back();
			}
                        
			return "";
		}*/
		std::string getChilFullName(const std::string&  fullname)
		{
			std::vector<std::string> comps;
			std::string str;
			boost::split(comps, fullname, boost::is_any_of( "." ) );
			if(comps.size() == 1) 
			{
				return fullname;
			}
			else if(comps.size() > 1) 
			{
				for(int i = 1; i < comps.size() ; i++)
				{
					if( i != 1) str += "." ;
					str += comps[i];
				}
			}
                        return str;
		}
                /**
                 * \private
                 * */
                std::string getFirstName(const std::string& fullname)
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
			if(it != end()) 
			{
				return it->second;
			}
			else
			{
				//esta linea es una cicanada, reparar pronto.
				/*for(iterator it = begin(); it != end(); it++)
				{
					//Extraño para mi find no encontro este valor(unas lineas arriba) en la linea 517 ###BOUG STL-FIAL-FIND-MAP
					if(((Symbol*)it->second)->getName().compare(str) == 0) return (Symbol*)it->second;
				}*/
			}
			return NULL;
		}
		/*ISpace* Table::searh(const std::string&)
		{
			
			return NULL;
		}*/
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
		
		
		Table* Space::addTable(symbols::Table* table)
		{
			//std::cout << table->getFullName() << " -> '" << getName() << "' Space::addTable" << std::endl;
			std::pair<const char*, symbols::ISpace*> newInser(table->getName().c_str(),table);
			insert(newInser);
			return table;
		}
		Space* Space::addSpace(const std::string& str)
		{
			//std::cout << "Space::addSpace" << std::endl;
			if(hasChild(str))
			{
				std::string top = getTopName(str);
				//std::cout << "Creando espacio '" << top << "' en '"  << name << "'" << std::endl;
				Space* space = new Space(top);
				std::pair<const char*, symbols::ISpace*> newInser(top.c_str(),space);
				insert(newInser);
				//std::cout << "Creando sub-espacio '" << getChilFullName(str) << "' en '"  << space->getName() << "'" << std::endl;
				return space->addSpace(getChilFullName(str));
			}
			else
			{
				//std::cout << "+ Creando espacio '" << str << " - " << space->getName() << "' en '"  << name << "'" << std::endl;
				Space* space = new Space(str);
				std::pair<const char*, symbols::ISpace*> newInser(space->getName().c_str(),space);
				insert(newInser);
				//std::cout << "Espacion creado '" << str << " - " << space->getName() << "' en '"  << name << "'" << std::endl;
				return space;
			}
		}
		
		Space* Space::findSpace(const std::string& str)
		{
			//std::cout << "Buscando '" << str << "' en '" << name << "' Space::findSpace" << std::endl;
			if(hasChild(str))
			{
				std::string top = getTopName(str);
				iterator it = find(top.c_str());
				if(it != end())
				{
					//std::cout << "Se encontro '" << top << "' en '" << name << "' Space::findSpace" << std::endl;
					if(it->second->what() == symbols::SpaceType::SPACE)
					{
						return ((Space*)it->second)->findSpace(getChilFullName(str));
					}
					else
					{
						return NULL;
					}
				}
				else
				{
					return NULL;
				}
			}
			else
			{
				iterator it2 = find(str.c_str());
				if(it2 != end())
				{
					//std::cout << "* Se encontro '" << str << "(" <<  it2->first << ")"<< "' en '" << name  << "'" << std::endl;
					ISpace* ispace = it2->second;
					if(ispace->what() == SpaceType::SPACE)
					{
						if(hasChild(str))
						{
							//std::cout << "1 Space::findSpace Buscando recursivamente '" << str << "' en '" << name << "'" << std::endl;
							return ((Space*)ispace)->findSpace(getChilFullName(str));
						}
						else
						{
							//std::cout << "return '" <<  it2->first << " - "<< ((Space*)ispace)->getName() << "'  en '" << name << "'" << std::endl;
							return (Space*)ispace;
						}
					}
					return NULL;
				}				
			}
			
			return NULL;
		}
		Table* Space::findTable(const std::string& tablename)
		{
			//std::cout << "Buscando Tabla '" << tablename << "' en '" << name << "' Space::findTable" << std::endl;
			if(hasChild(tablename))
			{
				std::string top = getTopName(tablename);
				iterator it = find(top.c_str());
				if(it != end())
				{
					Space* space = (Space*)(it->second);
					return space->findTable(getChilFullName(tablename));
				}
				else
				{
					return NULL;
				}
			}
			else
			{
				//std::cout << "No tiene hijos '" << tablename << "'" << std::endl;
				iterator it2 = find(tablename.c_str());
				if(it2 != end())
				{
					//std::cout << "Correct '" << tablename << "'" << std::endl;
					return (Table*)(it2->second);
				}
				else
				{
					//std::cout << "No encontrada '" << tablename << "'" << std::endl;
					/*for(iterator it = begin(); it != end(); it++)
					{
						if(it->second->what() == symbols::SpaceType::SPACE)
						{
							//std::cout << "S:" << ((Space*)it->second)->getName() << std::endl;
						}
						else if(it->second->what() == symbols::SpaceType::TABLE)
						{
							//Extraño para mi find no encontro este valor(unas lineas arriba) en la linea 692 ###BOUG STL-FIAL-FIND-MAP
							if(((Table*)it->second)->getName().compare(tablename) == 0) return (Table*)(it->second);
						}
					}*/
					return NULL;
				}
			}
			return NULL;
		}
		/*ISpace* Space::searh(const std::string&)
		{
			
			return NULL;
		}*/
		SpaceType Space::what()const
		{
			return SpaceType::SPACE; 
		}
		Space::Space(const std::string& str)
		{
			name = str;			
		}
		/*const std::string& Space::getFullName()const
		{
                        return name;
                }*/
        const std::string& Space::getName()const
        {
            return name;
        }
        /*
        const std::string& Space::getMiddleName()const
        {
                        return name;
        }
        */
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
