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
        for( std::map<const char*,ConfigureProject::Table*>::const_iterator itT = configureProject.downloads.begin(); itT != configureProject.downloads.end(); itT++)//std::vector<Table>
        {
            if(table.getName().compare(itT->second->getName()) != 0) 
            {
                continue;//buscar la configuracion de la tabla correspondiente
            }
                        
            //for (auto const& [key, val] : *(itT->second))//class Table : public std::map<const char*, const Function*>
            for(ConfigureProject::Table::iterator itCfTb = itT->second->begin(); itCfTb != itT->second->end(); itCfTb++)
            {
                if(configureProject.getInputLenguaje() == InputLenguajes::MySQL)
                {       
                    ofile << "\tpublic boolean " << itCfTb->first << "(octetos.db.mysql.Connector connector)  throws SQLException"<<std::endl;
                }
                else if(configureProject.getInputLenguaje() == InputLenguajes::MariaDB)
                {
                    ofile << "\tpublic boolean " << itCfTb->first << "(octetos.db.maria.Connector connector)  throws SQLException"<<std::endl;
                }
                else if(configureProject.getInputLenguaje() == InputLenguajes::PostgreSQL)
                {
                    ofile << "\tpublic boolean " << itCfTb->first << "(octetos.db.postgresql.Connector connector)  throws SQLException"<<std::endl;
                }
                else
                {
                    std::string msg = "Lenguaje no soportado " ;
                    throw BuildException(msg);
                }
                
                ofile << "\t{ " << std::endl;
                ofile << "\t\tString sqlString = \"SELECT ";
                const apidb::ConfigureProject::Parameters* params = itCfTb->second->getParameters();

                {
                apidb::ConfigureProject::Parameters::const_iterator itParamEnd = params->end();
                itParamEnd--;
                for(const std::string& param : *params)
                {
					ofile << param; 
					if(param != *itParamEnd)
					{
						ofile << ",";
					}
                }  
                if(configureProject.getInputLenguaje() == InputLenguajes::PostgreSQL)
				{
					ofile << " FROM \\\"" << table.getName() << "\\\" WHERE \";\n" ;
				}
				else
				{
					ofile << " FROM " << table.getName() << " WHERE \";\n" ;
				}
				
				auto kEnd = table.getKey().end();
                kEnd--;
                for(auto k : table.getKey())
                {
                    ofile << "\t\tsqlString = sqlString + \"" << k->getName()  << " = \" + ";  
                    if(k->classReferenced != NULL) //es un objeto
                    {
                        if(k->outType.compare("String") == 0)
                        {
                            ofile << "'" << k->getName() << " + \"'\";\n";
                        }
                        else
                        {
                            ofile << " \"'\" + " << k->getName();
                            getKey(ofile,k->symbolReferenced);
                            ofile<< ") + \"'\";\n";
                        }
                    }
                    else if(k->outType.compare("String") == 0)
                    {
                        ofile << " '\" + " <<  k->getName()  << " + \"'\";\n";
                    }
                    else
                    {
                        ofile << k->getName()  <<";\n";
                    }
                    
                    if(k != *kEnd)
                    {
                        ofile << " + \" and \" ";
                    }          
                }
                
                if(configureProject.getInputLenguaje() == InputLenguajes::MySQL)
                {        
                    ofile << "\t\tResultSet rs = null;"  << std::endl;
                }
                else if(configureProject.getInputLenguaje() == InputLenguajes::MariaDB)
                {
                    ofile << "\t\tResultSet rs = null;"  << std::endl;
                }
                else if(configureProject.getInputLenguaje() == InputLenguajes::PostgreSQL)
                {
                    ofile << "\t\tResultSet rs = null;"  << std::endl;
                }
                else
                {
                    std::string msg = "Lenguaje no soportado " ;
                                    throw BuildException(msg);
                }
                
                ofile << "\t\trs = connector.select(sqlString);"  << std::endl;
                ofile << "\t\tif(rs != null)"  << std::endl;
                ofile << "\t\t{" << std::endl;
                ofile << ""<< std::endl;
                ofile << "\t\t\twhile (rs.nextRow()) "<< std::endl;
                ofile << "\t\t\t{"<< std::endl;
                //ofile << "\t\t\t\tfor(int i = 0; i < num_fields; i++)"<< std::endl;
                //ofile << "\t\t\t\t{"<< std::endl;
                itParamEnd = params->end();
                int countparam = 1;
                for(const std::string& param : *params)
                {
                    //ofile << param; 
                    if(param != *itParamEnd)
                    {
                        //ofile << "\t\t\t\t\tthis->" << param << " = (row[i] ? row[i] : NULL);"<< std::endl;
                        ofile << "\t\t\t\t\tthis." << param << " = ";
                        auto fl = table.find(param.c_str());
                        if(fl != table.end())
                        {
							if((*fl).second->getClassReferenced() != NULL)
							{
								if((*fl).second->outType.compare("int"))
								{
									ofile << " new " << (*fl).second->getClassReferenced()->getName() << "(rs.getInt(" << countparam << ")" << ";" << std::endl ;
								}
								else if((*fl).second->outType.compare("long") or (*fl).second->outType.compare("long int"))
								{
									ofile << " new " << (*fl).second->getClassReferenced()->getName() << "(rs.getl(" << countparam << ")" << ";" << std::endl ;
								}
								else if((*fl).second->outType.compare("String"))
								{
									ofile << " new " << (*fl).second->getClassReferenced()->getName() << "(rs.getString(" << countparam << ")" << ";" << std::endl ;
								}
								else
								{
									ofile << " new " << (*fl).second->getClassReferenced()->getName() << "(rs.getString(" << countparam << ")" << ";" << std::endl ;
								}
							}
							else if((*fl).second->getOutType().compare("int") == 0)
							{
								ofile << " rs.getInt(" << countparam << ")" << ";"<< std::endl ;
							}
							else if((*fl).second->getOutType().compare("long") == 0)
							{
								ofile << " rs.getl(" << countparam << ")" << ";"<< std::endl ;
							}
							else if((*fl).second->getOutType().compare("String") == 0 || (*fl).second->getOutType().compare("const char*") == 0)
							{
								ofile << " rs.getString(" << countparam << ")" << ";" << std::endl ;
							}
							else
							{
								ofile << " rs.getString(" << countparam << ")" << ";" << std::endl ;
							}
                        }
                        else
                        {
                            std::string strmsg = "No se encontro el campo ";
                            strmsg = strmsg + "'" + param + "' en la tabla '" + table.getName() + "' File : generator-c++";
                            throw BuildException(strmsg);
                        }
					}
					countparam++;
				}
                                
				//ofile << "\t\t\t;"<< std::endl;
				//ofile << "\t\t\t\t}"<< std::endl;
				ofile << "\t\t\t}"<< std::endl;
				ofile << "\t\t\treturn true;" << std::endl;
				ofile << "\t\t}" << std::endl;
				ofile << "\t\telse" << std::endl;
				ofile << "\t\t{" << std::endl;
				ofile << "\t\t\treturn false;" << std::endl;
				ofile << "\t\t}" << std::endl;
				}
			ofile << "\t} " << std::endl;
			}         
		}    
		
		
        for(auto symbol : table)
        {
            if(symbol.second->isAutoIncrement() and symbol.second->isPrimaryKey()) continue;
            else
            {
                symbols::Symbol* symroot = getRootSymbol(symbol.second);
                if(symroot->isAutoIncrement() and symroot->isPrimaryKey()) continue;            
            }
                
            if(configureProject.getInputLenguaje() == InputLenguajes::MySQL)
            {        
                ofile << "\tpublic boolean down" << symbol.second->upperName << "(octetos.db.mysql.Connector connector) throws SQLException\n";
            }
            else if(configureProject.getInputLenguaje() == InputLenguajes::MariaDB)
            {        
                ofile << "\tpublic boolean down" << symbol.second->upperName << "(octetos.db.maria.Connector connector) throws SQLException\n";
            }
            else if(configureProject.getInputLenguaje() == InputLenguajes::PostgreSQL)
            {
                ofile << "\tpublic boolean down" << symbol.second->upperName << "(octetos.db.postgresql.Connector connector) throws SQLException\n";
            }
            else
            {
                std::string msg = "Lenguaje no soportado " ;
                throw BuildException(msg,__FILE__,__LINE__);
            }
            
            ofile << "\t{\n";
            ofile << "\t\tString sqlString = \"SELECT " << symbol.second->name << " \";\n";
             
            if(configureProject.getInputLenguaje() == InputLenguajes::PostgreSQL)
            {
                ofile << "\t\tsqlString = sqlString + \" FROM \\\"" << table.getName() << "\\\" WHERE \";\n" ;
            }
            else
            {
                ofile << "\t\tsqlString = sqlString + \" FROM " << table.getName() << " WHERE \";\n" ;
            }
            
            auto kEnd = table.getKey().end();
            kEnd--;
            for(auto k : table.getKey())
            {
                ofile << "\t\tsqlString = sqlString + \"" << k->getName()  << " = \" + ";  
                if(k->classReferenced != NULL) //es un objeto
                {
                    if(k->outType.compare("String") == 0)
                    {
                        ofile << "'" << k->getName() << " + \"'\";\n";
                    }
                    else
                    {
                        ofile << " \"'\" + " << k->getName();
                        getKey(ofile,k->symbolReferenced);
                        ofile<< " + \"'\";\n";
                    }
                }
                else if(k->outType.compare("String") == 0)
                {
                    ofile << " '\" + " <<  k->getName()  << " + \"'\";\n";
                }
                else
                {
                    ofile << "" << k->getName()  <<";\n";
                }
                    
                if(k != *kEnd)
                {
                    ofile << " + \" and \" ";
                }          
            }
            
            if(configureProject.getInputLenguaje() == InputLenguajes::MySQL)
            {        
                ofile << "\t\tResultSet rs = null;"  << std::endl;
            }
            else if(configureProject.getInputLenguaje() == InputLenguajes::MariaDB)
            {
                ofile << "\t\tResultSet rs = null;"  << std::endl;
            }
            else if(configureProject.getInputLenguaje() == InputLenguajes::PostgreSQL)
            {
                ofile << "\t\tResultSet rs = null;"  << std::endl;
            }
            else
            {
                std::string msg = "Lenguaje no soportado " ;
                throw BuildException(msg);
            }
                
            ofile << "\t\trs = connector.select(sqlString);"  << std::endl;
            ofile << "\t\tif(rs.next())"  << std::endl;
            ofile << "\t\t{\n";            
            if(symbol.second->symbolReferenced != NULL)
            {
                ofile << "\t\t\t" << symbol.second->name << " = new " << symbol.second->classReferenced->name << "(";
                if(symbol.second->outType.compare("String") == 0)
                {
                    ofile << symbol.second->getName();
                }
                else
                {
                    ofile << "(*" << symbol.second->getName() << ")";
                    getKey(ofile,symbol.second->symbolReferenced);
                }
                ofile << ");\n";
            }
            else
            {
                ofile << "\t\t\t" << symbol.second->name << " = rs.";
                if(symbol.second->outType.compare("String") == 0)
                {
                    ofile << "getString(1)";
                }
                else if(symbol.second->outType.compare("int") == 0)
                {
                    ofile << "getInt(1)";
                }
                else if(symbol.second->outType.compare("long") == 0)
                {
                    ofile << "getl(1)";
                }
                else if(symbol.second->outType.compare("long long") == 0)
                {
                    ofile << "getll(1)";
                }
                else if(symbol.second->outType.compare("float") == 0)
                {
                    ofile << "getFloat(1)";
                }
                else if(symbol.second->outType.compare("double") == 0)
                {
                    ofile << "getDouble(1)";
                }
                else if(symbol.second->outType.compare("char") == 0)
                {
                    ofile << "getchar(1)";
                }
                else if(symbol.second->outType.compare("unsigned char") == 0)
                {
                    ofile << "getByte(1)";
                }
                else if(symbol.second->outType.compare("byte") == 0)
                {
                    ofile << "getByte(1)";
                }
                else
                {
                    std::string msg = "El tipo de dato '";
                    msg += symbol.second->outType + "' no tiene conversion en el metodo de descarga.";
                    throw BuildException(msg,__FILE__,__LINE__);
                }
            }
            ofile << ";\n";
            ofile << "\t\t\treturn true;\n";
            ofile << "\t\t}\n";
            ofile << "\t\treturn false;\n";
            ofile << "\t}\n";
            
        }
    }
    void Java::writeInsert(const apidb::symbols::Table& table,std::ofstream& ofile)	
	{
        Insert insert(configureProject,table,ofile);
        insert.setImplementation(true);
        insert.generate();
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
            if(it->second->getClassReferenced() != NULL && (it->second->getOutType().compare("int") == 0 || it->second->getOutType().compare("String") == 0))
            {
                ofile << "\t" << it->second->getClassReferenced()->getName() << " "<< it->second->getName()<<";"<< std::endl;
            }
            else
            {
                //ofile <<"[3]"<<std::endl;
                ofile << "\t" << it->second->getOutType() << " " << it->second->getName() <<";"<< std::endl;
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
