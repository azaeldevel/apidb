
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
        bool ConfigureProject::readConfig(std::string filename)
        {
                if(toolkit::Error::check())
                {
                        throw toolkit::Error("Hay un error pendiente de atender",toolkit::Error::Codes::ERROR_NOTADDRESSED);
                }
                
                //std::cout << "Step 1. file " << filename << std::endl;
                FILE *apidbFilecheck = fopen(filename.c_str(), "r");
                if (apidbFilecheck == NULL )
                {
                        std::string msg = "La direecion especificada '";
                        msg += filename + "' no indica un archivo valido.";
                        toolkit::Error::write(toolkit::Error(msg,ErrorCodes::READFILE_INVALIDPATH));
                        return false;
                }
                fclose(apidbFilecheck);
                //std::cout << "Step 2." << std::endl;
                char tmp_filepath[] =  "/tmp/dxmg-XXXXXX";
                char * tmp_apidbDir  = mkdtemp(tmp_filepath);       
                //Descomomprimiendo archivo
                
                //std::cout << "Descomprimiendo achivo." << std::endl;
                TAR* tar_handle = NULL;
                tar_open(&tar_handle, (char*) filename.c_str(), NULL,  O_RDONLY,  0644,  TAR_GNU);
                if (tmp_apidbDir == NULL) 
                {
                        //fprintf(stderr, "Failed to build temp file.\n");
                        toolkit::Error::write(toolkit::Error("No se puede crear el directorio tempora para desempauqetar el archivo de proyecto.",ErrorCodes::READFILE_TEMPUNPACKFAIL));
                        return false;
                }
                //std::cout << "tar_handle is " << tmp_apidbDir << std::endl;
                //std::cout << "tmp_filepath "<< tmp_filepath  << std::endl;
                if(tar_extract_all(tar_handle, tmp_apidbDir) != 0)
                {
                        toolkit::Error::write( toolkit::Error("Fallo duraten las descompresion del archivo.",ErrorCodes::Read_UncomConfigFile));
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
                        toolkit::Error::write(toolkit::Error(msg,ErrorCodes::READFILE_INVALIDPATHVER,__FILE__,__LINE__));
                        return false;
                }
                fclose(apidbFilecheck2);
                /*if(projectVersion.fromFile(tmVerFileName)) 
                {
                        std::cout << "Version: " << projectVersion.toString() <<std::endl;
                }
                else 
                {              
                        std::string msg = "Fallo el parseo de la cadena de version en la llamada a Version::fromFile.";
                        toolkit::Error::write(toolkit::Error(msg,ErrorCodes::READFILE_FAILPARSERVER,__FILE__,__LINE__));
                        return false;
                }*/
                
                //leer xml
                //std::cout << "Leyendo XML." << std::endl;  
                xmlTextReaderPtr reader;
                int ret;                
                std::string xmlfile = tmp_filepath;
                xmlfile += "/apidb/main.xml";
                reader = xmlReaderForFile(xmlfile.c_str(), NULL, 0);
                if (reader != NULL) 
                {
                        ret = xmlTextReaderRead(reader);               
                        if (!processNode(reader)) 
                        {
                                //fprintf(stderr, "%s : failed to parse\n", xmlfile.c_str());
                                 toolkit::Error::write( toolkit::Error("Fallo duraten el parseo de nodo.",ErrorCodes::Read_FileFailParseNode));
                                return false;
                        }
                        xmlFreeTextReader(reader);
                }
                else 
                {
                        //fprintf(stderr, "Unable to open %s\n", xmlfile.c_str());
                        std::string msg = "Fallo al abrir el archivo '";
                        msg += msg + xmlfile + "'";
                         toolkit::Error::write( toolkit::Error(msg,ErrorCodes::READFILE_OPENXMLFILE));
                        return false;
                }
                                
                
                return true;
        }
        /*ConfigureProject::ConfigureProject(std::string filename)
        {    
        }*/
    

	bool ConfigureProject::checkXML(xmlTextReaderPtr reader)
	{                
                return true;
	}
	
        bool ConfigureProject::getProjectNodes(xmlTextReaderPtr reader)
        {         
        xmlTextReaderRead(reader);
        xmlTextReaderRead(reader);
        xmlTextReaderRead(reader);
        const xmlChar *name = xmlTextReaderConstName(reader);
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
                toolkit::Error::write(toolkit::Error(msgstr,ErrorCodes::CONFIGUREPROJECT_PARSE_XML,__FILE__,__LINE__));
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
                toolkit::Error::write(toolkit::Error(msgstr,ErrorCodes::CONFIGUREPROJECT_PARSE_XML,__FILE__,__LINE__));
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
                toolkit::Error::write(toolkit::Error(msgstr,ErrorCodes::CONFIGUREPROJECT_PARSE_XML,__FILE__,__LINE__));
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
                toolkit::Error::write(toolkit::Error(msgstr,ErrorCodes::CONFIGUREPROJECT_PARSE_XML,__FILE__,__LINE__));
                return false;
        }
        
        xmlTextReaderRead(reader);
        xmlTextReaderRead(reader);
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
                toolkit::Error::write(toolkit::Error(msgstr,ErrorCodes::CONFIGUREPROJECT_PARSE_XML,__FILE__,__LINE__));
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
                toolkit::Error::write(toolkit::Error(msgstr,ErrorCodes::CONFIGUREPROJECT_PARSE_XML,__FILE__,__LINE__));
                return false;
        }
        
        this->version.setNumbers(major,minor,patch);
       // this->version.setStage(toolkit::Version::Stage::alpha);
        
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
                toolkit::Error::write(toolkit::Error(msgstr,ErrorCodes::CONFIGUREPROJECT_PARSE_XML,__FILE__,__LINE__));
                return false;
        }
        name = xmlTextReaderConstName(reader);
        std::string host = "";
        if(strcmp((const char*)name,"#text") == 0)
        {
            host = (const char*)xmlTextReaderConstValue(reader);
        }
        else
        {
                std::string msgstr = "Fallo durante el parseo XML.";
                toolkit::Error::write(toolkit::Error(msgstr,ErrorCodes::CONFIGUREPROJECT_PARSE_XML,__FILE__,__LINE__));
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
                toolkit::Error::write(toolkit::Error(msgstr,ErrorCodes::CONFIGUREPROJECT_PARSE_XML,__FILE__,__LINE__));
                return false;
        }
        name = xmlTextReaderConstName(reader);
        int port = 0;
        if(strcmp((const char*)name,"#text") == 0)
        {
            port = atoi((const char*)xmlTextReaderConstValue(reader));
        }
        else
        {
                std::string msgstr = "Fallo durante el parseo XML.";
                toolkit::Error::write(toolkit::Error(msgstr,ErrorCodes::CONFIGUREPROJECT_PARSE_XML,__FILE__,__LINE__));
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
                toolkit::Error::write(toolkit::Error(msgstr,ErrorCodes::CONFIGUREPROJECT_PARSE_XML,__FILE__,__LINE__));
                return false;
        }
        name = xmlTextReaderConstName(reader);
        std::string database = "";
        if(strcmp((const char*)name,"#text") == 0)
        {
            database = (const char*)xmlTextReaderConstValue(reader);
        }
        else
        {
                std::string msgstr = "Fallo durante el parseo XML.";
                toolkit::Error::write(toolkit::Error(msgstr,ErrorCodes::CONFIGUREPROJECT_PARSE_XML,__FILE__,__LINE__));
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
                toolkit::Error::write(toolkit::Error(msgstr,ErrorCodes::CONFIGUREPROJECT_PARSE_XML,__FILE__,__LINE__));
                return false;
        }
        name = xmlTextReaderConstName(reader);
        std::string user = "";
        if(strcmp((const char*)name,"#text") == 0)
        {
            user = (const char*)xmlTextReaderConstValue(reader);
        }
        else
        {
                std::string msgstr = "Fallo durante el parseo XML.";
                toolkit::Error::write(toolkit::Error(msgstr,ErrorCodes::CONFIGUREPROJECT_PARSE_XML,__FILE__,__LINE__));
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
                toolkit::Error::write(toolkit::Error(msgstr,ErrorCodes::CONFIGUREPROJECT_PARSE_XML,__FILE__,__LINE__));
                return false;
        }
        name = xmlTextReaderConstName(reader);
        std::string password = "";
        if(strcmp((const char*)name,"#text") == 0)
        {
            password = (const char*)xmlTextReaderConstValue(reader);
        }
        else
        {
                std::string msgstr = "Fallo durante el parseo XML.";
                toolkit::Error::write(toolkit::Error(msgstr,ErrorCodes::CONFIGUREPROJECT_PARSE_XML,__FILE__,__LINE__));
                return false;
        }
        
        conectordb = new octetos::toolkit::clientdb::mysql::Datconnect(host,port,database,user,password);
        
        //if(projectVersion >= verlast)//la lectura es compatible con versiones anteriores del projecto
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
        if(inL.compare("MySQL") == 0)
        {
                inputLenguaje = InputLenguajes::MySQL;
        }
        else
        {
                std::string msgstr = "Fallo durante el parseo XML.";
                toolkit::Error::write(toolkit::Error(msgstr,ErrorCodes::CONFIGUREPROJECT_PARSE_XML,__FILE__,__LINE__));
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
        else
        {
                std::string msgstr = "Fallo durante el parseo XML.";
                toolkit::Error::write(toolkit::Error(msgstr,ErrorCodes::CONFIGUREPROJECT_PARSE_XML,__FILE__,__LINE__));
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
                toolkit::Error::write(toolkit::Error(msgstr,ErrorCodes::CONFIGUREPROJECT_PARSE_XML,__FILE__,__LINE__));
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
        else
        {
                std::string msgstr = "Fallo durante el parseo XML.";
                toolkit::Error::write(toolkit::Error(msgstr,ErrorCodes::CONFIGUREPROJECT_PARSE_XML,__FILE__,__LINE__));
                return false;
        }
        
        //
        xmlTextReaderRead(reader);
        xmlTextReaderRead(reader);
        xmlTextReaderRead(reader);
        xmlTextReaderRead(reader);
        xmlTextReaderRead(reader);
        
        for(int i = 0; i < 2; i++)
        {
                //std::cout << "Node  : " <<(const char*)xmlTextReaderConstName(reader)<<std::endl;
                //std::cout << ", count : " << (const char*)xmlGetProp(xmlTextReaderCurrentNode(reader), (const xmlChar *)"countTbs") << std::endl;
                std::string node = (const char*)xmlTextReaderConstName(reader);
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
                                Parameters* pparams = new Parameters();
                                for(int i = 0 ; i < counParams; i++)
                                {
                                        //std::cout << "\t\t\tParameter : " << (const char*)xmlTextReaderConstValue(reader) << std::endl;
                                        const char* strParam = (const char*)xmlTextReaderConstValue(reader);
                                        int strl = strlen(strParam);
                                        char* strnew = new char[strl + 1];
                                        strcpy(strnew,strParam);
                                        pparams->push_back(strnew);
                                        xmlTextReaderRead(reader);                
                                        xmlTextReaderRead(reader);              
                                        xmlTextReaderRead(reader);              
                                        xmlTextReaderRead(reader);
                                }
                                pfn->setHeader(pparams);
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
                                if(toolkit::Error::check())
                                {
                                        return false;
                                }
                                else
                                {
                                        std::string msgstr = "Fallo durante el parseo XML.";
                                        toolkit::Error::write(toolkit::Error(msgstr,ErrorCodes::CONFIGUREPROJECT_PARSE_XML,__FILE__,__LINE__));
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
