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


#include <libxml/xmlmemory.h>
#include <libxml/parser.h>


namespace apidb
{
	bool CG::saveConfig(const std::string &docname)
	{
		xmlDocPtr doc  = xmlNewDoc((const xmlChar *)"1.0");
		xmlNodePtr root_node = xmlNewNode(NULL, (const xmlChar *)"project");
		xmlDocSetRootElement(doc, root_node);
		
		xmlNewChild(root_node, NULL, (const xmlChar *)"name", (const xmlChar *)name.c_str());
		xmlNewChild(root_node, NULL, (const xmlChar *)"directory", (const xmlChar *)directory.c_str());
				
		if(xmlSaveFormatFileEnc(docname.c_str(), doc, "UTF-8", 1) == -1) return false;		
		return true;
	}

	bool CG::loadConfig(const std::string &docname)
	{
		xmlDocPtr doc;
		xmlNodePtr cur;

		doc = xmlParseFile(docname.c_str());
		
		if (doc == NULL ) {
			fprintf(stderr,"Document not parsed successfully. \n");
			xmlFreeDoc(doc);
			return false;
		}
		
		cur = xmlDocGetRootElement(doc);
		
		if (cur == NULL) {
			fprintf(stderr,"empty document\n");
			xmlFreeDoc(doc);
			return false;
		}
		
		if (xmlStrcmp(cur->name, (const xmlChar *) "generation")) {
			fprintf(stderr,"document of the wrong type, root node != generation");
			xmlFreeDoc(doc);
			return false;
		}
		
		cur = cur->xmlChildrenNode;
		if ((!xmlStrcmp(cur->name, (const xmlChar *)"name")))
		{
				
		}			 
		cur = cur->next;
		
		xmlFreeDoc(doc);
		
		
		
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
		
	CG::CG(const std::string& name,const std::string& directory,const toolkit::clientdb::Datconection& datconection,InputLenguajes inputLenguaje, OutputLenguajes outputLenguaje)
	{		
		this->inputLenguaje = inputLenguaje;
		this->outputLenguaje = outputLenguaje;
		if(this->inputLenguaje == apidb::InputLenguajes::MySQL_Server)
		{
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
					this->version.set(0,1,0,toolkit::Version::Stage::alpha);
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
