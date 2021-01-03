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
	
	const symbols::SymbolsTable& Generator::getSymbolsTable()const
	{
		return analyzer.symbolsTable;
	}
	
	Generator::Generator(const ConfigureProject& config,apidb::Analyzer& d)  : configureProject(config),analyzer(d)
	{
		
	}
	
// 	
	
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
	
}
}
}
