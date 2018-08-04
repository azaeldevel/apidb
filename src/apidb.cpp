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

#include "apidb.hpp"
#include "toolkit.hpp"
#include <exception>
#include <string.h>
#include <stdio.h>
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>


namespace apidb
{
	CG::CG()
	{
		datconection = NULL;
		connector = NULL;
	}
	
	bool CG::saveConfig(const std::string &docname)
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
		
		xmlNodePtr db_node = xmlNewChild(root_node, NULL, (const xmlChar *)"db", NULL);
		if(this->inputLenguaje == apidb::InputLenguajes::MySQL_Server)
		{
			toolkit::clientdb::DatconectionMySQL* dat = (toolkit::clientdb::DatconectionMySQL*)datconection;
			xmlNewChild(db_node, NULL, (const xmlChar *)"host", (const xmlChar *)dat->getHost().c_str());
			xmlNewChild(db_node, NULL, (const xmlChar *)"puerto", (const xmlChar *)std::to_string(dat->getPort()).c_str());
			xmlNewChild(db_node, NULL, (const xmlChar *)"db", (const xmlChar *)dat->getDatabase().c_str());
			xmlNewChild(db_node, NULL, (const xmlChar *)"user", (const xmlChar *)dat->getUsuario().c_str());
			xmlNewChild(db_node, NULL, (const xmlChar *)"pw", (const xmlChar *)dat->getPassword().c_str());
		}
		
				
		int ret = xmlSaveFormatFileEnc(docname.c_str(), doc, "UTF-8", 1);	
		xmlFreeDoc(doc);
		xmlCleanupParser();
		if( ret == -1) return false;	
		return true;
	}


	/**
	* processNode:
	* @reader: the xmlReader
	*
	* Dump information about the current node
	*/
	void CG::processNode(xmlTextReaderPtr reader) 
	{
		const xmlChar *name, *value;

		name = xmlTextReaderConstName(reader);
		if (name == NULL)
		name = BAD_CAST "--";
		value = xmlTextReaderConstValue(reader);
		//printf("%d %d %s %d %d", xmlTextReaderDepth(reader),xmlTextReaderNodeType(reader), name, xmlTextReaderIsEmptyElement(reader),xmlTextReaderHasValue(reader));
		//printf("%d %s ", xmlTextReaderDepth(reader),name);
		//if (value == NULL)
		//{
			//printf("\n");
		//}
		//else 
		{
			
			if (xmlStrlen(value) > 40)
			{
				//printf(" %.40s...\n", value);
			}
			else
			{
				//printf(" %s\n", value);
				if(strcmp((const char*)name,"project") == 0)
				{
					stackXML.push_front((const char*)name);
				}
				else if(value != NULL && strcmp((const char*)name,"name") == 0)
				{
					this->name = (const char*)value;
				}
				else if(value != NULL && strcmp((const char*)name,"directory") == 0)
				{
					this->directory = (const char*)value;
				}
				else if(strcmp((const char*)name,"InputLenguajes") == 0)
				{
					this->directory = (const char*)value;
					if(strcmp((const char*)name,"MySQL"))
					{
						this->inputLenguaje = apidb::InputLenguajes::MySQL_Server;
					}
				}
				else if(strcmp((const char*)name,"OutputLenguajes") == 0)
				{
					this->directory = (const char*)value;
					if(strcmp((const char*)name,"C++"))
					{
						this->outputLenguaje = apidb::OutputLenguajes::CPP;
					}
				}
				else if(strcmp((const char*)name,"version") == 0)
				{
					;
				}
				else if(strcmp((const char*)name,"major") == 0)
				{
					this->version.major = atoi((const char*)value);
				}
				else if(strcmp((const char*)name,"minor") == 0)
				{
					this->version.minor = atoi((const char*)value);
				}
				else if(strcmp((const char*)name,"patch") == 0)
				{
					this->version.patch = atoi((const char*)value);
				}
				else if(strcmp((const char*)name,"patch") == 0)
				{
					this->version.patch = atoi((const char*)value);
				}
				else if(strcmp((const char*)name,"ConectorDB") == 0)
				{
					datconection = new toolkit::clientdb::DatconectionMySQL();
				}
				else if(strcmp((const char*)name,"host") == 0)
				{
					((toolkit::clientdb::DatconectionMySQL*)datconection)->host = (const char*)value;
				}
				else if(strcmp((const char*)name,"port") == 0)
				{
					((toolkit::clientdb::DatconectionMySQL*)datconection)->port = atoi((const char*)value);
				}
				else if(strcmp((const char*)name,"nameDB") == 0)
				{
					((toolkit::clientdb::DatconectionMySQL*)datconection)->database = (const char*)value;
				}
				else if(strcmp((const char*)name,"user") == 0)
				{
					((toolkit::clientdb::DatconectionMySQL*)datconection)->usuario = (const char*)value;
				}
				else if(strcmp((const char*)name,"password") == 0)
				{
					((toolkit::clientdb::DatconectionMySQL*)datconection)->password = (const char*)value;
				}
			}
				
		}
		
		
		return;
	}

	/**
	 * streamFile:
	 * @filename: the file name to parse
	 *
	 * Parse, validate and print information about an XML file.
	 */
	void CG::streamFile(const char *filename) 
	{
		xmlTextReaderPtr reader;
		int ret;

		reader = xmlReaderForFile(filename, NULL,0);
		if (reader != NULL) 
		{
			ret = xmlTextReaderRead(reader);
			while (ret == 1) 
			{
				processNode(reader);
				ret = xmlTextReaderRead(reader);
			}
			xmlFreeTextReader(reader);
			if (ret != 0) 
			{
				fprintf(stderr, "%s : failed to parse\n", filename);
			}
		} 
		else 
		{
			fprintf(stderr, "Unable to open %s\n", filename);
		}
	}

	bool CG::loadConfig(const std::string &docname)
	{
		/*
		 * this initialize the library and check potential ABI mismatches
		 * between the version it was compiled for and the actual shared
		 * library used.
		 */
		streamFile(docname.c_str());

		/*
		 * Cleanup function for the XML library.
		 */
		xmlCleanupParser();
		/*
		 * this is to debug memory for regression tests
		 */
		xmlMemoryDump();
        
		connector = new toolkit::clientdb::Connector();
		analyzer = new mysql::Analyzer(inputLenguaje,outputLenguaje);
		std::cout<<"Analizador creado."<<std::endl;		
		try
		{
			std::cout<<"Conectando a BD."<<std::endl;
			if(datconection == NULL) std::cerr<<"datconection is NULL"<<std::endl;
			bool flag = connector->connect(*datconection);
			if(flag)
			{
				analyzer->setPramsProject(name,directory);
			}
			else
			{
				std::cerr<<"Falló la conexion la BD."<<std::endl;
			}
		}
		catch(toolkit::clientdb::SQLException ex)
		{
			analyzer->getErrorMessage() <<"Fallo la conexion el servidor de datos el cual respondio; "<<std::endl;
		}
    
		return true;		
	}

	OutputLenguajes CG::getOutputLenguaje() const
	{
		return generator->getOutputLenguaje();
	}
	
	bool CG::driving()
	{
		if(analyze())
		{
			if(generate()) return true;
		}
		
		return false;
	}
	
	bool CG::generate()
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
			
			apidb::generators::CPP cpp(*analyzer);
			generator = &cpp;
			flagCPP = cpp.generate();
			
			apidb::generators::CMake cmake(*analyzer);			
			flagCMAKE = cmake.generate();
			
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
	
	bool CG::analyze()
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
            for(internal::Table* table: analyzer->getListTable()) //reading attrubtes by table
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
				
				for(internal::Symbol* attribute: *table)
				{
					//std::cout<<attribute->inType<<std::endl;
					attribute->outType = analyzer->parse(attribute->inType);
				}				
			}				
        }  
          
		return true;
	}
		
	CG::CG(const std::string& name,const std::string& directory,const toolkit::clientdb::Datconection& datconection,InputLenguajes inputLenguaje, OutputLenguajes outputLenguaje,toolkit::Version version)
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
