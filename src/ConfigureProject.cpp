
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
#include <fstream>
#include <map>

//Tar>>>>>>>>>>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <libtar.h>
//Tar<<<<<<<<<<

#include "apidb.hpp"
#include "ConfigureProject.hpp"
#include "Errors.hpp"

namespace octetos
{
namespace apidb
{

        const std::string& ConfigureProject::Table::getName() const
        {
                return name;        
        }
        ConfigureProject::Table::Table(const std::string& name)
        {
                this->name = name;
        }
        ConfigureProject::Table::Table()
        {
                
        }
    
        std::string ConfigureProject::Function::listParams()const
        {
                std::string str;
                for(std::vector<const char*>::const_iterator it = header->begin(); it != header->end(); it ++)
                {
                        if(it == header->begin()) 
                        {
                                str = (*it);
                        }
                        else 
                        {
                                str += ",";
                                str += (*it);
                        }                        
                }
                
                return str;
        }
        ConfigureProject::Parameters::~Parameters()
        {
                for(auto str : *this)
                {
                        //delete str;¿munmap_chunk(): invalid pointer?
                }
                clear();
        }
        const ConfigureProject::Parameters* ConfigureProject::Function::getParameters() const 
        {
                return header;
        }    
    
        void ConfigureProject::Function::addParam(const char* p)
        {
                if(header == NULL)
                {
                        header = new Parameters();                        
                }
                header->push_back(p);
        }
        ConfigureProject::Function::~Function()
        {
                if(header != NULL)
                {
                        delete header;
                }
        }
        void ConfigureProject::Function::setHeader(Parameters* header)
        {
                this->header = header;
        }
        ConfigureProject::Function::Skeleton ConfigureProject::Function::getSkeleton() const
        {
                return skeleton;
        }
        const std::string& ConfigureProject::Function::getName() const
        {
                return name;
        }
        ConfigureProject::Function::Function(const std::string& name,Skeleton skeleton)
        {
                this->name = name;
                this->skeleton = skeleton;
                header = NULL;
        }
        ConfigureProject::Function::Function(const std::string& name)
        {
                this->name = name;
                header = NULL;
        }
        ConfigureProject::Function::Function()
        {
                header = NULL;
        }
            

	
	bool ConfigureProject::testConexion()
        {
                bool ret = false;
                if(inputLenguaje == apidb::InputLenguajes::MySQL)
                {
                       octetos::toolkit::clientdb::mysql::Connector  connector;
                       ret = connector.connect(conectordb);
                       connector.close();
                }
                return ret;
        }
        const octetos::toolkit::clientdb::mysql::Datconnect& ConfigureProject::getConector() const
        {
                return *conectordb;    
        }
        const octetos::toolkit::Version& ConfigureProject::getVersion()const
        {
                return version;
        }
        const std::string& ConfigureProject::getName()const
        {
                return name;
                
        }
        const std::string& ConfigureProject::getDirectory()const
        {
                return directory;    
        }
        ConfigureProject::ConfigureProject()
        {
                conectordb = NULL;
        }
    
        /*ConfigureProject::ConfigureProject(const ConfigureProject& configProy)
        {
                        this->name = configProy.name;
                        this->directory = configProy.directory;
                        this->version = configProy.version;
                        this->conectordb = configProy.conectordb;
                        this->inputLenguaje = configProy.inputLenguaje;
                        this->outputLenguaje = configProy.outputLenguaje;
                        this->mvc = configProy.mvc;
                        this->downloads = configProy.downloads;
                        this->selects = configProy.selects;
                        this->packing = configProy.packing;                
        }*/

	bool ConfigureProject::saveConfig(const std::string& filename)
	{
		xmlDocPtr doc  = xmlNewDoc((const xmlChar *)"1.0");
		xmlNodePtr root_node = xmlNewNode(NULL, (const xmlChar *)"project");
		xmlDocSetRootElement(doc, root_node);
		
		xmlNewChild(root_node, NULL, (const xmlChar *)"name", (const xmlChar *)name.c_str());
		//xmlNewChild(root_node, NULL, (const xmlChar *)"directory", (const xmlChar *)directory.c_str());
				
		xmlNodePtr version_node = xmlNewChild(root_node, NULL, (const xmlChar *)"version", NULL);
		xmlNewChild(version_node, NULL, (const xmlChar *)"major", (const xmlChar *)std::to_string(version.getMajor()).c_str());
		xmlNewChild(version_node, NULL, (const xmlChar *)"minor", (const xmlChar *)std::to_string(version.getMinor()).c_str());
		xmlNewChild(version_node, NULL, (const xmlChar *)"patch", (const xmlChar *)std::to_string(version.getPatch()).c_str());
		
		xmlNodePtr db_node = xmlNewChild(root_node, NULL, (const xmlChar *)"ConectorDB", NULL);
		if(inputLenguaje == apidb::InputLenguajes::MySQL)
		{
			xmlNewChild(db_node, NULL, (const xmlChar *)"host", (const xmlChar *)conectordb->getHost().c_str());
			xmlNewChild(db_node, NULL, (const xmlChar *)"port", (const xmlChar *)std::to_string(conectordb->getPort()).c_str());
			xmlNewChild(db_node, NULL, (const xmlChar *)"nameDB", (const xmlChar *)conectordb->getDatabase().c_str());
			xmlNewChild(db_node, NULL, (const xmlChar *)"user", (const xmlChar *)conectordb->getUser().c_str());
			xmlNewChild(db_node, NULL, (const xmlChar *)"pw", (const xmlChar *)conectordb->getPassword().c_str());
		}
		else
                {
                        return false;
                }
                                
                //
                xmlNodePtr inL_node = xmlNewChild(root_node, NULL, (const xmlChar *)"inputLenguaje", NULL);
                switch(inputLenguaje)
                {
                        case InputLenguajes::MySQL:
                                xmlNewChild(inL_node, NULL, (const xmlChar *)"name", (const xmlChar *)"MySQL");
                                break;
                }
                
                //
                xmlNodePtr outL_node = xmlNewChild(root_node, NULL, (const xmlChar *)"outputLenguaje", NULL);
                switch(outputLenguaje)
                {
                        case OutputLenguajes::CPP:
                                xmlNewChild(outL_node, NULL, (const xmlChar *)"name", (const xmlChar *)"C++");
                                break;
                }
                
                //
                 xmlNodePtr pk_node = xmlNewChild(root_node, NULL, (const xmlChar *)"packing", NULL);
                switch(packing)
                {
                        case PackingLenguajes::CMake:
                                xmlNewChild(pk_node, NULL, (const xmlChar *)"name", (const xmlChar *)"CMake");
                                break;
                }
                
                //
                 xmlNodePtr cmpl_node = xmlNewChild(root_node, NULL, (const xmlChar *)"compiled", NULL);
                switch(compiled)
                {
                        case Compiled::STATIC:
                                xmlNewChild(cmpl_node, NULL, (const xmlChar *)"name", (const xmlChar *)"STATIC");
                                break;
                }
                
                //
                xmlNodePtr downls_node = xmlNewChild(root_node, NULL, (const xmlChar *)"downloads", NULL);
                int countTbs = 0;
                int counFuns;
                for( std::map<const char*,ConfigureProject::Table*>::const_iterator itT =  downloads.begin(); itT != downloads.end(); itT++)//std::vector<Table>
                {
                        counFuns = 0;
                        countTbs++;
                        xmlNodePtr downls_tb_node = xmlNewChild(downls_node, NULL, (const xmlChar *)"Table", NULL);
                        xmlNewProp(downls_tb_node, BAD_CAST "name", BAD_CAST itT->second-> getName().c_str());
                        int countparams;
                        for(std::map<const char*, const Function*>::iterator  itfn = itT->second->begin() ;  itfn !=itT->second->end() ; itfn++)
                        {
                                countparams = 0;
                                counFuns++;
                                xmlNodePtr downls_fn_node = xmlNewChild(downls_tb_node, NULL, (const xmlChar *)"Function", NULL); 
                                xmlNewProp(downls_fn_node, BAD_CAST "name", BAD_CAST itfn->second->getName().c_str());
                                const std::vector<const char*>& params = (const std::vector<const char*>&)*(itfn->second->getParameters());
                                for(auto itParams : params)
                                {
                                        countparams++;
                                        xmlNodePtr downls_fn_param_node = xmlNewChild(downls_fn_node, NULL, (const xmlChar *)"parameter", (const xmlChar *)(itParams));
                                }
                                xmlNewProp(downls_fn_node, BAD_CAST "countParams", BAD_CAST std::to_string(countparams).c_str());
                        }
                        xmlNewProp(downls_tb_node, BAD_CAST "countFuns", BAD_CAST std::to_string(counFuns).c_str());
                }
                xmlNewProp(downls_node, BAD_CAST "countTbs", BAD_CAST std::to_string(countTbs).c_str());
                
                //
                xmlNodePtr selects_node = xmlNewChild(root_node, NULL, (const xmlChar *)"selects", NULL);
                countTbs = 0;
                for( std::map<const char*,ConfigureProject::Table*>::const_iterator itT =  selects.begin(); itT != selects.end(); itT++)//std::vector<Table>
                {
                        counFuns = 0;
                        countTbs++;
                        xmlNodePtr selects_tb_node = xmlNewChild(selects_node, NULL, (const xmlChar *)"Table", NULL);
                        xmlNewProp(selects_tb_node, BAD_CAST "name", BAD_CAST itT->second->getName().c_str());
                        int countparams;
                        for(std::map<const char*, const Function*>::iterator  itfn = itT->second->begin() ;  itfn !=itT->second->end() ; itfn++)
                        {
                                countparams = 0;
                                counFuns++;
                                xmlNodePtr selects_fn_node = xmlNewChild(selects_tb_node, NULL, (const xmlChar *)"Function", NULL); 
                                xmlNewProp(selects_fn_node, BAD_CAST "name", BAD_CAST itfn->second->getName().c_str());
                                const std::vector<const char*>& params = (const std::vector<const char*>&)*(itfn->second->getParameters());
                                for(auto itParams : params)
                                {
                                        countparams++;
                                        xmlNodePtr downls_fn_param_node = xmlNewChild(selects_fn_node, NULL, (const xmlChar *)"parameter", (const xmlChar *)(itParams));
                                }
                                xmlNewProp(selects_fn_node, BAD_CAST "countParams", BAD_CAST std::to_string(countparams).c_str());
                        }
                        xmlNewProp(selects_tb_node, BAD_CAST "countFuns", BAD_CAST std::to_string(counFuns).c_str());
                }
                xmlNewProp(selects_node, BAD_CAST "countTbs", BAD_CAST std::to_string(countTbs).c_str());
                
                //
                std::string dirProy = "";
		if((directory.empty()) || (directory.compare(".") == 0))
		{
			dirProy = "apidb";
		}
		else
                {
                        dirProy = directory + "/apidb";
                }
                
                char tmp_dirpath[] =  "/tmp/apidb-XXXXXX";
                char * tmp_apidbDir  = mkdtemp(tmp_dirpath);
                std::string tmpDir = tmp_apidbDir;
                
                std::string nameVerFile = tmp_dirpath;
                nameVerFile +=  "/version";
                std::ofstream verF;
                verF.open(nameVerFile);                
                verF << apidb::getPakageVersion().toString()<< std::endl;
                verF.flush();
                verF.close();
                
                 std::string xmlFile = tmp_dirpath;
                 xmlFile += "/main.xml";
                //std::cout<< "Generando archivo de " <<  xmlFile << std::endl;
		int ret = xmlSaveFormatFileEnc(xmlFile.c_str(), doc, "UTF-8", 1);	
		xmlFreeDoc(doc);
		xmlCleanupParser();
		if( ret == -1) return false;  
                
                //comprimiendo archivo
                //std::cout<< "Comprimiendo projecto." << std::endl;
                TAR *pTar;
                std::string tarFilename= dirProy + ".tar";
                tar_open(&pTar, (char*)tarFilename.c_str(), NULL, O_WRONLY | O_CREAT, 0644, TAR_IGNORE_MAGIC);
                std::string filenameProj = "apidb";
                tar_append_tree(pTar, tmp_dirpath, (char*)filenameProj.c_str());
                tar_append_eof(pTar);
                tar_close(pTar);
                //std::cout<< "Archivo comprimido" << std::endl;
                
                if(filename.size()>0)
                {
                        if(rename(tarFilename.c_str(),filename.c_str()) != 0)
                        {
                                std::string msgstr = "Fallo al re-escribir el archivo de proyecto.";
                                toolkit::Error::write(toolkit::Error(msgstr,ErrorCodes::CONFIGUREPROJECT_FAIL_ON_MOVE_FILE,__FILE__,__LINE__));
                                return false;
                        }                        
                }
                else
                {
                        if(rename(tarFilename.c_str(),dirProy.c_str()) != 0)
                        {
                                std::string msgstr = "Fallo al re-escribir el archivo de proyecto.";
                                toolkit::Error::write(toolkit::Error(msgstr,ErrorCodes::CONFIGUREPROJECT_FAIL_ON_MOVE_FILE,__FILE__,__LINE__));
                                return false;
                        }
                }
                
                //std::cout<< "Escritura de proyecto completada." << std::endl;
		return true;
	}
}
}
