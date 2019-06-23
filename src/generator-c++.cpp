/**
 * 
 *  This file is part of apidb.
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
 *  author: Azael Reyes
 * */

#include <list>
#include <vector>
#include <iostream>
#include <fstream>
#include <map>


#include "analyzer.hpp"
#include "generator.hpp"


namespace apidb
{
namespace generators
{
	void CPP::writeSelectsCPP(const apidb::symbols::Table& table, std::ofstream& ofile)
        {        
        //select(conector,where)
        ofile << "\tstd::vector<"  << table.name << "*>* " << table.name << "::select(octetos::toolkit::clientdb::mysql::Connector& connector, const std::string& where)"<<std::endl;
        ofile << "\t{" <<std::endl;
        ofile << "\t\tstd::string sqlString = \"SELECT ";
        //selecciona los campos de las llaves
        auto endK = table.key.end();
        endK--;
        for(auto k : table.key)
        {
            ofile << k->name;
            if(k != *endK)
            {
                ofile << ",";
            }
        }
        ofile << " FROM " << table.name << " WHERE \" + where ;"<< std::endl;
        ofile << "\t\tif(connector.query(sqlString.c_str()))"  << std::endl;
        ofile << "\t\t{" << std::endl;
        ofile << "\t\t\tMYSQL_RES *result = mysql_store_result((MYSQL*)connector.getServerConnector());" << std::endl;
        ofile << "\t\t\tif (result == NULL)"  << std::endl;
        ofile << "\t\t\t{"  << std::endl;
        ofile << "\t\t\t\t//throw octetos::toolkit::clientdb::SQLException(\"La descarga de los datos fallo con la consulta '\" + sqlString + \"'\");"<< std::endl;
        ofile << "\t\t\t\t return NULL;"<< std::endl;
        ofile << "\t\t\t}"  << std::endl;
        ofile << "\t\t\tMYSQL_ROW row;"<< std::endl;
        ofile << "\t\t\tstd::vector<"<< table.name << "*>* tmpVc = new std::vector<" << table.name << "*>;" << std::endl;
        ofile << "\t\t\twhile((row = mysql_fetch_row(result))) " << std::endl;
        ofile << "\t\t\t{"<< std::endl;
        ofile << "\t\t\t\t"<< table.name << "* tmp = NULL;" << std::endl;            
        ofile << "\t\t\t\ttmp = new " << table.name << "(";
        auto endK2 = table.key.end();
        endK2--;
        int count2 = 0;
        for(auto k : table.key)
        {
            if(k->outType.compare("std::string") == 0)
            {
                ofile << "row[" << count2 << "]";
            }
            else if(k->outType.compare("int") == 0)
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
        ofile << "\t\t\treturn tmpVc;" << std::endl;
        ofile << "\t\t}" << std::endl;
        ofile << "\t\treturn NULL;" << std::endl;
        
        ofile << "\t}" <<std::endl;
        
        
        //select from config
        std::vector<apidb::ConfigureProject::Table> tbs = configureProject.selects;
        for( auto tb: tbs)//std::vector<Table>
        {
            if(table.name.compare(tb.getName()) != 0) 
            {
                continue;//buscar la configuracion de la tabla correspondiente
            }
            
            for (auto const& [key, val] : tb)//class Table : public std::map<std::string,Function>
            {
                ofile << "\tstd::vector<" << table.name<< "*>* " << table.name << "::select(octetos::toolkit::clientdb::mysql::Connector& connector,";
                
                const apidb::ConfigureProject::Parameters& params = val->getParameters();
                {
                    apidb::ConfigureProject::Parameters::const_iterator itParamEnd = params.end();
                    itParamEnd--;
                    for(const char* param : params)
                    {
                        auto fl = table.find(param);
                        if(fl == table.end())
                        {
                            std::string strmsg = "No se encontro el campo '";
                            strmsg = strmsg + param + "' en la tabla '" + table.name + "'";
                            throw BuildException(strmsg);
                        }
                        if((*fl).second->outType.compare("std::string") == 0)
                        {
                            ofile << "const std::string& " << param; 
                        }
                        else if((*fl).second->symbolReferenced != NULL)
                        {
                                ofile << "const " << (*fl).second->symbolReferenced->classParent->name << "& " << param;
                        }
                        else
                        {
                            ofile << (*fl).second->outType << " " << param;
                        }
                        if(param != *itParamEnd)
                        {
                            ofile << ",";
                        }
                    }
                }
                ofile << ")"<<std::endl;
                ofile << "\t{"<<std::endl;
                ofile << "\t\tstd::string sqlString = \"SELECT ";
                //const apidb::ConfigureProject::Parameters& params = val->getParameters();
                {
                    auto endK = table.key.end();
                    endK--;
                    for(auto k : table.key)
                    {
                        ofile << k->name;
                        if(k != *endK)
                        {
                            ofile << ",";
                        }
                    }
                    ofile << " FROM " << table.name << " WHERE \";"<< std::endl;                    
                    apidb::ConfigureProject::Parameters::const_iterator itParamEnd = params.end();
                    --itParamEnd;
                    for(const char* param : params)
                    {
                        auto fl = table.find(param);
                        if(fl != table.end())
                        {
                            if((*fl).second->classReferenced != NULL && (*fl).second->outType.compare("int") == 0)
                            {
                                ofile << "\t\tsqlString = sqlString + \"" << param << " = \" + \"'\" + " << (*fl).second->name << ".get" << (*fl).second->upperName << "String() + \"'\"";
                            }
                            else if((*fl).second->outType.compare("int") == 0)
                            {
                                ofile << "\t\tsqlString = sqlString + \"" << param << " = \" + \"'\" + std::stoi(" << (*fl).second->name << ") + \"'\"";
                            }
                            else if((*fl).second->outType.compare("std::string") == 0)
                            {
                                ofile << "\t\tsqlString = sqlString + \"" << param << " = \" + \"'\" + " << (*fl).second->name << " + \"'\"";
                            }
                            else
                            {
                                std::string strmsg = "No se encontro el campo ";
                                strmsg = strmsg + "'" + (*fl).second->name + "' en la tabla '" + table.name + "'";
                                throw BuildException(strmsg);                                
                            }
                        }
                        else
                        {
                                std::string strmsg = "No se encontro el campo ";
                                strmsg = strmsg + "'" + param + "' en la tabla '" + table.name + "'";
                                throw BuildException(strmsg);
                        }
                        if(param != *itParamEnd)
                        {
                            ofile << " + \",\";"<< std::endl;
                        }
                    }
                    ofile << ";" << std::endl;
                    ofile << "\t\tif(connector.query(sqlString.c_str()))"  << std::endl;
                    ofile << "\t\t{" << std::endl;
                    ofile << "\t\t\tMYSQL_RES *result = mysql_store_result((MYSQL*)connector.getServerConnector());" << std::endl;
                    ofile << "\t\t\tif (result == NULL)"  << std::endl;
                    ofile << "\t\t\t{"  << std::endl;
                    ofile << "\t\t\t\t//throw octetos::toolkit::clientdb::SQLException(\"La descarga de los datos fallo con la consulta '\" + sqlString + \"'\");"<< std::endl;
                    ofile << "\t\t\t\treturn NULL;"<< std::endl;
                    ofile << "\t\t\t}"  << std::endl;
                    ofile << "\t\t\tstd::vector<"<< table.name << "*>* tmpVc = new std::vector<" << table.name << "*>;" << std::endl;
                    ofile << "\t\t\tMYSQL_ROW row;"<< std::endl;
                    ofile << "\t\t\twhile((row = mysql_fetch_row(result))) "<< std::endl;
                    ofile << "\t\t\t{"<< std::endl;     
                    ofile << "\t\t\t\t"<< table.name << "* tmp = NULL;" << std::endl;             
                    ofile << "\t\t\t\ttmp = new " << table.name << "(";              
                    auto endK2 = table.key.end();
                    endK2--;
                    int count2 = 0;
                    for(auto k : table.key)
                    {
                        if(k->outType.compare("std::string") == 0)
                        {
                            ofile << "row[" << count2 << "]";
                        }
                        else if(k->outType.compare("int") == 0)
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
                    ofile << "\t\t\treturn tmpVc;"<< std::endl;
                    ofile << "\t\t}" << std::endl;
                    ofile << "\t\telse" << std::endl;
                    ofile << "\t\t{" << std::endl;
                    ofile << "\t\t\treturn NULL;" << std::endl;
                    ofile << "\t\t}" << std::endl;
                }
                ofile << "\t}"<<std::endl;
                
            }
        }
    }
	void CPP::writeDownloadsCPP(const apidb::symbols::Table& table, std::ofstream& ofile)
    {        
        std::vector<apidb::ConfigureProject::Table> tbs = configureProject.downloads;
        for( auto tb: tbs)//std::vector<Table>
        {
            if(table.name.compare(tb.getName()) != 0) 
            {                
                continue;//buscar la configuracion de la tabla correspondiente
            }
            
            for (auto const& [key, val] : tb)//class Table : public std::map<const char*, const Function*>
            {
                ofile << "\tbool " << table.name << "::download_" << key << "(octetos::toolkit::clientdb::mysql::Connector& connector)"<<std::endl;
                ofile << "\t{ " << std::endl;
                ofile << "\t\tstd::string sqlString = \"SELECT ";
                const apidb::ConfigureProject::Parameters& params = val->getParameters();
                {
                    apidb::ConfigureProject::Parameters::const_iterator itParamEnd = params.end();
                    itParamEnd--;
                    for(const char* param : params)
                    {
                        ofile << param; 
                        if(param != *itParamEnd)
                        {
                            ofile << ",";
                        }
                    }                    
                    ofile << " FROM " << table.name << " WHERE " ;
                    for(auto k : table.key)
                    {                        
                        if(k->outType.compare("std::string") == 0)
                        {
                            ofile << k->name << " = '\" + " << k->name << " + \"'\"";
                        }
                        else
                        {
                            ofile << k->name << " = '\" + std::to_string(" << k->name << ") + \"'\"";
                        }
                        auto endK = table.key.end();
                        endK--;
                        if(*endK != k)
                        {
                            ofile << " \" and \" + ";
                        }
                    }
                    ofile << ";" << std::endl;
                    ofile << "\t\tif(connector.query(sqlString.c_str()))"  << std::endl;
                    ofile << "\t\t{" << std::endl;
                    ofile << "\t\t\tMYSQL_RES *result = mysql_store_result((MYSQL*)connector.getServerConnector());" << std::endl;
                    ofile << "\t\t\tif (result == NULL)"  << std::endl;
                    ofile << "\t\t\t{"  << std::endl;
                    ofile << "\t\t\t\treturn false;"  << std::endl;
                    ofile << "\t\t\t}"  << std::endl;
                    //ofile << "\t\t\tint num_fields = mysql_num_fields(result);"<< std::endl;
                    ofile << "\t\t\tMYSQL_ROW row;"<< std::endl;
                    ofile << "\t\t\twhile ((row = mysql_fetch_row(result))) "<< std::endl;
                    ofile << "\t\t\t{"<< std::endl;
                    //ofile << "\t\t\t\tfor(int i = 0; i < num_fields; i++)"<< std::endl;
                    ofile << "\t\t\t\t{"<< std::endl;
                    itParamEnd = params.end();
                    int countparam = 0;
                    for(const char* param : params)
                    {
                        //ofile << param; 
                        if(param != *itParamEnd)
                        {
                            //ofile << "\t\t\t\t\tthis->" << param << " = (row[i] ? row[i] : NULL);"<< std::endl;
                            ofile << "\t\t\t\t\tthis->" << param << " = ";
                            auto fl = table.find(param);
                            if(fl != table.end())
                            {
                                if((*fl).second->classReferenced != NULL)
                                {
                                    ofile << " new " << (*fl).second->classReferenced->name << "(row[" << countparam << "])" << ";" << std::endl ;
                                }
                                else if((*fl).second->outType.compare("int") == 0)
                                {
                                    ofile << " std::stoi(row[" << countparam << "] ? row[" << countparam << "] : 0)" << ";"<< std::endl ;
                                }
                                else if((*fl).second->outType.compare("long") == 0)
                                {
                                    ofile << " std::stol(row[" << countparam << "] ? row[" << countparam << "] : 0)" << ";"<< std::endl ;
                                }
                                else if((*fl).second->outType.compare("std::string") == 0 || (*fl).second->outType.compare("const char*") == 0)
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
                                strmsg = strmsg + "'" + param + "' en la tabla '" + table.name + "'";
                                throw BuildException(strmsg);
                            }
                        }
                        countparam++;
                    }
                    //ofile << "\t\t\t;"<< std::endl;
                    ofile << "\t\t\t\t}"<< std::endl;
                    ofile << "\t\t\t}"<< std::endl;;
                    ofile << "\t\t\tmysql_free_result(result);" << std::endl;
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
    void CPP::writeInsertCPP(const apidb::symbols::Table& table,std::ofstream& ofile)	
	{        
		// Methodo insert
        ofile << "\t"<< "bool ";
        ofile <<table.name<< "::insert(octetos::toolkit::clientdb::mysql::Connector& connector";
        for(std::list<symbols::Symbol*>::const_iterator i = table.required.begin(); i != table.required.end(); i++)
        {
            if((*i)->outType.compare("int") == 0 && (*i)->isPrimaryKey() && (*i)->isAutoIncrement()) continue; //la llave no se optine como parametro
            
            if(i != table.required.end())
            {
                ofile << ","; //se agrega la coma si hay un segundo parametro
            }
            
            if((*i)->classReferenced == NULL)
            {
                ofile << (*i)->outType << " ";
            }
            else
            {
                ofile << "const " << (*i)->classReferenced->name << "& ";
            }
            ofile << (*i)->name;
        }
        ofile << ")"<<std::endl;
        ofile << "\t{"<<std::endl;
        ofile << "\t\t"<<"std::string sqlString = \"\";"<<std::endl;
        ofile << "\t\t"<<"sqlString = sqlString + \"INSERT INTO \" + TABLE_NAME; "<<std::endl;
        ofile << "\t\t"<<"sqlString = sqlString + \"(";
        for(std::list<symbols::Symbol*>::const_iterator i = table.required.begin(); i != table.required.end(); ++i)
        {
            if((*i)->outType.compare("int") == 0 && (*i)->isPrimaryKey() && (*i)->isAutoIncrement()) continue; //la llave no se optine como parametro
            
			ofile << (*i)->name;
            auto penultimo = --table.required.end();
			if(i != penultimo)
			{
				ofile << ","; //se agrega la coma si hay un segundo parametro
			}			
		}
		ofile << ")\";" << std::endl;
		ofile << "\t\tsqlString = sqlString + \" VALUES(\"";
        for(std::list<symbols::Symbol*>::const_iterator i = table.required.begin(); i != table.required.end(); ++i)
        {
            if((*i)->outType.compare("int") == 0 && (*i)->isPrimaryKey() && (*i)->isAutoIncrement()) continue; //la llave no se optine como parametro
            
            if((*i)->classReferenced != NULL && (*i)->outType.compare("int") == 0)
            {
                ofile << " + \"'\" + " << (*i)->name << ".get" << (*i)->symbolReferenced->upperName << "String() + \"'\" ";
            }
            else if((*i)->outType.compare("std::string") == 0)
            {
                ofile << " + \"'\" + " << (*i)->name << " + \"'\" ";
            }
            else
            {
                ofile << " + std::to_string(" << (*i)->name << ")";
            }
            auto penultimo = --table.required.end();
			if(i != penultimo)
			{
				ofile << " + \",\" ";
			}
		}
		ofile << " + \")\";"<< std::endl;
		
        if(table.key.size() > 0)
        {
            if(table.key.size() == 1 && table.key.at(0)->outType.compare("int") == 0 && table.key.at(0)->classReferenced != NULL)
            {
                ofile << "\t\tthis->" << table.key.at(0)->name << " = new " << table.key.at(0)->classReferenced->name << "(";
                ofile << " connector.insert(sqlString.c_str()));" << std::endl;
                ofile << "\t\tif(this->" << table.key.at(0)->name << " != NULL) return true;"<< std::endl;
                ofile << "\t\telse return false;"<< std::endl;                 
                    
            }
            else if(table.key.size() == 1 && table.key.at(0)->outType.compare("int") == 0)
            {
                ofile << "\t\tthis->" << table.key.at(0)->name << " = ";
                ofile << " connector.insert(sqlString.c_str());" << std::endl;
                ofile << "\t\tif(this->" << table.key.at(0)->name << " > 0) return true;"<< std::endl;
                ofile << "\t\telse return false;"<< std::endl;                
            }            
            else if(table.key.size() > 1)
            {
                ofile << "\t\tif(connector.query(sqlString.c_str()))" << std::endl;
                ofile << "\t\t{" << std::endl;
                for(std::list<symbols::Symbol*>::const_iterator i = table.required.begin(); i != table.required.end(); ++i)
                {
                    ofile << "\t\t\tthis->" << (*i)->name << " = " << (*i)->name << ";" << std::endl;
                }
                 ofile << "\t\t\treturn true;" << std::endl;
                ofile << "\t\t}" << std::endl;
            }
        }
        else if(table.key.size() == 0)
        {
            ofile << "\t\tthis->" << table.key.at(0)->name;
            ofile << " = connector.query(sqlString.c_str());"<< std::endl;
            ofile << "\t\tif(this->" << table.key[0]->name << " > 0) return true;"<< std::endl;
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
		ofile <<"\t"<<table.name<< "::" <<table.name<<"()"<<std::endl;
		ofile <<"\t{"<<std::endl;
		ofile <<"\t}"<<std::endl;
	}
	void CPP::writeCopyContructorCPP(const apidb::symbols::Table& table,std::ofstream& ofile)
	{
		//constructor de copias 
		ofile << "\t" << table.name << "::" << table.name <<"(const " << table.name <<"& obj)"<<std::endl;
		ofile << "\t{"<<std::endl;
                for (auto const& [key, attr] : table)
		{
			ofile << "\t\tthis->"<< attr->name << " = obj." << attr->name<<";"<<std::endl;
		}
		ofile << "\t}"<<std::endl;
	}
	void CPP::writeKeyContructorCPP(const apidb::symbols::Table& table,std::ofstream& ofile)
	{
		//constructor que toma key como parametro
        if(table.key.size() > 0)//tiene key
        {
            ofile << "\t" <<table.name << "::" << table.name << "(";
            auto endIt = table.key.end();
            endIt--;
            for(auto k : table.key)
            {
                if(k->outType.compare("std::string") == 0)
                {
                    ofile << "const " << k->outType << "& " << k->name;
                }
                else
                {
                    ofile << k->outType << " " << k->name;                    
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
			msg = msg + table.name;
            msg = msg + "'  no tiene llave, revise su configuracion o so modelo de DB";
			throw BuildException(msg);
		}
		ofile << "\t{" <<std::endl;
        if(table.key.size() > 0)//tiene key
        {
            for(auto k : table.key)
            {
                if(k->classReferenced != NULL)
                {
                    ofile << "\t\tthis->" << k->name << " = new " << k->classReferenced->name << "(" << k->name << ");" << std::endl;
                }
                else
                {
                    ofile << "\t\tthis->" << k->name << " = " << k->name << ";" << std::endl;
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
                for (auto const& [key, attr] : table) //para cada atributo se crean las funciones de operacion get, set y update
                {
                        //gets
			if((attr->outType.compare("char") == 0) | (attr->outType.compare("short") == 0) | (attr->outType.compare("int") == 0) | (attr->outType.compare("long") == 0) | (attr->outType.compare("float") == 0) | (attr->outType.compare("double") == 0))
			{
				if(attr->classReferenced == NULL)//si es foreing key
				{
					ofile <<"\t"<< attr->outType << " ";						
				}
				else
				{
					ofile <<"\t"<< "const " << attr->classReferenced->name << "& ";
				}
			}
			else
			{
				ofile <<"\t" << "const " << attr->outType <<"& ";
			}
				
			ofile << table.name <<"::" << attr->get << " const"<< std::endl;
			ofile << "\t{"<<std::endl;	
			if((attr->outType.compare("char") == 0) | (attr->outType.compare("short") == 0) | (attr->outType.compare("int") == 0) | (attr->outType.compare("long") == 0) | (attr->outType.compare("float") == 0) | (attr->outType.compare("double") == 0))
			{
				if(attr->classReferenced == NULL)//si es foreing key
				{
					ofile <<"\t\treturn "<< attr->name<<";"<< std::endl;						
				}
				else
				{
					ofile <<"\t\treturn *"<< attr->name <<";"<< std::endl;
				}						
			}
			else
			{
				ofile <<"\t\treturn " << attr->name <<";"<< std::endl;
			}								
			ofile << "\t}"<<std::endl;
			
                        
                        
                        
                        //gets foreing key                        
                         if(attr->isPrimaryKey() && !attr->isForeignKey())
                        {
                                ofile <<"\t"<< attr->outType << " " << table.name << "::getKey" << attr->upperName << "() const"<< std::endl;
                                ofile <<"\t{"<< std::endl;
                                if(attr->outType.compare("int") == 0)
                                {
                                        ofile <<"\t\t" << "return " << attr->name << ";" << std::endl;
                                }
                                ofile <<"\t}"<< std::endl;
                        }
                        else if(attr->isForeignKey())
                        {
                                ofile <<"\t"<< attr->outType << " " << table.name << "::getKey" << attr->upperName << "() const"<< std::endl;
                                ofile <<"\t{"<< std::endl;
                                if(attr->outType.compare("int") == 0)
                                {
                                        ofile <<"\t\t" << "return " << attr->name << "->getKey" << attr->symbolReferenced->upperName << "();" << std::endl;
                                }
                                ofile <<"\t}"<< std::endl;
                        }                     
                        
			
			//getString()		
			ofile << "\tstd::string "<< table.name <<"::get" << attr->upperName << "String() const "<< std::endl;
			ofile << "\t{"<< std::endl;
                        
                        ofile << "\t\treturn ";
            
                        if(attr->classReferenced != NULL)
                        {
                                ofile <<"get" << attr->upperName << "String();";   
                        }
                        else
                        {
                                if(attr->outType.compare("std::string") == 0 || attr->outType.compare("const char*") == 0)
                                {
                                ofile << attr->name << ";";                    
                                }
                                else
                                {
                                ofile <<"std::to_string(" << attr->name << ");";
                                }
                        }
                        ofile << std::endl;			
			ofile << "\t}"<< std::endl;
            
			//updates
                        if((*attr).isPrimaryKey()) goto postUpdatePosition; //si es una llave primary no se puede modificar
			ofile << "\tbool " << table.name <<"::update" << attr->upperName << "(octetos::toolkit::clientdb::mysql::Connector& connector,";
			if((attr->outType.compare("std::string") == 0 || attr->outType.compare("int") == 0) && attr->classReferenced != NULL)
                        {
                                ofile << "const " << attr->classReferenced->name << "& " << attr->name;
                        }
                        else if(attr->outType.compare("std::string") == 0)
                        {
                                ofile << "const " << attr->outType << "& " << attr->name;
                        }
                        else
                        {
                                ofile << attr->outType << " " << attr->name;
                        }
			ofile <<")"<< std::endl;
			ofile << "\t{"<<std::endl;
			ofile << "\t\tstd::string sqlString = \"\";"<<std::endl;
			ofile << "\t\tsqlString = \"UPDATE \" + TABLE_NAME;"<<std::endl;
			ofile << "\t\tsqlString = sqlString + \" SET " ;
            
                        ofile << attr->name << " = " ;
                         if( attr->outType.compare("int") == 0 && attr->symbolReferenced != NULL)
                         {
                                ofile << "'\" +  std::to_string(" << attr->name  << ".getKey" << attr->symbolReferenced->upperName << "())+ \"'\";" << std::endl;                                    
                         }
                        else if( attr->outType.compare("int") == 0 && attr->symbolReferenced == NULL)
                        {
                                ofile << "'\" +  std::to_string(" << attr->name  << ")+ \"'\";" << std::endl;                            
                        }
                        else if(attr->outType.compare("std::string") == 0 && attr->symbolReferenced != NULL)
                        {
                                ofile << "'\" + " << attr->name << " + \"'\";" << std::endl;
                        }
                        else if(attr->outType.compare("std::string") == 0  && attr->symbolReferenced == NULL)
                        {
                                ofile << "'\" + " << attr->name << " + \"'\";" << std::endl;
                        }
                        else
                        {
                                ofile << "\" + std::to_string(" << attr->name << ");" << std::endl;
                        }
			
			ofile << "\t\tsqlString = sqlString + \" WHERE \" ";
                        if(table.key.size() > 0)
                        {
                                auto kEnd = table.key.end();
                                kEnd--;
                                for(auto k : table.key)
                                {
                                        if(k->outType.compare("int") == 0 && k->symbolReferenced != NULL)
                                        {
                                                ofile << " + \"" << k->name << " = \" + std::to_string(" << k->name << "->getKey" << k->symbolReferenced->upperName << "())";
                                        }
                                        else if(k->outType.compare("int") == 0 && k->symbolReferenced == NULL)
                                        {
                                                 ofile << " + \"" << k->name << " = \" +  std::to_string(" << k->name << ")";
                                        }
                                        else if(k->outType.compare("std::string") == 0)
                                        {
                                                ofile << " + \"" << k->name << " = \" + \"'\" + " << k->name <<" + \"'\" ";
                                        }
                                        else
                                        {
                                                ofile << " + \"" << k->name << " = \" + std::to_string(" << k->name <<") ";
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
		file << "\tconst std::string " <<  nameClass << "::TABLE_NAME = \""<<  nameClass << "\";" << std::endl;
		createClassMethodesCPP(cl,file);        
		file<< std::endl<< std::endl;
    }
    void CPP::createSpaceCPP(std::ofstream& file)
    {
		if(configureProject.mvc == apidb::MVC::NO)
		{
			file <<"namespace "<< analyzer.getNameProject() << std::endl;
			file <<"{"<<std::endl;
		}
		else 
		{
			file <<"namespace "<< analyzer.getNameProject() << std::endl;
			file <<"{" <<std::endl;
			file <<"namespace  controller" <<std::endl;
			file <<"{" <<std::endl;
		}
                std::map<const char*,symbols::Tables*,symbols::cmp_str>& spacies = analyzer.getListTable();
                
                for(auto const& [keySpace, AttSpace]  : spacies)
                {
                        for(auto table: *AttSpace) //reading attrubtes by table
                        {
                                for (auto const& [key, attribute] : *table)
                                {
                                        createClassCPP(*table,file,table->name);       
                                }
                        }
                }
		if(configureProject.mvc == apidb::MVC::NO)
		{
			file <<"}"<<std::endl;
		}
		else 
		{
			file <<"}" <<std::endl;
			file <<"}" <<std::endl;
		}
    }
    void CPP::writeDownloadsH(const apidb::symbols::Table& table, std::ofstream& ofile)
    {                
        std::vector<apidb::ConfigureProject::Table> tbs = configureProject.downloads;
        for( auto tb: tbs)//std::vector<Table>
        {
            //std::cout<<"Iterate on '" << tb.getName() << "'" << std::endl;
            if(table.name.compare(tb.getName()) != 0) 
            {          
                continue;//buscar la configuracion de la tabla correspondiente
            }            
            for (auto const& [key, val] : tb)//class Table : public std::map<std::string,Function>
            {
                ofile << "\t\tbool download_" << key << "(octetos::toolkit::clientdb::mysql::Connector& connector);"<<std::endl;
            }         
        }
    }
	void CPP::writeSelectsH(const apidb::symbols::Table& table, std::ofstream& ofile)
    {
        ofile << "\t\tstatic std::vector<" << table.name << "*>* select(octetos::toolkit::clientdb::mysql::Connector& connector,const std::string& where);"<<std::endl;
        
        std::vector<apidb::ConfigureProject::Table> tbs = configureProject.selects;
        for( auto tb: tbs)//std::vector<Table>
        {
            if(table.name.compare(tb.getName()) != 0) 
            {
                continue;//buscar la configuracion de la tabla correspondiente
            }
            for (auto const& [key, val] : tb)//class Table : public std::map<std::string,Function>
            {
                ofile << "\t\tstatic std::vector<" << table.name << "*>* select(octetos::toolkit::clientdb::mysql::Connector& connector,";
                
                const apidb::ConfigureProject::Parameters& params = val->getParameters();
                {
                    apidb::ConfigureProject::Parameters::const_iterator itParamEnd = params.end();
                    itParamEnd--;
                    for(const char* param : params)
                    {
                        auto fl = table.find(param);
                        if(fl != table.end())
                        {
                            if((*fl).second->outType.compare("std::string") == 0)
                            {
                                ofile << "const std::string& ";
                            }
                            else if((*fl).second->symbolReferenced != NULL)
                            {
                                ofile << "const " << (*fl).second->symbolReferenced->classParent->name << "& ";
                            }
                            else
                            {
                                ofile << (*fl).second->outType << " ";                            
                            }
                        }
                        ofile << param; 
                        if(param != *itParamEnd)
                        {
                            ofile << ",";
                        }
                    }
                }
                ofile << ");"<<std::endl;
                
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
        for(std::list<symbols::Symbol*>::const_iterator i = table.required.begin(); i != table.required.end(); i++)
        {
            if((*i)->outType.compare("int") == 0 && (*i)->isPrimaryKey() && (*i)->isAutoIncrement()) continue; //la llave no se optine como parametro
            
			countFIelds++;
			if(i != table.required.end())
			{
				ofile << " ,"; //se agrega la coma si hay un segundo parametro
			}
				
			//
			if((*i)->classReferenced == NULL)
			{
				ofile << (*i)->outType << " ";
			}
			else
			{
				ofile << "const " << (*i)->classReferenced->name << "& ";
				//ofile << "const " << (*i)->outType <<"& ";
			}
			ofile << (*i)->name;
		}
        ofile << ");"<<std::endl;
        
		//if(countFIelds == 0) throw BuildException(table.name + " no tiene campo requerido por lo que no se puede generar metodo insert."); 
	}
	void CPP::writeDefaultContructorH(const apidb::symbols::Table& table,std::ofstream& ofile)
	{
		ofile <<"\t\t"<<table.name<<"();"<<std::endl;
	}
	void CPP::writeCopyContructorH(const apidb::symbols::Table& table,std::ofstream& ofile)
	{
		//constructor de copias 
		ofile << "\t\t" << table.name <<"(const " << table.name <<"&);"<< std::endl;
	}
    void CPP::writeKeyContructorH(const apidb::symbols::Table& table,std::ofstream& ofile)
	{
        //constructor que toma key como parametro
        if(table.key.size() > 0)//tiene key
        {
            ofile << "\t\t" <<table.name << "(";
            auto endIt = table.key.end();
            endIt--;
            for(auto k : table.key)
            {
                if(k->outType.compare("std::string") == 0)
                {
                    ofile << "const " << k->outType << "&";
                }
                else
                {
                    ofile << k->outType;                    
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
			msg = msg + table.name;
            msg = msg + "'  no tiene llave, revise su configuracion o so modelo de DB";
			throw BuildException(msg);
		}
	}
	void CPP::createClassMethodesH(const apidb::symbols::Table& table,std::ofstream& ofile)
        {
		std::string insertMethode = "";
                for (auto const& [key, attr] : table)
                {
			//get
			if((attr->outType.compare("char") == 0) | (attr->outType.compare("short") == 0) | (attr->outType.compare("int") == 0) | (attr->outType.compare("long") == 0) | (attr->outType.compare("float") == 0) | (attr->outType.compare("double") == 0))
			{
				if(attr->classReferenced == NULL)//si es foreing key
				{
					ofile <<"\t\t"<< attr->outType << " ";						
				}
				else
				{
					ofile <<"\t\t"<< "const " << attr->classReferenced->name << "& ";
				}
			}
			else
			{
				ofile <<"\t\t" << "const " << attr->outType <<"& ";
			}		
			ofile << attr->get << " const;"<< std::endl;
			
                        
                        //get key
                         if(attr->isPrimaryKey() && !attr->isForeignKey())
                        {
                                ofile <<"\t\t"<< attr->outType << " getKey" << attr->upperName << "() const;"<< std::endl;
                        }
                        else if(attr->isForeignKey())
                        {
                                ofile <<"\t\t"<< attr->outType << " getKey" << attr->upperName << "() const;"<< std::endl;
                        }
			
			//getString()			
			ofile << "\t\tstd::string get" << attr->upperName << "String() const;"<< std::endl;		
			//update
			ofile << "\t\tbool " << "update" << attr->upperName << "(octetos::toolkit::clientdb::mysql::Connector& connector,";
			if(attr->classReferenced != 0)
                        {
                                ofile << " const " << attr->classReferenced->name << "& " << attr->name;
                        }
                        else if((attr->outType.compare("std::string") == 0))
                        {
                                ofile << "const std::string& " << attr->name;
                        }
                        else if((attr->outType.compare("int") == 0) | (attr->outType.compare("long") == 0))
			{
				ofile << attr->outType << " " << attr->name;						
			}
			else
			{
				ofile << attr->outType << " " << attr->name;
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
        for (auto const& [key, attr] : table)
        {
			//ofile <<"["<<attr->outType<<"]"<<std::endl;
            if(attr->classReferenced != NULL && (attr->outType.compare("int") == 0 || attr->outType.compare("std::string") == 0))
            {
                ofile << "\t\t" << attr->classReferenced->name << "* "<< attr->name<<";"<< std::endl;
            }
            else
            {
                //ofile <<"[3]"<<std::endl;
                ofile << "\t\t" << attr->outType << " " << attr->name <<";"<< std::endl;
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
    void CPP::createClassH(const apidb::symbols::Table& cl,std::ofstream& file,const std::string& nameClass)
    {
		//file <<"keyword"<<std::endl;
		analyzer.getOutputMessage() <<"\tHeading class " << cl.name<<std::endl;
                file <<"\tclass "<<nameClass<<std::endl;        
                file <<"\t{"<<std::endl;
                //file <<"private"<<std::endl;
                createClassPrivateH(file);
                file << "\t\tstatic const std::string TABLE_NAME;" <<std::endl;
                //file <<"atributes"<<std::endl;
                createClassAttributesH(cl,file);
                createClassPublicH(file);
                //file <<"methodes"<<std::endl;
                createClassMethodesH(cl,file);
                file <<"\t};"<<std::endl;
    }
    void CPP::createSpaceH(std::ofstream& file)
    {
		if(configureProject.mvc == apidb::MVC::NO)
		{
			file <<"namespace "<< analyzer.getNameProject() <<std::endl;
			file <<"{"<<std::endl;
		}
		else if(configureProject.mvc != apidb::MVC::NO)
		{
			file <<"namespace "<< analyzer.getNameProject() <<std::endl;
			file <<"{" <<std::endl;
			file <<"namespace  controller" <<std::endl;
			file <<"{" <<std::endl;
		}
                std::map<const char*,symbols::Tables*,symbols::cmp_str> spacies = analyzer.getListTable();
		for(auto const& [keySpace, AttSpace]  : spacies)
                {
                        for(auto table: *AttSpace) //reading attrubtes by table
                        {
                                for (const apidb::symbols::Table* table : *AttSpace) 
                                {
                                        file << "\tclass " << table->name << ";"<<std::endl;
                                }
                        }
                }
		file<<std::endl;
                for(auto const& [keySpace, AttSpace]  : spacies)
                {
                        for(auto table: *AttSpace) //reading attrubtes by table
                        {
                        for (const apidb::symbols::Table* table : *AttSpace) 
                        {
                                //file <<"Declare Table " << table->name << std::endl;
                                createClassH(*table,file,table->name);       
                        }
                        }
                }
		if(configureProject.mvc == apidb::MVC::NO)
		{
			file <<"}"<<std::endl;
		}
		else  if(configureProject.mvc != apidb::MVC::NO)
		{
			file <<"}" <<std::endl;
			file <<"}" <<std::endl;
		}
    }
    
    
}
}
