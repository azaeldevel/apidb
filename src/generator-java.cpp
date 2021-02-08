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
    void Java::writeRemoves(const apidb::symbols::Table& table, std::ofstream& ofile)
    {
        Remove remove(configureProject,table,ofile);
        remove.setImplementation(true);
        remove.generate();
    }
    void Java::writeUppdaters(const apidb::symbols::Table& table, std::ofstream& ofile)
    {
        Update update(configureProject,table,ofile);
        update.setImplementation(true);
        update.generate();
    }
    
    void Java::writeGetters(const apidb::symbols::Table& table, std::ofstream& ofile)
    {
        Getter getter(configureProject,table,ofile);
        getter.setImplementation(true);
        getter.generate();
    }
    
	Java::~Java()
	{
		
	}
	Java::Java(apidb::Analyzer& d,const ConfigureProject& config) : apidb::generators::Generator(config,d)
	{
        if(config.outputLenguaje != OutputLenguajes::JAVA)
        {
            std::string msg = "La configuracion indica '" ;
            msg = msg + getOutputLenguajeString()+ "', pero el componente es Java.";
            throw BuildException(msg);
        }
	}
	bool Java::generate(bool log)
	{
		if(log)analyzer.getOutput().add("Generando archivos de codigo fuente... \n");
		std::string msg1 = "\tLenguaje resultado: " ;
		msg1 += getOutputLenguajeString() ;
		if(log)analyzer.getOutput().add(msg1);;
        
		if(create(log,getSymbolsTable()) == false)
		{
			return false;
		}
          
        return true;    
    }
		
    void Java::writeSelects(const apidb::symbols::Table& table, std::ofstream& ofile)
    {  
        Select select(configureProject,table,ofile);
        select.setImplementation(true);
        select.generate();
    }
	void Java::writeDownloads(const apidb::symbols::Table& table, std::ofstream& ofile)
    {        
        Download download(configureProject,table,ofile);
        download.setImplementation(true);
        download.generate();
    }
    void Java::writeInsert(const apidb::symbols::Table& table,std::ofstream& ofile)	
	{
        Insert insert(configureProject,table,ofile,Insert::Mode::CreateParent);
        insert.setImplementation(true);
        insert.generate();
        Insert insert2(configureProject,table,ofile,Insert::Mode::ReferencedParent);
        insert2.setImplementation(true);
        insert2.generate();
	}
	void Java::writeDefaultContructor(const apidb::symbols::Table& table,std::ofstream& ofile)
    {
		ofile <<"\tpublic "<<table.getName()<<"()"<<std::endl;
		ofile <<"\t{"<<std::endl;
		ofile <<"\t}"<<std::endl;
	}
	void Java::writeCopyContructor(const apidb::symbols::Table& table,std::ofstream& ofile)
	{
		//constructor de copias 
		ofile << "\tpublic " << table.getName() <<"(" << table.getName() <<" obj)"<<std::endl;
		ofile << "\t{"<<std::endl;                
        //for (auto const& [key, attr] : table)
        for(std::map<const char*,symbols::Symbol*,symbols::cmp_str>::const_iterator it = table.begin(); it != table.end(); it++)
		{
			ofile << "\t\tthis."<< it->second->getName()  << " = obj." << it->second->getName() <<";"<<std::endl;
		}
		ofile << "\t}"<<std::endl;
	}
	void Java::writeKeyRawDataContructor(const apidb::symbols::Table& table,std::ofstream& ofile)
	{
		//constructor que toma key como parametro
        if(table.getKey().size() > 0)//tiene key
        {
            ofile << "\tpublic " << table.getName() << "(";
            auto endIt = table.getKey().end();
            endIt--;
            for(auto k : table.getKey())
            {
                if(k->getOutType().compare("String") == 0)
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
			//no tiene constructor con llaves
		}
        if(table.getKey().size() > 0)//tiene key
        {
            ofile << "\t{" <<std::endl;
            for(auto k : table.getKey())
            {
                if(k->getClassReferenced() != NULL)
                {
                    ofile << "\t\tthis." << k->getName()  << " = new " << k->getClassReferenced()->getName() << "(" << k->getName()  << ");" << std::endl;
                }
                else
                {
                    ofile << "\t\tthis." << k->getName()  << " = " << k->getName()  << ";" << std::endl;
                }
            }
            ofile << "\t}" <<std::endl;
        }
        else 
        {
            
        }
	}
	void Java::createClassMethodes(const apidb::symbols::Table& table,std::ofstream& ofile)
	{
		writeDefaultContructor(table,ofile);
		writeKeyRawDataContructor(table,ofile);
		writeCopyContructor(table,ofile);
        
        ofile << "\n\n";
        writeGetters(table,ofile);
        ofile << "\n\n";
        writeUppdaters(table,ofile);
		ofile << "\n\n";
		writeInsert(table,ofile);
        ofile << "\n\n";        
        //writeSelectInstancetObjectData(table,ofile);
		writeSelects(table,ofile);
        ofile << "\n\n";        
        writeDownloads(table,ofile);
        ofile << "\n\n"; 
        writeRemoves(table,ofile);
        ofile << "\n\n"; 
    }
        
    void Java::createClass(const apidb::symbols::Table& cl,std::ofstream& file,const std::string& nameClass)
    {        
        file << "package " << configureProject.space << ";\n\n";
        file << "import java.sql.ResultSet;\n";
        file << "import java.sql.SQLException;\n";
        file << "import java.util.ArrayList;\n\n";
        file << "\npublic class " << cl.getUpperName() << "\n{\n";
        if(configureProject.getInputLenguaje() == InputLenguajes::MySQL or configureProject.getInputLenguaje() == InputLenguajes::MariaDB)
        {
            file << "\tString " << "TABLE_NAME = \"`"<<  cl.getFullName() << "`\";" << std::endl;
        }
        else if(configureProject.getInputLenguaje() == InputLenguajes::PostgreSQL)
        {
            file << "\tString " << "TABLE_NAME = \""<<  cl.getFullName() << "\";" << std::endl;
        }
        else
        {
            std::string msg = "Lenguaje de entrado no soportado.";
			throw BuildException(msg,__FILE__,__LINE__);
        }
        file<< "\n";
        createClassAttributes(cl,file);
        file<< "\n\n";
		createClassMethodes(cl,file);   
        file<< "}\n";
		file<< std::endl;
    }
    bool Java::create(bool log,const symbols::ISpace* ispace)
	{
		if(ispace->what() == symbols::SpaceType::TABLE)
        {
            symbols::Table* table = (symbols::Table*) ispace;
            std::string strFilename = configureProject.builDirectory;
            strFilename += "/" + table->getName() + ".java";
            std::ofstream file(strFilename, std::ofstream::out);
            createClass(*table,file,table->getName());
            file.flush();
            file.close();
        }
        else if(ispace->what() == symbols::SpaceType::SPACE)
        {
            symbols::Space* space = (symbols::Space*) ispace;
            //std::cout << "Es Espacio " << space->getFullName() << std::endl;
            //file << "namespace " ;
            if(space->getName().empty())
            {
                //file << configureProject.name;
            }
            else
            {
                //file << space->getName() << std::endl;
            }
            //file << "\n{\n";
            //std::cout << "Espacio '" << space->getFullName() << "'" << std::endl;
            for(symbols::Space::iterator it = space->begin(); it != space->end(); it++)
            {
                create(log,it->second);
            }
            //file << "\n}\n";
            //file << std::endl;				
        }
		
		return true;
	}
    bool Java::create(bool log,const symbols::SymbolsTable& stb)
	{
        if(configureProject.getInputLenguaje() == InputLenguajes::MySQL)
        {
            throw core::Exception("Faltas los encabazados de MySQL",__FILE__,__LINE__);
        }
        else if(configureProject.getInputLenguaje() == InputLenguajes::MariaDB)
        {
            
        }
        else if(configureProject.getInputLenguaje() == InputLenguajes::PostgreSQL)
        {
            throw core::Exception("Faltas los encabazados de PosrgreSQL",__FILE__,__LINE__);
        }
        else
        {
            std::string msg = "Lenguaje no soportado " ;
            throw BuildException(msg);
            return false;
        }

		//getSourceOutput()<< "#include \"" << getHeaderName() <<"\""<<std::endl<<std::endl;
        
		for(symbols::SymbolsTable::const_iterator it = stb.begin(); it != stb.end(); it++)
		{
			symbols::ISpace* ispace = it->second;
			if(ispace->what() == symbols::SpaceType::TABLE)
            {
                symbols::Table* table = (symbols::Table*) ispace;
                std::string strFilename = configureProject.builDirectory;
                strFilename += "/" + table->getName() + ".java";
                std::ofstream file(strFilename, std::ofstream::out);
                createClass(*table,file,table->getName());
                file.flush();
                file.close();
            }
            else if(ispace->what() == symbols::SpaceType::SPACE)
            {
                symbols::Space* space = (symbols::Space*) ispace;
                //file << "namespace " ;
                //if(space->getName().empty())
                {
                    //file << configureProject.name;
                }
                //else
                {
                    //file << space->getName() << std::endl;
                }
                //file << "{\n";
                //std::cout << "Espacio '" << space->getFullName() << "'" << std::endl;
                //file << "\n\n";
                for(symbols::Space::iterator it = space->begin(); it != space->end(); it++)
                {
                    create(log,it->second);
                }
                //file << "\n}\n";
                //file << std::endl;
            }
		}
		return true;
	}
    void Java::createClassAttributes(const apidb::symbols::Table& table,std::ofstream& ofile)
    {
        for(std::map<const char*,symbols::Symbol*,symbols::cmp_str>::const_iterator it = table.begin(); it != table.end(); it++)
        {
            if(it->second->symbolReferenced != NULL)
            {
                ofile << "\t" << it->second->classReferenced->name << " " << it->second->name<<";\n";
            }
            else
            {
                ofile << "\t" << it->second->getOutType() << " " << it->second->name <<";\n";
            }
        }
    }
    bool Java::createDatconnect(std::ofstream& file,bool log)
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
                file << "\tstatic const octetos.db.mysql.Datconnect " << configureProject.writeDatconnect  << "(";
            }
            else if(configureProject.getInputLenguaje() == InputLenguajes::MariaDB)
            {                    
                file << "\tstatic const octetos.db.maria.Datconnect " << configureProject.writeDatconnect  << "(";
            }
            else if(configureProject.getInputLenguaje() == InputLenguajes::PostgreSQL)
            {
                file << "\tstatic const octetos.db.postgresql.Datconnect " << configureProject.writeDatconnect  << "(";
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
}
}
}
