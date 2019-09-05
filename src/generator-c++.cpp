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
		void CPP::writeSelectsCPP(const apidb::symbols::Table& table, std::ofstream& ofile)
        {
        //select(conector,where)
        ofile << "\tstd::vector<"  << table.getName() << "*>* " << table.getName() << "::select(octetos::toolkit::clientdb::mysql::Connector& connector, const std::string& where, int leng)"<<std::endl;
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
        ofile << " FROM " << table.getName() << " WHERE \" + where ;"<< std::endl;
        ofile << "\t\tif(leng > 0)"  << std::endl;
        ofile << "\t\t{"  << std::endl;
        ofile << "\t\t\tsqlString += \" LIMIT  \"  + std::to_string(leng);"  << std::endl;
        ofile << "\t\t}"  << std::endl;
        ofile << "\t\toctetos::toolkit::clientdb::Datresult* dt = connector.query(sqlString.c_str());"  << std::endl;
        ofile << "\t\tif(dt!=NULL)"  << std::endl;
        ofile << "\t\t{" << std::endl;
        ofile << "\t\t\tMYSQL_ROW row;"<< std::endl;
        ofile << "\t\t\tstd::vector<"<< table.getName() << "*>* tmpVc = new std::vector<" << table.getName() << "*>;" << std::endl;
        ofile << "\t\t\twhile((row = mysql_fetch_row((MYSQL_RES*)(dt->getResult()))))" << std::endl;
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
                ofile << "row[" << count2 << "]";
            }
            else if(k->getOutType().compare("int") == 0)
            {
                ofile << "std::stoi(row[" << count2 << "])";
            }
            else
            {
                ofile << "row[" << count2 << "]";                
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
        ofile << "\t\t\tdelete dt;" << std::endl;
        ofile << "\t\t\treturn tmpVc;" << std::endl;
        ofile << "\t\t}" << std::endl;
        ofile << "\t\tdelete dt;" << std::endl;
        ofile << "\t\treturn NULL;" << std::endl;
        
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
                ofile << "\tstd::vector<" << table.getName()<< "*>* " << table.getName() << "::select_" << itCfTb->second->getName() << "(octetos::toolkit::clientdb::mysql::Connector& connector,";

                
                const apidb::ConfigureProject::Parameters* params = itCfTb->second->getParameters();
                {
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
                }
                ofile << ", int leng)"<<std::endl;
                ofile << "\t{"<<std::endl;
                ofile << "\t\tstd::string sqlString = \"SELECT ";
                //const apidb::ConfigureProject::Parameters& params = val->getParameters();
                {
                    auto endK = table.getKey().end();
                    endK--;
                    for(auto k : table.getKey())
                    {
                        ofile << k->getName();
                        if(k != *endK)
                        {
                            ofile << " and ";
                        }
                    }
                    ofile << " FROM " << table.getName() << " WHERE \";"<< std::endl;                    
                    apidb::ConfigureProject::Parameters::const_iterator itParamEnd = params->end();
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
                            ofile << " + \",\";"<< std::endl;
                        }
                    }
                    ofile << ";" << std::endl;
					ofile << "\t\tif(leng > 0)"  << std::endl;
					ofile << "\t\t{"  << std::endl;
					ofile << "\t\t\tsqlString += \" LIMIT  \" + std::to_string(leng);"  << std::endl;
					ofile << "\t\t}"  << std::endl;
					ofile << "\t\toctetos::toolkit::clientdb::Datresult* dt = connector.query(sqlString.c_str());"  << std::endl;
					ofile << "\t\tif(dt!=NULL)"  << std::endl;
					ofile << "\t\t{" << std::endl;
                    ofile << "\t\t\tstd::vector<"<< table.getName() << "*>* tmpVc = new std::vector<" << table.getName() << "*>;" << std::endl;
                    ofile << "\t\t\tMYSQL_ROW row;"<< std::endl;
                    ofile << "\t\t\twhile((row = mysql_fetch_row((MYSQL_RES*)(dt->getResult())))) "<< std::endl;
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
                            ofile << "row[" << count2 << "]";
                        }
                        else if(k->getOutType().compare("int") == 0)
                        {
                            ofile << "std::stoi(row[" << count2 << "])";
                        }
                        else
                        {
                            ofile << "row[" << count2 << "]";                
                        }
                        if(k != *endK2)
                        {
                            ofile << " and ";
                        }
                        count2++;
                    }
                    ofile << ")" << ";" << std::endl;
                    ofile << "\t\t\t\ttmpVc->push_back(tmp);" << std::endl;
                    ofile << "\t\t\t}"<< std::endl;
                        ofile << "\t\t\tdelete dt;" << std::endl;
                    ofile << "\t\t\treturn tmpVc;"<< std::endl;
                    ofile << "\t\t}" << std::endl;
                    ofile << "\t\telse" << std::endl;
                    ofile << "\t\t{" << std::endl;
                        ofile << "\t\tdelete dt;" << std::endl;
                    ofile << "\t\t\treturn NULL;" << std::endl;
                    ofile << "\t\t}" << std::endl;
                }
                ofile << "\t}"<<std::endl;
                
            }
        }
    }
	void CPP::writeDownloadsCPP(const apidb::symbols::Table& table, std::ofstream& ofile)
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
                                ofile << "\tbool " << table.getName() << "::download_" << itCfTb->first << "(octetos::toolkit::clientdb::mysql::Connector& connector)"<<std::endl;
                                ofile << "\t{ " << std::endl;
                                ofile << "\t\tstd::string sqlString = \"SELECT ";
                                const apidb::ConfigureProject::Parameters* params = itCfTb->second->getParameters();
                                /*for(auto pr : params)
                                {
                                        std::cout << "param : " << pr << std::endl;
                                }*/
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
                                ofile << " FROM " << table.getName() << " WHERE " ;
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
                                ofile << "\t\toctetos::toolkit::clientdb::Datresult* dt = connector.query(sqlString.c_str());"  << std::endl;
                                ofile << "\t\tif(dt!=NULL)"  << std::endl;
                                ofile << "\t\t{" << std::endl;
                                ofile << "\t\t\tMYSQL_ROW row;"<< std::endl;
                                ofile << "\t\t\twhile ((row = mysql_fetch_row((MYSQL_RES*)(dt->getResult())))) "<< std::endl;
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
                                                ofile << " new " << (*fl).second->getClassReferenced()->getName() << "(row[" << countparam << "])" << ";" << std::endl ;
                                                }
                                                else if((*fl).second->getOutType().compare("int") == 0)
                                                {
                                                ofile << " std::stoi(row[" << countparam << "] ? row[" << countparam << "] : 0)" << ";"<< std::endl ;
                                                }
                                                else if((*fl).second->getOutType().compare("long") == 0)
                                                {
                                                ofile << " std::stol(row[" << countparam << "] ? row[" << countparam << "] : 0)" << ";"<< std::endl ;
                                                }
                                                else if((*fl).second->getOutType().compare("std::string") == 0 || (*fl).second->getOutType().compare("const char*") == 0)
                                                {
                                                ofile << " row[" << countparam << "] ? row[" << countparam << "] : \"NULL\"" << ";" << std::endl ;
                                                }
                                                else
                                                {
                                                ofile << " row[" << countparam << "] ? row[" << countparam << "] : \"NULL\"" << ";" << std::endl ;
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
                                ofile << "\t\t\tdelete dt;" << std::endl;
                                ofile << "\t\t\treturn true;" << std::endl;
                                ofile << "\t\t}" << std::endl;
                                ofile << "\t\telse" << std::endl;
                                ofile << "\t\t{" << std::endl;
                                ofile << "\t\t\tdelete dt;" << std::endl;
                                ofile << "\t\t\treturn false;" << std::endl;
                                ofile << "\t\t}" << std::endl;
                                }
                                ofile << "\t} " << std::endl;
                        }         
                }        
    }
    void CPP::writeInsertCPP(const apidb::symbols::Table& table,std::ofstream& ofile)	
	{        
		// Methodo insert
        ofile << "\t"<< "bool ";
        ofile <<table.getName()<< "::insert(octetos::toolkit::clientdb::mysql::Connector& connector";
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
        ofile << "\t\t"<<"std::string sqlString = \"\";"<<std::endl;
        ofile << "\t\t"<<"sqlString = sqlString + \"INSERT INTO \" + TABLE_NAME; "<<std::endl;
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
                ofile << " connector.insert(sqlString.c_str()));" << std::endl;
                ofile << "\t\tif(this->" << table.getKey().at(0)->getName() << " != NULL) return true;"<< std::endl;
                ofile << "\t\telse return false;"<< std::endl;                 
                    
            }
            else if(table.getKey().size() == 1 && table.getKey().at(0)->getOutType().compare("int") == 0)
            {
                ofile << "\t\tthis->" << table.getKey().at(0)->getName() << " = ";
                ofile << " connector.insert(sqlString.c_str());" << std::endl;
                ofile << "\t\tif(this->" << table.getKey().at(0)->getName() << " > 0) return true;"<< std::endl;
                ofile << "\t\telse return false;"<< std::endl;                
            }            
            else if(table.getKey().size() > 1)
            {
                ofile << "\t\tif(connector.query(sqlString.c_str()))" << std::endl;
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
            ofile << " = connector.query(sqlString.c_str());"<< std::endl;
            ofile << "\t\tif(this->" << table.getKey()[0]->getName()  << " > 0) return true;"<< std::endl;
            ofile << "\t\telse return false;"<< std::endl;   
        }
        else 
        {
            
        }
        ofile << "\t\treturn false;"<<std::endl;
        ofile << "\t}"<<std::endl;
	}
	void CPP::writeDefaultContructorCPP(const apidb::symbols::Table& table,std::ofstream& ofile)
        {
		ofile <<"\t"<<table.getName()<< "::" <<table.getName()<<"()"<<std::endl;
		ofile <<"\t{"<<std::endl;
		ofile <<"\t}"<<std::endl;
	}
	void CPP::writeCopyContructorCPP(const apidb::symbols::Table& table,std::ofstream& ofile)
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
	void CPP::writeKeyContructorCPP(const apidb::symbols::Table& table,std::ofstream& ofile)
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
			throw BuildException(msg);
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
	void CPP::createClassMethodesCPP(const apidb::symbols::Table& table,std::ofstream& ofile)
	{
                
                //for (auto const& [key, attr] : table) //para cada atributo se crean las funciones de operacion get, set y update
                for(std::map<const char*,symbols::Symbol*,symbols::cmp_str>::const_iterator it = table.begin(); it != table.end(); it++)
                {
                        //gets
			if((it->second->getOutType().compare("char") == 0) | (it->second->getOutType().compare("short") == 0) | (it->second->getOutType().compare("int") == 0) | (it->second->getOutType().compare("long") == 0) | (it->second->getOutType().compare("float") == 0) | (it->second->getOutType().compare("double") == 0))
			{
				if(it->second->getClassReferenced() == NULL)//si es foreing key
				{
					ofile <<"\t"<< it->second->getOutType() << " ";						
				}
				else
				{
					ofile <<"\t"<< "const " << it->second->getClassReferenced()->getName() << "& ";
				}
			}
			else
			{
				ofile <<"\t" << "const " << it->second->getOutType() <<"& ";
			}
				
			ofile << table.getName() <<"::" << it->second->getGet() << " const"<< std::endl;
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
			ofile << "\tbool " << table.getName() <<"::update" << it->second->getUpperName() << "(octetos::toolkit::clientdb::mysql::Connector& connector,";
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
			ofile << "\t\tsqlString = \"UPDATE \" + TABLE_NAME;"<<std::endl;
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
			
			ofile <<"\t\treturn connector.query(sqlString.c_str());"<<std::endl;
			ofile << "\t}"<<std::endl;	
                postUpdatePosition:;
                
                }
		
		writeKeyContructorCPP(table,ofile);
		writeCopyContructorCPP(table,ofile);
		writeDefaultContructorCPP(table,ofile);			
		writeInsertCPP(table,ofile);
                writeDownloadsCPP(table,ofile);
		writeSelectsCPP(table,ofile);
		ofile << std::endl; 
    }
        
    void CPP::createClassCPP(const apidb::symbols::Table& cl,std::ofstream& file,const std::string& nameClass)
    {
		file << "\tconst std::string " <<  nameClass << "::TABLE_NAME = \"`"<<  cl.getFullName() << "`\";" << std::endl;
		createClassMethodesCPP(cl,file);        
		file<< std::endl<< std::endl;
    }
    bool CPP::createCPP(std::ofstream& file,bool log,const symbols::ISpace* ispace)
	{
		if(configureProject.inputLenguaje == InputLenguajes::MySQL)
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
							createCPP(file,log,it->second);
					}
					file << "\n}\n";
					file << std::endl;				
			}
		}
		else
		{
			toolkit::Error::write(toolkit::Error("El lenguaje de entrada no esá soportado.",ErrorCodes::ERROR_UNNSOPORTED_INPUTLANGUAGE,__FILE__,__LINE__));
			return false;
		}
		
		return true;
	}
    bool CPP::createCPP(std::ofstream& file,bool log,const symbols::SymbolsTable& stb)
	{
		for(symbols::SymbolsTable::const_iterator it = stb.begin(); it != stb.end(); it++)
		{
			symbols::ISpace* ispace = it->second;
			if(configureProject.inputLenguaje == InputLenguajes::MySQL)
			{
				if(ispace->what() == symbols::SpaceType::TABLE)
				{					
					symbols::Table* table = (symbols::Table*) ispace;
					createClassCPP(*table,file,table->getName());
				}
				else if(ispace->what() == symbols::SpaceType::SPACE)
				{
					symbols::Space* space = (symbols::Space*) ispace;
					file << "namespace " ;
					if(space->getName().empty())
					{
							file << configureProject.name;
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
			else
			{
				toolkit::Error::write(toolkit::Error("El lenguaje de entrada no esá soportado.",ErrorCodes::ERROR_UNNSOPORTED_INPUTLANGUAGE,__FILE__,__LINE__));
				return false;
			}
		}	
		return true;
	}
    /*void CPP::createSpaceCPP(std::ofstream& file)
    {
                file <<"namespace "<< configureProject.name << std::endl;
                file <<"{"<<std::endl;
		
                const std::map<const char*,symbols::Space*,symbols::cmp_str>& spacies = analyzer.getListTableConst();
                
                //for(auto const& [keySpace, AttSpace]  : spacies)
                for(std::map<const char*,symbols::Space*,symbols::cmp_str>::const_iterator it = spacies.begin(); it != spacies.end(); it++)
                {
                       if(strcmp(it->first,"") != 0)
                        {
                                short level = symbols::getSpaceLevel(it->first);
                                for(short i = 0; i < level ; i++) file << "\t";
                                file << "namespace " << it->second->getFirstName()  << std::endl;
                                for(short i = 0; i < level ; i++) file << "\t";
                                file << "{" << std::endl;
                        }
                        //for(auto table: *(it->second)) //reading attrubtes by table
                        for(std::list<symbols::Table*>::iterator itT = it->second->begin(); itT != it->second->end(); itT++ )
                        {
                                createClassCPP(**itT,file,(*itT)->getName());
                        }
                        if(strcmp(it->first,"") != 0)
                        {
                                short level = symbols::getSpaceLevel(it->first);
                                for(short i = 0; i < level ; i++) file << "\t";
                                file << "}" << std::endl;
                        }
                }
                file <<"}"<<std::endl;
    }*/
        void CPP::writeDownloadsH(const apidb::symbols::Table& table, std::ofstream& ofile)
        {                
                //for(std::map<const char*,ConfigureProject::Table*>::const_iterator it = configureProject.downloads.begin(); it != configureProject.downloads.end(); it++)
			const ConfigureProject::Table* tb = configureProject.findSelectTable(table.getName());
			if(tb != NULL)
			{
                        for(std::map<const char*, const apidb::ConfigureProject::Function*>::const_iterator itF = tb->begin(); itF != tb->end(); itF++)
                        {
                                ofile << "\t\tbool download_" << itF->first << "(octetos::toolkit::clientdb::mysql::Connector& connector);"<<std::endl;
                        }
			}
        }
	void CPP::writeSelectsH(const apidb::symbols::Table& table, std::ofstream& ofile)
	{
			ofile << "\t\tstatic std::vector<" << table.getName() << "*>* select(octetos::toolkit::clientdb::mysql::Connector& connector,const std::string& where, int leng = 0);"<<std::endl;
                
			//std::cout << "List Select Count : " << configureProject.selects.size() << std::endl;
			//for(std::map<const char*,ConfigureProject::Table*>::const_iterator it = configureProject.selects.begin(); it != configureProject.selects.end(); it++)
			
				
                        //std::cout << " Verificando " << table.getName();
                        //std::cout << " tenga selects para : " << it->second->getName()  << std::endl;
                        /*if(table.getName().compare(it->second->getName()) != 0) 
                        {
                                continue;//buscar la configuracion de la tabla correspondiente
                        }*/
				const ConfigureProject::Table* tb = configureProject.findSelectTable(table.getName());
				if(tb != NULL) 
				{
					//std::cout << "Se encontro la tabla '" << table.getName() << std::endl;
                        for(std::map<const char*, const apidb::ConfigureProject::Function*>::const_iterator itT = tb->begin(); itT != tb->end(); itT++)
                        {
                                ofile << "\t\tstatic std::vector<" << table.getName() << "*>* select_" << itT->second->getName() << "(octetos::toolkit::clientdb::mysql::Connector& connector,";
                                //std::cout << "Generando codigo para  : " << itT->second->getName() << std::endl;
                                const apidb::ConfigureProject::Parameters* params = itT->second->getParameters();                                
                                apidb::ConfigureProject::Parameters::const_iterator itParamEnd = params->end();
                                itParamEnd--;
                                for(const std::string& param : *params)
                                {
                                        auto fl = table.find(param.c_str());
                                        if(fl != table.end())
                                        {
                                        if((*fl).second->getOutType().compare("std::string") == 0)
                                        {
                                                ofile << "const std::string& ";
                                        }
                                        else if((*fl).second->getSymbolReferenced() != NULL)
                                        {
                                                ofile << "const " << (*fl).second->getSymbolReferenced()->getClassParent()->getName() << "& ";
                                        }
                                        else
                                        {
                                                ofile << (*fl).second->getOutType() << " ";                            
                                        }
                                        }
                                        ofile << param; 
                                        if(param != *itParamEnd)
                                        {
                                        ofile << ",";
                                        }
                                }                                
                                ofile << ", int leng = 0);"<<std::endl;                                
                        }
                }
	}
        /**
        * Genera las funciones insert solo con los datos marcados como requeridos en la DB.
        */
        void CPP::writeInsertH(const apidb::symbols::Table& table,std::ofstream& ofile)
	{
		int countFIelds = 0;
		// creando insert
        ofile << "\t\t"<< "bool ";
        ofile << "insert(octetos::toolkit::clientdb::mysql::Connector& connector";
        for(std::list<symbols::Symbol*>::const_iterator i = table.getRequired().begin(); i != table.getRequired().end(); i++)
        {
            if((*i)->getOutType().compare("int") == 0 && (*i)->isPrimaryKey() && (*i)->isAutoIncrement()) continue; //la llave no se optine como parametro
            
			countFIelds++;
			if(i != table.getRequired().end())
			{
				ofile << " ,"; //se agrega la coma si hay un segundo parametro
			}
				
			//
			if((*i)->getClassReferenced() == NULL)
			{
				ofile << (*i)->getOutType() << " ";
			}
			else
			{
				ofile << "const " << (*i)->getClassReferenced()->getName() << "& ";
				//ofile << "const " << (*i)->outType <<"& ";
			}
			ofile << (*i)->getName();
		}
        ofile << ");"<<std::endl;
        
		//if(countFIelds == 0) throw BuildException(table.name + " no tiene campo requerido por lo que no se puede generar metodo insert."); 
	}
	void CPP::writeDefaultContructorH(const apidb::symbols::Table& table,std::ofstream& ofile)
	{
		ofile <<"\t\t"<<table.getName()<<"();"<<std::endl;
	}
	void CPP::writeCopyContructorH(const apidb::symbols::Table& table,std::ofstream& ofile)
	{
		//constructor de copias 
		ofile << "\t\t" << table.getName() <<"(const " << table.getName() <<"&);"<< std::endl;
	}
    void CPP::writeKeyContructorH(const apidb::symbols::Table& table,std::ofstream& ofile)
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
                if(k != *endIt)
                {
                    ofile << ",";
                }
            }
            ofile << ");"<<std::endl;
		}
		else
		{
			std::string msg = "Requirio la generacion de constructor de llave, pero la tabla '";
			msg = msg + table.getName();
            msg = msg + "'  no tiene llave, revise su configuracion o so modelo de DB";
			throw BuildException(msg);
		}
	}
	void CPP::createClassMethodesH(const apidb::symbols::Table& table,std::ofstream& ofile)
        {
		std::string insertMethode = "";
                for(std::map<const char*,symbols::Symbol*,symbols::cmp_str>::const_iterator it = table.begin(); it != table.end(); it++)
                {
			//get
			if((it->second->getOutType().compare("char") == 0) | (it->second->getOutType().compare("short") == 0) | (it->second->getOutType().compare("int") == 0) | (it->second->getOutType().compare("long") == 0) | (it->second->getOutType().compare("float") == 0) | (it->second->getOutType().compare("double") == 0))
			{
				if(it->second->getClassReferenced() == NULL)//si es foreing key
				{
					ofile <<"\t\t"<< it->second->getOutType() << " ";						
				}
				else
				{
					ofile <<"\t\t"<< "const " << it->second->getClassReferenced()->getName() << "& ";
				}
			}
			else
			{
				ofile <<"\t\t" << "const " << it->second->getOutType() <<"& ";
			}		
			ofile << it->second->getGet() << " const;"<< std::endl;
			
                        
                        //get key
                         if(it->second->isPrimaryKey() && !it->second->isForeignKey())
                        {
                                ofile <<"\t\t"<< it->second->getOutType() << " getKey" << it->second->getUpperName() << "() const;"<< std::endl;
                        }
                        else if(it->second->isForeignKey())
                        {
                                ofile <<"\t\t"<< it->second->getOutType() << " getKey" << it->second->getUpperName() << "() const;"<< std::endl;
                        }
			
			//getString()			
			ofile << "\t\tstd::string get" << it->second->getUpperName() << "String() const;"<< std::endl;		
			//update
			ofile << "\t\tbool " << "update" << it->second->getUpperName() << "(octetos::toolkit::clientdb::mysql::Connector& connector,";
			if(it->second->getClassReferenced() != 0)
                        {
                                ofile << " const " << it->second->getClassReferenced()->getName() << "& " << it->second->getName();
                        }
                        else if((it->second->getOutType().compare("std::string") == 0))
                        {
                                ofile << "const std::string& " << it->second->getName();
                        }
                        else if((it->second->getOutType().compare("int") == 0) | (it->second->getOutType().compare("long") == 0))
			{
				ofile << it->second->getOutType() << " " << it->second->getName();						
			}
			else
			{
				ofile << it->second->getOutType() << " " << it->second->getName();
			}
			ofile << ");"<< std::endl;            
                }  
                 
		writeKeyContructorH(table,ofile);		
		writeCopyContructorH(table,ofile);
		writeDefaultContructorH(table,ofile);
		writeInsertH(table,ofile);	
                writeSelectsH(table,ofile);
                writeDownloadsH(table,ofile);
    }
    void CPP::createClassAttributesH(const apidb::symbols::Table& table,std::ofstream& ofile)
    {
                for(std::map<const char*,symbols::Symbol*,symbols::cmp_str>::const_iterator it = table.begin(); it != table.end(); it++)
                {
                        if(it->second->getClassReferenced() != NULL && (it->second->getOutType().compare("int") == 0 || it->second->getOutType().compare("std::string") == 0))
                        {
                                ofile << "\t\t" << it->second->getClassReferenced()->getName() << "* "<< it->second->getName()<<";"<< std::endl;
                        }
                        else
                        {
                                //ofile <<"[3]"<<std::endl;
                                ofile << "\t\t" << it->second->getOutType() << " " << it->second->getName() <<";"<< std::endl;
                        }
                }
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
		if(log)analyzer.getOutput().add(msg1);
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
		if(configureProject.inputLenguaje == InputLenguajes::MySQL)
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
				if(space->getName().empty())
				{
					file << configureProject.name;
				}
				else
				{
					file << space->getName() << std::endl;
				}
				file << "{\n";
				//std::cout << "Espacio '" << space->getFullName() << "'" << std::endl;
				for(symbols::Space::iterator it = space->begin(); it != space->end(); it++)
				{
					createH(file,log,it->second);
				}
				file << "\n}\n";
			}
		}
		else
		{
			toolkit::Error::write(toolkit::Error("El lenguaje de entrada no esá soportado.",ErrorCodes::ERROR_UNNSOPORTED_INPUTLANGUAGE,__FILE__,__LINE__));
			return false;
		}
		
		return true;
	}
    bool CPP::createDatconnectHPP(std::ofstream& file,bool log)
	{
		if(!configureProject.writeDatconnect.empty() and configureProject.writeDatconnect.compare("¿?") != 0)
		{
			if(configureProject.inputLenguaje == InputLenguajes::MySQL)
			{
				file << "\tstatic const octetos::toolkit::clientdb::mysql::Datconnect " << configureProject.writeDatconnect  << "(";
			}
			else
			{
				return false;
			}
			file << "\"" << configureProject.conectordb->getHost() << "\",";
			file << configureProject.conectordb->getPort() << ",";
			file << "\"" << configureProject.conectordb->getDatabase() << "\",";
			file << "\"" << configureProject.conectordb->getUser() << "\",";
			file << "\"" << configureProject.conectordb->getPassword() << "\"";
			file << ");\n";
		}
		
		return true;
	}
    bool CPP::createH(std::ofstream& file,bool log,const symbols::SymbolsTable& stb)
	{
		for(symbols::SymbolsTable::const_iterator it = stb.begin(); it != stb.end(); it++)
		{			
			symbols::ISpace* ispace = it->second;
			if(configureProject.inputLenguaje == InputLenguajes::MySQL)
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
					if(space->getName().empty())
					{
							file << configureProject.name;
					}
					else
					{
							file << space->getName() << std::endl;
					}
					file << "{\n";
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
			else
			{
				toolkit::Error::write(toolkit::Error("El lenguaje de entrada no esá soportado.",ErrorCodes::ERROR_UNNSOPORTED_INPUTLANGUAGE,__FILE__,__LINE__));
				return false;
			}
		}	
		return true;
	}    
}
}
}
