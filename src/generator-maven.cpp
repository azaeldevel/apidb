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
#include "Errors.hpp"

namespace octetos
{
namespace apidb
{
namespace generators
{
	Maven::Maven(apidb::Analyzer& a,const ConfigureProject& c): Generator(c,a)
	{
	}
	Maven::~Maven()
	{

	}
	bool Maven::generate(bool log)
	{
		
		return true;
	}
	
}
}
}
