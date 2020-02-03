
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

#include <fstream>
#include <map>
#include <dlfcn.h>

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
    void ConfigureProject::setInputLenguaje(InputLenguajes in)
    {
        inputLenguaje = in;
        loadLibrary();
    }
    InputLenguajes ConfigureProject::getInputLenguaje()const
    {
        return inputLenguaje;
    }
	const ConfigureProject::Table* ConfigureProject::findSelectTable(const std::string& str)const
	{	
		//std::cout << "Buscar '" << str << "' en lista de select." << std::endl;
		std::map<const char*,Table*,symbols::cmp_str>::const_iterator it = selects.find(str.c_str());
		//std::cout << "Select count '" << selects.size() << std::endl;
		if(it != selects.end()) 
		{
			//std::cout << "Se encontro '" << str << "'" << std::endl;
			return it->second;		
		}
		return NULL;
	}
	const ConfigureProject::Table* ConfigureProject::findDownloadTable(const std::string& str)const
	{
		std::map<const char*,Table*,symbols::cmp_str>::const_iterator it = downloads.find(str.c_str());
		if(it != downloads.end()) return it->second;
		return NULL;		
	}
        const std::string& ConfigureProject::Table::getName() const
        {
                return name;        
        }
        ConfigureProject::Table::Table(const std::string& name)
        {
                this->name = name;
        }
        ConfigureProject::Table::~Table()
        {
                for(std::map<const char*, const Function*>::iterator it = begin(); it != end(); it++)
                {
                        delete (it->second);
                }
                clear();
        }
        std::string ConfigureProject::Function::listParams()const
        {
                std::string str;
                for(std::vector<std::string>::const_iterator it = header->begin(); it != header->end(); it ++)
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
        }
        const ConfigureProject::Parameters* ConfigureProject::Function::getParameters() const 
        {
                return header;
        }    
    
        void ConfigureProject::Function::addParam(const std::string& p)
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
                        //header = NULL;
                }
        }
        const std::string& ConfigureProject::Function::getName() const
        {
                return name;
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
            

        /*const std::string& ConfigureProject::getName()const
        {
                return name;
        }
        void ConfigureProject::setName(const std::string& str)
        {
                name = str; 
        }
        const std::string& ConfigureProject::getBuildDirectory()const
        {
                return builDirectory;
        }
        void ConfigureProject::setBuildDirectory(const char* str)
        {
                builDirectory = str;
        }
        const std::string& ConfigureProject::getExecutableTarget()const
        {
                return executable_target;
        }
        void ConfigureProject::setExecutableTarget(const char* str)
        {
                executable_target = str;
        }*/
    ConfigureProject::~ConfigureProject()
    {
        if(!handle) dlclose(handle);        
    }
                
	bool ConfigureProject::testConexion()
    {
        bool ret = false;
        if(inputLenguaje == apidb::InputLenguajes::MySQL)
        {
            //std::cout << "2 - createConnection:" << createConnection << "\n";
            octetos::db::Connector*  connector = createConnection();
            ret = connector->connect(*conectordb);
            connector->close();
        }
        else if(inputLenguaje == apidb::InputLenguajes::PostgreSQL)
        {
            octetos::db::Connector*  connector = createConnection();
            ret = connector->connect(*conectordb);
            connector->close();
        }
        return ret;
    }
    const core::Semver& ConfigureProject::getVersionProject()const
    {
        return projectVersion;
    }
    bool ConfigureProject::loadLibrary()
    {
        //std::cout << "Step 0\n";
        if(inputLenguaje == apidb::InputLenguajes::MySQL)
        {
            handle = dlopen("libapidb-MySQL.so", RTLD_LAZY);
            //std::cout << "Step 1\n";
            if(!handle)
            {
                std::string msgErr ="dlopen fallo con 'libapidb-MySQL.so' : ";
                msgErr = msgErr + dlerror();
                core::Error err(msgErr,core::Error::ERROR_UNKNOW,__FILE__,__LINE__);            
                core::Error::write(err);
                return false;
            }
            //std::cout << "Step 2\n";
            createConnection = (octetos::db::Connector* (*)())dlsym(handle, "createConnector");
            //std::cout << "Step 3\n";
            if(!createConnection)
            {
                std::string msgErr ="dlsym fallo con parse_string:\n" ;
                msgErr = msgErr + "\t" + dlerror();
                core::Error err(msgErr,core::Error::ERROR_UNKNOW,__FILE__,__LINE__);            
                core::Error::write(err);
                return false;
            }
            createDatConnection = (octetos::db::Datconnect* (*)())dlsym(handle, "createDatconnect");
            if(!createDatConnection)
            {
                std::string msgErr ="dlsym fallo con createDatconnect:\n" ;
                msgErr = msgErr + "\t" + dlerror();
                core::Error err(msgErr,core::Error::ERROR_UNKNOW,__FILE__,__LINE__);            
                core::Error::write(err);
                return false;
            }
        }
        else if(inputLenguaje == apidb::InputLenguajes::PostgreSQL)
        {
            handle = dlopen("libapidb-PostgreSQL.so", RTLD_LAZY);
            if(!handle)
            {
                std::string msgErr ="dlopen fallo con 'libapidb-MySQL.so' : ";
                msgErr = msgErr + dlerror();
                core::Error err(msgErr,core::Error::ERROR_UNKNOW,__FILE__,__LINE__);            
                core::Error::write(err);
                return false;
            }
            createConnection = (octetos::db::Connector* (*)())dlsym(handle, "createConnector");
            if(!createConnection)
            {                    
                std::string msgErr ="dlsym fallo con parse_string:\n" ;
                msgErr = msgErr + "\t" + dlerror();
                core::Error err(msgErr,core::Error::ERROR_UNKNOW,__FILE__,__LINE__);            
                core::Error::write(err);
                return false;
            }
            createDatConnection = (octetos::db::Datconnect* (*)())dlsym(handle, "createDatconnect");
            if(!createDatConnection)
            {                    
                std::string msgErr ="dlsym fallo con parse_string:\n" ;
                msgErr = msgErr + "\t" + dlerror();
                core::Error err(msgErr,core::Error::ERROR_UNKNOW,__FILE__,__LINE__);            
                core::Error::write(err);
                return false;
            }
        }
        else
        {      
            std::string msgErr ="dlsym fallo con parse_string:\n" ;
            msgErr = msgErr + "\t" + dlerror();
            core::Error err(msgErr,core::Error::ERROR_UNKNOW,__FILE__,__LINE__);            
            core::Error::write(err);
            return false;
        }
        
        return true;
    }
    ConfigureProject::ConfigureProject()
    {
        handle = NULL;
        createConnection = NULL;
        createDatConnection = NULL;
        conectordb = NULL;
    }
	bool ConfigureProject::saveConfig(const std::string& filename)
	{
        //std::cout << "Saving to " << filename << std::endl;
		xmlDocPtr doc  = xmlNewDoc((const xmlChar *)"1.0");
		xmlNodePtr root_node = xmlNewNode(NULL, (const xmlChar *)"project");
		xmlDocSetRootElement(doc, root_node);
		
		xmlNewChild(root_node, NULL, (const xmlChar *)"name", (const xmlChar *)name.c_str());
		//xmlNewChild(root_node, NULL, (const xmlChar *)"directory", (const xmlChar *)directory.c_str());
				
		xmlNodePtr version_node = xmlNewChild(root_node, NULL, (const xmlChar *)"version", NULL);
		xmlNewChild(version_node, NULL, (const xmlChar *)"major", (const xmlChar *)std::to_string(versionResult.getMajor()).c_str());
		xmlNewChild(version_node, NULL, (const xmlChar *)"minor", (const xmlChar *)std::to_string(versionResult.getMinor()).c_str());
		xmlNewChild(version_node, NULL, (const xmlChar *)"patch", (const xmlChar *)std::to_string(versionResult.getPatch()).c_str());
		
		xmlNodePtr db_node = xmlNewChild(root_node, NULL, (const xmlChar *)"ConectorDB", NULL);
		if(inputLenguaje == apidb::InputLenguajes::MySQL)
		{
			xmlNewChild(db_node, NULL, (const xmlChar *)"host", (const xmlChar *)conectordb->getHost().c_str());
			xmlNewChild(db_node, NULL, (const xmlChar *)"port", (const xmlChar *)std::to_string(conectordb->getPort()).c_str());
			xmlNewChild(db_node, NULL, (const xmlChar *)"nameDB", (const xmlChar *)conectordb->getDatabase().c_str());
			xmlNewChild(db_node, NULL, (const xmlChar *)"user", (const xmlChar *)conectordb->getUser().c_str());
			xmlNewChild(db_node, NULL, (const xmlChar *)"pw", (const xmlChar *)conectordb->getPassword().c_str());
		}
		else if(inputLenguaje == apidb::InputLenguajes::PostgreSQL)
		{
			xmlNewChild(db_node, NULL, (const xmlChar *)"host", (const xmlChar *)conectordb->getHost().c_str());
			xmlNewChild(db_node, NULL, (const xmlChar *)"port", (const xmlChar *)std::to_string(conectordb->getPort()).c_str());
			xmlNewChild(db_node, NULL, (const xmlChar *)"nameDB", (const xmlChar *)conectordb->getDatabase().c_str());
			xmlNewChild(db_node, NULL, (const xmlChar *)"user", (const xmlChar *)conectordb->getUser().c_str());
			xmlNewChild(db_node, NULL, (const xmlChar *)"pw", (const xmlChar *)conectordb->getPassword().c_str());
		}
		else
		{
            core::Error::write(core::Error("Lenguaje de entrada desconocido.",ErrorCodes::CONFIGUREPROJECT_WRITE,__FILE__,__LINE__));
			return false;
		}
                                
        //
        xmlNodePtr inL_node = xmlNewChild(root_node, NULL, (const xmlChar *)"inputLenguaje", NULL);
        switch(inputLenguaje)
        {
            case InputLenguajes::MySQL:
                xmlNewChild(inL_node, NULL, (const xmlChar *)"name", (const xmlChar *)"MySQL");
                break;
            case InputLenguajes::PostgreSQL:
                xmlNewChild(inL_node, NULL, (const xmlChar *)"name", (const xmlChar *)"PostgreSQL");
                break;
            default:
                core::Error::write(core::Error("Lenguaje de entrada desconocido.",ErrorCodes::CONFIGUREPROJECT_WRITE,__FILE__,__LINE__));
                return false;                                
        }
                
                //
                xmlNodePtr outL_node = xmlNewChild(root_node, NULL, (const xmlChar *)"outputLenguaje", NULL);
                switch(outputLenguaje)
                {
                        case OutputLenguajes::CPP:
                                xmlNewChild(outL_node, NULL, (const xmlChar *)"name", (const xmlChar *)"C++");
                                break;
                        case OutputLenguajes::C:
                                xmlNewChild(outL_node, NULL, (const xmlChar *)"name", (const xmlChar *)"C");
                                break;
                        default:
                                core::Error::write(core::Error("Lenguaje de salida desconocido.",ErrorCodes::CONFIGUREPROJECT_WRITE,__FILE__,__LINE__));
                                return false;  
                }
                
                //
                 xmlNodePtr pk_node = xmlNewChild(root_node, NULL, (const xmlChar *)"packing", NULL);
                switch(packing)
                {
                        case PackingLenguajes::CMake:
                                xmlNewChild(pk_node, NULL, (const xmlChar *)"name", (const xmlChar *)"CMake");
                                break;
                        default:
                                core::Error::write(core::Error("Opcion de enpaquetado desconocida.",ErrorCodes::CONFIGUREPROJECT_WRITE,__FILE__,__LINE__));
                                return false;  
                }
                
                //
                 xmlNodePtr cmpl_node = xmlNewChild(root_node, NULL, (const xmlChar *)"compiled", NULL);
                switch(compiled)
                {
                        case Compiled::STATIC:
                                xmlNewChild(cmpl_node, NULL, (const xmlChar *)"name", (const xmlChar *)"STATIC");
                                break;
                        case Compiled::SHARED:
                                xmlNewChild(cmpl_node, NULL, (const xmlChar *)"name", (const xmlChar *)"SHARED");
                                break;
                        default:
                                core::Error::write(core::Error("Opcion de compilado desconocida.",ErrorCodes::CONFIGUREPROJECT_WRITE,__FILE__,__LINE__));
                                return false;  
                }
                
                //
                xmlNewChild(root_node, NULL, (const xmlChar *)"buildDirectory", (const xmlChar *)builDirectory.c_str());
                //std::cout << "Buildd dir " << builDirectory << std::endl;
				
				if(namespace_detect.empty())
				{
					xmlNewChild(root_node, NULL, (const xmlChar *)"namespace_detect", (const xmlChar *)"¿?");
				}
				else
				{
					xmlNewChild(root_node, NULL, (const xmlChar *)"namespace_detect", (const xmlChar *)namespace_detect.c_str());
				}
				 
				if(executable_target.empty())
				{
					xmlNewChild(root_node, NULL, (const xmlChar *)"executable_target", (const xmlChar *)"¿?");
				}
				else
				{
					xmlNewChild(root_node, NULL, (const xmlChar *)"executable_target", (const xmlChar *)executable_target.c_str());
				}
				
				if(writeDatconnect.empty())
				{
					xmlNewChild(root_node, NULL, (const xmlChar *)"writeDatconnect", (const xmlChar *)"¿?");
				}
				else
				{
					xmlNewChild(root_node, NULL, (const xmlChar *)"writeDatconnect", (const xmlChar *)writeDatconnect.c_str());
				}
				//std::cout << std::endl << "Testing 1" << std::endl;
                //
                xmlNodePtr downls_node = xmlNewChild(root_node, NULL, (const xmlChar *)"downloads", NULL);
                int countTbs = 0;
                int counFuns;
                for( std::map<const char*,ConfigureProject::Table*>::const_iterator itT =  downloads.begin(); itT != downloads.end(); itT++)//std::vector<Table>
                {
                        counFuns = 0;
                        countTbs++;
						//std::cout << std::endl << "Testing 1 " << itT->second->getName() << std::endl;
                        xmlNodePtr downls_tb_node = xmlNewChild(downls_node, NULL, (const xmlChar *)"Table", NULL);
                        xmlNewProp(downls_tb_node, BAD_CAST "name", BAD_CAST itT->second->getName().c_str());
                        int countparams;
                        for(std::map<const char*, const Function*>::iterator  itfn = itT->second->begin() ;  itfn !=itT->second->end() ; itfn++)
                        {
                                countparams = 0;
                                counFuns++;
                                xmlNodePtr downls_fn_node = xmlNewChild(downls_tb_node, NULL, (const xmlChar *)"Function", NULL); 
                                xmlNewProp(downls_fn_node, BAD_CAST "name", BAD_CAST itfn->second->getName().c_str());
                                const std::vector<std::string>& params = (const std::vector<std::string>&)*(itfn->second->getParameters());
                                for(auto itParams : params)
                                {
                                        countparams++;
                                        xmlNewChild(downls_fn_node, NULL, (const xmlChar *)"parameter", (const xmlChar *)(itParams.c_str()));
                                }
                                xmlNewProp(downls_fn_node, BAD_CAST "countParams", BAD_CAST std::to_string(countparams).c_str());
                        }
                        xmlNewProp(downls_tb_node, BAD_CAST "countFuns", BAD_CAST std::to_string(counFuns).c_str());
                }
                if(countTbs == 0)
                {
                       xmlNewChild(downls_node, NULL, (const xmlChar *)"Table", NULL);
                }
                xmlNewProp(downls_node, BAD_CAST "countTbs", BAD_CAST std::to_string(countTbs).c_str());
                //std::cout << std::endl << "Testing 2" << std::endl;
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
                                const std::vector<std::string>& params = (const std::vector<std::string>&)*(itfn->second->getParameters());
                                for(auto itParams : params)
                                {
                                        countparams++;
                                        xmlNewChild(selects_fn_node, NULL, (const xmlChar *)"parameter", (const xmlChar *)(itParams.c_str()));
                                }
                                xmlNewProp(selects_fn_node, BAD_CAST "countParams", BAD_CAST std::to_string(countparams).c_str());
                        }
                        xmlNewProp(selects_tb_node, BAD_CAST "countFuns", BAD_CAST std::to_string(counFuns).c_str());
                }
                if(countTbs == 0)
                {
                        xmlNewChild(selects_node, NULL, (const xmlChar *)"Table", NULL);
                }
                xmlNewProp(selects_node, BAD_CAST "countTbs", BAD_CAST std::to_string(countTbs).c_str());
                
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
                std::string tarFilename= "apidb.tar";
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
                        core::Error::write(core::Error(msgstr,ErrorCodes::CONFIGUREPROJECT_FAIL_ON_MOVE_FILE,__FILE__,__LINE__));
                        return false;
                    }                        
                }
                else
                {
                    std::string msgstr = "Especifique el nombre completo del archivo.";
                    core::Error::write(core::Error(msgstr,ErrorCodes::CONFIGUREPROJECT_NOFULL_PATCH_PROJECT,__FILE__,__LINE__));
                    return false;
                }
                
                //std::cout<< "Escritura de proyecto completada." << std::endl;
		return true;
	}
}
}
