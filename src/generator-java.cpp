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


#include "analyzer.hpp"
#include "generator.hpp"
#include "Errors.hpp"

namespace octetos
{
namespace apidb
{
namespace generators
{
	Java::~Java()
	{
		
	}
	std::ofstream& Java::getSourceOutput()
	{
		return *writeResult;
	}
	Java::Java(apidb::Analyzer& d,const ConfigureProject& config) : apidb::generators::Generator(config,d)
	{                
        if(config.outputLenguaje != OutputLenguajes::JAVA)
        {
            std::string msg = "La configuracion indica '" ;
            msg = msg + getOutputLenguajeString()+ "', pero el componente es Java.";
            throw BuildException(msg);
        } 
		
		if((configureProject.builDirectory.empty()) | (configureProject.builDirectory.compare(".") == 0)) 
		{
			
		}
		else
		{
			
		}
	}
	bool Java::generate(bool log)
	{
		if(log)analyzer.getOutput().add("Generando archivos de codigo fuente... \n");
		std::string msg1 = "\tLenguaje resultado: " ;
		msg1 += getOutputLenguajeString() ;
		if(log)analyzer.getOutput().add(msg1);;
		//includes in header file
		std::string headers = "";     
        if(configureProject.getInputLenguaje() == InputLenguajes::MySQL)
        {
            
        }
        else if(configureProject.getInputLenguaje() == InputLenguajes::MariaDB)
        {
            
        }
        else if(configureProject.getInputLenguaje() == InputLenguajes::PostgreSQL)
        {
            
        }
        else
        {
            std::string msg = "Lenguaje no soportado " ;
            throw BuildException(msg);
            return false;
        }
		        
			
		
		//writing code
		if(create(log,getSymbolsTable()) == false)
		{
			return false;
		}
          
        return true;    
    }
		void Java::writeSelects(const apidb::symbols::Table& table, std::ofstream& ofile)
        {
			//select(conector,where)	
            
            if(configureProject.getInputLenguaje() == InputLenguajes::MySQL)
            {        
                ofile << "\tstd::vector<"  << table.getName() << "*>* " << table.getName() << "::select(octetos::db::mysql::Connector& connector, const std::string& where, int leng)"<<std::endl;
            }
            else if(configureProject.getInputLenguaje() == InputLenguajes::MariaDB)
            {
                ofile << "\tstd::vector<"  << table.getName() << "*>* " << table.getName() << "::select(octetos::db::mariadb::Connector& connector, const std::string& where, int leng)"<<std::endl;
            }
            else if(configureProject.getInputLenguaje() == InputLenguajes::PostgreSQL)
            {
                ofile << "\tstd::vector<"  << table.getName() << "*>* " << table.getName() << "::select(octetos::db::postgresql::Connector& connector, const std::string& where, int leng)"<<std::endl;
            }
            else
            {
                std::string msg = "Lenguaje no soportado " ;
                throw BuildException(msg);
            }
        ofile << "\t{" <<std::endl;
        ofile << "\t\tstd::string sqlString = \"SELECT ";
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
			ofile << " FROM \\\"" << table.getName() << "\\\" WHERE \" + where ;"<< std::endl;
		}
		else
		{
			ofile << " FROM " << table.getName() << " WHERE \" + where ;"<< std::endl;
		}
        ofile << "\t\tif(leng > 0)"  << std::endl;
        ofile << "\t\t{"  << std::endl;
        ofile << "\t\t\tsqlString += \" LIMIT  \"  + std::to_string(leng);"  << std::endl;
        ofile << "\t\t}"  << std::endl;
        if(configureProject.getInputLenguaje() == InputLenguajes::MySQL)
        {        
            ofile << "\t\toctetos::db::mysql::Datresult dt;"  << std::endl;
        }
        else if(configureProject.getInputLenguaje() == InputLenguajes::MariaDB)
        {
            ofile << "\t\toctetos::db::mariadb::Datresult dt;"  << std::endl;
        }
        else if(configureProject.getInputLenguaje() == InputLenguajes::PostgreSQL)
        {
            ofile << "\t\toctetos::db::postgresql::Datresult dt;"  << std::endl;
        }
        else
        {
            std::string msg = "Lenguaje no soportado " ;
            throw BuildException(msg);
        }
        
        if(configureProject.getInputLenguaje() == InputLenguajes::MySQL)
        {
            ofile << "\t\tbool flag = connector.execute(sqlString,dt);"  << std::endl;
            ofile << "\t\tif(flag)"  << std::endl;
            ofile << "\t\t{" << std::endl;
            ofile << "\t\t\tstd::vector<"<< table.getName() << "*>* tmpVc = new std::vector<" << table.getName() << "*>;" << std::endl;
            ofile << "\t\t\twhile(dt.nextRow())" << std::endl;
            ofile << "\t\t\t{"<< std::endl;
            ofile << "\t\t\t\t"<< table.getName() << "* tmp = NULL;" << std::endl;            
            ofile << "\t\t\t\ttmp = new " << table.getName() << "(";
            auto endK2 = table.getKey().end();
            endK2--;
            int count2 = 0;
            for(auto k : table.getKey())
            {
                if(k->getOutType().compare("std::string") == 0)
                {
                    ofile << "dt.getint(" << count2 << ")";
                }
                else if(k->getOutType().compare("int") == 0)
                {
                    ofile << "dt.getString(" << count2 << ")";
                }
                else
                {
                    ofile << "dt.getString(" << count2 << ")";                
                }
                if(k != *endK2)
                {
                    ofile << ",";
                }
                count2++;
            }
            ofile << ")" << ";" << std::endl;
            ofile << "\t\t\t\ttmpVc->push_back(tmp);" << std::endl;
            
            ofile << "\t\t\t}"<< std::endl;
            ofile << "\t\t\treturn tmpVc;" << std::endl;
            ofile << "\t\t}" << std::endl;
            ofile << "\t\treturn NULL;" << std::endl;
            
            ofile << "\t}" <<std::endl;
        }
        else
        {
            ofile << "\t\tbool flag = connector.execute(sqlString,dt);"  << std::endl;
            ofile << "\t\tif(flag)"  << std::endl;
            ofile << "\t\t{" << std::endl;
            ofile << "\t\t\tstd::vector<"<< table.getName() << "*>* tmpVc = new std::vector<" << table.getName() << "*>;" << std::endl;
            ofile << "\t\t\twhile(dt.nextRow())" << std::endl;
            ofile << "\t\t\t{"<< std::endl;
            ofile << "\t\t\t\t"<< table.getName() << "* tmp = NULL;" << std::endl;            
            ofile << "\t\t\t\ttmp = new " << table.getName() << "(";
            auto endK2 = table.getKey().end();
            endK2--;
            int count2 = 0;
            for(auto k : table.getKey())
            {
                if(k->getOutType().compare("std::string") == 0)
                {
                    ofile << "dt.getString(" << count2 << ")";
                }
                else if(k->getOutType().compare("int") == 0)
                {
                    ofile << "dt.getint(" << count2 << ")";
                }
                else
                {
                    ofile << "dt.getString(" << count2 << ")";                
                }
                if(k != *endK2)
                {
                    ofile << ",";
                }
                count2++;
            }
            ofile << ")" << ";" << std::endl;
            ofile << "\t\t\t\ttmpVc->push_back(tmp);" << std::endl;
            ofile << "\t\t\t}"<< std::endl;
            ofile << "\t\t\treturn tmpVc;" << std::endl;
            ofile << "\t\t}" << std::endl;
            ofile << "\t\treturn NULL;" << std::endl;
            ofile << "\t}" <<std::endl;
        }
        
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
                    ofile << "\tstd::vector<" << table.getName()<< "*>* " << table.getName() << "::" << itCfTb->second->getName() << "(octetos::db::mysql::Connector& connector,";
                }
                else if(configureProject.getInputLenguaje() == InputLenguajes::MariaDB)
                {
                    ofile << "\tstd::vector<" << table.getName()<< "*>* " << table.getName() << "::" << itCfTb->second->getName() << "(octetos::db::mariadb::Connector& connector,";
                }
                else if(configureProject.getInputLenguaje() == InputLenguajes::PostgreSQL)
                {
                    ofile << "\tstd::vector<" << table.getName()<< "*>* " << table.getName() << "::" << itCfTb->second->getName() << "(octetos::db::postgresql::Connector& connector,";
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
                        if((*fl).second->getOutType().compare("std::string") == 0)
                        {
                            ofile << "const std::string& " << param; 
                        }
                        else if((*fl).second->getSymbolReferenced() != NULL)
                        {
                                ofile << "const " << (*fl).second->getSymbolReferenced()->getClassParent()->getName() << "& " << param;
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
                ofile << "\t\tstd::string sqlString = \"SELECT ";
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
                                ofile << "\t\tsqlString = sqlString + \"" << param << " = \" + \"'\" + " << (*fl).second->getName() << ".get" << (*fl).second->getUpperName() << "String() + \"'\"";
                            }
                            else if((*fl).second->getOutType().compare("int") == 0)
                            {
                                ofile << "\t\tsqlString = sqlString + \"" << param << " = \" + \"'\" + std::to_string(" << (*fl).second->getName() << ") + \"'\"";
                            }
                            else if((*fl).second->getOutType().compare("std::string") == 0)
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
                        ofile << "\t\toctetos::db::mysql::Datresult dt;"  << std::endl;
                    }
                    else if(configureProject.getInputLenguaje() == InputLenguajes::MariaDB)
                    {
                        ofile << "\t\toctetos::db::mariadb::Datresult dt;"  << std::endl;
                    }
                    else if(configureProject.getInputLenguaje() == InputLenguajes::PostgreSQL)
                    {
                        ofile << "\t\toctetos::db::postgresql::Datresult dt;"  << std::endl;
                    }
                    else
                    {
                        std::string msg = "Lenguaje no soportado " ;
                        throw BuildException(msg);
                    }
					ofile << "\t\tbool flag = connector.execute(sqlString,dt);"  << std::endl;
					ofile << "\t\tif(flag)"  << std::endl;
					ofile << "\t\t{" << std::endl;
                    ofile << "\t\t\tstd::vector<"<< table.getName() << "*>* tmpVc = new std::vector<" << table.getName() << "*>;" << std::endl;
                    ofile << "\t\t\twhile(dt.nextRow()) "<< std::endl;
                    ofile << "\t\t\t{"<< std::endl;     
                    ofile << "\t\t\t\t"<< table.getName() << "* tmp = NULL;" << std::endl;             
                    ofile << "\t\t\t\ttmp = new " << table.getName() << "(";              
                    auto endK2 = table.getKey().end();
                    endK2--;
                    int count2 = 0;
                    for(auto k : table.getKey())
                    {
                        if(k->getOutType().compare("std::string") == 0)
                        {
                            ofile << "dt.getString(" << count2 << ")";
                        }
                        else if(k->getOutType().compare("int") == 0)
                        {
                            ofile << "dt.getint(" << count2 << ")";
                        }
                        else
                        {
                            ofile << "dt.getString(" << count2 << ")";               
                        }
                        if(k != *endK2)
                        {
                            ofile << ",";
                        }
                        count2++;
                    }
                    ofile << ")" << ";" << std::endl;
                    ofile << "\t\t\t\ttmpVc->push_back(tmp);" << std::endl;
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
                    ofile << "\tbool " << table.getName() << "::" << itCfTb->first << "(octetos::db::mysql::Connector& connector)"<<std::endl;
                }
                else if(configureProject.getInputLenguaje() == InputLenguajes::MariaDB)
                {
                    ofile << "\tbool " << table.getName() << "::" << itCfTb->first << "(octetos::db::mariadb::Connector& connector)"<<std::endl;
                }
                else if(configureProject.getInputLenguaje() == InputLenguajes::PostgreSQL)
                {
                    ofile << "\tbool " << table.getName() << "::" << itCfTb->first << "(octetos::db::postgresql::Connector& connector)"<<std::endl;
                }
                else
                {
                    std::string msg = "Lenguaje no soportado " ;
                    throw BuildException(msg);
                }
                
                ofile << "\t{ " << std::endl;
                ofile << "\t\tstd::string sqlString = \"SELECT ";
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
					ofile << " FROM \\\"" << table.getName() << "\\\" WHERE " ;
				}
				else
				{
					ofile << " FROM " << table.getName() << " WHERE " ;
				}
                for(auto k : table.getKey())
                {                        
					if(k->getOutType().compare("std::string") == 0)
					{
						ofile << k->getName() << " = '\" + " << k->getName() << " + \"'\"";
					}
					else
					{
						ofile << k->getName() << " = '\" + std::to_string(" << k->getName() << ") + \"'\"";
					}
					
					auto endK = table.getKey().end();
					endK--;
					if(*endK != k)
					{
						ofile << " \" and \" + ";
					}
                }
                ofile << ";" << std::endl;
                if(configureProject.getInputLenguaje() == InputLenguajes::MySQL)
                {        
                    ofile << "\t\toctetos::db::mysql::Datresult dt;"  << std::endl;
                }
                else if(configureProject.getInputLenguaje() == InputLenguajes::MariaDB)
                {
                    ofile << "\t\toctetos::db::mariadb::Datresult dt;"  << std::endl;
                }
                else if(configureProject.getInputLenguaje() == InputLenguajes::PostgreSQL)
                {
                    ofile << "\t\toctetos::db::postgresql::Datresult dt;"  << std::endl;
                }
                else
                {
                    std::string msg = "Lenguaje no soportado " ;
                                    throw BuildException(msg);
                }
                
                ofile << "\t\tbool flag = connector.execute(sqlString,dt);"  << std::endl;
                ofile << "\t\tif(flag)"  << std::endl;
                ofile << "\t\t{" << std::endl;
                ofile << ""<< std::endl;
                ofile << "\t\t\twhile (dt.nextRow()) "<< std::endl;
                ofile << "\t\t\t{"<< std::endl;
                //ofile << "\t\t\t\tfor(int i = 0; i < num_fields; i++)"<< std::endl;
                //ofile << "\t\t\t\t{"<< std::endl;
                itParamEnd = params->end();
                int countparam = 0;
                for(const std::string& param : *params)
                {
                    //ofile << param; 
                    if(param != *itParamEnd)
                    {
                        //ofile << "\t\t\t\t\tthis->" << param << " = (row[i] ? row[i] : NULL);"<< std::endl;
                        ofile << "\t\t\t\t\tthis->" << param << " = ";
                        auto fl = table.find(param.c_str());
                        if(fl != table.end())
                        {
							if((*fl).second->getClassReferenced() != NULL)
							{
								if((*fl).second->outType.compare("int"))
								{
									ofile << " new " << (*fl).second->getClassReferenced()->getName() << "(dt.getint(" << countparam << ")" << ";" << std::endl ;
								}
								else if((*fl).second->outType.compare("long") or (*fl).second->outType.compare("long int"))
								{
									ofile << " new " << (*fl).second->getClassReferenced()->getName() << "(dt.getl(" << countparam << ")" << ";" << std::endl ;
								}
								else if((*fl).second->outType.compare("std::String"))
								{
									ofile << " new " << (*fl).second->getClassReferenced()->getName() << "(dt.getString(" << countparam << ")" << ";" << std::endl ;
								}
								else
								{
									ofile << " new " << (*fl).second->getClassReferenced()->getName() << "(dt.getString(" << countparam << ")" << ";" << std::endl ;
								}
							}
							else if((*fl).second->getOutType().compare("int") == 0)
							{
								ofile << " dt.getint(" << countparam << ")" << ";"<< std::endl ;
							}
							else if((*fl).second->getOutType().compare("long") == 0)
							{
								ofile << " dt.getl(" << countparam << ")" << ";"<< std::endl ;
							}
							else if((*fl).second->getOutType().compare("std::string") == 0 || (*fl).second->getOutType().compare("const char*") == 0)
							{
								ofile << " dt.getString(" << countparam << ")" << ";" << std::endl ;
							}
							else
							{
								ofile << " dt.getString(" << countparam << ")" << ";" << std::endl ;
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
    }
    void Java::writeInsert(const apidb::symbols::Table& table,std::ofstream& ofile)	
	{        
		// Methodo insert
        ofile << "\t"<< "bool ";
        if(configureProject.getInputLenguaje() == InputLenguajes::MySQL)
        {        
            ofile <<table.getName()<< "::insert(octetos::db::mysql::Connector& connector";
        }
        else if(configureProject.getInputLenguaje() == InputLenguajes::MariaDB)
        {
            ofile <<table.getName()<< "::insert(octetos::db::mariadb::Connector& connector";
        }
        else if(configureProject.getInputLenguaje() == InputLenguajes::PostgreSQL)
        {
            ofile <<table.getName()<< "::insert(octetos::db::postgresql::Connector& connector";
        }
        else
        {
            std::string msg = "Lenguaje no soportado " ;
            throw BuildException(msg);
        }
        for(std::list<symbols::Symbol*>::const_iterator i = table.getRequired().begin(); i != table.getRequired().end(); i++)
        {
            if((*i)->getOutType().compare("int") == 0 && (*i)->isPrimaryKey() && (*i)->isAutoIncrement()) continue; //la llave no se optine como parametro
            
            if(i != table.getRequired().end())
            {
                ofile << ","; //se agrega la coma si hay un segundo parametro
            }
            
            if((*i)->getClassReferenced() == NULL)
            {
                ofile << (*i)->getOutType() << " ";
            }
            else
            {
                ofile << "const " << (*i)->getClassReferenced()->getName() << "& ";
            }
            ofile << (*i)->getName();
        }
        ofile << ")"<<std::endl;
        ofile << "\t{"<<std::endl;
        if(configureProject.getInputLenguaje() == InputLenguajes::MySQL)
        {        
            ofile << "\t\toctetos::db::mysql::Datresult dt;" << std::endl;
        }
        else if(configureProject.getInputLenguaje() == InputLenguajes::MariaDB)
        {
            ofile << "\t\toctetos::db::mariadb::Datresult dt;" << std::endl;
        }
        else if(configureProject.getInputLenguaje() == InputLenguajes::PostgreSQL)
        {
            ofile << "\t\toctetos::db::postgresql::Datresult dt;" << std::endl;
        }
        else
        {
            std::string msg = "Lenguaje no soportado " ;
            throw BuildException(msg);
        }
        ofile << "\t\t"<<"std::string sqlString = \"\";"<<std::endl;
		if(configureProject.getInputLenguaje() == InputLenguajes::PostgreSQL)
		{
			ofile << "\t\t"<<"sqlString = sqlString + \"INSERT INTO \\\"\"  + TABLE_NAME + \"\\\"\" ; "<<std::endl;
		}
		else
		{
			ofile << "\t\t"<<"sqlString = sqlString + \"INSERT INTO \" + TABLE_NAME ; "<<std::endl;
		}
        ofile << "\t\t"<<"sqlString = sqlString + \"(";
        for(std::list<symbols::Symbol*>::const_iterator i = table.getRequired().begin(); i != table.getRequired().end(); ++i)
        {
            if((*i)->getOutType().compare("int") == 0 && (*i)->isPrimaryKey() && (*i)->isAutoIncrement()) continue; //la llave no se optine como parametro
            
			ofile << (*i)->getName();
            auto penultimo = --table.getRequired().end();
			if(i != penultimo)
			{
				ofile << ","; //se agrega la coma si hay un segundo parametro
			}			
		}
		ofile << ")\";" << std::endl;
		ofile << "\t\tsqlString = sqlString + \" VALUES(\"";
        for(std::list<symbols::Symbol*>::const_iterator i = table.getRequired().begin(); i != table.getRequired().end(); ++i)
        {
            if((*i)->getOutType().compare("int") == 0 && (*i)->isPrimaryKey() && (*i)->isAutoIncrement()) continue; //la llave no se optine como parametro
            
            if((*i)->getClassReferenced() != NULL && (*i)->getOutType().compare("int") == 0)
            {
                ofile << " + \"'\" + " << (*i)->getName() << ".get" << (*i)->getClassReferenced()->getUpperName() << "String() + \"'\" ";
            }
            else if((*i)->getOutType().compare("std::string") == 0)
            {
                ofile << " + \"'\" + " << (*i)->getName() << " + \"'\" ";
            }
            else
            {
                ofile << " + std::to_string(" << (*i)->getName() << ")";
            }
            auto penultimo = --table.getRequired().end();
			if(i != penultimo)
			{
				ofile << " + \",\" ";
			}
		}
		ofile << " + \")\";"<< std::endl;
		
        if(table.getKey().size() > 0)
        {
            if(table.getKey().size() == 1 && table.getKey().at(0)->getOutType().compare("int") == 0 && table.getKey().at(0)->getClassReferenced() != NULL)
            {
                ofile << "\t\tthis->" << table.getKey().at(0)->getName() << " = new " << table.getKey().at(0)->getClassReferenced()->getName() << "(";
                ofile << " connector.insert(sqlString,dt));" << std::endl;
                ofile << "\t\tif(this->" << table.getKey().at(0)->getName() << " != NULL) return true;"<< std::endl;
                ofile << "\t\telse return false;"<< std::endl;                 
                    
            }
            else if(table.getKey().size() == 1 && table.getKey().at(0)->getOutType().compare("int") == 0)
            {
                ofile << "\t\tthis->" << table.getKey().at(0)->getName() << " = ";
                ofile << " connector.insert(sqlString,dt);" << std::endl;
                ofile << "\t\tif(this->" << table.getKey().at(0)->getName() << " > 0) return true;"<< std::endl;
                ofile << "\t\telse return false;"<< std::endl;                
            }            
            else if(table.getKey().size() > 1)
            {
                ofile << "\t\tif(connector.execute(sqlString,dt))" << std::endl;
                ofile << "\t\tif(connector.execute(sqlString,dt))" << std::endl;
                ofile << "\t\t{" << std::endl;
                for(std::list<symbols::Symbol*>::const_iterator i = table.getRequired().begin(); i != table.getRequired().end(); ++i)
                {
                    ofile << "\t\t\tthis->" << (*i)->getName()  << " = " << (*i)->getName()  << ";" << std::endl;
                }
                 ofile << "\t\t\treturn true;" << std::endl;
                ofile << "\t\t}" << std::endl;
            }
        }
        else if(table.getKey().size() == 0)
        {
            ofile << "\t\tthis->" << table.getKey().at(0)->getName() ;
            if(configureProject.getInputLenguaje() == InputLenguajes::MySQL)
            {        
                ofile << "\t\toctetos::db::mysql::Datresult dt;" << std::endl;
            }
            else if(configureProject.getInputLenguaje() == InputLenguajes::MariaDB)
            {
                ofile << "\t\toctetos::db::mariadb::Datresult dt;" << std::endl;
            }
            else if(configureProject.getInputLenguaje() == InputLenguajes::PostgreSQL)
            {
                ofile << "\t\toctetos::db::postgresql::Datresult dt;" << std::endl;
            }
            else
            {
                std::string msg = "Lenguaje no soportado " ;
                throw BuildException(msg);
            }
            ofile << " = connector.execute(sqlString,dt);"<< std::endl;
            ofile << "\t\tif(this->" << table.getKey()[0]->getName()  << " > 0) return true;"<< std::endl;
            ofile << "\t\telse return false;"<< std::endl;   
        }
        else 
        {
            
        }
        ofile << "\t\treturn false;"<<std::endl;
        ofile << "\t}"<<std::endl;
	}
	void Java::writeDefaultContructor(const apidb::symbols::Table& table,std::ofstream& ofile)
        {
		ofile <<"\t"<<table.getName()<< "::" <<table.getName()<<"()"<<std::endl;
		ofile <<"\t{"<<std::endl;
		ofile <<"\t}"<<std::endl;
	}
	void Java::writeCopyContructor(const apidb::symbols::Table& table,std::ofstream& ofile)
	{
		//constructor de copias 
		ofile << "\t" << table.getName() << "::" << table.getName() <<"(const " << table.getName() <<"& obj)"<<std::endl;
		ofile << "\t{"<<std::endl;                
        //for (auto const& [key, attr] : table)
        for(std::map<const char*,symbols::Symbol*,symbols::cmp_str>::const_iterator it = table.begin(); it != table.end(); it++)
		{
			ofile << "\t\tthis->"<< it->second->getName()  << " = obj." << it->second->getName() <<";"<<std::endl;
		}
		ofile << "\t}"<<std::endl;
	}
	void Java::writeKeyContructor(const apidb::symbols::Table& table,std::ofstream& ofile)
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
			std::string msg = "Requirio la generacion de constructor de llave, pero la tabla '";
			msg = msg + table.getName();
            msg = msg + "'  no tiene llave, revise su configuracion o so modelo de DB";
#ifndef NDEBUG
			throw BuildException(msg,__FILE__,__LINE__);
#else
            throw BuildException(msg);
#endif
		}
		ofile << "\t{" <<std::endl;
        if(table.getKey().size() > 0)//tiene key
        {
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
        }
        else 
        {
            
        }
		ofile << "\t}" <<std::endl;
	}
	void Java::createClassMethodes(const apidb::symbols::Table& table,std::ofstream& ofile)
	{
        //for (auto const& [key, attr] : table) //para cada atributo se crean las funciones de operacion get, set y update
        for(std::map<const char*,symbols::Symbol*,symbols::cmp_str>::const_iterator it = table.begin(); it != table.end(); it++)
        {                        //gets
			if((it->second->getOutType().compare("char") == 0) | (it->second->getOutType().compare("short") == 0) | (it->second->getOutType().compare("int") == 0) | (it->second->getOutType().compare("long") == 0) | (it->second->getOutType().compare("float") == 0) | (it->second->getOutType().compare("double") == 0))
			{
				if(it->second->getClassReferenced() == NULL)//si es foreing key
				{
					ofile <<"\t"<< it->second->getOutType() << " ";						
				}
				else
				{
					ofile <<"\t"<< "final " << it->second->getClassReferenced()->getName() << "& ";
				}
			}
			else
			{
				ofile <<"\t" << "final " << it->second->getOutType() <<" ";
			}
				
			ofile << table.getName() <<"::" << it->second->getGet() << " final"<< std::endl;
			ofile << "\t{"<<std::endl;	
			if((it->second->getOutType().compare("char") == 0) | (it->second->getOutType().compare("short") == 0) | (it->second->getOutType().compare("int") == 0) | (it->second->getOutType().compare("long") == 0) | (it->second->getOutType().compare("float") == 0) | (it->second->getOutType().compare("double") == 0))
			{
				if(it->second->getClassReferenced() == NULL)//si es foreing key
				{
					ofile <<"\t\treturn "<< it->second->getName() <<";"<< std::endl;						
				}
				else
				{
					ofile <<"\t\treturn *"<< it->second->getName()  <<";"<< std::endl;
				}						
			}
			else
			{
				ofile <<"\t\treturn " << it->second->getName()  <<";"<< std::endl;
			}								
			ofile << "\t}"<<std::endl;
			
            //gets foreing key                        
            if(it->second->isPrimaryKey() && !it->second->isForeignKey())
            {
                ofile <<"\t"<< it->second->getOutType() << " " << table.getName() << "::getKey" << it->second->getUpperName() << "() const"<< std::endl;
                ofile <<"\t{"<< std::endl;
                if(it->second->getOutType().compare("int") == 0)
                {
                    ofile <<"\t\t" << "return " << it->second->getName()  << ";" << std::endl;
                }
                ofile <<"\t}"<< std::endl;
            }
            else if(it->second->isForeignKey())
            {
                ofile <<"\t"<< it->second->getOutType() << " " << table.getName() << "::getKey" << it->second->getUpperName() << "() const"<< std::endl;
                ofile <<"\t{"<< std::endl;
                if(it->second->getOutType().compare("int") == 0)
                {
                    ofile <<"\t\t" << "return " << it->second->getName()  << "->getKey" << it->second->getSymbolReferenced()->getUpperName() << "();" << std::endl;
                }
                ofile <<"\t}"<< std::endl;
            }
                        
			
			//getString()		
			ofile << "\tstd::string "<< table.getName() <<"::get" << it->second->getUpperName() << "String() const "<< std::endl;
			ofile << "\t{"<< std::endl;
                        
            ofile << "\t\treturn ";
            
            if(it->second->getClassReferenced() != NULL)
            {
                                ofile <<"get" << it->second->getUpperName() << "String();";   
            }
            else
            {
                if(it->second->getOutType().compare("std::string") == 0 || it->second->getOutType().compare("const char*") == 0)
                {
                                        ofile << it->second->getName()  << ";";                    
                }
                else
                {
                                        ofile <<"std::to_string(" << it->second->getName()  << ");";
                }
            }
            ofile << std::endl;			
			ofile << "\t}"<< std::endl;
            
			//updates
            if((*it->second).isPrimaryKey()) goto postUpdatePosition; //si es una llave primary no se puede modificar
            if(configureProject.getInputLenguaje() == InputLenguajes::MySQL)
            {        
                ofile << "\tbool " << table.getName() <<"::update" << it->second->getUpperName() << "(octetos::db::mysql::Connector& connector,";
            }
            else if(configureProject.getInputLenguaje() == InputLenguajes::MariaDB)
            {
                ofile << "\tbool " << table.getName() <<"::update" << it->second->getUpperName() << "(octetos::db::mariadb::Connector& connector,";
            }
            else if(configureProject.getInputLenguaje() == InputLenguajes::PostgreSQL)
            {
                ofile << "\tbool " << table.getName() <<"::update" << it->second->getUpperName() << "(octetos::db::postgresql::Connector& connector,";
            }
            else
            {
                std::string msg = "Lenguaje no soportado " ;
                throw BuildException(msg);
            }
			if((it->second->getOutType().compare("std::string") == 0 || it->second->getOutType().compare("int") == 0) && it->second->getClassReferenced() != NULL)
            {
                ofile << "const " << it->second->getClassReferenced()->getName()  << "& " << it->second->getName() ;
            }
            else if(it->second->getOutType().compare("std::string") == 0)
            {
                ofile << "const " << it->second->getOutType() << "& " << it->second->getName() ;
            }
            else
            {
                ofile << it->second->getOutType() << " " << it->second->getName() ;
            }
			ofile <<")"<< std::endl;
			ofile << "\t{"<<std::endl;
			ofile << "\t\tstd::string sqlString = \"\";"<<std::endl;
			if(configureProject.getInputLenguaje() == InputLenguajes::PostgreSQL)
			{
				ofile << "\t\tsqlString = \"UPDATE \\\"\" + TABLE_NAME + \"\\\"\";"<<std::endl;
			}
			else
			{
				ofile << "\t\tsqlString = \"UPDATE \" + TABLE_NAME;"<<std::endl;
			}
			ofile << "\t\tsqlString = sqlString + \" SET " ;
            
                        ofile << it->second->getName()  << " = " ;
                         if( it->second->getOutType().compare("int") == 0 && it->second->getSymbolReferenced() != NULL)
                         {
                                ofile << "'\" +  std::to_string(" << it->second->getName()   << ".getKey" << it->second->getSymbolReferenced()->getUpperName() << "())+ \"'\";" << std::endl;                                    
                         }
                        else if( it->second->getOutType().compare("int") == 0 && it->second->getSymbolReferenced() == NULL)
                        {
                                ofile << "'\" +  std::to_string(" << it->second->getName()   << ")+ \"'\";" << std::endl;                            
                        }
                        else if(it->second->getOutType().compare("std::string") == 0 && it->second->getSymbolReferenced() != NULL)
                        {
                                ofile << "'\" + " << it->second->getName()  << " + \"'\";" << std::endl;
                        }
                        else if(it->second->getOutType().compare("std::string") == 0  && it->second->getSymbolReferenced() == NULL)
                        {
                                ofile << "'\" + " << it->second->getName()  << " + \"'\";" << std::endl;
                        }
                        else
                        {
                                ofile << "\" + std::to_string(" << it->second->getName()  << ");" << std::endl;
                        }
			
			ofile << "\t\tsqlString = sqlString + \" WHERE \" ";
                        if(table.getKey().size() > 0)
                        {
                                auto kEnd = table.getKey().end();
                                kEnd--;
                                for(auto k : table.getKey())
                                {
                                        if(k->getOutType().compare("int") == 0 && k->getSymbolReferenced() != NULL)
                                        {
                                                ofile << " + \"" << k->getName()  << " = \" + std::to_string(" << k->getName()  << "->getKey" << k->getSymbolReferenced()->getUpperName() << "())";
                                        }
                                        else if(k->getOutType().compare("int") == 0 && k->getSymbolReferenced() == NULL)
                                        {
                                                 ofile << " + \"" << k->getName()  << " = \" +  std::to_string(" << k->getName()  << ")";
                                        }
                                        else if(k->getOutType().compare("std::string") == 0)
                                        {
                                                ofile << " + \"" << k->getName()  << " = \" + \"'\" + " << k->getName()  <<" + \"'\" ";
                                        }
                                        else
                                        {
                                                ofile << " + \"" << k->getName()  << " = \" + std::to_string(" << k->getName()  <<") ";
                                        }                     
                                        if(k != *kEnd)
                                        {
                                                ofile << " + \" and \" ";
                                        }
                                }
                                ofile << ";" << std::endl;
                        }
                        else
                        {
                                throw BuildException("No hay soporte para table sin llave");
                        }
			
                if(configureProject.getInputLenguaje() == InputLenguajes::MySQL)
                {        
                    ofile << "\t\toctetos::db::mysql::Datresult dt56239;" << std::endl;
                }
                else if(configureProject.getInputLenguaje() == InputLenguajes::MariaDB)
                {
                    ofile << "\t\toctetos::db::mariadb::Datresult dt56239;" << std::endl;
                }
                else if(configureProject.getInputLenguaje() == InputLenguajes::PostgreSQL)
                {
                    ofile << "\t\toctetos::db::postgresql::Datresult dt56239;" << std::endl;
                }
                else
                {
                    std::string msg = "Lenguaje no soportado " ;
                    throw BuildException(msg);
                }
			ofile <<"\t\treturn connector.execute(sqlString,dt56239);"<<std::endl;
			ofile << "\t}"<<std::endl;	
            postUpdatePosition:;
                
        }
		
		writeKeyContructor(table,ofile);
		writeCopyContructor(table,ofile);
		writeDefaultContructor(table,ofile);			
		writeInsert(table,ofile);
        writeDownloads(table,ofile);
		writeSelects(table,ofile);
		ofile << std::endl; 
    }
        
    void Java::createClass(const apidb::symbols::Table& cl,std::ofstream& file,const std::string& nameClass)
    {
        if(configureProject.getInputLenguaje() == InputLenguajes::MySQL or configureProject.getInputLenguaje() == InputLenguajes::MariaDB)
        {
            file << "\tstatic String TABLE_NAME = \"`"<<  cl.getFullName() << "`\";" << std::endl;
        }
        else if(configureProject.getInputLenguaje() == InputLenguajes::PostgreSQL)
        {
            file << "\tstatic String TABLE_NAME = \""<<  cl.getFullName() << "\";" << std::endl;
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
		createClassMethodes(cl,file);        
		file<< std::endl<< std::endl;
    }
    bool Java::create(std::ofstream& file,bool log,const symbols::ISpace* ispace)
	{
		if(ispace->what() == symbols::SpaceType::TABLE)
        {
            symbols::Table* table = (symbols::Table*) ispace;
            //std::cout << "Es table " << table->getName() << std::endl;
            createClass(*table,file,table->getName());
        }
        else if(ispace->what() == symbols::SpaceType::SPACE)
        {
            symbols::Space* space = (symbols::Space*) ispace;
            //std::cout << "Es Espacio " << space->getFullName() << std::endl;
            file << "namespace " ;
            if(space->getName().empty())
            {
                file << configureProject.name;
            }
            else
            {
                file << space->getName() << std::endl;
            }
            file << "\n{\n";
            //std::cout << "Espacio '" << space->getFullName() << "'" << std::endl;
            for(symbols::Space::iterator it = space->begin(); it != space->end(); it++)
            {
                create(file,log,it->second);
            }
            file << "\n}\n";
            file << std::endl;				
        }
		
		return true;
	}
    bool Java::create(bool log,const symbols::SymbolsTable& stb)
	{
		
		for(symbols::SymbolsTable::const_iterator it = stb.begin(); it != stb.end(); it++)
		{
			symbols::ISpace* ispace = it->second;
			if(ispace->what() == symbols::SpaceType::TABLE)
            {				
                symbols::Table* table = (symbols::Table*) ispace;
				writeResult = new std::ofstream();
				filename = table->getName();
				filename +=  ".java";
				writeResult->open(filename);
                createClass(*table,*writeResult,table->getName());
				writeResult->flush();
				writeResult->close();
				delete writeResult;
            }
            else if(ispace->what() == symbols::SpaceType::SPACE)
            {
                
            }
		}
		
		return true;
	}
    
        
	
	
    bool Java::createDatconnect(std::ofstream& file,bool log)
	{
		if(!configureProject.writeDatconnect.empty() and configureProject.writeDatconnect.compare("¿?") != 0)
		{

            if(configureProject.getInputLenguaje() == InputLenguajes::MySQL)
            {        
                file << "\tstatic const octetos::db::mysql::Datconnect " << configureProject.writeDatconnect  << "(";
            }
            else if(configureProject.getInputLenguaje() == InputLenguajes::MariaDB)
            {                    
                file << "\tstatic const octetos::db::mariadb::Datconnect " << configureProject.writeDatconnect  << "(";
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
       
}
}
}
