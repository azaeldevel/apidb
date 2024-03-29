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

#include "analyzer.hpp"
#include "Errors.hpp"

namespace octetos
{
namespace apidb
{	
	bool Analyzer::fillKeyType(symbols::ISpace* ispace,core::ActivityProgress* progress)
    {
		if(configureProject.getInputLenguaje() == InputLenguajes::MySQL)
		{
			if(ispace->what() == symbols::SpaceType::TABLE)
			{
				if((((symbols::Table*)ispace)->fillKeyType(*connector,symbolsTable)) == false) return false;
			}
			else if(ispace->what() == symbols::SpaceType::SPACE)
			{
				symbols::Space* space = (symbols::Space*) ispace;
				//std::cout << "Espacio '" << space->getFullName() << "'" << std::endl;
				for(symbols::Space::iterator it = space->begin(); it != space->end(); it++)
				{
					if(fillKeyType(it->second,progress) == false) return false;
				}				
			}
		}
		else if(configureProject.getInputLenguaje() == InputLenguajes::MariaDB)
		{
			if(ispace->what() == symbols::SpaceType::TABLE)
			{
				if((((symbols::Table*)ispace)->fillKeyType(*connector,symbolsTable)) == false) return false;
			}
			else if(ispace->what() == symbols::SpaceType::SPACE)
			{
				symbols::Space* space = (symbols::Space*) ispace;
				//std::cout << "Espacio '" << space->getFullName() << "'" << std::endl;
				for(symbols::Space::iterator it = space->begin(); it != space->end(); it++)
				{
					if(fillKeyType(it->second,progress) == false) return false;
				}				
			}
		}
		else if(configureProject.getInputLenguaje() == InputLenguajes::PostgreSQL)
		{
			if(ispace->what() == symbols::SpaceType::TABLE)
			{
				if((((symbols::Table*)ispace)->fillKeyType(*connector,symbolsTable)) == false) return false;
			}
			else if(ispace->what() == symbols::SpaceType::SPACE)
			{
				symbols::Space* space = (symbols::Space*) ispace;
				//std::cout << "Espacio '" << space->getFullName() << "'" << std::endl;
				for(symbols::Space::iterator it = space->begin(); it != space->end(); it++)
				{
					if(fillKeyType(it->second,progress) == false) return false;
				}				
			}
		}
		else
		{
            throw core::Exception("El lenguaje de entrada no esá soportado.",__FILE__,__LINE__);
		}
		
		return true;
	}
	bool Analyzer::basicSymbols(symbols::ISpace* ispace,core::ActivityProgress* progress)
	{
		//std::cout << "Analyzer::basicSymbols : Step 1\n";
		if(progress != NULL)
		{
			if(ispace->what() == symbols::SpaceType::TABLE)
			{
				std::string msg =  "\tCreando simbolos basicos para " ;
				msg +=  ((symbols::Table*)ispace)->getName() + "\n";
				progress->add(msg);
			}
		}
		//std::cout << "Analyzer::basicSymbols : Step 2\n";
		if(ispace->what() == symbols::SpaceType::TABLE)
        {
            //std::cout << "Tabla " << ((symbols::Table*)ispace)->getName() << std::endl;
            if(((symbols::Table*)ispace)->basicSymbols(*connector) == false) return false;
        }
        else if(ispace->what() == symbols::SpaceType::SPACE)
        {
            symbols::Space* space = (symbols::Space*) ispace;
            //std::cout << "Espacio '" << space->getFullName() << "'" << std::endl;
            for(symbols::Space::iterator it = space->begin(); it != space->end(); it++)
            {
                if(basicSymbols(it->second,progress) == false) return false;
            }
        }
        
		return true;
	}
	
	core::ActivityProgress& Analyzer::getOutput()
        {
                return *progress;
        }
        const ConfigureProject& Analyzer::getConfigureProject()const
        {
                return configureProject;
        }
                
	Analyzer::~Analyzer()
	{
	}
	Analyzer::Analyzer(const ConfigureProject& config,octetos::db::Connector* conn,core::ActivityProgress* p) : connector(conn),configureProject(config),progress(p),symbolsTable(config)
	{
	}
	
}
}
