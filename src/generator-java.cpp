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
    void Java::writeUppdaters(const apidb::symbols::Table& table, std::ofstream& ofile)
    {
        for(std::map<const char*,symbols::Symbol*,symbols::cmp_str>::const_iterator it = table.begin(); it != table.end(); it++)
        {
            if(not it->second->isPrimaryKey())
			{
		        if(configureProject.getInputLenguaje() == InputLenguajes::MySQL)
		        {
		            ofile << "\tpublic boolean " << "update" << it->second->getUpperName() << "(octetos.db.mysql.Connector connector,";
		        }
		        else if(configureProject.getInputLenguaje() == InputLenguajes::MariaDB)
		        {
		            ofile << "\tpublic boolean " <<"update" << it->second->getUpperName() << "(octetos.db.maria.Connector connector,";
		        }
		        else if(configureProject.getInputLenguaje() == InputLenguajes::PostgreSQL)
		        {
		            ofile << "\tpublic boolean " <<"update" << it->second->getUpperName() << "(octetos.db.postgresql.Connector connector,";
		        }
		        else
		        {
		            std::string msg = "Lenguaje no soportado " ;
		            throw BuildException(msg);
		        }
				if((it->second->getOutType().compare("String") == 0 || it->second->getOutType().compare("int") == 0) && it->second->getClassReferenced() != NULL)
		        {
		            ofile << it->second->getClassReferenced()->getName()  << "& " << it->second->getName() ;
		        }
		        else if(it->second->getOutType().compare("String") == 0)
		        {
		            ofile << it->second->getOutType() << " " << it->second->getName() ;
		        }
		        else
		        {
		            ofile << it->second->getOutType() << " " << it->second->getName() ;
		        }
				ofile <<") throws SQLException"<< std::endl;
				ofile << "\t{"<<std::endl;
				ofile << "\t\tString sqlString = \"\";"<<std::endl;
				if(configureProject.getInputLenguaje() == InputLenguajes::PostgreSQL)
				{
					ofile << "\t\tsqlString = \"UPDATE \\\"\" ;+ TABLE_NAME + \"\\\"\";"<<std::endl;
				}
				else
				{
					ofile << "\t\tsqlString = \"UPDATE \" + TABLE_NAME;"<<std::endl;
					//ofile << "\t\tsqlString += \" FROM \";"<<std::endl;
				}
				ofile << "\t\tsqlString = sqlString + \" SET " ;
		        
				ofile << it->second->getName()  << " = " ;
                            if( it->second->getOutType().compare("int") == 0 && it->second->getSymbolReferenced() != NULL)
                            {
		                            ofile << "'\" +  " << it->second->getName()   << ".getKey" << it->second->getSymbolReferenced()->getUpperName() << "() + \"'\";" << std::endl;                                    
                            }
		                    else if( it->second->getOutType().compare("int") == 0 && it->second->getSymbolReferenced() == NULL)
		                    {
		                            ofile << "'\" +  " << it->second->getName()   << " + \"'\";" << std::endl;                            
		                    }
		                    else if(it->second->getOutType().compare("String") == 0 && it->second->getSymbolReferenced() != NULL)
		                    {
		                            ofile << "'\" + " << it->second->getName()  << " + \"'\";" << std::endl;
		                    }
		                    else if(it->second->getOutType().compare("String") == 0  && it->second->getSymbolReferenced() == NULL)
		                    {
		                            ofile << "'\" + " << it->second->getName()  << " + \"'\";" << std::endl;
		                    }
		                    else
		                    {
		                            ofile << "\" + " << it->second->getName()  << ";" << std::endl;
		                    }
				
				ofile << "\t\tsqlString = sqlString + \" WHERE ";
                if(table.getKey().size() > 0)
                {
                    auto kEnd = table.getKey().end();
                    kEnd--;
                    for(auto k : table.getKey())
                    {
                            ofile << k->getName()  << " = \" + ";  
                            if(k->classReferenced != NULL) //es un objeto
                            {
                                if(k->outType.compare("String") == 0)
                                {
                                    ofile << k->getName() << ";\n";
                                }
                                else
                                {
                                    ofile << k->getName();
                                    getKeyJava(ofile,k->symbolReferenced);
                                    ofile<< ";\n";
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
                }
                else
                {
                    throw BuildException("No hay soporte para table sin llave",__FILE__,__LINE__);
                }
				
                if(configureProject.getInputLenguaje() == InputLenguajes::MySQL)
		        {
                    ofile <<"\t\tResultSet rs  = null;\n";
		        }
		        else if(configureProject.getInputLenguaje() == InputLenguajes::MariaDB)
		        {
		            ofile <<"\t\tResultSet rs = null;\n";
		        }
		        else if(configureProject.getInputLenguaje() == InputLenguajes::PostgreSQL)
		        {
		            ofile <<"\t\tResultSet rs = null;\n";
		        }
		        else
		        {
		            std::string msg = "Lenguaje no soportado " ;
		            throw BuildException(msg);
		        }
                
				ofile <<"\t\treturn connector.update(sqlString,rs);\n";
				ofile << "\t}"<<std::endl;	
            } 
        }
    }
    
    void Java::writeGetters(const apidb::symbols::Table& table, std::ofstream& ofile)
    {
        for(std::map<const char*,symbols::Symbol*,symbols::cmp_str>::const_iterator it = table.begin(); it != table.end(); it++)
        {
			if(it->second->outType.compare("String") == 0)
			{
				ofile <<"\tpublic "<< it->second->getOutType() << " ";
			}
			else if(it->second->getSymbolReferenced())
			{
				ofile <<"\tpublic " << it->second->getClassReferenced()->getName() << " ";
			}
			else 
			{
				ofile <<"\t"<< it->second->getOutType() << " ";
			}
			ofile << "get" << it->second->getUpperName() << "()\n";			
			ofile << "\t{"<<std::endl;
			if(it->second->outType.compare("String") == 0)
			{
				ofile <<"\t\treturn "<< it->second->getName()  <<";"<< std::endl;
			}
			else if(it->second->symbolReferenced)
			{
				ofile <<"\t\treturn "<< it->second->getName()  <<";"<< std::endl;
			}
			else 
			{
				ofile <<"\t\treturn "<< it->second->getName() <<";"<< std::endl;
			}
			ofile << "\t}"<<std::endl;                
        }
    }
    
	Java::~Java()
	{
		//delete[] writeResults;
	}
	/*const std::string& Java::getHeaderName() const
	{
		return projectH;
	}
	std::ofstream& Java::getSourceOutput()
	{
		return writeResults[1];
	}
	std::ofstream& Java::getHeaderOutput()
	{
		return writeResults[0];
	}*/
	Java::Java(apidb::Analyzer& d,const ConfigureProject& config) : apidb::generators::Generator(config,d)
	{
        if(config.outputLenguaje != OutputLenguajes::JAVA)
        {
            std::string msg = "La configuracion indica '" ;
            msg = msg + getOutputLenguajeString()+ "', pero el componente es Java.";
            throw BuildException(msg);
        }
                
		//outputLenguaje = d.getOutputLenguaje();
		/*
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
		*/
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
		
    void Java::writeSelectStatic(const apidb::symbols::Table& table, std::ofstream& ofile)
    {
        if(configureProject.getInputLenguaje() == InputLenguajes::MySQL)
        {
            ofile << "\tpublic static ArrayList<"  << table.getName() << "> " << "select(octetos.db.mysql.Connector connector,String where, int leng)  throws SQLException"<<std::endl;
        }
        else if(configureProject.getInputLenguaje() == InputLenguajes::MariaDB)
        {
            ofile << "\tpublic static ArrayList<"  << table.getName() << "> " << "select(octetos.db.maria.Connector connector,String where, int leng)  throws SQLException"<<std::endl;
        }
        else if(configureProject.getInputLenguaje() == InputLenguajes::PostgreSQL)
        {
            ofile << "\tpublic static ArrayList<"  << table.getName() << "> " << "select(octetos.db.postgresql.Connector connector,String where, int leng) throws SQLException"<<std::endl;
        }
        else
        {
            std::string msg = "Lenguaje no soportado " ;
            throw BuildException(msg);
        }
            
        ofile << "\t{" <<std::endl;
        ofile << "\t\tString sqlString = \"SELECT ";
        //selecciona los campos de las llaves
        auto endK = table.getKey().end();
        endK--;
        for(auto k : table.getKey())
        {
            ofile << k->getName();
            if(k != *endK)
            {
                ofile << ",";
            }
        }
        if(configureProject.getInputLenguaje() == InputLenguajes::PostgreSQL)
		{
			ofile << " FROM \\\"" << table.getName() << "\\\" WHERE \";"<< std::endl;
		}
		else
		{
			ofile << " FROM " << table.getName() << " WHERE \";"<< std::endl;
		}
		ofile << "\t\tsqlString += where;\n";
        ofile << "\t\tif(leng > 0)"  << std::endl;
        ofile << "\t\t{"  << std::endl;
        ofile << "\t\t\tsqlString += \" LIMIT  \";"  << std::endl;
		ofile << "\t\t\tsqlString += leng;\n ";
        ofile << "\t\t}"  << std::endl;
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
        ofile << "\t\t\tArrayList<"<< table.getName() << "> tmpVc = new ArrayList<" << table.getName() << ">();" << std::endl;
        ofile << "\t\t\twhile(rs.next())" << std::endl;
        ofile << "\t\t\t{"<< std::endl;
        ofile << "\t\t\t\t"<< table.getName() << " tmp;" << std::endl;            
        ofile << "\t\t\t\ttmp = new " << table.getName() << "(";
        auto endK2 = table.getKey().end();
        endK2--;
        int count2 = 1;
        for(auto k : table.getKey())
        {
            if(k->getOutType().compare("String") == 0)
            {
                ofile << "rs.getString(" << count2 << ")";
            }
            else if(k->getOutType().compare("int") == 0)
            {
                ofile << "rs.getInt(" << count2 << ")";
            }
            else
            {
                ofile << "rs.getString(" << count2 << ")";                
            }
            if(k != *endK2)
            {
                ofile << ",";
            }
            count2++;
        }
            ofile << ")" << ";" << std::endl;
            ofile << "\t\t\t\ttmpVc.add(tmp);" << std::endl;
            ofile << "\t\t\t}"<< std::endl;
            ofile << "\t\t\treturn tmpVc;" << std::endl;
            ofile << "\t\t}" << std::endl;
            ofile << "\t\treturn null;" << std::endl;
            ofile << "\t}" <<std::endl;
        
        
        //select from config
        //std::vector<apidb::ConfigureProject::Table> tbs = configureProject.selects;
        for( std::map<const char*,ConfigureProject::Table*>::const_iterator itT =  configureProject.selects.begin(); itT != configureProject.selects.end(); itT++)//std::vector<Table>
        {
            if(table.getName().compare(itT->second->getName()) != 0) 
            {
                continue;//buscar la configuracion de la tabla correspondiente
            }
            
            //for (auto const& [key, val] : *(itT->second))//class Table : public std::map<std::string,Function>
            for(ConfigureProject::Table::iterator itCfTb = itT->second->begin(); itCfTb != itT->second->end(); itCfTb++)
            {
                
                if(configureProject.getInputLenguaje() == InputLenguajes::MySQL)
                {        
                    ofile << "\tpublic ArrayList<" << table.getName()<< "> " << itCfTb->second->getName() << "(octetos.db.mysql.Connector connector,";
                }
                else if(configureProject.getInputLenguaje() == InputLenguajes::MariaDB)
                {
                    ofile << "\tpublic ArrayList<" << table.getName()<< "> " << itCfTb->second->getName() << "(octetos.db.maria.Connector connector,";
                }
                else if(configureProject.getInputLenguaje() == InputLenguajes::PostgreSQL)
                {
                    ofile << "\tpublic ArrayList<" << table.getName()<< "> " << itCfTb->second->getName() << "(octetos.db.postgresql.Connector connector,";
                }
                else
                {
                    std::string msg = "Lenguaje no soportado " ;
                    throw BuildException(msg);
                }
                
                const apidb::ConfigureProject::Parameters* params = itCfTb->second->getParameters();
                
                    apidb::ConfigureProject::Parameters::const_iterator itParamEnd = params->end();
                    itParamEnd--;
                    for(const std::string& param : *params)
                    {
                        auto fl = table.find(param.c_str());
                        if(fl == table.end())
                        {
                            std::string strmsg = "No se encontro el campo '";
                            strmsg = strmsg + param + "' en la tabla '" + table.getName() + "'" + "File : generator-c++";
                            throw BuildException(strmsg);
                        }
                        if((*fl).second->getOutType().compare("String") == 0)
                        {
                            ofile << "String " << param; 
                        }
                        else if((*fl).second->getSymbolReferenced() != NULL)
                        {
                                ofile << (*fl).second->getSymbolReferenced()->getClassParent()->getName() << param;
                        }
                        else
                        {
                            ofile << (*fl).second->getOutType() << " " << param;
                        }
                        if(param != *itParamEnd)
                        {
                            ofile << ",";
                        }
                    }
                
                ofile << ", int leng)"<<std::endl;
                ofile << "\t{"<<std::endl;
                ofile << "\t\tString sqlString = \"SELECT ";
                //const apidb::ConfigureProject::Parameters& params = val->getParameters();
                
                    auto endK = table.getKey().end();
                    endK--;
                    for(auto k : table.getKey())
                    {
                        ofile << k->getName();
                        if(k != *endK)
                        {
                            ofile << ",";
                        }
                    }
				if(configureProject.getInputLenguaje() == InputLenguajes::PostgreSQL)
				{
                    ofile << " FROM \\\"" << table.getName() << "\\\" WHERE \";"<< std::endl;  
				}
				else
				{
					ofile << " FROM " << table.getName() << " WHERE \";"<< std::endl; 
				}
                itParamEnd = params->end();
                --itParamEnd;
                for(const std::string& param : *params)
                {
                        auto fl = table.find(param.c_str());
                        if(fl != table.end())
                        {
                            if((*fl).second->getClassReferenced() != NULL && (*fl).second->getOutType().compare("int") == 0)
                            {
                                ofile << "\t\tsqlString = sqlString + \"" << param << " = \" + \"'\" + std::to_string(" << (*fl).second->getName() << ") + \"'\"";
                            }
                            else if((*fl).second->getOutType().compare("int") == 0)
                            {
                                ofile << "\t\tsqlString = sqlString + \"" << param << " = \" + \"'\" + std::to_string(" << (*fl).second->getName() << ") + \"'\"";
                            }
                            else if((*fl).second->getOutType().compare("String") == 0)
                            {
                                ofile << "\t\tsqlString = sqlString + \"" << param << " = \" + \"'\" + " << (*fl).second->getName() << " + \"'\"";
                            }
                            else
                            {
                                std::string strmsg = "No se encontro el campo ";
                                strmsg = strmsg + "'" + (*fl).second->getName() + "' en la tabla '" + table.getName() + "'" + "File : generator-c++";
                                throw BuildException(strmsg);                                
                            }
                        }
                        else
                        {
                                std::string strmsg = "No se encontro el campo ";
                                strmsg = strmsg + "'" + param + "' en la tabla '" + table.getName() + "'";
                                throw BuildException(strmsg);
                        }
                        if(param != *itParamEnd)
                        {
                            ofile << " + \" and \";"<< std::endl;
                        }
                }
                    ofile << ";" << std::endl;
					ofile << "\t\tif(leng > 0)"  << std::endl;
					ofile << "\t\t{"  << std::endl;
					ofile << "\t\t\tsqlString += \" LIMIT  \" + std::to_string(leng);"  << std::endl;
					ofile << "\t\t}"  << std::endl;
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
					ofile << "\t\trs = connector.execute(sqlString,rs);"  << std::endl;
					ofile << "\t\tif(flag)"  << std::endl;
					ofile << "\t\t{" << std::endl;
                    ofile << "\t\t\tstd::vector<"<< table.getName() << "*>* tmpVc = new ArrayList<" << table.getName() << ">;" << std::endl;
                    ofile << "\t\t\twhile(rs.next()) "<< std::endl;
                    ofile << "\t\t\t{"<< std::endl;     
                    ofile << "\t\t\t\t"<< table.getName() << "* tmp = NULL;" << std::endl;             
                    ofile << "\t\t\t\ttmp = new " << table.getName() << "(";              
                    auto endK2 = table.getKey().end();
                    endK2--;
                    int count2 = 1;
                    for(auto k : table.getKey())
                    {
                        if(k->getOutType().compare("String") == 0)
                        {
                            ofile << "rs.getString(" << count2 << ")";
                        }
                        else if(k->getOutType().compare("int") == 0)
                        {
                            ofile << "rs.getint(" << count2 << ")";
                        }
                        else
                        {
                            ofile << "rs.getString(" << count2 << ")";               
                        }
                        if(k != *endK2)
                        {
                            ofile << ",";
                        }
                        count2++;
                    }
                    ofile << ")" << ";" << std::endl;
                    ofile << "\t\t\t\ttmpVc.add(tmp);" << std::endl;
                    ofile << "\t\t\t}"<< std::endl;
                    ofile << "\t\t\treturn tmpVc;"<< std::endl;
                    ofile << "\t\t}" << std::endl;
                    ofile << "\t\telse" << std::endl;
                    ofile << "\t\t{" << std::endl;
                    ofile << "\t\t\treturn NULL;" << std::endl;
                    ofile << "\t\t}" << std::endl;
                
                ofile << "\t}"<<std::endl;
                
            }
        }
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
                            getKeyJava(ofile,k->symbolReferenced);
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
                        getKeyJava(ofile,k->symbolReferenced);
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
                    getKeyJava(ofile,symbol.second->symbolReferenced);
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
    
    void Java::getKey2Java(std::ofstream& ofile, const symbols::Symbol* k)
    {
        if(k->symbolReferenced != NULL)
        {            
            ofile << ".get" << k->getUpperName() << "()";
            getKey2Java(ofile,k->symbolReferenced);
        }
        else
        {
            ofile << ".get" << k->getUpperName() << "()";
        }
    }
    void Java::writeSelectInstancetObjectData(const apidb::symbols::Table& table,std::ofstream& ofile)	
	{
		ofile << "\tpublic boolean ";
        if(configureProject.getInputLenguaje() == InputLenguajes::MySQL)
        {        
            ofile << "select(octetos.db.mysql.Connector connector";
        }
        else if(configureProject.getInputLenguaje() == InputLenguajes::MariaDB)
        {
            ofile << "select(octetos.db.maria.Connector connector";
        }
        else if(configureProject.getInputLenguaje() == InputLenguajes::PostgreSQL)
        {
            ofile << "select(octetos.db.postgresql.Connector connector";
        }
        else
        {
            std::string msg = "Lenguaje no soportado " ;
            throw BuildException(msg);
        }
        for(symbols::Symbol* k : table.getKey())
        {
			if(k->symbolReferenced != NULL)
			{
				ofile << "," << k->classReferenced->getName() << " ";
			}
			else
			{
				ofile << "," << k->getOutType() << " " ;
			}  
			
			ofile << k->getName();
		}
        ofile << ") throws SQLException"<<std::endl;
        ofile << "\t{\n";
        ofile << "\t\tString sql = \"SELECT ";
        symbols::Key::const_iterator penultimo = --table.getKey().end();
        //symbols::Symbol* penultimo = (--(table.end()))->second;
        for(symbols::Symbol* k : table.getKey())
        {
			ofile << " " << k->getName();
            if(k != (*penultimo))
            {
                ofile << ",";
            }
		}
		ofile << "\";\n\t\tsql = sql + \" FROM \" + TABLE_NAME " <<  " + \" WHERE ";
        for(symbols::Symbol* k : table.getKey())
        {
			ofile << k->getName() << " = \" + " ;  
            if(k->symbolReferenced != NULL)
            {
                if(k->outType.compare("String") == 0)
                {
                    ofile << k->getName();
                    getKey2Java(ofile,k->symbolReferenced);
                }
                else
                {
                    
                    ofile << k->getName();
                    getKey2Java(ofile,k->symbolReferenced);
                }
            }
            else if(k->outType.compare("String") == 0)
            {
                ofile << k->getName();
            }
            else
            {
                ofile << k->getName();
            }
            if(k != (*penultimo) )
            {
                ofile << " and ";
            }
		}
		ofile << ";\n";
        
        if(configureProject.getInputLenguaje() == InputLenguajes::MySQL)
        {        
            ofile << "\t\tResultSet rs = null;\n";
        }
        else if(configureProject.getInputLenguaje() == InputLenguajes::MariaDB)
        {
            ofile << "\t\tResultSet rs = null;\n";
        }
        else if(configureProject.getInputLenguaje() == InputLenguajes::PostgreSQL)
        {
            ofile << "\t\tResultSet rs = null;\n";
        }
        else
        {
            std::string msg = "Lenguaje no soportado " ;
            throw BuildException(msg);
        }
		ofile << "\t\trs = connector.select(sql);\n";
        ofile << "\t\tif(rs != null)\n";
        ofile << "\t\t{\n";
        for(symbols::Symbol* k : table.getKey())
        {
            if(k->symbolReferenced != NULL)
            {
                ofile << "\t\t\tthis." << k->name << " = new " << k->classReferenced->name << "(" << k->name << ");\n";
            }
            else
            {
                ofile << "\t\t\tthis." << k->name << " = " << k->name << ";\n";
            }
        }        
        ofile << "\t\t\treturn true;\n";
        ofile << "\t\t}\n";
        //ofile << "\t\t\n";
        ofile << "\t\treturn false;\n";
        ofile << "\t}\n";
	}
    void Java::writeInsert(const apidb::symbols::Table& table,std::ofstream& ofile)	
	{
        auto penultimoReq = --table.getRequired().end();
        // Methodo insert Raw datas
        ofile << "\t"<< "boolean ";
        if(configureProject.getInputLenguaje() == InputLenguajes::MySQL)
        {
            ofile << "insert(octetos.db.mysql.Connector connector";
        }
        else if(configureProject.getInputLenguaje() == InputLenguajes::MariaDB)
        {
            ofile << "insert(octetos.db.maria.Connector connector";
        }
        else if(configureProject.getInputLenguaje() == InputLenguajes::PostgreSQL)
        {
            ofile << "insert(octetos.db.postgresql.Connector connector";
        }
        else
        {
            std::string msg = "Lenguaje no soportado " ;
            throw BuildException(msg);
        }
        for(symbols::Symbol* k : table.getRequired())
        {
            if(k->symbolReferenced!= NULL)
            {
                for(symbols::Symbol* l : k->classReferenced->getRequired())
                {
                    symbols::Symbol* rootS = getRootSymbol(l);
                    if(rootS->isPrimaryKey() and rootS->isAutoIncrement()) continue;
                    
                    if(l->symbolReferenced!= NULL)
                    {                        
                        ofile << ",";
                        insertParamsRaw(ofile,l,k);
                    }
                    else if(l->outType.compare("String") == 0)
                    {
                        ofile << ",String " << k->name << l->upperName;
                    }
                    else
                    {
                        ofile << "," << k->getOutType() << " " << k->name << l->upperName;
                    }
                }
            }
			else if(k->outType.compare("String") == 0)
            {
                ofile << ",String " << k->name;
            }
            else
            {
                if(not k->isAutoIncrement() and not k->isPrimaryKey()) ofile << "," << k->getOutType() << " " << k->name;
            }
		}
        ofile << ") throws SQLException"<<std::endl;
        ofile << "\t{"<<std::endl;
        
        //para cada campo foraneo
        for(symbols::Symbol* k : table.getRequired())
        {
            if(k->isPrimaryKey() && k->isAutoIncrement()) continue;
            
			if(k->symbolReferenced != NULL)
            {
                ofile << "\t\tthis." << k->name << " = new " << k->classReferenced->name << "();\n";
                ofile << "\t\tif(" << k->name << ".insert(connector";
                for(symbols::Symbol* l : k->symbolReferenced->classParent->getRequired())
                {
                    if(l->symbolReferenced != NULL)
                    {
                        for(symbols::Symbol* m : l->symbolReferenced->classParent->getRequired())
                        {                            
                            symbols::Symbol* rootS = getRootSymbol(m);
                            if(rootS->isPrimaryKey() and rootS->isAutoIncrement()) continue;
                            
                            ofile << ",";
                            if(m->symbolReferenced!= NULL)
                            {
                                insertValueRaw(ofile,m,l);
                            }
                            else if(m->outType.compare("String") == 0)
                            {
                                ofile << k->name << m->upperName;
                            }
                            else
                            {
                                ofile << k->name << m->upperName;
                            }
                        }
                    }
                    else if(l->outType.compare("String") == 0)
                    {
                        symbols::Symbol* rootS = getRootSymbol(l);
                        if(rootS->isPrimaryKey() and rootS->isAutoIncrement()) continue;
                        
                        ofile << "," << k->name << l->upperName;
                    }
                    else
                    {
                        symbols::Symbol* rootS = getRootSymbol(l);
                        if(rootS->isPrimaryKey() and rootS->isAutoIncrement()) continue;
                        
                        ofile << "," << k->name << l->upperName;
                    }
                }
                ofile << ") == false) return false;\n";
            }            
		}

                
        ofile << "\t\t"<<"String sqlString = \"\";"<<std::endl;
		if(configureProject.getInputLenguaje() == InputLenguajes::PostgreSQL)
		{
			ofile << "\t\t"<<"sqlString = sqlString + \"INSERT INTO \\\"\"  + TABLE_NAME + \"\\\"\" ; "<<std::endl;
		}
		else
		{
			ofile << "\t\t"<<"sqlString = sqlString + \"INSERT INTO \" + TABLE_NAME ; "<<std::endl;
		}
        ofile << "\t\t"<<"sqlString = sqlString + \"(";
        for(symbols::Symbol* k : table.getRequired())
        {
            if(k->isPrimaryKey() and k->isAutoIncrement()) continue;
            
            ofile << k->name;
            if(*penultimoReq != k)
            {
                ofile << ",";
            }
        }
		ofile << ")\";" << std::endl;
		ofile << "\t\tsqlString = sqlString + \" VALUES(\"";
		for(symbols::Symbol* k : table.getRequired())
        {
            if(k->isPrimaryKey() and k->isAutoIncrement()) continue;
            
            if(k->symbolReferenced != NULL)
            {                
                if(getRootSymbol(k->symbolReferenced)->outType.compare("String") == 0)
                {
                    ofile << " + \"'\" ";
                    insertValueRaw(ofile,k->symbolReferenced,k);
                    ofile << " + \"'\" ";
                }
                else
                {
                    ofile << " + " << k->name;
                    getKeyJava(ofile,k->symbolReferenced);
                }
            }
            else if(k->outType.compare("String") == 0)
            {
                ofile << " + \"'\" + " << k->name << " + \"'\"";
            }
            else
            {
                ofile << " + " << k->name;
            }
            if(*penultimoReq != k)
            {
                ofile << " + \",\" ";
            }
        }
		ofile << " + \")\";"<< std::endl;
		
        
        //ofile << "\t\tstd::cout << sqlString << std::endl;\n";
        if(configureProject.getInputLenguaje() == InputLenguajes::MySQL)
        {        
            ofile << "\t\tResultSet rs = null;" << std::endl;
            //iniciar llave?
            if(table.getKey().size() > 1)
            {//es llave compuesta?
                std::string msg = "No hay soporte aun para llaves compuestas" ;
                throw BuildException(msg,__FILE__,__LINE__);
            }
            else if(table.getKey().size() == 1 and (*table.getKey().begin())->symbolReferenced == NULL and (*(table.getKey().begin()))->outType.compare("int") == 0)
            {//es llave simple y no tiene campos foraneos?
                ofile << "\t\tif(connector.insert(sqlString,rs))\n";
                ofile << "\t\t{\n";
                ofile << "\t\t\t" << (*(table.getKey().begin()))->name;
                ofile << " = connector.last_inserted_id();\n";
                ofile << "\t\t\tif(" << (*(table.getKey().begin()))->name << " > 0) return true;\n";
                ofile << "\t\t}\n";
                ofile << "\t\treturn false;\n";
            }
            else if(table.getKey().size() == 1 and (*table.getKey().begin())->symbolReferenced != NULL )
            {//es llave simple y no tiene campos foraneos?
                ofile << "\t\tif(connector.insert(sqlString,rs)) return true;\n";
                ofile << "\t\treturn false;\n";
            }
            else if(table.getKey().size() < 1 )
            {
                std::string msg = "No hay llave para esta table " ;
                throw BuildException(msg,__FILE__,__LINE__);
            }  
        }
        else if(configureProject.getInputLenguaje() == InputLenguajes::MariaDB)
        {
            ofile << "\t\tResultSet rs = null;" << std::endl;
            //iniciar llave?
            if(table.getKey().size() > 1)
            {//es llave compuesta?
                std::string msg = "No hay soporte aun para llaves compuestas" ;
                throw BuildException(msg,__FILE__,__LINE__);
            }
            else if(table.getKey().size() == 1 and (*table.getKey().begin())->symbolReferenced == NULL and (*(table.getKey().begin()))->outType.compare("int") == 0)
            {//es llave simple y no tiene campos foraneos?
                ofile << "\t\tif(connector.insert(sqlString,rs))\n";
                ofile << "\t\t{\n";
                ofile << "\t\t\t" << (*(table.getKey().begin()))->name;
                ofile << " = connector.last_inserted_id();\n";
                ofile << "\t\t\tif(" << (*(table.getKey().begin()))->name << " > 0) return true;\n";
                ofile << "\t\t}\n";
                ofile << "\t\treturn false;\n";
            }
            else if(table.getKey().size() == 1 and (*table.getKey().begin())->symbolReferenced != NULL )
            {//es llave simple y no tiene campos foraneos?
                ofile << "\t\tif(connector.insert(sqlString,rs)) return true;\n";
                ofile << "\t\treturn false;\n";
            }
            else if(table.getKey().size() < 1 )
            {
                std::string msg = "No hay llave para esta table " ;
                throw BuildException(msg,__FILE__,__LINE__);
            }  
        }
        else if(configureProject.getInputLenguaje() == InputLenguajes::PostgreSQL)
        {
            ofile << "\t\tResultSet rs = null;" << std::endl;
            throw BuildException("No se ha emplemetnedo el insert en postgresql",__FILE__,__LINE__);
        }
        else
        {
            std::string msg = "Lenguaje no soportado " ;
            throw BuildException(msg,__FILE__,__LINE__);
        }   
        
        ofile << "\t}"<<std::endl;
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
			std::string msg = "Requirio la generacion de constructor de llave, pero la tabla '";
			msg = msg + table.getName();
            msg = msg + "'  no tiene llave, revise su configuracion o so modelo de DB";
			throw core::Exception(msg,__FILE__,__LINE__);
		}
		ofile << "\t{" <<std::endl;
        if(table.getKey().size() > 0)//tiene key
        {
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
        }
        else 
        {
            
        }
		ofile << "\t}" <<std::endl;
	}
	void Java::getKeyJava(std::ofstream& ofile, const symbols::Symbol* k)
    {
        if(k->symbolReferenced != NULL)
        {            
            ofile << ".get" << k->getUpperName() << "()";
            getKey2Java(ofile,k->symbolReferenced);
        }
        else
        {
            ofile << ".get" << k->getUpperName() << "()";
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
        writeSelectInstancetObjectData(table,ofile);
		writeSelectStatic(table,ofile);
        ofile << "\n\n";        
        writeDownloads(table,ofile);
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
    
	symbols::Symbol* getRootSymbolJava(symbols::Symbol* k)
    {
        if(k == NULL) return NULL;
        
        if(k->symbolReferenced != NULL)
        {
            return getRootSymbol(k->symbolReferenced);
        }
        else
        {
            return k;
        }
    }
    
	void insertParamsRawJava(std::ofstream& ofile,symbols::Symbol* k,symbols::Symbol* parent)
    {
        if(k->symbolReferenced != NULL)
        {
            if(k->symbolReferenced->symbolReferenced != NULL)
            {
                insertParamsRaw(ofile,k->symbolReferenced,parent);
            }     
            else
            {
                auto penultimo = k->symbolReferenced->classParent->getRequired().begin();
                penultimo--;
                penultimo--;
                for(symbols::Symbol* l : k->symbolReferenced->classParent->getRequired())
                {
                    ofile << l->outType << " " << parent->name << l->upperName;
                    if(*penultimo != l)
                    {
                        ofile << ",";
                    }
                }
            }
        }
    }
    
	void insertValueRawJava(std::ofstream& ofile,symbols::Symbol* k,symbols::Symbol* parent)
    {
        if(k->symbolReferenced != NULL)
        {
            if(k->symbolReferenced->symbolReferenced != NULL)
            {
                insertValueRaw(ofile,k->symbolReferenced,parent);
            }     
            else
            {
                auto penultimo = k->symbolReferenced->classParent->getRequired().begin();
                penultimo--;
                penultimo--;
                for(symbols::Symbol* l : k->symbolReferenced->classParent->getRequired())
                {
                    ofile << parent->name << l->upperName;
                    if(*penultimo != l)
                    {
                        ofile << ",";
                    }
                }
            }
        }
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
