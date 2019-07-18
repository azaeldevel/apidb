
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

//local
#include "common.hpp"
#include "apidb.hpp"


namespace octetos
{
namespace apidb
{
        bool ConfigureProject::readConfig(std::string filename)
        {
                //variables no almacenas en el archivo de configuracion.
                //inputLenguaje = apidb::InputLenguajes::MySQL;
                //outputLenguaje = apidb::OutputLenguajes::CPP;
                //packing = PackingLenguajes::CMake;
                
                //leeer xml
                xmlTextReaderPtr reader;
                int ret;                
                //std::cout << "Descomprimiendo achivo." << std::endl;
                TAR* tar_handle;
                tar_open(&tar_handle, (char*) filename.c_str(), NULL,  O_RDONLY,  0644,  TAR_GNU);
                char tmp_filepath[] =  "/tmp/dxmg-XXXXXX";
                char * tmp_apidbDir  = mkdtemp(tmp_filepath);
                if (tmp_apidbDir == NULL) 
                {
                        fprintf(stderr, "Failed to build temp file.\n");
                        return false;
                        //exitcode = 2;
                }
                tar_extract_all(tar_handle, tmp_filepath);
                tar_close(tar_handle);
                std::cout << "Lellendo archivo." << std::endl;  
                std::string xmlfile = tmp_apidbDir;
                xmlfile += "/apidb/main.xml";
                reader = xmlReaderForFile(xmlfile.c_str(), NULL, 0);
                if (reader != NULL) 
                {
                        ret = xmlTextReaderRead(reader);               
                        if (!processNode(reader)) 
                        {
                                fprintf(stderr, "%s : failed to parse\n", xmlfile.c_str());
                                return false;
                        }
                        xmlFreeTextReader(reader);
                }
                else 
                {
                        fprintf(stderr, "Unable to open %s\n", xmlfile.c_str());
                        return false;
                }
                
                return true;
        }
        /*ConfigureProject::ConfigureProject(std::string filename)
        {    
        }*/
    
	bool ConfigureProject::saveConfig()
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
                switch(conectordb->getServerType())
                {
                        case octetos::toolkit::clientdb::Datconnect::ServerType::MySQL:
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
                for(Table table : downloads)
                {
                        counFuns = 0;
                        countTbs++;
                        xmlNodePtr downls_tb_node = xmlNewChild(downls_node, NULL, (const xmlChar *)"Table", NULL);
                        xmlNewProp(downls_tb_node, BAD_CAST "name", BAD_CAST table.getName().c_str());
                        int countparams;
                        for(std::map<const char*, const Function*>::iterator  itfn = table.begin() ;  itfn !=table.end() ; itfn++)
                        {
                                countparams = 0;
                                counFuns++;
                                xmlNodePtr downls_fn_node = xmlNewChild(downls_tb_node, NULL, (const xmlChar *)"Function", NULL); 
                                xmlNewProp(downls_fn_node, BAD_CAST "name", BAD_CAST itfn->second->getName().c_str());
                                const std::vector<const char*>& params = (const std::vector<const char*>&)(itfn->second->getParameters());
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
                for(Table table : selects)
                {
                        counFuns = 0;
                        countTbs++;
                        xmlNodePtr selects_tb_node = xmlNewChild(selects_node, NULL, (const xmlChar *)"Table", NULL);
                        xmlNewProp(selects_tb_node, BAD_CAST "name", BAD_CAST table.getName().c_str());
                        int countparams;
                        for(std::map<const char*, const Function*>::iterator  itfn = table.begin() ;  itfn !=table.end() ; itfn++)
                        {
                                countparams = 0;
                                counFuns++;
                                xmlNodePtr selects_fn_node = xmlNewChild(selects_tb_node, NULL, (const xmlChar *)"Function", NULL); 
                                xmlNewProp(selects_fn_node, BAD_CAST "name", BAD_CAST itfn->second->getName().c_str());
                                const std::vector<const char*>& params = (const std::vector<const char*>&)(itfn->second->getParameters());
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
                tar_append_tree(pTar, tmp_dirpath, "apidb");
                tar_append_eof(pTar);
                tar_close(pTar);
                //std::cout<< "Archivo comprimido" << std::endl;
                
                if(rename(tarFilename.c_str(),dirProy.c_str()) != 0)
                {
                        std::cout << "Fallo al mover el archivo de proyecto." << std::endl;
                        return false;
                }
                
                std::cout<< "Escritura de proyecto completada." << std::endl;
		return true;
	}

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
            return true;
        }
        name = xmlTextReaderConstName(reader);
        short major = 0;
        if(strcmp((const char*)name,"#text") == 0)
        {  
            major = atoi((const char*)xmlTextReaderConstValue(reader));
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
            return true;
        }  
        name = xmlTextReaderConstName(reader);
        short minor = 0;
        if(strcmp((const char*)name,"#text") == 0)
        {
            minor = atoi((const char*)xmlTextReaderConstValue(reader));
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
            return true;
        }
        name = xmlTextReaderConstName(reader);
        short patch = 0;
        if(strcmp((const char*)name,"#text") == 0)
        {
            patch = atoi((const char*)xmlTextReaderConstValue(reader));
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
            return true;
        }
        name = xmlTextReaderConstName(reader);
        std::string host = "";
        if(strcmp((const char*)name,"#text") == 0)
        {
            host = (const char*)xmlTextReaderConstValue(reader);
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
            return true;
        }
        name = xmlTextReaderConstName(reader);
        int port = 0;
        if(strcmp((const char*)name,"#text") == 0)
        {
            port = atoi((const char*)xmlTextReaderConstValue(reader));
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
            return true;
        }
        name = xmlTextReaderConstName(reader);
        std::string database = "";
        if(strcmp((const char*)name,"#text") == 0)
        {
            database = (const char*)xmlTextReaderConstValue(reader);
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
            return true;
        }
        name = xmlTextReaderConstName(reader);
        std::string user = "";
        if(strcmp((const char*)name,"#text") == 0)
        {
            user = (const char*)xmlTextReaderConstValue(reader);
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
            return true;
        }
        name = xmlTextReaderConstName(reader);
        std::string password = "";
        if(strcmp((const char*)name,"#text") == 0)
        {
            password = (const char*)xmlTextReaderConstValue(reader);
        }
        
        conectordb = new octetos::toolkit::clientdb::mysql::Datconnect(host,port,database,user,password);
        
        
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
                std::string msg = "El Lenguaje de entrada '";
                msg += inL;
                msg += "' no tiene soporte.";
                throw msg;
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
        }
        else
        {
                std::string msg = "El Lenguaje de salida '";
                msg += outL;
                msg += "' no tiene soporte.";
                throw msg;
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
                std::string msg = "El Lenguaje de empaquetado '";
                msg += pk;
                msg += "' no tiene soporte.";
                throw msg;
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
                std::string msg = "El Lenguaje de empaquetado '";
                msg += pk;
                msg += "' no tiene soporte.";
                throw msg;
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
                int counTbs = atoi((const char*)xmlGetProp(xmlTextReaderCurrentNode(reader), (const xmlChar *)"countTbs"));
                for(int i = 0; i < counTbs; i++)
                {
                        xmlTextReaderRead(reader);
                        xmlTextReaderRead(reader);        
                        Table* ptb = new Table((const char*)xmlGetProp(xmlTextReaderCurrentNode(reader), (const xmlChar *)"name"));
                        //std::cout << "\tTable  : " <<(const char*)xmlGetProp(xmlTextReaderCurrentNode(reader), (const xmlChar *)"name") << std::endl;
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
                        if(i == 0)
                        {
                                downloads.push_back(*ptb);
                        }
                        else if(i == 1)
                        {
                                selects.push_back(*ptb);
                        }
                }
                xmlTextReaderRead(reader);
                xmlTextReaderRead(reader);
                xmlTextReaderRead(reader);
                xmlTextReaderRead(reader);
        }
        
        return true;
    }


    bool ConfigureProject::processNode(xmlTextReaderPtr reader) 
    {
        const xmlChar *name = xmlTextReaderConstName(reader);

        if(xmlTextReaderNodeType(reader) == 1) //es d apertura?
        {        	
            if(checkXML(reader))
            {
                //std::cout<<(const char*)name<<std::endl;
                getProjectNodes(reader);
            }
            else
            {
            	std::cerr << "El XML no cumple con el formato requerido(el roden de los nodos es importante)" << std::endl;
            	return false;
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
