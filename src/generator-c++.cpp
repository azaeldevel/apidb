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

#include <list>
#include <vector>
#include <iostream>
#include <fstream>
#include <map>
#include <locale>

#include "analyzer.hpp"
#include "generator.hpp"
#include "Errors.hpp"

namespace octetos
{
namespace apidb
{
namespace generators
{
    void CPP::writeForwardClass(std::ofstream& file,bool log,const symbols::ISpace* ispace)
    {
        if(ispace->what() == symbols::SpaceType::TABLE)
        {
            symbols::Table* table = (symbols::Table*) ispace;
            //createClassH(*table,file,table->getName(),log);
            file << "\tclass " << table->name << ";\n";
        }
        else if(ispace->what() == symbols::SpaceType::SPACE)
        {
            symbols::Space* space = (symbols::Space*) ispace;
            file << "namespace " ;
            if(not configureProject.space.empty())
            {
                file << configureProject.space << "\n";
            }
            else if(space->getName().empty())
            {
                file << configureProject.name;
            }
            else
            {
                file << space->getName() << std::endl;
            }
            file << "{\n";
            //std::cout << "Espacio '" << space->getName() << "'" << std::endl;
            for(symbols::Space::iterator it = space->begin(); it != space->end(); it++)
            {
                //createH(file,log,it->second);
                writeForwardClass(file,log,it->second);
            }
            file << "\n}\n";
        }
    }
    void CPP::writeRemovesCPP(const apidb::symbols::Table& table, std::ofstream& ofile)
    {
        Remove remove(configureProject,table,ofile);
        remove.setImplementation(true);
        remove.generate();        
    }
    void CPP::writeRemovesH(const apidb::symbols::Table& table, std::ofstream& ofile)
    {
        Remove remove(configureProject,table,ofile);
        remove.setDefinition(true);
        remove.generate();        
    }
    void CPP::writeUppdatersCPP(const apidb::symbols::Table& table, std::ofstream& ofile)
    {
        Update update(configureProject,table,ofile);
        update.setImplementation(true);
        update.generate();
    }
    void CPP::writeGettersCPP(const apidb::symbols::Table& table, std::ofstream& ofile)
    {
        Getter getter(configureProject,table,ofile);
        getter.setImplementation(true);
        getter.generate();
    }
    void CPP::writeUppdatersH(const apidb::symbols::Table& table, std::ofstream& ofile)
    {
        Update update(configureProject,table,ofile);
        update.setDefinition(true);
        update.generate();
    }
	void CPP::writeGettersH(const apidb::symbols::Table& table, std::ofstream& ofile)
    {
        Getter getter(configureProject,table,ofile);
        getter.setDefinition(true);
        getter.generate();
    }
	CPP::~CPP()
	{
		delete[] writeResults;
	}
	const std::string& CPP::getHeaderName() const
	{
		return projectH;
	}
	std::ofstream& CPP::getSourceOutput()
	{
		return writeResults[1];
	}
	std::ofstream& CPP::getHeaderOutput()
	{
		return writeResults[0];
	}
	CPP::CPP(apidb::Analyzer& d,const ConfigureProject& config) : apidb::generators::Generator(config,d)
	{
        if(config.outputLenguaje != OutputLenguajes::CPP)
        {
            std::string msg = "La configuracion indica '" ;
            msg = msg + getOutputLenguajeString()+ "', pero el componente es C++.";
            throw BuildException(msg);
        }
                
		//outputLenguaje = d.getOutputLenguaje();
		writeResults = new std::ofstream[2];
		if((configureProject.builDirectory.empty()) | (configureProject.builDirectory.compare(".") == 0)) 
		{
			projectH = configureProject.name + ".hpp";
			writeResults[0].open(projectH);
			projectCPP = configureProject.name + ".cpp";
			writeResults[1].open(projectCPP);
		}
		else
		{
			projectH = configureProject.name + ".hpp";
			projectCPP = configureProject.name + ".cpp";
			writeResults[0].open(configureProject.builDirectory + "/" + projectH);
			writeResults[1].open(configureProject.builDirectory + "/" + projectCPP);
		}
	}
	bool CPP::generate(bool log)
	{
		if(log)analyzer.getOutput().add("Generando archivos de codigo fuente... \n");
		std::string msg1 = "\tLenguaje resultado: " ;
		msg1 += getOutputLenguajeString() ;
		if(log)analyzer.getOutput().add(msg1);;
				
		//writing code				
		if(createH(getHeaderOutput(),log,getSymbolsTable()) == false)  
		{
			return false;
		}
		if(createCPP(getSourceOutput(),log,getSymbolsTable()) == false)
		{
			return false;
		}
          
        return true;    
    }
    void CPP::writeSelectsCPP(const apidb::symbols::Table& table, std::ofstream& ofile)
    {
        Select select(configureProject,table,ofile);
        select.setImplementation(true);
        select.generate();
    }
	void CPP::writeDownloadsCPP(const apidb::symbols::Table& table, std::ofstream& ofile)
    {
        Download download(configureProject,table,ofile);
        download.setImplementation(true);
        download.generate();
    }
    void CPP::writeInsertCPP(const apidb::symbols::Table& table,std::ofstream& ofile)	
	{        
        /*Insert insert(configureProject,table,ofile,Insert::Mode::CreateParent);
        insert.setImplementation(true);
        insert.generate();*/
        Insert insert2(configureProject,table,ofile,Insert::Mode::ReferencedParent);
        insert2.setImplementation(true);
        insert2.generate();
	}
	void CPP::writeDefaultContructorCPP(const apidb::symbols::Table& table,std::ofstream& ofile)
    {
		ofile <<"\t"<<table.getName()<< "::" <<table.getName()<<"()"<<std::endl;
		ofile <<"\t{\n";
        for(symbols::Symbol* k : table.getKey())
        {
            if(k->symbolReferenced != NULL)
            {
                ofile << "\t\t" << k->name << " = NULL;\n";
            }            
        }
		ofile <<"\t}\n";
	}		
	void CPP::writeDestructorCPP(const apidb::symbols::Table& table,std::ofstream& ofile)
    {
		ofile <<"\t"<<table.getName()<< "::~" << table.getName() << "()\n";
		ofile <<"\t{\n";
        for(symbols::Symbol* k : table.getKey())
        {
            if(k->symbolReferenced != NULL)
            {
                ofile << "\t\tif(" << k->name << " != NULL)\n"; 
                ofile << "\t\t{\n";
                ofile << "\t\t\tdelete " << k->name << ";\n";
                //ofile << "\t\t\t" << k->name << " = NULL;\n";
                ofile << "\t\t}\n";
            }
        }
		ofile <<"\t}\n";
	}
	void CPP::writeDestructorH(const apidb::symbols::Table& table,std::ofstream& ofile)
    {
		ofile <<"\t\t~" << table.getName()<<"();\n"<<std::endl;
	}
	void CPP::writeCopyContructorCPP(const apidb::symbols::Table& table,std::ofstream& ofile)
	{
		//constructor de copias 
		ofile << "\t" << table.getName() << "::" << table.getName() <<"(const " << table.getName() <<"& obj)"<<std::endl;
		ofile << "\t{"<<std::endl;                
        //for (auto const& [key, attr] : table)
        for(std::map<const char*,symbols::Symbol*,symbols::cmp_str>::const_iterator it = table.begin(); it != table.end(); it++)
		{
            if(it->second->symbolReferenced)
            {
                ofile << "\t\tthis->"<< it->second->getName()  << " = new " << it->second->classReferenced->name << "(*obj." << it->second->getName() <<");"<<std::endl;
            }
            else
            {
                ofile << "\t\tthis->"<< it->second->getName()  << " = obj." << it->second->getName() <<";"<<std::endl;
            }
		}
		ofile << "\t}"<<std::endl;
	}
	void CPP::writeKeyRawDataContructorCPP(const apidb::symbols::Table& table,std::ofstream& ofile)
	{
		//constructor que toma key como parametro
        if(table.getKey().size() > 0)//tiene key
        {
            ofile << "\t" <<table.getName() << "::" << table.getName() << "(";
            auto endIt = table.getKey().end();
            endIt--;
            for(auto k : table.getKey())
            {
                if(k->getOutType().compare("std::string") == 0)
                {
                    ofile << "const " << k->getOutType() << "& " << k->getName() ;
                }
                else
                {
                    ofile << k->getOutType() << " " << k->getName() ;                    
                }
                if(k != *endIt)
                {
                    ofile << ",";
                }
            }
            ofile << ")"<<std::endl;
		}
		else
		{
			//no tiene contructor con llaves
		}
		
        if(table.getKey().size() > 0)//tiene key
        {
            ofile << "\t{" <<std::endl;
            for(auto k : table.getKey())
            {
                if(k->getClassReferenced() != NULL)
                {
                    ofile << "\t\tthis->" << k->getName()  << " = new " << k->getClassReferenced()->getName() << "(" << k->getName()  << ");" << std::endl;
                }
                else
                {
                    ofile << "\t\tthis->" << k->getName()  << " = " << k->getName()  << ";" << std::endl;
                }
            }
            ofile << "\t}" <<std::endl;
        }
        else 
        {
            
        }
	}
	void CPP::createClassMethodesCPP(const apidb::symbols::Table& table,std::ofstream& ofile)
	{
		writeDefaultContructorCPP(table,ofile);        
		writeKeyRawDataContructorCPP(table,ofile);
		writeCopyContructorCPP(table,ofile);   
        writeDestructorCPP(table,ofile);
        ofile << "\n\n";
        writeGettersCPP(table,ofile);
        ofile << "\n\n";
        writeUppdatersCPP(table,ofile);
		ofile << "\n\n";
		writeInsertCPP(table,ofile);
        ofile << "\n\n";        
		writeSelectsCPP(table,ofile);
        ofile << "\n\n";        
        writeDownloadsCPP(table,ofile);
        ofile << "\n\n";        
        writeRemovesCPP(table,ofile);
        ofile << "\n\n";
    } 
    void CPP::createClassCPP(const apidb::symbols::Table& cl,std::ofstream& file,const std::string& nameClass)
    {
        if(configureProject.getInputLenguaje() == InputLenguajes::MySQL or configureProject.getInputLenguaje() == InputLenguajes::MariaDB)
        {
            file << "\tconst std::string " <<  nameClass << "::TABLE_NAME = \"`"<<  cl.getFullName() << "`\";" << std::endl;
        }
        else if(configureProject.getInputLenguaje() == InputLenguajes::PostgreSQL)
        {
            file << "\tconst std::string " <<  nameClass << "::TABLE_NAME = \""<<  cl.getFullName() << "\";" << std::endl;
        }
        else
        {
            std::string msg = "Lenguaje de entrado no soportado.";
#ifndef NDEBUG
			throw BuildException(msg,__FILE__,__LINE__);
#else
            throw BuildException(msg);
#endif
        }
		createClassMethodesCPP(cl,file);        
		file<< std::endl<< std::endl;
    }
    bool CPP::createCPP(std::ofstream& file,bool log,const symbols::ISpace* ispace)
	{
		if(ispace->what() == symbols::SpaceType::TABLE)
        {
            symbols::Table* table = (symbols::Table*) ispace;
            //std::cout << "Es table " << table->getName() << std::endl;
            createClassCPP(*table,file,table->getName());
        }
        else if(ispace->what() == symbols::SpaceType::SPACE)
        {
            symbols::Space* space = (symbols::Space*) ispace;
            //std::cout << "Es Espacio " << space->getFullName() << std::endl;
            file << "namespace " ;
            if(not configureProject.space.empty())
            {
                    file << configureProject.space << "\n";
            }
            else if(space->getName().empty())
            {
                file << configureProject.name << "\n";
            }
            else
            {
                file << space->getName() << std::endl;
            }
            file << "\n{\n";
            //std::cout << "Espacio '" << space->getFullName() << "'" << std::endl;
            for(symbols::Space::iterator it = space->begin(); it != space->end(); it++)
            {
                createCPP(file,log,it->second);
            }
            file << "\n}\n";
            file << std::endl;				
        }
		
		return true;
	}
    bool CPP::createCPP(std::ofstream& file,bool log,const symbols::SymbolsTable& stb)
	{
        if(configureProject.getInputLenguaje() == InputLenguajes::MySQL)
        {
            throw oct::core::Exception("Faltas los encabazados de MySQL",__FILE__,__LINE__);
        }
        else if(configureProject.getInputLenguaje() == InputLenguajes::MariaDB)
        {
            getSourceOutput()<< "#if defined LINUX_ARCH\n";
                getSourceOutput()<< "\t#include <mysql/mysql.h>\n";
            getSourceOutput()<< "#elif defined LINUX_GENTOO\n";
                getSourceOutput()<< "\t#include <mariadb/mysql.h>\n";
            getSourceOutput()<< "#elif defined LINUX_DEBIAN\n";
                getSourceOutput()<< "\t#include <mariadb/mysql.h>\n";
            getSourceOutput()<< "#elif defined WINDOWS_MINGW && defined BUILDING_DLL\n";
                getSourceOutput()<< "\t#include <mariadb/mysql.h>\n";
            getSourceOutput()<< "#else\n";
                getSourceOutput()<< "\t#error \"Plataforma desconocida.\"\n";
            getSourceOutput()<< "#endif\n";
        }
        else if(configureProject.getInputLenguaje() == InputLenguajes::PostgreSQL)
        {
            throw oct::core::Exception("Faltas los encabazados de PosrgreSQL",__FILE__,__LINE__);
        }
        else
        {
            std::string msg = "Lenguaje no soportado " ;
            throw BuildException(msg);
            return false;
        }

		getSourceOutput()<< "#include \"" << getHeaderName() <<"\""<<std::endl<<std::endl;
        
		for(symbols::SymbolsTable::const_iterator it = stb.begin(); it != stb.end(); it++)
		{
			symbols::ISpace* ispace = it->second;
			if(ispace->what() == symbols::SpaceType::TABLE)
            {
                symbols::Table* table = (symbols::Table*) ispace;
                createClassCPP(*table,file,table->getName());
            }
            else if(ispace->what() == symbols::SpaceType::SPACE)
            {
                symbols::Space* space = (symbols::Space*) ispace;
                file << "namespace " ;
                if(not configureProject.space.empty())
                {
                    file << configureProject.space << "\n";
                }
                else if(space->getName().empty())
                {
                    file << configureProject.name << "\n";
                }
                else
                {
                    file << space->getName() << std::endl;
                }
                file << "{\n";
                //std::cout << "Espacio '" << space->getFullName() << "'" << std::endl;
                file << "\n\n";
                for(symbols::Space::iterator it = space->begin(); it != space->end(); it++)
                {
                    createCPP(file,log,it->second);
                }
                file << "\n}\n";
                file << std::endl;
            }
		}	
		return true;
	}
    void CPP::writeDownloadsH(const apidb::symbols::Table& table, std::ofstream& ofile)
    {
        Download download(configureProject,table,ofile);
        download.setDefinition(true);
        download.generate();
    }
        
	void CPP::writeSelectsH(const apidb::symbols::Table& table, std::ofstream& ofile)
	{
        Select select(configureProject,table,ofile);
        select.setDefinition(true);
        select.generate();
	}
        /**
        * Genera las funciones insert solo con los datos marcados como requeridos en la DB.
        */
    void CPP::writeInsertH(const apidb::symbols::Table& table,std::ofstream& ofile)
	{
        /*Insert insert(configureProject,table,ofile,Insert::Mode::CreateParent);
        insert.setDefinition(true);
        insert.generate();*/
        Insert insertObj(configureProject,table,ofile,Insert::Mode::ReferencedParent);
        insertObj.setDefinition(true);
        insertObj.generate();
	}
	void CPP::writeDefaultContructorH(const apidb::symbols::Table& table,std::ofstream& ofile)
	{
        //contructor default
		ofile <<"\t\t"<<table.getName()<<"();"<<std::endl;
        
	}
	void CPP::writeCopyContructorH(const apidb::symbols::Table& table,std::ofstream& ofile)
	{
		//constructor de copias 
		ofile << "\t\t" << table.getName() <<"(const " << table.getName() <<"&);"<< std::endl;
	}
    
    void CPP::writeKeyRawDataContructorH(const apidb::symbols::Table& table,std::ofstream& ofile)
	{
        //constructor que toma key como parametro
        if(table.getKey().size() > 0)//tiene key
        {
            ofile << "\t\t" <<table.getName() << "(";
            auto endIt = table.getKey().end();
            endIt--;
            for(auto k : table.getKey())
            {
                if(k->getOutType().compare("std::string") == 0)
                {
                    ofile << "const " << k->getOutType() << "&";
                }
                else
                {
                    ofile << k->getOutType();                    
                }
                ofile << " " << k->name;
                if(k != *endIt)
                {
                    ofile << ",";
                }
            }
            ofile << ");"<<std::endl;
		}
		else
		{
			//no genera contructor con llaves
		}
	}
	
	void CPP::createClassMethodesH(const apidb::symbols::Table& table,std::ofstream& ofile)
	{
        //contructors
		writeDefaultContructorH(table,ofile);
		writeCopyContructorH(table,ofile);
		writeKeyRawDataContructorH(table,ofile);
        writeDestructorH(table,ofile);
        ofile << "\n";
        //getter
        writeGettersH(table,ofile);
        ofile << "\n";
        //updaters
        writeUppdatersH(table,ofile);
        ofile << "\n";
        //inserts
		writeInsertH(table,ofile);
        ofile << "\n";
        //selects
        writeSelectsH(table,ofile);
        ofile << "\n";
        //downloaders
        writeDownloadsH(table,ofile);
        ofile << "\n";
        //delete
        writeRemovesH(table,ofile);
    }
    void CPP::createClassAttributesH(const apidb::symbols::Table& table,std::ofstream& ofile)
    {
        Field field(configureProject,table,ofile);
        field.setDefinition(true);
        field.generate();
    }
    void CPP::createClassPublicH(std::ofstream& file)
    {
        file << "\tpublic:" <<std::endl;
    }
    
    void CPP::createClassPrivateH(std::ofstream& file)
    {
        file << "\tprivate:" <<std::endl;
    }
    void CPP::createClassH(const apidb::symbols::Table& cl,std::ofstream& file,const std::string& nameClass,bool log)
    {
		//file <<"keyword"<<std::endl;
        std::string msg1 = "\n\tHeading class ";
        msg1 += cl.getName() + "";
		if(log) analyzer.getOutput().add(msg1);
        short level = symbols::getSpaceLevel(cl.getFullName());
        for(short i =0; i < level ; i++) file << "\t";
        file <<"\tclass "<<nameClass<<std::endl;  
        for(short i =0; i < level ; i++) file << "\t";      
        file <<"\t{"<<std::endl;
        //file <<"private"<<std::endl;
        createClassPrivateH(file);
        for(short i =0; i < level ; i++) file << "\t";
        file << "\t\tstatic const std::string TABLE_NAME;" <<std::endl;
        //file <<"atributes"<<std::endl;
        createClassAttributesH(cl,file);
        createClassPublicH(file);
        //file <<"methodes"<<std::endl;
        createClassMethodesH(cl,file);
        for(short i =0; i < level ; i++) file << "\t";
        file <<"\t};"<<std::endl;
    }
    bool CPP::createH(std::ofstream& file,bool log,const symbols::ISpace* ispace)
	{
		if(ispace->what() == symbols::SpaceType::TABLE)
        {
            symbols::Table* table = (symbols::Table*) ispace;
            createClassH(*table,file,table->getName(),log);
        }
        else if(ispace->what() == symbols::SpaceType::SPACE)
        {
            symbols::Space* space = (symbols::Space*) ispace;
            file << "namespace " ;
            if(not configureProject.space.empty())
            {
                file << configureProject.space << "\n";
            }
            else if(space->getName().empty())
            {
                file << configureProject.name;
            }
            else
            {
                file << space->getName() << std::endl;
            }
            file << "{\n";
            //std::cout << "Espacio '" << space->getName() << "'" << std::endl;
            for(symbols::Space::iterator it = space->begin(); it != space->end(); it++)
            {
                createH(file,log,it->second);
            }
            file << "\n}\n";
        }
		
		return true;
	}
	bool CPP::createDatconnectCPP(std::ofstream& file,bool log)
	{
		if(!configureProject.writeDatconnect.empty() and configureProject.writeDatconnect.compare("¿?") != 0)
		{
            if(configureProject.getInputLenguaje() == InputLenguajes::MySQL)
            {        
                file << "\tstatic const octetos::db::mysql::Datconnect " << configureProject.writeDatconnect  << "(";
            }
            else if(configureProject.getInputLenguaje() == InputLenguajes::MariaDB)
            {                    
                file << "\tstatic const octetos::db::maria::Datconnect " << configureProject.writeDatconnect  << "(";
            }
            else if(configureProject.getInputLenguaje() == InputLenguajes::PostgreSQL)
            {
                file << "\tstatic const octetos::db::postgresql::Datconnect " << configureProject.writeDatconnect  << "(";
            }
            else
            {
                std::string msg = "Lenguaje no soportado " ;
                core::Error::write(core::Error(msg,ErrorCodes::GENERATOR_FAIL,__FILE__,__LINE__));
                return false;
            }

			file << "\"" << configureProject.getDatconnection()->getHost() << "\",";
			file << configureProject.getDatconnection()->getPort() << ",";
			file << "\"" << configureProject.getDatconnection()->getDatabase() << "\",";
			file << "\"" << configureProject.getDatconnection()->getUser() << "\",";
			file << "\"" << configureProject.getDatconnection()->getPassword() << "\"";
			file << ");\n";
		}
		
		return true;
	}
    bool CPP::createDatconnectHPP(std::ofstream& file,bool log)
	{
        //is function?
        int lgf = configureProject.writeDatconnect.length();
        std::string strF = configureProject.writeDatconnect.substr(lgf - 2,lgf);
        if(strF.compare("()") == 0)
        {
            file << configureProject.writeDatconnect  << "\n";
            return true;
        }
        
		if(!configureProject.writeDatconnect.empty() and configureProject.writeDatconnect.compare("¿?") != 0)
		{
            if(configureProject.getInputLenguaje() == InputLenguajes::MySQL)
            {        
                file << "\tstatic const octetos::db::mysql::Datconnect " << configureProject.writeDatconnect  << "(";
            }
            else if(configureProject.getInputLenguaje() == InputLenguajes::MariaDB)
            {                    
                file << "\tstatic const octetos::db::maria::Datconnect " << configureProject.writeDatconnect  << "(";
            }
            else if(configureProject.getInputLenguaje() == InputLenguajes::PostgreSQL)
            {
                file << "\tstatic const octetos::db::postgresql::Datconnect " << configureProject.writeDatconnect  << "(";
            }
            else
            {
                std::string msg = "Lenguaje no soportado " ;
                core::Error::write(core::Error(msg,ErrorCodes::GENERATOR_FAIL,__FILE__,__LINE__));
                return false;
            }

			file << "\"" << configureProject.getDatconnection()->getHost() << "\",";
			file << configureProject.getDatconnection()->getPort() << ",";
			file << "\"" << configureProject.getDatconnection()->getDatabase() << "\",";
			file << "\"" << configureProject.getDatconnection()->getUser() << "\",";
			file << "\"" << configureProject.getDatconnection()->getPassword() << "\"";
			file << ");\n";
		}
		
		return true;
	}
    bool CPP::createH(std::ofstream& file,bool log,const symbols::SymbolsTable& stb)
	{
        std::locale loc;
        std::string strupper;
        for (std::string::size_type i=0; i<configureProject.name.length(); ++i) 
        {
            strupper +=  std::toupper(configureProject.name[i],loc);
        }
        getHeaderOutput()<< "#ifndef " << strupper << "_HPP\n";
        getHeaderOutput()<< "#define " << strupper << "_HPP\n";
        if(configureProject.getInputLenguaje() == InputLenguajes::MySQL)
        {
            getHeaderOutput()<< "#include <octetos/db/clientdb-mysql.hh>"<<std::endl<<std::endl;
            //getSourceOutput()<< "#include <mysql/mysql.h>"<<std::endl;
        }
        else if(configureProject.getInputLenguaje() == InputLenguajes::MariaDB)
        {
            getHeaderOutput()<< "#include <octetos/db/clientdb-maria.hh>\n";
        }
        else if(configureProject.getInputLenguaje() == InputLenguajes::PostgreSQL)
        {            
            getHeaderOutput()<< "#include <octetos/db/clientdb-postgresql.hh>"<<std::endl<<std::endl;
        }
        else
        {
            std::string msg = "Lenguaje no soportado " ;
            throw BuildException(msg);
            return false;
        }
		getHeaderOutput()<< "#include <string>\n";        
			
        file << "\n";
        for(symbols::SymbolsTable::const_iterator it = stb.begin(); it != stb.end(); it++)
		{
            symbols::ISpace* ispace = it->second;
            if(ispace->what() == symbols::SpaceType::TABLE)
            {
                symbols::Table* table = (symbols::Table*) ispace;
                //createClassH(*table,file,table->getName(),log);
                file << "class " << table->name << ";";
            }
            else if(ispace->what() == symbols::SpaceType::SPACE)
            {
                symbols::Space* space = (symbols::Space*) ispace;
                file << "namespace " ;
                if(not configureProject.space.empty())
                {
                    file << configureProject.space << "\n";
                }
                else if(space->getName().empty())
                {
                    file << configureProject.name;
                }
                else
                {
                    file << space->getName() << std::endl;
                }
                file << "{\n";
                //std::cout << "Espacio '" << space->getName() << "'" << std::endl;
                for(symbols::Space::iterator it = space->begin(); it != space->end(); it++)
                {
                    //createH(file,log,it->second);
                    writeForwardClass(file,log,it->second);
                }
                file << "\n}\n";
            }
        }
		
		for(symbols::SymbolsTable::const_iterator it = stb.begin(); it != stb.end(); it++)
		{
			symbols::ISpace* ispace = it->second;
            if(ispace->what() == symbols::SpaceType::TABLE)
            {
                symbols::Table* table = (symbols::Table*) ispace;
                createClassH(*table,file,table->getName(),log);
            }
            else if(ispace->what() == symbols::SpaceType::SPACE)
            {
                symbols::Space* space = (symbols::Space*) ispace;
                file << "namespace " ;
                if(not configureProject.space.empty())
                {
                    file << configureProject.space << "\n";
                }
                else if(space->getName().empty())
                {
                    file << configureProject.name;
                }
                else
                {
                    file << space->getName() << std::endl;
                }
                file << "{\n";
                file << "\n";
                
                file << "\n";
                createDatconnectHPP(file,log);
                file << "\n\n";
                //std::cout << "Espacio '" << space->getFullName() << "'" << std::endl;
                for(symbols::Space::iterator it = space->begin(); it != space->end(); it++)
                {
                    createH(file,log,it->second);
                }
                file << "\n}\n";
                file << std::endl;
            }
		}	
		getHeaderOutput()<< "#endif\n";
		return true;
	}    
}
}
}
