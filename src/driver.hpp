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

#include <list>
#include <vector>
#include <iostream>
#include <fstream>
#include <set>
#include <string>

#include "analyzer.hpp"
#include "generator.hpp"

#include <libxml/xmlreader.h>


namespace apidb
{
	
	class Configuration
	{
	public:
		std::string nameProject;
		std::string directory;
		toolkit::Version version;
		toolkit::clientdb::Datconection* datconection;	
	};
	
	
	class Driver
	{
	public:
		OutputLenguajes getOutputLenguaje() const;
		virtual bool analyze();
		virtual bool generate();
		bool driving();
		Driver(const std::string& name,const std::string& directory,const toolkit::clientdb::Datconection& datconection,InputLenguajes inputLenguaje,OutputLenguajes outputLenguaje,toolkit::Version version);
		Driver();
		bool loadConfig(const std::string &filename);//fixme: hay que determinar como leeer correctamente el archivo de configuracion.
		bool saveConfig(const std::string &filename);
		
	private:
		toolkit::clientdb::Connector* connector;
		apidb::Analyzer* analyzer;
		apidb::generators::Generator* generator;
		InputLenguajes inputLenguaje;
		OutputLenguajes outputLenguaje;	
		
		std::string name;
		std::string directory;
		toolkit::Version version;
		toolkit::clientdb::Datconection* datconection;
		std::list<std::string> stackXML;
		void processNode(xmlTextReaderPtr reader);
		void streamFile(const char *filename);
	};
}

#endif

