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

namespace octetos
{
namespace apidb
{	
	toolkit::ActivityProgress& Analyzer::getOutput()
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
	Analyzer::Analyzer(const ConfigureProject& config,octetos::toolkit::clientdb::Connector* conn,toolkit::ActivityProgress* p) : configureProject(config), connector(conn),progress(p)
	{
	}
	std::map<const char*,symbols::ISpace*,symbols::cmp_str> Analyzer::copyListTable() const
	{
		return symbolsTable;
	}	
	const std::map<const char*,symbols::ISpace*,symbols::cmp_str>& Analyzer::getListTableConst() const
	{
		return symbolsTable;
	}
}
}
