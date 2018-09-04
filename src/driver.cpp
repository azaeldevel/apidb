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

#include <exception>
#include <string.h>
#include <stdio.h>


#include "apidb.hpp"
#include "toolkit.hpp"


namespace apidb
{
    Driver::Driver(const ConfigureProject config)
	{		
		this->inputLenguaje = config.inputLenguaje;
		this->outputLenguaje = config.outputLenguaje;
		if(this->inputLenguaje == apidb::InputLenguajes::MySQL_Server)
		{
			this->datconection = new toolkit::clientdb::DatconectionMySQL(config.conectordb);
			connector = new toolkit::clientdb::Connector();
			analyzer = new mysql::Analyzer(inputLenguaje,outputLenguaje);		
			try
			{
				bool flag = connector->connect(config.conectordb);
				if(flag)
				{
					analyzer->setPramsProject(config.name,config.directory);
					this->name = config.name;
					this->directory = config.directory;
					this->version = config.version;
				}
			}
			catch(toolkit::clientdb::SQLException ex)
			{
				analyzer->getErrorMessage() <<"Fallo la conexion el servidor de datos el cual respondio; "<<std::endl;
			}
		}
		else
		{
			analyzer->getErrorMessage() <<"Lenguaje de entrada desconocido."<<std::endl;
		}	
	}
	
	Driver::Driver()
	{
		datconection = NULL;
		connector = NULL;
	}
	
	bool ConfigureProject::saveConfig()
	{
		xmlDocPtr doc  = xmlNewDoc((const xmlChar *)"1.0");
		xmlNodePtr root_node = xmlNewNode(NULL, (const xmlChar *)"project");
		xmlDocSetRootElement(doc, root_node);
		
		xmlNewChild(root_node, NULL, (const xmlChar *)"name", (const xmlChar *)name.c_str());
		xmlNewChild(root_node, NULL, (const xmlChar *)"directory", (const xmlChar *)directory.c_str());
				
		xmlNodePtr version_node = xmlNewChild(root_node, NULL, (const xmlChar *)"version", NULL);
		xmlNewChild(version_node, NULL, (const xmlChar *)"major", (const xmlChar *)std::to_string(version.getMajor()).c_str());
		xmlNewChild(version_node, NULL, (const xmlChar *)"minor", (const xmlChar *)std::to_string(version.getMinor()).c_str());
		xmlNewChild(version_node, NULL, (const xmlChar *)"patch", (const xmlChar *)std::to_string(version.getPatch()).c_str());
		//xmlNewChild(version_node, NULL, (const xmlChar *)"stage", (const xmlChar *)version.stage );
		
		xmlNodePtr db_node = xmlNewChild(root_node, NULL, (const xmlChar *)"ConectorDB", NULL);
		if(inputLenguaje == apidb::InputLenguajes::MySQL_Server)
		{
			//toolkit::clientdb::DatconectionMySQL* dat = (toolkit::clientdb::DatconectionMySQL*)connector;
			xmlNewChild(db_node, NULL, (const xmlChar *)"host", (const xmlChar *)conectordb.getHost().c_str());
			xmlNewChild(db_node, NULL, (const xmlChar *)"puerto", (const xmlChar *)std::to_string(conectordb.getPort()).c_str());
			xmlNewChild(db_node, NULL, (const xmlChar *)"nameDB", (const xmlChar *)conectordb.getDatabase().c_str());
			xmlNewChild(db_node, NULL, (const xmlChar *)"user", (const xmlChar *)conectordb.getUsuario().c_str());
			xmlNewChild(db_node, NULL, (const xmlChar *)"pw", (const xmlChar *)conectordb.getPassword().c_str());
		}
		else
        {
            return false;
        }
		
        std::string nmFile = "";
		if((directory.empty()) || (directory.compare(".") == 0))
		{
			nmFile = "apidb";
		}
		else
        {
            std::ifstream ifile(directory);
            if (ifile) 
            {            
                nmFile = directory + "/apidb";
            }
            else
            {            
                return false;
            }
        }
        
		int ret = xmlSaveFormatFileEnc(nmFile.c_str(), doc, "UTF-8", 1);	
		xmlFreeDoc(doc);
		xmlCleanupParser();
		if( ret == -1) return false;	
		return true;
	}



	
	
	OutputLenguajes Driver::getOutputLenguaje() const
	{
		return generator->getOutputLenguaje();
	}
	
	bool Driver::driving()
	{
		if(analyze())
		{
			if(generate()) return true;
		}
		
		return false;
	}
	
	bool Driver::generate()
	{				
		if((analyzer->getDirectoryProject().empty()) | (analyzer->getDirectoryProject().compare(".") == 0))
		{
			
		}
		else
		{
			std::string direct = analyzer->getDirectoryProject();
			std::ifstream ifile(direct);
			if (!ifile) 
			{
				std::string cmd = "mkdir ";
				cmd = cmd + direct;
				system(cmd.c_str());
			}			
		}
		
		if(outputLenguaje == apidb::OutputLenguajes::CPP)
		{
			bool flagCPP,flagCMAKE;
			
			std::cout<<"apidb::generators::CPP cpp(*analyzer);..."<<std::endl;
			apidb::generators::CPP cpp(*analyzer);
			generator = &cpp;
			flagCPP = cpp.generate();
			
			//std::cout<<"apidb::generators::CMake cmake(*analyzer);..."<<std::endl;
			apidb::generators::CMake cmake(*analyzer);			
			flagCMAKE = cmake.generate();
			
			///std::cout<<"if(flagCPP && flagCMAKE)..."<<std::endl;
			if(flagCPP && flagCMAKE)
			{
				analyzer->getOutputMessage() <<"Completado."<<std::endl;				
				return true;				
			}
			else
			{
				analyzer->getOutputMessage() <<"Fail."<<std::endl;
				return false;
			}
		}
		else
		{
			return false;
		}
	}
	
	bool Driver::analyze()
	{
		analyzer->getOutputMessage() << "Analisis de codigo..." << std::endl;
		analyzer->getOutputMessage() << "\tLenguaje de entrada: " << analyzer->getInputLenguajeString() << std::endl;
		
		apidb::mysql::Analyzer* analyzer = NULL;
		if(this->analyzer->getInputLenguaje() == apidb::InputLenguajes::MySQL_Server)
		{
			analyzer = (apidb::mysql::Analyzer*)(this->analyzer);
		}
		else
		{
			return false;
		}
		
		analyzer->setPramsLenguajes(inputLenguaje,outputLenguaje);
		
		if(analyzer->listing(*connector)) //reading tables
        {
            for(symbols::Table* table: analyzer->getListTable()) //reading attrubtes by table
            {
				analyzer->getOutputMessage() << "\tCreating basic simbols for " << table->name  << "." << std::endl;
                //simbolos basicos 
                if(!table->basicSymbols(*connector))
                {
					//std::cerr<<"Faill on basicSymbols"<<std::endl;
					return false;
				}				
				//foreign key's
				if(!table->fillKeyType(*connector,analyzer->getListTable()))
                {
					//std::cerr<<"Faill on fillKeyType"<<std::endl;
					return false;
				}//parsing imput types
				//std::cout<<"\t"<<table->name<<std::endl;
				for(symbols::Symbol* attribute: *table)
				{
					//std::cout<<"\t"<<attribute->inType<<std::endl;
					attribute->outType = analyzer->parse(attribute->inType);
				}				
			}	
			analyzer->getListTable().reorder();
        }  
          
		return true;
	}
		
	Driver::Driver(const std::string& name,const std::string& directory,const toolkit::clientdb::Datconection& datconection,InputLenguajes inputLenguaje, OutputLenguajes outputLenguaje,toolkit::Version version)
	{		
		this->inputLenguaje = inputLenguaje;
		this->outputLenguaje = outputLenguaje;
		if(this->inputLenguaje == apidb::InputLenguajes::MySQL_Server)
		{
			this->datconection = new toolkit::clientdb::DatconectionMySQL((toolkit::clientdb::DatconectionMySQL&)datconection);
			connector = new toolkit::clientdb::Connector();
			analyzer = new mysql::Analyzer(inputLenguaje,outputLenguaje);		
			try
			{
				bool flag = connector->connect(datconection);
				if(flag)
				{
					analyzer->setPramsProject(name,directory);
					this->name = name;
					this->directory = directory;
					this->version = version;
				}
			}
			catch(toolkit::clientdb::SQLException ex)
			{
				analyzer->getErrorMessage() <<"Fallo la conexion el servidor de datos el cual respondio; "<<std::endl;
			}
		}
		else
		{
			analyzer->getErrorMessage() <<"Lenguaje de entrada desconocido."<<std::endl;
		}	
	}
} 

