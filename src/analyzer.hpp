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

#ifndef APIDB_ANALYZER_HPP
#define APIDB_ANALYZER_HPP

#include "common.hpp"
#include "ConfigureProject.hpp"

namespace octetos
{
namespace apidb
{    
	class Analyzer
	{
	public:
		virtual std::ostream& getOutputMessage();
		virtual std::ostream& getErrorMessage();
		virtual std::string parse(const std::string& line) = 0;
		virtual bool analyze(bool log) = 0;
                
		std::map<const char*,symbols::Tables*,symbols::cmp_str>& getListTable(); 
                const std::map<const char*,symbols::Tables*,symbols::cmp_str>& getListTableConst() const;
                std::map<const char*,symbols::Tables*,symbols::cmp_str> copyListTable()const;
			
		const std::string& getNameProject();
		//const std::string& getDirectoryProject();				
		InputLenguajes getInputLenguaje() const;
		OutputLenguajes getOutputLenguaje() const;
		Analyzer(const ConfigureProject&, octetos::toolkit::clientdb::Connector*);
                virtual ~Analyzer(); 
    protected:
		//symbols::Tables symbolsTables;		
                std::map<const char*,symbols::Tables*,symbols::cmp_str> spacies;
		octetos::toolkit::clientdb::Connector* connector;
		std::ostream* outputMessages;//out stream
		std::ostream* errorMessages;//out stream		
		const ConfigureProject& configureProject;
                //
		 bool listing(octetos::toolkit::clientdb::mysql::Connector& connect);  
	};
}
}
#endif
