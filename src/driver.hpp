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

#ifndef APIDB_DRIVER_HPP
#define APIDB_DRIVER_HPP

#include "analyzer.hpp"
#include "generator.hpp"

namespace octetos
{
namespace apidb
{
	class Driver
	{
	public:
		OutputLenguajes getOutputLenguaje() const;
		virtual bool analyze(bool log);
		virtual bool generate();
		bool driving();
                Driver(const ConfigureProject&);
		const Analyzer& getAnalyzer() const;
                
	private:
		octetos::toolkit::clientdb::Connector* connector;
		apidb::Analyzer* analyzer;
		apidb::generators::Generator* generator;
		const ConfigureProject& configureProject;
	};
}
}

#endif

