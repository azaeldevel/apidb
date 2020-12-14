
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
//#include <libtar.h>
//Tar<<<<<<<<<<

#include "apidb.hpp"
#include "ConfigureProject.hpp"
#include "Errors.hpp"

namespace octetos
{
namespace apidb
{
	bool ConfigureProject::checkFailLoadDat()const
	{
		return failLoadDat;
	}
    void ConfigureProject::deleteConnector(octetos::db::Connector* c)const
    {
        destroyConnector(c);
    }
    octetos::db::Connector* ConfigureProject::newConnector()const
    {
        return createConnector();
    }
    octetos::db::Datconnect* ConfigureProject::newDatConnection()
    {
        if(conectordb)
        {
            destroyDatConnection(conectordb);
            conectordb = NULL;
        }
        conectordb = createDatConnection();
		return conectordb;
    }
    void* ConfigureProject::getfnDatConection()
    {
        return (void*) createDatConnection;
    }
    void* ConfigureProject::getfnCreateConector()
    {
        return (void*) createConnector;
    }
    void* ConfigureProject::getLibraryHandle()const
    {
        return handle;
    }
    octetos::db::Datconnect*  ConfigureProject::getDatconnection() const
    {
        return conectordb;
    }
    void ConfigureProject::setInputs(InputLenguajes in,octetos::db::Datconnect& dat)
    {
        conectordb = & dat;
        inputLenguaje = in;
        loadLibrary();
    }
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
        }
        */
    ConfigureProject::~ConfigureProject()
    {
        if(handle != NULL) dlclose(handle);
    }

	bool ConfigureProject::testConexion()
    	{
		bool ret = false;
		if(inputLenguaje == apidb::InputLenguajes::MySQL)
		{
		    //std::cout << "2 - createConnection:" << createConnection << "\n";
		    octetos::db::Connector*  connector = createConnector();
		    ret = connector->connect(*conectordb);
		    connector->close();
		}
		else if(inputLenguaje == apidb::InputLenguajes::PostgreSQL)
		{
		    octetos::db::Connector*  connector = createConnector();
		    ret = connector->connect(*conectordb);
		    connector->close();
		}
		else if(inputLenguaje == apidb::InputLenguajes::MariaDB)
		{
		    octetos::db::Connector*  connector = createConnector();
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
        std::string libname = "";
        if(inputLenguaje == apidb::InputLenguajes::MySQL)
        {
            libname = "libapidb-MySQL.so";
        }
        else if(inputLenguaje == apidb::InputLenguajes::MariaDB)
        {
            libname ="libapidb-MariaDB.so";
        }
        else if(inputLenguaje == apidb::InputLenguajes::PostgreSQL)
        {
            libname = "libapidb-PostgreSQL.so";
        }
        else
        {
            std::string msgErr ="No se reconoce el driver solicitado :\n" ;
            msgErr = msgErr + "\t" + dlerror();
            core::Error err(msgErr,core::Error::ERROR_UNKNOW,__FILE__,__LINE__);
            return false;
        }

        handle = dlopen(libname.c_str(), RTLD_LAZY);
        //std::cout << "Step 1\n";
        if(!handle)
        {
            std::string msgErr ="dlopen fallo con '" ;
            msgErr += libname + "' : ";
            msgErr += + dlerror();
            core::Error err(msgErr,core::Error::ERROR_UNKNOW,__FILE__,__LINE__);
            core::Error::write(err);
            return false;
        }
        //std::cout << "Step 2\n";


        /////>>>>>>>>>>>>>>>>>>>>>>>>>>>
        createConnector = (octetos::db::Connector* (*)())dlsym(handle, "createConnector");
        //std::cout << "Step 3\n";
        if(!createConnector)
        {
            std::string msgErr ="dlsym fallo con parse_string:\n" ;
            msgErr = msgErr + "\t" + dlerror();
            core::Error err(msgErr,core::Error::ERROR_UNKNOW,__FILE__,__LINE__);
            core::Error::write(err);
            return false;
        }
        destroyConnector = (void (*)(octetos::db::Connector*))dlsym(handle, "destroyConector");
        if(!destroyConnector)
        {
            std::string msgErr ="dlsym fallo con destroyConnector:\n" ;
            msgErr = msgErr + "\t" + dlerror();
            core::Error err(msgErr,core::Error::ERROR_UNKNOW,__FILE__,__LINE__);
            core::Error::write(err);
            return false;
        }

        //>>>>>>>>>>>>>>>>>
        createDatConnection = (octetos::db::Datconnect* (*)())dlsym(handle, "createDatconnect");
        if(!createDatConnection)
        {
            std::string msgErr ="dlsym fallo con createDatconnect:\n" ;
                msgErr = msgErr + "\t" + dlerror();
            core::Error err(msgErr,core::Error::ERROR_UNKNOW,__FILE__,__LINE__);
            core::Error::write(err);
            return false;
        }
        destroyDatConnection = (void (*)(octetos::db::Datconnect*))dlsym(handle, "destroyDatconnect");
        if(!createDatConnection)
        {
            std::string msgErr ="dlsym fallo con createDatconnect:\n" ;
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
        createConnector = NULL;
        createDatConnection = NULL;
        conectordb = NULL;
	#ifdef APIDB_POSTGRESQL
		enabledPostgreSQl=true;
	#else
		enabledPostgreSQl=false;
	#endif
	#ifdef APIDB_MARIADB
		enabledMariaDB=true;
	#else
		enabledMariaDB=false;
	#endif
	#ifdef APIDB_MYSQL
		enabledMySQL=true;
	#else
		enabledMySQL=false;
	#endif
		failLoadDat=false;
    }

}
}
