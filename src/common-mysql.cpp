/**
 * 
 *  This file is part of apidb.
 * 
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.

 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.

 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * 
 * */

#include "apidb.hpp"
#include "common.hpp"

#include <iostream>
#include <mysql/mysql.h>

namespace octetos
{
namespace apidb
{
        /**
        * Rellena los campos 'classReferenced' y 'symbolReferenced' de la tabla
        */
	bool symbols::Table::fillKeyType(octetos::toolkit::clientdb::Connector& connect,std::map<const char*,symbols::Tables*,symbols::cmp_str>& tables)
	{
                /**
                * Lista las relaciones de llaves foraneas para la tabla actual
                */
		std::string fks = "SELECT k.COLUMN_NAME, k.REFERENCED_TABLE_NAME, k.REFERENCED_COLUMN_NAME FROM information_schema.TABLE_CONSTRAINTS i,information_schema.KEY_COLUMN_USAGE k WHERE i.CONSTRAINT_NAME = k.CONSTRAINT_NAME  AND i.CONSTRAINT_TYPE = 'FOREIGN KEY' AND i.TABLE_SCHEMA =k.TABLE_SCHEMA AND i.TABLE_NAME = "; 
                fks += "'";
                if(space.compare("") == 0)
                {
                        fks += "`" + space + "." + name + "`";
                }
                else
                {
                        fks += name;
                }
		fks += "' AND i.CONSTRAINT_SCHEMA =  '" ;
		fks += ((octetos::toolkit::clientdb::Datconnect*)(connect.getDatconection()))->getDatabase();
		fks += "'";
		//std::cout<<fks<<std::endl;
                octetos::toolkit::clientdb::Datresult* dt = connect.query(fks.c_str());
                if(dt != NULL)
                {
			//MYSQL_RES *result = mysql_store_result((MYSQL*)connect.getServerConnector());                        
			MYSQL_ROW row;
			while ((row = mysql_fetch_row((MYSQL_RES*)(dt->getResult()))))
			{
                                //std::cout<<"Buscando tabla '" << row[1] << "'" << std::endl;
                                Tables::iterator itTBReference;
                                bool flFinded = false;
                                for(auto const& [keySpace, AttSpace]  : tables)
                                {
                                        itTBReference = (*AttSpace).find(row[1]);//buscar la tabla del campo que se refiere en el registro actual 'row[1]'
                                        if(itTBReference != (*AttSpace).end()) 
                                        {
                                                break;
                                                flFinded = true;
                                        }
                                }
                                if(flFinded)
                                {//si se encontro la tabla
                                                Table* tbReference = *itTBReference;
                                                //std::cout<<"Se encontro tabla '" << tbFinded->name << "'" << std::endl;
                                                //std::cout<<"Buscando campo '" << row[0] << "'" << std::endl;
                                                iterator itatt = find(row[0]);//buscar el compo referido (REFERENCED_COLUMN_NAME) en la tabla
                                                if(itatt != tbReference->end()) //buscar
                                                {//si se encontro la tabla                                                 
                                                        //if(itFinded != tables.end())
                                                        Symbol* attribute = itatt->second;
                                                        //std::cout<<"Se encontro campo '"<< attribute->name << "'" << std::endl;
                                                        //std::cout  << attribute->classParent->name << "-->" << attribute->classReferenced->name << std::endl;
                                                        tbReference->countRef++;//contando la cantiad de veces que es referida la clase
                                                        Table::iterator itAttRefence = tbReference->find(row[2]);
                                                        if(itAttRefence != attribute->classReferenced->end())
                                                        {
                                                                attribute->classReferenced = tbReference;
                                                                attribute->symbolReferenced = (*itAttRefence).second;
                                                                attribute->isFK = true;
                                                                //std::cout <<  attribute->classParent->name << ":" << attribute->name << "-->" << attribute->classReferenced->name << ":" << attribute->symbolReferenced->name << std::endl;
                                                        }
                                                        else
                                                        {
                                                                std::string strmsg = "No se encontro el campo '";
                                                                strmsg = strmsg + row[0] + "' en la tabla '" + row[1] + "', es necesario para construir la referencia a dicho campo.";
                                                                throw BuildException(strmsg);
                                                        }
                                                }
                                                else
                                                {
                                                        std::string strmsg = "No se encontro el campo '";
                                                         strmsg = strmsg + row[2] + "' en la tabla '" + row[1] + "', es necesario para construir la referencia a dicho campo.";
                                                        throw BuildException(strmsg); 
                                                }
                                }
                                else
                                {
                                        std::string strmsg = "No se encontro el la tabla '";
                                        strmsg = strmsg + row[1] + "'";
                                        throw BuildException(strmsg);
                                }
			}
			delete dt; 
			return true;	
                }
                
                delete dt;                
                return false;
	}
	
        bool symbols::Table::basicSymbols(octetos::toolkit::clientdb::Connector& connect)
        {
		std::string str = "DESCRIBE ";
                if(space.compare("") != 0)
                {
                        str += "`" + space + "." + name + "`";
                }
                else
                {
                        str += name;
                }
                octetos::toolkit::clientdb::Datresult* dt = connect.query(str.c_str());
		if(dt != NULL) 
		{
			//std::cout<<str<<std::endl;
			//MYSQL_RES *result = mysql_store_result((MYSQL*)connect.getServerConnector());
			MYSQL_ROW row;
			bool setkey = false;
			while ((row = mysql_fetch_row((MYSQL_RES*)(dt->getResult()))))
			{
				Symbol* attrribute = new Symbol();
				attrribute->classParent = this;
				attrribute->name = row[0];
                                //std::cout<<attrribute->name<<std::endl;
				std::string strName = attrribute->name;
				if(strName.compare("id") == 0)
				{
					strName = "ID";
				}
				else
				{
					strName[0] = toupper(strName[0]);	
				}
				attrribute->get = "get"; attrribute->get += strName; attrribute->get += "()";
				attrribute->upperName = strName;
				attrribute->inType = row[1];
				std::string requiered = row[2];
				if(requiered.compare("NO") == 0)//NULL permited in DB?
				{
					attrribute->required = true;
				}
				else if(requiered.compare("YES") == 0)//NULL permited in DB?
				{
					attrribute->required = false;
				}
				//std::string keyType = row[3];
				if(strcmp(row[3],"PRI") == 0)
                                {
                                        attrribute->keyType = Symbol::KeyType::PRIMARY;
                                }
                                else if(strcmp(row[3],"MUL") == 0)
                                {
                                        attrribute->keyType = Symbol::KeyType::FOREIGN_UNIQUE;
                                }
				std::string extra = row[5];

				if(attrribute->required && attrribute->keyType == Symbol::KeyType::PRIMARY && extra.compare("auto_increment") == 0)//primary key
				{
					key.push_back(attrribute);
					attrribute->isPK = true;//attrribute->keyType = symbols::Symbol::KeyType::PRIMARY;
                                        attrribute->isAutoInc = true;
				}
				else if(attrribute->required && attrribute->keyType == Symbol::KeyType::PRIMARY)//unique constraing
				{
					key.push_back(attrribute);
					attrribute->isPK = true;//attrribute->keyType = symbols::Symbol::KeyType::PRIMARY;
                                        attrribute->isAutoInc = false;
				}
				
                                insert(std::make_pair(attrribute->name.c_str(),attrribute));
				if(attrribute->required)
				{
					required.push_back(attrribute);//si attrribute->required tambie se agrega a un lista especial
				}
				//std::cout<<"Termitade with:" << attrribute->name << "(" << attrribute->upperName << ")" <<std::endl;
			}
			delete dt;//mysql_free_result(result);
			return true;			
		}
		else
		{
			std::cerr<<"Faill on basicSymbols  : "<< str <<std::endl;
			delete dt;//mysql_free_result(result);
			return false;
		}
        }
    
    
	bool symbols::listing(octetos::toolkit::clientdb::mysql::Connector& connect, std::map<const char*,symbols::Tables*,symbols::cmp_str>& tables)
	{
		std::string db = connect.getDatconection()->getDatabase();
		//std::cout<< "db:" << db <<std::endl;
		std::string str = "SELECT TABLE_NAME FROM information_schema.tables WHERE TABLE_SCHEMA = '";
                str = str + db + "' and TABLE_TYPE = 'BASE TABLE' ORDER BY TABLE_NAME ASC";
                octetos::toolkit::clientdb::Datresult* dt = connect.query(str.c_str());   
		if(dt != NULL) 
		{
			MYSQL_ROW row;
			while ((row = mysql_fetch_row((MYSQL_RES*)(dt->getResult()))))
			{
				Table* prw = new Table();
				prw->name = getSpaceName(row[0]);
                                //prw->shortname = getTableName(row[0]);
                                std::string upper = row[0];
                                upper[0] = toupper(upper[0]);
                                prw->upperName = upper;
                                prw->space = getSpacePatch(row[0]);
                                prw->fullname = row[0];
                                
                                std::map<const char*,symbols::Tables*,symbols::cmp_str>::iterator it = tables.find(prw->space.c_str());                                
                                if(it == tables.end())
                                {
                                        Tables* newSpace  = new Tables();
                                        newSpace->name = prw->space;
                                        newSpace->push_back(prw);
                                        std::pair<const char*, symbols::Tables*> newInser(prw->space.c_str(),newSpace);
                                        tables.insert(newInser);
                                }
                                else
                                {
                                        (*it).second->push_back(prw);
                                }
                                //std::cout <<  prw->space << "::" << prw->name << std::endl;
			}
                        delete dt;//mysql_free_result((MYSQL_RES*)(dt->getResult()));
                        //std::cout << "Count : " << tables.size() << std::endl;
			return true;
		}
		else
                {
                        std::string msg = "";
                        msg = msg + " MySQL Server Error No. : '";
                        msg = msg + std::to_string(mysql_errno((MYSQL*)connect.getServerConnector()));
                        msg = msg + "' ";
                        msg = msg + mysql_error((MYSQL*)connect.getServerConnector());
                        throw octetos::toolkit::clientdb::SQLException(msg);                         
                }
		return false;	
	}
}
}
