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
 
#include <list>
#include <vector>
#include <iostream>
#include <fstream>
#include <map>
#include <sys/stat.h>


#include "analyzer.hpp"
#include "generator.hpp"

namespace octetos
{
namespace apidb
{
namespace generators
{ 
    void Operation::echoCopyParamsJava()const
    {
        for(symbols::Symbol* k : table.getKey())
        {
            ofile << "\t\t\t";
            ofile << "this." << k->name << " = "  << k->name << ";\n";
        }        
    }
    void Operation::echoCopyParams()const
    {
        for(symbols::Symbol* k : table.getKey())
        {
            if(k->symbolReferenced != NULL)
            {
                ofile << "\t\t\t";
                if(configureProject.outputLenguaje == OutputLenguajes::PHP) ofile << "$";
                ofile << "this";
                switch(configureProject.outputLenguaje)
                {
                    case OutputLenguajes::CPP:
                        ofile << "->";
                        break;
                    case OutputLenguajes::JAVA:
                        ofile << ".";
                        break;
                    case OutputLenguajes::PHP:
                        ofile << "->";
                        break;
                    default:
                            throw BuildException("Lenguaje no soportado",__FILE__,__LINE__);            
                }
                ofile << k->name << " = new " << k->classReferenced->name;
                if(configureProject.outputLenguaje == OutputLenguajes::PHP)
                {
                    ofile << "();\n";
                    ofile << "\t\t\t$this->" << k->name << "->copy($" << k->name <<");\n"; 
                }
                else
                {
                    ofile << "(";
                    ofile << k->name;
                    ofile <<");\n";
                }
            }
            else
            {
                ofile << "\t\t\t";
                if(configureProject.outputLenguaje == OutputLenguajes::PHP) ofile << "$";
                ofile << "this";
                switch(configureProject.outputLenguaje)
                {
                    case OutputLenguajes::CPP:
                        ofile << "->";
                        break;
                    case OutputLenguajes::JAVA:
                        ofile << ".";
                        break;
                    case OutputLenguajes::PHP:
                        ofile << "->";
                        break;
                    default:
                            throw BuildException("Lenguaje no soportado",__FILE__,__LINE__);            
                }
                ofile << k->name << " = ";
                if(configureProject.outputLenguaje == OutputLenguajes::PHP) ofile << "$";
                ofile << k->name << ";\n";
            }
        }        
    }
    void Operation::echoCopyParamsRaw()const
    {
        for(symbols::Symbol* k : table.getKey())
        {
            if(k->symbolReferenced == NULL)
            {
                ofile << "\t\t\t";
                if(configureProject.outputLenguaje == OutputLenguajes::PHP) ofile << "$";
                ofile << "this";
                switch(configureProject.outputLenguaje)
                {
                    case OutputLenguajes::CPP:
                        ofile << "->";
                        break;
                    case OutputLenguajes::JAVA:
                        ofile << ".";
                        break;
                    case OutputLenguajes::PHP:
                        ofile << "->";
                        break;
                    default:
                            throw BuildException("Lenguaje no soportado",__FILE__,__LINE__);            
                }
                ofile << k->name << " = ";
                if(configureProject.outputLenguaje == OutputLenguajes::PHP) ofile << "$";
                ofile << k->name << ";\n";
            }
        }        
    }
    bool Operation::echoKeyListName()const
    {
        //if(table.getKey().size() > 1) throw BuildException("No hay soporte para llaves complejas.",__FILE__,__LINE__);
        if(table.getKey().size() == 0) return false;
        
        const symbols::Key& key = table.getKey();
        std::vector<symbols::Symbol*>::const_iterator itend = key.end();
        itend--;
        for(std::vector<symbols::Symbol*>::const_iterator it = key.begin(); it < key.end(); it++)
        {
            ofile << (*it)->name;
            if(it < itend) 
            {
                ofile << ",";
            }
        }
        
        return true;
    }
    bool Operation::echoDataRawParam(const symbols::Symbol* k)const
    {
        //for(symbols::Symbol* k : table.getKey())
        {
            ofile << " \"" << k->name << " = ";
            if(k->symbolReferenced != NULL)
            {
                if(k->getOutType().compare("int") == 0 or k->getOutType().compare("long")  == 0  or k->getOutType().compare(integerType()) == 0 )
                {
                    switch(configureProject.outputLenguaje)
                    {
                        case OutputLenguajes::CPP:
                            ofile << "\" + std::to_string(";
                            ofile << k->name;
                            inheritField(ofile,k->symbolReferenced,opReference());
                            ofile << ")";
                            break;
                        case OutputLenguajes::JAVA:
                            ofile << "\" + ";
                            ofile << k->name;
                            inheritField(ofile,k->symbolReferenced,opReference());
                            break;
                        case OutputLenguajes::PHP:
                            ofile << "\" . $this->";
                            ofile << k->name;
                            inheritField(ofile,k->symbolReferenced,opReference());
                            break;
                        default:
                            throw BuildException("Lgenguaje no soportado",__FILE__,__LINE__);            
                    }
                }
                else if(k->getOutType().compare(stringType()) == 0)
                {
                    std::string msg = "No hay soporte para llave con string : ";
                    msg += "'" + k->classParent->name + "'";
                    throw BuildException(msg,__FILE__,__LINE__); 
                }
            }
            else
            {
                if(k->getOutType().compare("int") == 0 or k->getOutType().compare("long")  == 0 or k->getOutType().compare(integerType()) == 0)
                {
                    switch(configureProject.outputLenguaje)
                    {
                        case OutputLenguajes::CPP:
                            ofile << "\" + std::to_string(" << k->name << ")";
                            break;
                        case OutputLenguajes::JAVA:
                            ofile << "\" + " << k->name;
                            break;
                        case OutputLenguajes::PHP:
                            ofile << "\" . $" << k->name;
                            break;
                        default:
                            throw BuildException("Lgenguaje no soportado",__FILE__,__LINE__);            
                    }
                }
                else if(k->getOutType().compare(stringType()) == 0)           
                {
                    switch(configureProject.outputLenguaje)
                    {
                        case OutputLenguajes::CPP:
                            ofile << "\" + \"'\" + " << k->name << " + \"'\"";
                            break;
                        case OutputLenguajes::JAVA:
                            ofile << "\" + \"'\" + " << k->name << " + \"'\"";
                            break;
                        case OutputLenguajes::PHP:
                            ofile << "\" . \"'\" . $" << k->name << " . \"'\"";
                            break;
                        default:
                            throw BuildException("Lgenguaje no soportado",__FILE__,__LINE__);            
                    }
                }
                else         
                {
                    switch(configureProject.outputLenguaje)
                    {
                        case OutputLenguajes::CPP:
                            ofile << "\" + std::to_string(" << k->name << ")";
                            break;
                        case OutputLenguajes::JAVA:
                            ofile << "\" + " << k->name;
                            break;
                        case OutputLenguajes::PHP:
                            ofile << "\" . $" << k->name;
                            break;
                        default:
                            throw BuildException("Lenguaje no soportado",__FILE__,__LINE__);            
                    } 
                }
            }
        }
        
        return true;
    }
    bool Operation::echoKeyRawParam()const
    {
        const symbols::Key& key = table.getKey();
        std::vector<symbols::Symbol*>::const_iterator itend = key.end();
        itend--;
        symbols::Symbol* endK = *itend;
        
        for(std::vector<symbols::Symbol*>::const_iterator it = key.begin(); it < key.end(); it++)
        {
            symbols::Symbol* k = *it;
            ofile << " \"" << k->name << " = ";
            if(k->symbolReferenced != NULL)
            {
                if(k->getOutType().compare("int") == 0 or k->getOutType().compare("long")  == 0  or k->getOutType().compare(integerType()) == 0 )
                {
                    switch(configureProject.outputLenguaje)
                    {
                        case OutputLenguajes::CPP:
                            ofile << "\" + std::to_string(";
                            ofile << k->name;
                            inheritField(ofile,k->symbolReferenced,opReference());
                            ofile << ")";
                            break;
                        case OutputLenguajes::JAVA:
                            ofile << "\" + ";
                            ofile << k->name;
                            inheritField(ofile,k->symbolReferenced,opReference());
                            break;
                        case OutputLenguajes::PHP:
                            ofile << "\" . $this->";
                            ofile << k->name;
                            inheritField(ofile,k->symbolReferenced,opReference());
                            break;
                        default:
                            throw BuildException("Lgenguaje no soportado",__FILE__,__LINE__);            
                    }
                }
                else if(k->getOutType().compare(stringType()) == 0)
                {
                    switch(configureProject.outputLenguaje)
                    {
                        case OutputLenguajes::CPP:
                            ofile << "\" + \"'\" + " << k->name << " + \"'\"";
                            break;
                        case OutputLenguajes::JAVA:
                            ofile << "\" + \"'\" + " << k->name << " + \"'\"";
                            break;
                        case OutputLenguajes::PHP:
                            ofile << "\" . \"'\" . $this->" << k->name << " . \"'\"";
                            break;
                        default:
                            throw BuildException("Lgenguaje no soportado",__FILE__,__LINE__);            
                    } 
                }
            }
            else
            {
                if(k->getOutType().compare("int") == 0 or k->getOutType().compare("long")  == 0 or k->getOutType().compare(integerType()) == 0)
                {
                    switch(configureProject.outputLenguaje)
                    {
                        case OutputLenguajes::CPP:
                            ofile << "\" + std::to_string(" << k->name << ")";
                            break;
                        case OutputLenguajes::JAVA:
                            ofile << "\" + " << k->name;
                            break;
                        case OutputLenguajes::PHP:
                            ofile << "\" . $" << k->name;
                            break;
                        default:
                            throw BuildException("Lgenguaje no soportado",__FILE__,__LINE__);            
                    }
                }
                else if(k->getOutType().compare(stringType()) == 0)           
                {
                    switch(configureProject.outputLenguaje)
                    {
                        case OutputLenguajes::CPP:
                            ofile << "\" + \"'\" + " << k->name << " + \"'\"";
                            break;
                        case OutputLenguajes::JAVA:
                            ofile << "\" + \"'\" + " << k->name << " + \"'\"";
                            break;
                        case OutputLenguajes::PHP:
                            ofile << "\" . \"'\" . $this->" << k->name << " . \"'\"";
                            break;
                        default:
                            throw BuildException("Lgenguaje no soportado",__FILE__,__LINE__);            
                    } 
                }
            }
            if(it < itend) 
            {
                switch(configureProject.outputLenguaje)
                {
                    case OutputLenguajes::CPP:
                        ofile << " + \" and \" + ";
                        break;
                    case OutputLenguajes::JAVA:
                        ofile << " + \" and \" + ";
                        break;
                    case OutputLenguajes::PHP:
                        ofile << " . \" and \" . ";
                        break;
                    default:
                            throw BuildException("Lgenguaje no soportado",__FILE__,__LINE__);            
                }
            }
        }
        return true;
    }
    bool Operation::echoKey()const
    {
        const symbols::Key& key = table.getKey();
        std::vector<symbols::Symbol*>::const_iterator itend = key.end();
        itend--;
        symbols::Symbol* endK = *itend;
        for(std::vector<symbols::Symbol*>::const_iterator it = key.begin(); it < key.end(); it++)
        {
            symbols::Symbol* k = *it;
            ofile << " \"" << k->name << " = ";
            if(k->symbolReferenced != NULL)
            {
                if(k->getOutType().compare(stringType()) == 0)
                {
                    switch(configureProject.outputLenguaje)
                    {
                        case OutputLenguajes::CPP:
                            ofile << "\" + \"'\" + " << k->name << " + \"'\"";
                            break;
                        case OutputLenguajes::JAVA:
                            ofile << "\" + \"'\" + " << k->name << " + \"'\"";
                            break;
                        case OutputLenguajes::PHP:
                            ofile << "\" . \"'\" . $this->" << k->name << " . \"'\"";
                            break;
                        default:
                            throw BuildException("Lgenguaje no soportado",__FILE__,__LINE__);            
                    } 
                }
                else if(k->getOutType().compare("int") == 0 or k->getOutType().compare("long")  == 0  or k->getOutType().compare(integerType()) == 0 )
                {
                    switch(configureProject.outputLenguaje)
                    {
                        case OutputLenguajes::CPP:
                            ofile << "\" + std::to_string(";
                            ofile << "(*" << k->name << ")";
                            inheritField(ofile,k->symbolReferenced,opReference());
                            ofile << ")";
                            break;
                        case OutputLenguajes::JAVA:
                            ofile << "\" + ";
                            ofile << k->name;
                            inheritField(ofile,k->symbolReferenced,opReference());
                            break;
                        case OutputLenguajes::PHP:
                            ofile << "\" . $this->";
                            ofile << k->name;
                            inheritField(ofile,k->symbolReferenced,opReference());
                            break;
                        default:
                            throw BuildException("Lenguaje no soportado",__FILE__,__LINE__);            
                    }
                }
            }
            else
            {
                if(k->getOutType().compare(stringType()) == 0)
                {
                    switch(configureProject.outputLenguaje)
                    {
                        case OutputLenguajes::CPP:
                            ofile << "\" + \"'\" + " << k->name << " + \"'\"";
                            break;
                        case OutputLenguajes::JAVA:
                            ofile << "\" + \"'\" + " << k->name << " + \"'\"";
                            break;
                        case OutputLenguajes::PHP:
                            ofile << "\" . \"'\" . $this->" << k->name << " . \"'\"";
                            break;
                        default:
                            throw BuildException("Lgenguaje no soportado",__FILE__,__LINE__);            
                    } 
                }
                else if(k->getOutType().compare("int") == 0 or k->getOutType().compare("long")  == 0 or k->getOutType().compare(integerType()) == 0)
                {
                    switch(configureProject.outputLenguaje)
                    {
                        case OutputLenguajes::CPP:
                            ofile << "\" + std::to_string(" << k->name << ")";
                            break;
                        case OutputLenguajes::JAVA:
                            ofile << "\" + " << k->name;
                            break;
                        case OutputLenguajes::PHP:
                            ofile << "\" . $this->" << k->name;
                            break;
                        default:
                            throw BuildException("Lgenguaje no soportado",__FILE__,__LINE__);            
                    }
                }
                /*
                else if(k->getOutType().compare(stringType()) == 0)
                {
                    switch(configureProject.outputLenguaje)
                    {
                        case OutputLenguajes::CPP:
                            ofile << "\" + \"'\" + " << k->name << " + \"'\"";
                            break;
                        case OutputLenguajes::JAVA:
                            ofile << "\" + \"'\" + " << k->name << " + \"'\"";
                            break;
                        case OutputLenguajes::PHP:
                            ofile << "\" . \"'\" . $this->" << k->name << " + \"'\"";
                            break;
                        default:
                            throw BuildException("Lgenguaje no soportado",__FILE__,__LINE__);            
                    }
                }
                */
            }
            
            if(it < itend) 
            {
                switch(configureProject.outputLenguaje)
                {
                    case OutputLenguajes::CPP:
                        ofile << " + \" and \" + ";
                        break;
                    case OutputLenguajes::JAVA:
                        ofile << " + \" and \" + ";
                        break;
                    case OutputLenguajes::PHP:
                        ofile << " . \" and \" . ";
                        break;
                    default:
                            throw BuildException("Lgenguaje no soportado",__FILE__,__LINE__);            
                }
            }
        }
        
        return true;
    }
	Operation::Operation(const ConfigureProject& c,const apidb::symbols::Table& t,std::ofstream& o) : configureProject(c), table(t), ofile(o)
    {
        definition = false;
        implementation = false;
    }
    const char* Operation::opConcat() const
    {
        switch(configureProject.outputLenguaje)
        {
            case OutputLenguajes::CPP:
                return "+";
            case OutputLenguajes::JAVA:
                return "+";
            case OutputLenguajes::PHP:
                return ".";
            default:
                return NULL;            
        }
    }
    const char* Operation::opReference() const
    {
        switch(configureProject.outputLenguaje)
        {
            case OutputLenguajes::CPP:
                return ".";
            case OutputLenguajes::JAVA:
                return ".";
            case OutputLenguajes::PHP:
                return "->";
            default:
                return NULL;            
        }
    }
    const char* Operation::stringType() const
    {
        switch(configureProject.outputLenguaje)
        {
            case OutputLenguajes::CPP:
                return "std::string";
            case OutputLenguajes::JAVA:
                return "String";
            case OutputLenguajes::PHP:
                return "String";
            default:
                return NULL;            
        }   
    }
    const char* Operation::integerType() const
    {
        switch(configureProject.outputLenguaje)
        {
            case OutputLenguajes::CPP:
                return "int";
            case OutputLenguajes::JAVA:
                return "int";
            case OutputLenguajes::PHP:
                return "Integer";
            default:
                return NULL;            
        }
    }
    const char* Operation::getsqlString()const
    {
        switch(configureProject.outputLenguaje)
        {
            case OutputLenguajes::CPP:
                return "sqlString";
            case OutputLenguajes::JAVA:
                return "sqlString";
            case OutputLenguajes::PHP:
                return "$sqlString";
            default:
                return NULL;            
        }
    }
    const char* Operation::byteType() const
    {
        switch(configureProject.outputLenguaje)
        {
            case OutputLenguajes::CPP:
                return "unsigned char";
            case OutputLenguajes::JAVA:
                return "byte";
            case OutputLenguajes::PHP:
                return "Byte";
            default:
                return NULL;            
        }
    }
    const char* Operation::floatType() const
    {
        switch(configureProject.outputLenguaje)
        {
            case OutputLenguajes::CPP:
                return "float";
            case OutputLenguajes::JAVA:
                return "float";
            case OutputLenguajes::PHP:
                return "Float";
            default:
                return NULL;            
        }
    }
    const char* Operation::doubleType() const
    {
        switch(configureProject.outputLenguaje)
        {
            case OutputLenguajes::CPP:
                return "double";
            case OutputLenguajes::JAVA:
                return "double";
            case OutputLenguajes::PHP:
                return "Double";
            default:
                return NULL;            
        }
    }
    void Operation::setDefinition(bool d)
    {
        definition = d;
    }
    void Operation::setImplementation(bool i)
    {
        implementation = i;
    }
    void Operation::inheritField(std::ofstream& ofile, const symbols::Symbol* k, const char* separator) const
    {
        if(k->symbolReferenced != NULL)
        {            
            ofile << separator << "get" << k->getUpperName() << "()";
            inheritField(ofile,k->symbolReferenced,separator);
        }
        else if(k != NULL)
        {
            ofile << separator << "get" << k->getUpperName() << "()";
        }
    }
    symbols::Symbol* Operation::getRootSymbol(symbols::Symbol* k)
    {
        if(k == NULL) return NULL;
        
        if(k->symbolReferenced != NULL)
        {
            return getRootSymbol(k->symbolReferenced);
        }
        else
        {
            return k;
        }
    }
    
    
    
    
    
    
    
    
    
    
	const symbols::SymbolsTable& Generator::getSymbolsTable()const
	{
		return analyzer.symbolsTable;
	}
	
	Generator::Generator(const ConfigureProject& config,apidb::Analyzer& d)  : configureProject(config),analyzer(d)
	{
		
	}
	std::string Generator::getPackingLenguajeString()const
	{
		switch(configureProject.packing)
		{
			case PackingLenguajes::CMake:		
				return "CMake";
            case PackingLenguajes::Maven:
				return "Maven";
			default:
				return "Unknow";
		}
	}
	PackingLenguajes Generator::getPackingLenguaje()const
	{
		return configureProject.packing;
	}
	std::string Generator::getOutputLenguajeString()const
	{
		switch(configureProject.outputLenguaje)
		{
			case OutputLenguajes::CPP:
				return "C++";
			case OutputLenguajes::JAVA:
				return "Java";
			default:
				return "Unknow";
		}
	}
    
	OutputLenguajes Generator::getOutputLenguaje()const
	{
		return configureProject.outputLenguaje;
	}
	/*symbols::Symbol* Generator::getRootSymbol(symbols::Symbol* k)
    {
        if(k == NULL) return NULL;
        
        if(k->symbolReferenced != NULL)
        {
            return getRootSymbol(k->symbolReferenced);
        }
        else
        {
            return k;
        }
    }
	void Generator::insertParamsRaw(std::ofstream& ofile,symbols::Symbol* k,symbols::Symbol* parent)
    {
        if(k->symbolReferenced != NULL)
        {
            if(k->symbolReferenced->symbolReferenced != NULL)
            {
                insertParamsRaw(ofile,k->symbolReferenced,parent);
            }     
            else
            {
                auto penultimo = k->symbolReferenced->classParent->getRequired().begin();
                penultimo--;
                penultimo--;
                for(symbols::Symbol* l : k->symbolReferenced->classParent->getRequired())
                {
                    ofile << l->outType << " " << parent->name << l->upperName;
                    if(*penultimo != l)
                    {
                        ofile << ",";
                    }
                }
            }
        }
    }
	void Generator::insertValueRaw(std::ofstream& ofile,symbols::Symbol* k,symbols::Symbol* parent)
    {
        if(k->symbolReferenced != NULL)
        {
            if(k->symbolReferenced->symbolReferenced != NULL)
            {
                insertValueRaw(ofile,k->symbolReferenced,parent);
            }     
            else
            {
                auto penultimo = k->symbolReferenced->classParent->getRequired().begin();
                penultimo--;
                penultimo--;
                for(symbols::Symbol* l : k->symbolReferenced->classParent->getRequired())
                {
                    ofile << parent->name << l->upperName;
                    if(*penultimo != l)
                    {
                        ofile << ",";
                    }
                }
            }
        }
    }
    void Generator::getKey(std::ofstream& ofile, const symbols::Symbol* k)
    {
        if(k->symbolReferenced != NULL)
        {            
            ofile << ".get" << k->getUpperName() << "()";
            getKey(ofile,k->symbolReferenced);
        }
        else
        {
            ofile << ".get" << k->getUpperName() << "()";
        }
    }
    
    void Generator::getKey2(std::ofstream& ofile, const symbols::Symbol* k)
    {
        if(k->symbolReferenced != NULL)
        {            
            ofile << ".get" << k->getUpperName() << "()";
            getKey2(ofile,k->symbolReferenced);
        }
        else
        {
            ofile << ".get" << k->getUpperName() << "()";
        }
    }*/
}
}
}
