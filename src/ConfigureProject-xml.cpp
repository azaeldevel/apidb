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

//generic
#include <stdio.h>

//file operations
#include <fstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

//xml
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>

//
#include <cctype>
#include <cassert>
#include <sstream>
#include <string.h>
#include <iostream>
#include <map>

//Tar>>>>>>>>>>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#if defined WINDOWS_MINGW

#else
    #include <libtar.h>
#endif // defined
//Tar<<<<<<<<<<


#include "common.hpp"
#include "apidb.hpp"
#include "Errors.hpp"


namespace octetos
{
namespace apidb
{
    void ConfigureProject::saveConfig(const std::string& filename)
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
		else if(inputLenguaje == apidb::InputLenguajes::MariaDB)
		{
			xmlNewChild(db_node, NULL, (const xmlChar *)"host", (const xmlChar *)conectordb->getHost().c_str());
			xmlNewChild(db_node, NULL, (const xmlChar *)"port", (const xmlChar *)std::to_string(conectordb->getPort()).c_str());
			xmlNewChild(db_node, NULL, (const xmlChar *)"nameDB", (const xmlChar *)conectordb->getDatabase().c_str());
			xmlNewChild(db_node, NULL, (const xmlChar *)"user", (const xmlChar *)conectordb->getUser().c_str());
			xmlNewChild(db_node, NULL, (const xmlChar *)"pw", (const xmlChar *)conectordb->getPassword().c_str());
		}
		else
		{
            		throw core::Error("Lenguaje de entrada desconocido.",ErrorCodes::CONFIGUREPROJECT_WRITE,__FILE__,__LINE__);
		}

        //
        xmlNodePtr inL_node = xmlNewChild(root_node, NULL, (const xmlChar *)"inputLenguaje", NULL);
        switch(inputLenguaje)
        {
            case InputLenguajes::MySQL:
                xmlNewChild(inL_node, NULL, (const xmlChar *)"name", (const xmlChar *)"MySQL");
                break;
            case InputLenguajes::MariaDB:
                xmlNewChild(inL_node, NULL, (const xmlChar *)"name", (const xmlChar *)"MariaDB");
                break;
            case InputLenguajes::PostgreSQL:
                xmlNewChild(inL_node, NULL, (const xmlChar *)"name", (const xmlChar *)"PostgreSQL");
                break;
            default:
                throw core::Error("Lenguaje de entrada desconocido.",ErrorCodes::CONFIGUREPROJECT_WRITE,__FILE__,__LINE__);
        }

                //
                xmlNodePtr outL_node = xmlNewChild(root_node, NULL, (const xmlChar *)"outputLenguaje", NULL);
                switch(outputLenguaje)
                {
                        case OutputLenguajes::CPP:
                                xmlNewChild(outL_node, NULL, (const xmlChar *)"name", (const xmlChar *)"C++");
                                break;
                        case OutputLenguajes::JAVA:
                                xmlNewChild(outL_node, NULL, (const xmlChar *)"name", (const xmlChar *)"Java");
                                break;
                        default:
                                throw core::Exception("Lenguaje de salida desconocido.",__FILE__,__LINE__);
                }

                //
                 xmlNodePtr pk_node = xmlNewChild(root_node, NULL, (const xmlChar *)"packing", NULL);
                switch(packing)
                {
                        case PackingLenguajes::CMake:
                                xmlNewChild(pk_node, NULL, (const xmlChar *)"name", (const xmlChar *)"CMake");
                                break;
                        case PackingLenguajes::OnlyCode:
                                xmlNewChild(pk_node, NULL, (const xmlChar *)"name", (const xmlChar *)"OnlyCode");
                                break;
                        default:
                                throw core::Exception("Opcion de enpaquetado desconocida.",__FILE__,__LINE__);
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
                                throw core::Exception("Opcion de compilado desconocida.",__FILE__,__LINE__);
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
                #if defined WINDOWS_MINGW
                    int rettmp  = _mktemp_s(tmp_dirpath, strlen(tmp_dirpath));
                #else
                    char * tmp_apidbDir  = mkdtemp(tmp_dirpath);
                #endif // defined
                std::string tmpDir = tmp_dirpath;

                std::string nameVerFile = tmp_dirpath;
                nameVerFile +=  "/version";
                std::ofstream verF;
                verF.open(nameVerFile);
                verF << (std::string)apidb::getPakageVersion();
                verF.flush();
                verF.close();

                 std::string xmlFile = tmp_dirpath;
                 xmlFile += "/main.xml";
                //std::cout<< "Generando archivo de " <<  xmlFile << std::endl;
		int ret = xmlSaveFormatFileEnc(xmlFile.c_str(), doc, "UTF-8", 1);
		xmlFreeDoc(doc);
		xmlCleanupParser();
		if( ret == -1)
		{
			throw core::Error("No se confirmo el guardado de archivo.",ErrorCodes::CONFIGUREPROJECT_WRITE,__FILE__,__LINE__);
		}

                //comprimiendo archivo
                //std::cout<< "Comprimiendo projecto." << std::endl;
      			std::string tarFilename= "apidb.tar";
                compress(tarFilename,tmp_dirpath,"apidb");
                //std::cout<< "Archivo comprimido" << std::endl;

                if(filename.size()>0)
                {
                    if(rename(tarFilename.c_str(),filename.c_str()) != 0)
                    {
                        std::string msgstr = "Fallo al re-escribir el archivo de proyecto.";
                        throw core::Error(msgstr,ErrorCodes::CONFIGUREPROJECT_FAIL_ON_MOVE_FILE,__FILE__,__LINE__);
                    }
                }
                else
                {
                    std::string msgstr = "Especifique el nombre completo del archivo.";
                    throw core::Error(msgstr,ErrorCodes::CONFIGUREPROJECT_NOFULL_PATCH_PROJECT,__FILE__,__LINE__);
                }
	}

	void ConfigureProject::readConfig(const std::string& filename)
	{
        //std::cout << "ConfigureProject::readConfig: Step 2\n";
        std::cout << "ConfigureProject::readConfig Reading : 1 - > " << filename << std::endl;
        FILE *apidbFilecheck = fopen(filename.c_str(), "r");
        if (apidbFilecheck == NULL )
        {
            std::string msg = "La dirección especificada '";
            msg += filename + "' no indica un archivo válido.";
            throw core::Exception(msg,__FILE__,__LINE__);
        }
        //std::cout << "ConfigureProject::readConfig: Step 3\n";

        fclose(apidbFilecheck);
        std::cout << "ConfigureProject::readConfig Step 2.\n" << std::endl;
        char tmp_filepath[] =  "/tmp/XXXXXX";
        #if defined WINDOWS_MINGW
            int rettmp  = _mktemp_s(tmp_filepath, strlen(tmp_filepath));
        #else
            if(mkdtemp(tmp_filepath) == NULL)
            {
                throw core::Exception("Fallo al crear archivo tmporal para descomprimir",__FILE__,__LINE__);
            }
        #endif

        std::cout << "Reading : " << tmp_filepath << "\n";
		bool retpree = unmcompress(filename,tmp_filepath);
        if(!retpree)
        {
            std::string msg = "Fallo al decomprimir archivo '";
			msg += filename + "'";
            throw core::Exception("Fallo al descomprimir con libtar.",__FILE__,__LINE__);
        }

        std::cout << "ConfigureProject::readConfig: Step 3\n";
		std::string tmVerFileName = tmp_filepath;
		tmVerFileName += "/apidb/version";
		//tmVerFileName="apidb/apidbcopy/version";
		//std::cout << "Leyendo version de proyecto. from " << tmVerFileName << std::endl;
		FILE *apidbFilecheck2 = fopen(tmVerFileName.c_str(), "r");
		if (apidbFilecheck2 == NULL )
		{
			std::string msg = "La direecion especificada '";
			msg += tmVerFileName + "' no indica un archivo valido.";
			throw core::Exception(msg,__FILE__,__LINE__);
		}
        std::cout << "ConfigureProject::readConfig: Step 4\n";
        int maxlsize = 20;
		char line_buf[maxlsize];
		if(fgets(line_buf, maxlsize, apidbFilecheck2) == NULL)
        {
			throw core::Exception("Fallo al leer la version del projecto.",__FILE__,__LINE__);            
        }
		std::string strver;
		std::cout << "ConfigureProject::readConfig: Step 5\n";
		std::cout << "strver : '"  << line_buf << "'\n";
		//std::cout << "ConfigureProject::readConfig: Step 8.2\n";
		projectVersion.set(line_buf);
		//projectVersion.setNumbers(5,0,0);
		std::cout << "ConfigureProject::readConfig: Step 6\n";
		//std::cout << "ConfigureProject::readConfig: Step 9\n";

        //std::cout << "ConfigureProject::readConfig: Step 8\n";
		fclose(apidbFilecheck2);

		//std::cout << "Leyendo XML." << std::endl;
		xmlTextReaderPtr reader;
		int ret;
		std::string xmlfile = tmp_filepath;
		xmlfile += "/apidb/main.xml";
		//std::cout << "Parseando XML " << xmlfile << std::endl;
        std::cout << "ConfigureProject::readConfig: Step 7\n";
        reader = xmlReaderForFile(xmlfile.c_str(), NULL, 0);
        //std::cout << "ConfigureProject::readConfig: Step 9.2 " << xmlfile <<  "\n";

        if (reader != NULL)
        {
            //std::cout << "ConfigureProject::readConfig: Step 9.2.1\n";
            ret = xmlTextReaderRead(reader);
			if(ret == -1)
			{
				std::string msg = "Fallo en la lectura de los nodos XML.";
				throw core::Error(msg,0,__FILE__,__LINE__);
			}
            if (!processNode(reader))
            {
                //fprintf(stderr, "%s : failed to parse\n", xmlfile.c_str());
                //throw  core::Error("Fallo duraten el parseo de nodo.",ErrorCodes::Read_FileFailParseNode,__FILE__,__LINE__);
                return;
            }
            //std::cout << "ConfigureProject::readConfig: Step 9.2.2\n";
            xmlFreeTextReader(reader);
            //std::cout << "ConfigureProject::readConfig: Step 9.2.3\n";
        }
        else
        {
				//fprintf(stderr, "Unable to open %s\n", xmlfile.c_str());
				std::string msg = "Fallo al abrir el archivo '";
				msg += msg + xmlfile + "'";
                throw core::Error(msg,ErrorCodes::READFILE_OPENXMLFILE,__FILE__,__LINE__);
        }
	}

	bool ConfigureProject::getProjectNodes(xmlTextReaderPtr reader)
	{
		//std::cout << "ConfigureProject::getProjectNodes: Step 1\n";
		if(projectVersion < ver100)
		{
			std::string msgstr = "La version del proyecto es inferior a '";
			msgstr += ver100.toString() + "' por lo que no es posible abrir el archivo, se recomiend crear un nuevo proyecto con los mismo parametros.";
			core::Error::write(core::Error(msgstr,ErrorCodes::CONFIGUREPROJECT_NOCOMPATIBLE_PROJECT,__FILE__,__LINE__));
			return false;
		}

		const xmlChar *name;

		//std::cout << "ConfigureProject::getProjectNodes: Step 2\n";
        std::string password,user,host,database;
        int port;
        if(projectVersion >= ver100)
        {
			//std::cout << "ConfigureProject::getProjectNodes: Step 2.1\n";
			xmlTextReaderRead(reader);
			xmlTextReaderRead(reader);
			xmlTextReaderRead(reader);
			//std::cout << "ConfigureProject::getProjectNodes: Step 2.1.1\n";
			name = xmlTextReaderConstName(reader);
			//std::cout << "ConfigureProject::getProjectNodes: Step 2.1.2\n";
			if(strcmp((const char*)name,"#text") == 0)
			{
				//std::cout << "ConfigureProject::getProjectNodes: Step 2.1.2.1\n";
				this->name = (const char*)xmlTextReaderConstValue(reader);
				//std::cout << "ConfigureProject::getProjectNodes: Step 2.1.2.2\n";
			}

			//std::cout << "ConfigureProject::getProjectNodes: Step 2.2\n";
                xmlTextReaderRead(reader);
                xmlTextReaderRead(reader);
                xmlTextReaderRead(reader);
                xmlTextReaderRead(reader);
                xmlTextReaderRead(reader);
                name = xmlTextReaderConstName(reader);
                if(strcmp((const char*)name,"major") == 0)
                {
                xmlTextReaderRead(reader);
                }
                else
                {
                        std::string msgstr = "Fallo durante el parseo XML.";
                        core::Error::write(core::Error(msgstr,ErrorCodes::CONFIGUREPROJECT_PARSE_XML,__FILE__,__LINE__));
                        return false;
                }
                name = xmlTextReaderConstName(reader);
                short major = 0;
                if(strcmp((const char*)name,"#text") == 0)
                {
                major = atoi((const char*)xmlTextReaderConstValue(reader));
                }
                else
                {
                        std::string msgstr = "Fallo durante el parseo XML.";
                        core::Error::write(core::Error(msgstr,ErrorCodes::CONFIGUREPROJECT_PARSE_XML,__FILE__,__LINE__));
                        return false;
                }
		//std::cout << "ConfigureProject::getProjectNodes: Step 2.3\n";
                xmlTextReaderRead(reader);
                xmlTextReaderRead(reader);
                xmlTextReaderRead(reader);
                name = xmlTextReaderConstName(reader);
                if(strcmp((const char*)name,"minor") == 0)
                {
                xmlTextReaderRead(reader);
                }
                else
                {
                        std::string msgstr = "Fallo durante el parseo XML.";
                        core::Error::write(core::Error(msgstr,ErrorCodes::CONFIGUREPROJECT_PARSE_XML,__FILE__,__LINE__));
                        return false;
                }
		//std::cout << "ConfigureProject::getProjectNodes: Step 2.4\n";
                name = xmlTextReaderConstName(reader);
                short minor = 0;
                if(strcmp((const char*)name,"#text") == 0)
                {
                minor = atoi((const char*)xmlTextReaderConstValue(reader));
                }
                else
                {
                        std::string msgstr = "Fallo durante el parseo XML.";
                        core::Error::write(core::Error(msgstr,ErrorCodes::CONFIGUREPROJECT_PARSE_XML,__FILE__,__LINE__));
                        return false;
                }
		//std::cout << "ConfigureProject::getProjectNodes: Step 2.5\n";
                xmlTextReaderRead(reader);
                xmlTextReaderRead(reader);
                xmlTextReaderRead(reader);
                name = xmlTextReaderConstName(reader);
                if(strcmp((const char*)name,"patch") == 0)
                {
                xmlTextReaderRead(reader);
                }
                else
                {
                        std::string msgstr = "Fallo durante el parseo XML.";
                        core::Error::write(core::Error(msgstr,ErrorCodes::CONFIGUREPROJECT_PARSE_XML,__FILE__,__LINE__));
                        return false;
                }
		//std::cout << "ConfigureProject::getProjectNodes: Step 2.6\n";
                name = xmlTextReaderConstName(reader);
                short patch = 0;
                if(strcmp((const char*)name,"#text") == 0)
                {
                patch = atoi((const char*)xmlTextReaderConstValue(reader));
                }
                else
                {
                        std::string msgstr = "Fallo durante el parseo XML.";
                        core::Error::write(core::Error(msgstr,ErrorCodes::CONFIGUREPROJECT_PARSE_XML,__FILE__,__LINE__));
                        return false;
                }
		//std::cout << "ConfigureProject::getProjectNodes: Step 2.7\n";
                //std::cout << "Numbers readed : " << major << "." << minor << "." << patch << std::endl;
                this->versionResult.setNumbers(major,minor,patch);
		//std::cout << "ConfigureProject::getProjectNodes: Step 2.8\n";
        // this->version.setStage(toolkit::Version::Stage::alpha);

                xmlTextReaderRead(reader);
                xmlTextReaderRead(reader);
                xmlTextReaderRead(reader);
                xmlTextReaderRead(reader);
                xmlTextReaderRead(reader);
                xmlTextReaderRead(reader);
                xmlTextReaderRead(reader);
                name = xmlTextReaderConstName(reader);
                if(strcmp((const char*)name,"host") == 0)
                {
                    xmlTextReaderRead(reader);
                }
                else
                {
                    std::string msgstr = "Fallo durante el parseo XML.";
                    throw core::Exception(msgstr,__FILE__,__LINE__);
                }
                name = xmlTextReaderConstName(reader);
                host = "";
                if(strcmp((const char*)name,"#text") == 0)
                {
                    host = (const char*)xmlTextReaderConstValue(reader);
                }
                else
                {
                    std::string msgstr = "Fallo durante el parseo XML.";
                    throw core::Exception(msgstr,__FILE__,__LINE__);
                }

                xmlTextReaderRead(reader);
                xmlTextReaderRead(reader);
                xmlTextReaderRead(reader);
                name = xmlTextReaderConstName(reader);
                if(strcmp((const char*)name,"port") == 0)
                {
                    xmlTextReaderRead(reader);
                }
                else
                {
                    std::string msgstr = "Fallo durante el parseo XML.";
                    throw core::Exception(msgstr,__FILE__,__LINE__);
                }
                name = xmlTextReaderConstName(reader);
                port = 0;
                if(strcmp((const char*)name,"#text") == 0)
                {
                    port = atoi((const char*)xmlTextReaderConstValue(reader));
                }
                else
                {
                    std::string msgstr = "Fallo durante el parseo XML.";
                    throw core::Exception(msgstr,__FILE__,__LINE__);
                }

                xmlTextReaderRead(reader);
                xmlTextReaderRead(reader);
                xmlTextReaderRead(reader);
                name = xmlTextReaderConstName(reader);
                if(strcmp((const char*)name,"nameDB") == 0)
                {
                    xmlTextReaderRead(reader);
                }
                else
                {
                    std::string msgstr = "Fallo durante el parseo XML.";
                    throw core::Exception(msgstr,__FILE__,__LINE__);
                }
                name = xmlTextReaderConstName(reader);
                database = "";
                if(strcmp((const char*)name,"#text") == 0)
                {
                    database = (const char*)xmlTextReaderConstValue(reader);
                }
                else
                {
                    std::string msgstr = "Fallo durante el parseo XML.";
                    throw core::Exception(msgstr,__FILE__,__LINE__);
                }

                xmlTextReaderRead(reader);
                xmlTextReaderRead(reader);
                xmlTextReaderRead(reader);
                name = xmlTextReaderConstName(reader);
                if(strcmp((const char*)name,"user") == 0)
                {
                    xmlTextReaderRead(reader);
                }
                else
                {
                    std::string msgstr = "Fallo durante el parseo XML.";
                    throw core::Exception(msgstr,__FILE__,__LINE__);
                }
                name = xmlTextReaderConstName(reader);
                user = "";
                if(strcmp((const char*)name,"#text") == 0)
                {
                user = (const char*)xmlTextReaderConstValue(reader);
                }
                else
                {
                    std::string msgstr = "Fallo durante el parseo XML.";
                    throw core::Exception(msgstr,__FILE__,__LINE__);
                }

                xmlTextReaderRead(reader);
                xmlTextReaderRead(reader);
                xmlTextReaderRead(reader);
                name = xmlTextReaderConstName(reader);
                if(strcmp((const char*)name,"pw") == 0)
                {
                    xmlTextReaderRead(reader);
                }
                else
                {
                    std::string msgstr = "Fallo durante el parseo XML.";
                    throw core::Exception(msgstr,__FILE__,__LINE__);
                }
                name = xmlTextReaderConstName(reader);
                password = "";
                if(strcmp((const char*)name,"#text") == 0)
                {
                    password = (const char*)xmlTextReaderConstValue(reader);
                }
                else
                {
                    std::string msgstr = "Fallo durante el parseo XML.";
                    throw core::Exception(msgstr,__FILE__,__LINE__);
                }

        }

        //std::cout << "ConfigureProject::getProjectNodes: Step 3\n";
                /******************************************************************************************
                * Version 1.1.0
                *
                * *//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //std::cout << "ConfigureProject::getProjectNodes : v1.1.0 update.\n";
        if(projectVersion >= ver110)//la lectura es compatible con versiones anteriores del projecto
        {
            std::cout << "ConfigureProject::getProjectNodes : projectVersion 1.\n";
                xmlTextReaderRead(reader);
                xmlTextReaderRead(reader);
                xmlTextReaderRead(reader);
                xmlTextReaderRead(reader);
                xmlTextReaderRead(reader);
                xmlTextReaderRead(reader);
                xmlTextReaderRead(reader);
                xmlTextReaderRead(reader);
                name = xmlTextReaderConstName(reader);
                std::string inL = (const char*)xmlTextReaderConstValue(reader);
                //std::cout << "ConfigureProject::getProjectNodes : v1.1.0 update - 2.\n";
                if(inL.compare("MySQL") == 0 )
                {
                    std::cout << "ConfigureProject::getProjectNodes : projectVersion 2.\n";
                	if(checkLibrary(InputLenguajes::MySQL))
                	{
		                setInputLenguaje(InputLenguajes::MySQL);
		                conectordb = createDatConnection();
		                conectordb->set(InputLenguajes::MySQL,host,port,database,user,password);
                    }
                    else if(checkLibrary(InputLenguajes::MariaDB))
                	{
		                setInputLenguaje(InputLenguajes::MariaDB);
		                conectordb = createDatConnection();
		                conectordb->set(InputLenguajes::MariaDB,host,port,database,user,password);
                    }
                    else
                    {
		                failLoadDat = true;
                    }
                    failLoadDat = false;
                }
                else if(inL.compare("PostgreSQL") == 0)
                {
                    std::cout << "ConfigureProject::getProjectNodes : projectVersion 2.\n";
                	if(checkLibrary(InputLenguajes::PostgreSQL))
                	{
		                setInputLenguaje(InputLenguajes::PostgreSQL);
		                conectordb = createDatConnection();
		                conectordb->set(InputLenguajes::PostgreSQL,host,port,database,user,password);
                    }
                    else
                    {
		                failLoadDat = true;
                    }
                    failLoadDat = false;
                }
                else if(inL.compare("MariaDB") == 0)
                {
                    std::cout << "ConfigureProject::getProjectNodes : projectVersion 3.\n";
                	if(checkLibrary(InputLenguajes::MariaDB))
                	{
                        std::cout << "ConfigureProject::getProjectNodes : projectVersion 3.1a.\n";
		                setInputLenguaje(InputLenguajes::MariaDB);
		                conectordb = createDatConnection();
		                conectordb->set(InputLenguajes::MariaDB,host,port,database,user,password);
                    }
                    else if(checkLibrary(InputLenguajes::MySQL))
                	{
                        std::cout << "ConfigureProject::getProjectNodes : projectVersion 3.1b.\n";
		                setInputLenguaje(InputLenguajes::MySQL);
		                conectordb = createDatConnection();
		                conectordb->set(InputLenguajes::MySQL,host,port,database,user,password);
                    }
                    else
                    {
                        std::cout << "ConfigureProject::getProjectNodes : projectVersion 31c.\n";
		                failLoadDat = true;
                    }
                    std::cout << "ConfigureProject::getProjectNodes : projectVersion 3.2.\n";
                    failLoadDat = false;
                }
                else
                {
                    std::string msgstr = "Fallo durante el parseo XML. El driver '";
                    msgstr += inL;
                    msgstr += "' solicitado no existe o no hay soporte activo para dicho driver.";
                    core::Exception(msgstr,__FILE__,__LINE__);
                }
                /*if(handle == NULL)
                {
                	std::string msgstr = "Fallo durante el parseo XML. El driver '";
                    msgstr += inL;
                    msgstr += "' solicitado no existe o no hay soporte activo para dicho driver.";
                    core::Error::write(core::Error(msgstr,ErrorCodes::CONFIGUREPROJECT_PARSE_XML,__FILE__,__LINE__));
                    return false;
                }*/
                //std::cout << "ConfigureProject::getProjectNodes : v1.1.0 update - 3\n";
                //
                xmlTextReaderRead(reader);
                xmlTextReaderRead(reader);
                xmlTextReaderRead(reader);
                xmlTextReaderRead(reader);
                xmlTextReaderRead(reader);
                xmlTextReaderRead(reader);
                xmlTextReaderRead(reader);
                xmlTextReaderRead(reader);
                name = xmlTextReaderConstName(reader);
                std::string outL = (const char*)xmlTextReaderConstValue(reader);
                if(outL.compare("C++") == 0)
                {
                        outputLenguaje = OutputLenguajes::CPP;
                }
                else if(outL.compare("Java") == 0)
                {
                        outputLenguaje = OutputLenguajes::JAVA;
                }
                else
                {
                        std::string msgstr = "Fallo durante el parseo XML.";
                        core::Error::write(core::Error(msgstr,ErrorCodes::CONFIGUREPROJECT_PARSE_XML,__FILE__,__LINE__));
                        return false;
                }

                //
                xmlTextReaderRead(reader);
                xmlTextReaderRead(reader);
                xmlTextReaderRead(reader);
                xmlTextReaderRead(reader);
                xmlTextReaderRead(reader);
                xmlTextReaderRead(reader);
                xmlTextReaderRead(reader);
                xmlTextReaderRead(reader);
                name = xmlTextReaderConstName(reader);
                std::string pk = (const char*)xmlTextReaderConstValue(reader);
                if(pk.compare("CMake") == 0)
                {
                    packing = PackingLenguajes::CMake;
                }
                else if(pk.compare("OnlyCode") == 0)
                {
                    packing = PackingLenguajes::OnlyCode;
                }
                else
                {
                        std::string msgstr = "Fallo durante el parseo XML.";
                        core::Error::write(core::Error(msgstr,ErrorCodes::CONFIGUREPROJECT_PARSE_XML,__FILE__,__LINE__));
                        return false;
                }

                //
                xmlTextReaderRead(reader);
                xmlTextReaderRead(reader);
                xmlTextReaderRead(reader);
                xmlTextReaderRead(reader);
                xmlTextReaderRead(reader);
                xmlTextReaderRead(reader);
                xmlTextReaderRead(reader);
                xmlTextReaderRead(reader);
                name = xmlTextReaderConstName(reader);
                std::string cmpl = (const char*)xmlTextReaderConstValue(reader);
                if(cmpl.compare("STATIC") == 0)
                {
                        compiled = Compiled::STATIC;
                }
                else if(cmpl.compare("SHARED") == 0)
                {
                        compiled = Compiled::SHARED;
                }
                else
                {
                        std::string msgstr = "Fallo durante el parseo XML.";
                        core::Error::write(core::Error(msgstr,ErrorCodes::CONFIGUREPROJECT_PARSE_XML,__FILE__,__LINE__));
                        return false;
                }

                xmlTextReaderRead(reader);
                xmlTextReaderRead(reader);
                xmlTextReaderRead(reader);
                xmlTextReaderRead(reader);
                xmlTextReaderRead(reader);
                name = xmlTextReaderConstName(reader);
                if(strcmp((const char*)name,"buildDirectory") == 0)
                {
                        //std::cout << "Se encontro build directory." << std::endl;
                        xmlTextReaderRead(reader);
                        builDirectory = (const char*)xmlTextReaderConstValue(reader);
                        //std::cout << "Se encontro build directory : " << builDirectory << std::endl;
                }
                else
                {
                        std::string msgstr = "Fallo durante el parseo XML.";
                        core::Error::write(core::Error(msgstr,ErrorCodes::CONFIGUREPROJECT_PARSE_XML,__FILE__,__LINE__));
                        return false;
        }

		//std::cout << "ConfigureProject::getProjectNodes: Step 4\n";

        //std::cout << "ConfigureProject::getProjectNodes : v2.0.0 feactures.\n";
		if(projectVersion >= ver200)
		{
                xmlTextReaderRead(reader);
                xmlTextReaderRead(reader);
                xmlTextReaderRead(reader);
                name = xmlTextReaderConstName(reader);
                if(strcmp((const char*)name,"namespace_detect") == 0)
                {
                        //std::cout << "Se encontro build directory." << std::endl;
                        xmlTextReaderRead(reader);
                        namespace_detect = (const char*)xmlTextReaderConstValue(reader);
                        //std::cout << "Se encontro build directory : " << builDirectory << std::endl;
                }
                else
                {
                        std::string msgstr = "Fallo durante el parseo XML.";
                        core::Error::write(core::Error(msgstr,ErrorCodes::CONFIGUREPROJECT_PARSE_XML,__FILE__,__LINE__));
                        return false;
                }

                xmlTextReaderRead(reader);
                xmlTextReaderRead(reader);
                xmlTextReaderRead(reader);
                name = xmlTextReaderConstName(reader);
                if(strcmp((const char*)name,"executable_target") == 0)
                {
                        //std::cout << "Se encontro build directory." << std::endl;
                        xmlTextReaderRead(reader);
                        executable_target = (const char*)xmlTextReaderConstValue(reader);
                        //std::cout << "Se encontro build directory : " << builDirectory << std::endl;
                }
                else
                {
                        std::string msgstr = "Fallo durante el parseo XML.";
                        core::Error::write(core::Error(msgstr,ErrorCodes::CONFIGUREPROJECT_PARSE_XML,__FILE__,__LINE__));
                        return false;
                }
		}

		//std::cout << "ConfigureProject::getProjectNodes: Step 5\n";

		if(projectVersion >= ver220)
		{
			xmlTextReaderRead(reader);
			xmlTextReaderRead(reader);
			xmlTextReaderRead(reader);
			name = xmlTextReaderConstName(reader);
			if(strcmp((const char*)name,"writeDatconnect") == 0)
			{
				//std::cout << "Se encontro build directory." << std::endl;
				xmlTextReaderRead(reader);
				writeDatconnect = (const char*)xmlTextReaderConstValue(reader);
				//std::cout << "Se encontro build directory : " << builDirectory << std::endl;
			}
			else
			{
					std::string msgstr = "Fallo durante el parseo XML.";
					core::Error::write(core::Error(msgstr,ErrorCodes::CONFIGUREPROJECT_PARSE_XML,__FILE__,__LINE__));
					return false;
			}
		}
		//std::cout << "ConfigureProject::getProjectNodes: Step 6\n";

        //
        xmlTextReaderRead(reader);
        xmlTextReaderRead(reader);
        xmlTextReaderRead(reader);

		//const xmlChar *nameNodeList;
		for(int i = 0; i < 2; i++)
		{
			//std::cout << "Node  : " <<(const char*)xmlTextReaderConstName(reader)<<std::endl;
			//std::cout << ", count : " << (const char*)xmlGetProp(xmlTextReaderCurrentNode(reader), (const xmlChar *)"countTbs") << std::endl;
			std::string node = (const char*)xmlTextReaderConstName(reader);
			if(node.compare("downloads") != 0 and node.compare("selects") != 0)
			{
				std::string msgstr = "Fallo durante el parseo XML.";
				core::Error::write(core::Error(msgstr,ErrorCodes::CONFIGUREPROJECT_PARSE_XML,__FILE__,__LINE__));
				return false;
			}
                int counTbs = atoi((const char*)xmlGetProp(xmlTextReaderCurrentNode(reader), (const xmlChar *)"countTbs"));
                for(int j = 0; j < counTbs; j++)
                {
                        xmlTextReaderRead(reader);
                        xmlTextReaderRead(reader);
                        //std::cout << "\tTable  : " <<(const char*)xmlGetProp(xmlTextReaderCurrentNode(reader), (const xmlChar *)"name") << std::endl;
                        Table* ptb = new Table((const char*)xmlGetProp(xmlTextReaderCurrentNode(reader), (const xmlChar *)"name"));
                        int counFuns = atoi((const char*)xmlGetProp(xmlTextReaderCurrentNode(reader), (const xmlChar *)"countFuns"));
                        xmlTextReaderRead(reader);
                        xmlTextReaderRead(reader);
                        for(int i = 0; i < counFuns; i++)
                        {
                                //std::cout << "\t\tFn : " << (const char*)xmlGetProp(xmlTextReaderCurrentNode(reader), (const xmlChar *)"name") << std::endl;
                                Function*  pfn = new Function((const char*)xmlGetProp(xmlTextReaderCurrentNode(reader), (const xmlChar *)"name"));
                                int counParams = atoi((const char*)xmlGetProp(xmlTextReaderCurrentNode(reader), (const xmlChar *)"countParams"));
                                xmlTextReaderRead(reader);
                                xmlTextReaderRead(reader);
                                xmlTextReaderRead(reader);
                                //Parameters* pparams = new Parameters();
                                for(int i = 0 ; i < counParams; i++)
                                {
                                        //std::cout << "\t\t\tParameter : " << (const char*)xmlTextReaderConstValue(reader) << std::endl;
                                        const char* strParam = (const char*)xmlTextReaderConstValue(reader);
                                        int strl = strlen(strParam);
                                        char* strnew = new char[strl + 1];
                                        strcpy(strnew,strParam);
                                        //pparams->push_back(strnew);
                                        pfn->addParam(strnew);
                                        xmlTextReaderRead(reader);
                                        xmlTextReaderRead(reader);
                                        xmlTextReaderRead(reader);
                                        xmlTextReaderRead(reader);
                                }
                                //pfn->setHeader(pparams);
                                ptb->insert(std::make_pair(pfn->getName().c_str(), pfn));
                                xmlTextReaderRead(reader);
                        }
                        if(node.compare("downloads") == 0)
                        {
                                //std::cout << "\tAdded " << ptb->getName() << std::endl;
                                downloads.insert(std::make_pair(ptb->getName().c_str(),ptb));
                        }
                        else if(node.compare("selects") == 0)
                        {
                                //std::cout << "\tAdded " << ptb->getName() << std::endl;
                                selects.insert(std::make_pair(ptb->getName().c_str(),ptb));
                        }
                }
                xmlTextReaderRead(reader);
                xmlTextReaderRead(reader);
                xmlTextReaderRead(reader);
                xmlTextReaderRead(reader);
        }
        }

        //std::cout << "ConfigureProject::getProjectNodes: Step 7\n";

        return true;
    }


    bool ConfigureProject::processNode(xmlTextReaderPtr reader)
    {
        //const xmlChar *name = xmlTextReaderConstName(reader);
        //std::cout << "ConfigureProject::processNode: Step 1\n";
        if(xmlTextReaderNodeType(reader) == 1) //es d apertura?
        {
            ///std::cout << "ConfigureProject::processNode: Step 1.1\n";
            if(!getProjectNodes(reader))
            {
                                if(core::Error::check())
                                {
                                        return false;
                                }
                                else
                                {
                                        std::string msgstr = "Fallo durante el parseo XML.";
                                        core::Error::write(core::Error(msgstr,ErrorCodes::CONFIGUREPROJECT_PARSE_XML,__FILE__,__LINE__));
                                        return false;
                                }
            }
        }
        else if( xmlTextReaderNodeType(reader) == 15) //es d cerradura?
        {
                //stack.pop_front();
        }
        //std::cout << "ConfigureProject::processNode: Step 2\n";

        return true;
    }

}
}
