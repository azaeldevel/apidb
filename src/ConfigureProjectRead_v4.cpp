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
#include <libtar.h>
//Tar<<<<<<<<<<


#include "common.hpp"
#include "apidb.hpp"
#include "Errors.hpp"


namespace octetos
{
namespace apidb
{
	bool ConfigureProject::readConfig(const std::string filename)
	{
		if(core::Error::check())
		{
			//throw core::Error("Hay un error pendiente de atender",core::Error::Codes::ERROR_NOTADDRESSED,__FILE__,__LINE__);
		}
        
        //std::cout << "Reading : " << filename << std::endl;
        FILE *apidbFilecheck = fopen(filename.c_str(), "r");
        if (apidbFilecheck == NULL )
        {
            std::string msg = "La dirección especificada '";
            msg += filename + "' no indica un archivo válido.";
            core::Error::write(core::Error(msg,ErrorCodes::READFILE_INVALIDPATH,__FILE__,__LINE__));
            return false;
        }
                fclose(apidbFilecheck);
                //std::cout << "Step 2." << std::endl;
                char tmp_filepath[] =  "/tmp/XXXXXXXXX";
                char * tmp_apidbDir  = mkdtemp(tmp_filepath);       
                //Descomomprimiendo archivo
                
                //std::cout << "Descomprimiendo achivo." << std::endl;
                TAR* tar_handle = NULL;
                tar_open(&tar_handle, (char*) filename.c_str(), NULL,  O_RDONLY,  0644,  TAR_GNU);
                if (tmp_apidbDir == NULL) 
                {
                        //fprintf(stderr, "Failed to build temp file.\n");
                        core::Error::write(core::Error("No se puede crear el directorio tempora para desempauqetar el archivo de proyecto.",ErrorCodes::READFILE_TEMPUNPACKFAIL,__FILE__,__LINE__));
                        return false;
                }
                //std::cout << "tar_handle is " << tmp_apidbDir << std::endl;
                //std::cout << "tmp_filepath "<< tmp_filepath  << std::endl;
                if(tar_extract_all(tar_handle, tmp_apidbDir) != 0)
                {
                        core::Error::write( core::Error("Fallo duraten las descompresion del archivo.",ErrorCodes::Read_UncomConfigFile,__FILE__,__LINE__));
                        //std::cout << "Fallo duraten las descompresion del archivo." << std::endl;
                }
                tar_close(tar_handle);
                                
                std::string tmVerFileName = tmp_apidbDir;
                tmVerFileName += "/apidb/version";
                //tmVerFileName="apidb/apidbcopy/version";
                //std::cout << "Leyendo version de proyecto. from " << tmVerFileName << std::endl;                
                FILE *apidbFilecheck2 = fopen(tmVerFileName.c_str(), "r");
                if (apidbFilecheck2 == NULL )
                {
                        std::string msg = "La direecion especificada '";
                        msg += tmVerFileName + "' no indica un archivo valido.";
                        core::Error::write(core::Error(msg,ErrorCodes::READFILE_INVALIDPATHVER,__FILE__,__LINE__));
                        return false;
                }
                char *line_buf = NULL;
                size_t line_buf_size = 0;
                int line_count = 0;
                ssize_t line_size;
                line_size = getline(&line_buf, &line_buf_size, apidbFilecheck2);
                std::string strver;
                if(line_size > 0)
                {
                    strver = line_buf;
                }
                fclose(apidbFilecheck2);
                if(!projectVersion.set(strver)) 
                {
                    core::Semver ver;
                    if(!ver.extractNumbers(strver))
                    {
                        std::string msg = "Fallo el parseo de la cadena de version en la llamada a Version::fromFile.'";
                        msg += strver + "'";
                        core::Error::write(core::Error(msg,ErrorCodes::READFILE_FAILPARSERVER,__FILE__,__LINE__));
                        return false;
                    }
                    else
                    {
                        projectVersion = ver;
                    }
                }
                
                //std::cout << "Leyendo XML." << std::endl;  
                xmlTextReaderPtr reader;
                int ret;                
                std::string xmlfile = tmp_filepath;
                xmlfile += "/apidb/main.xml";
                //std::cout << "Parseando XML " << xmlfile << std::endl;  
                reader = xmlReaderForFile(xmlfile.c_str(), NULL, 0);
                if (reader != NULL) 
                {
                        ret = xmlTextReaderRead(reader);               
                        if (!processNode(reader)) 
                        {
                                //fprintf(stderr, "%s : failed to parse\n", xmlfile.c_str());
                                 core::Error::write( core::Error("Fallo duraten el parseo de nodo.",ErrorCodes::Read_FileFailParseNode,__FILE__,__LINE__));
                                return false;
                        }
                        xmlFreeTextReader(reader);
                }
                else 
                {
                        //fprintf(stderr, "Unable to open %s\n", xmlfile.c_str());
                        std::string msg = "Fallo al abrir el archivo '";
                        msg += msg + xmlfile + "'";
                         core::Error::write( core::Error(msg,ErrorCodes::READFILE_OPENXMLFILE,__FILE__,__LINE__));
                        return false;
                }
                                
                
                return true;
	}
       
	bool ConfigureProject::getProjectNodes(xmlTextReaderPtr reader)
	{         
                if(projectVersion < ver100)
                {
                        std::string msgstr = "La version del proyecto es inferior a '";
                        msgstr += ver100.toString() + "' por lo que no es posible abrir el archivo, se recomiend crear un nuevo proyecto con los mismo parametros.";
                        core::Error::write(core::Error(msgstr,ErrorCodes::CONFIGUREPROJECT_NOCOMPATIBLE_PROJECT,__FILE__,__LINE__));
                        return false;
                }
                
                const xmlChar *name;
                
        std::string password,user,host,database;
        int port;
        if(projectVersion >= ver100)
        {
                
                xmlTextReaderRead(reader);
                xmlTextReaderRead(reader);
                xmlTextReaderRead(reader);
                name = xmlTextReaderConstName(reader);
                if(strcmp((const char*)name,"#text") == 0)
                {
                //std::cout<<"Find text."<<std::endl;   
                this->name = (const char*)xmlTextReaderConstValue(reader);
                }
                 
        
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
                //std::cout << "Numbers readed : " << major << "." << minor << "." << patch << std::endl;
                this->versionResult.setNumbers(major,minor,patch);
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
                        core::Error::write(core::Error(msgstr,ErrorCodes::CONFIGUREPROJECT_PARSE_XML,__FILE__,__LINE__));
                        return false;
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
                        core::Error::write(core::Error(msgstr,ErrorCodes::CONFIGUREPROJECT_PARSE_XML,__FILE__,__LINE__));
                        return false;
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
                        core::Error::write(core::Error(msgstr,ErrorCodes::CONFIGUREPROJECT_PARSE_XML,__FILE__,__LINE__));
                        return false;
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
                        core::Error::write(core::Error(msgstr,ErrorCodes::CONFIGUREPROJECT_PARSE_XML,__FILE__,__LINE__));
                        return false;
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
                        core::Error::write(core::Error(msgstr,ErrorCodes::CONFIGUREPROJECT_PARSE_XML,__FILE__,__LINE__));
                        return false;
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
                        core::Error::write(core::Error(msgstr,ErrorCodes::CONFIGUREPROJECT_PARSE_XML,__FILE__,__LINE__));
                        return false;
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
                        core::Error::write(core::Error(msgstr,ErrorCodes::CONFIGUREPROJECT_PARSE_XML,__FILE__,__LINE__));
                        return false;
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
                        core::Error::write(core::Error(msgstr,ErrorCodes::CONFIGUREPROJECT_PARSE_XML,__FILE__,__LINE__));
                        return false;
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
                        core::Error::write(core::Error(msgstr,ErrorCodes::CONFIGUREPROJECT_PARSE_XML,__FILE__,__LINE__));
                        return false;
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
                        core::Error::write(core::Error(msgstr,ErrorCodes::CONFIGUREPROJECT_PARSE_XML,__FILE__,__LINE__));
                        return false;
                }
                
        }
        
                /******************************************************************************************
                * Version 1.1.0
                * 
                * *//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                if(projectVersion >= ver110)//la lectura es compatible con versiones anteriores del projecto
                {
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
                std::string inL = (const char*)xmlTextReaderConstValue(reader);
                if(inL.compare("MySQL") == 0 and enabledMySQL)
                {
                    setInputLenguaje(InputLenguajes::MySQL);
                    conectordb = createDatConnection();
                    conectordb->set(InputLenguajes::MySQL,host,port,database,user,password);
                }
                else if(inL.compare("PostgreSQL") == 0 and enabledPostgreSQL)
                {
                    setInputLenguaje(InputLenguajes::PostgreSQL);
                    conectordb = createDatConnection();
                    conectordb->set(InputLenguajes::PostgreSQL,host,port,database,user,password);
                }
                else if(inL.compare("MariaDB") == 0 and enabledMariaDB)
                {
                    setInputLenguaje(InputLenguajes::MariaDB);
                    conectordb = createDatConnection();
                    conectordb->set(InputLenguajes::MariaDB,host,port,database,user,password);
                }
                else
                {
                    std::string msgstr = "Fallo durante el parseo XML. El driver solicitado no existe o no hay soporte activo para dicho driver.";
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
                std::string outL = (const char*)xmlTextReaderConstValue(reader);        
                if(outL.compare("C++") == 0)
                {
                        outputLenguaje = OutputLenguajes::CPP;
                        //std::cout << "LANG = C++" <<  std::endl;
                }     
                else if(outL.compare("C") == 0)
                {
                        outputLenguaje = OutputLenguajes::C;
                        //std::cout << "LANG = C++" <<  std::endl;
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
        
        return true;
    }


        bool ConfigureProject::processNode(xmlTextReaderPtr reader) 
        {
                //const xmlChar *name = xmlTextReaderConstName(reader);

                if(xmlTextReaderNodeType(reader) == 1) //es d apertura?
                {        	
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
        
                return true;
        }
    
}
}
