#include <stdio.h>
#include <libpq-fe.h>

#include "apidb.hpp"
#include "common.hpp"
#include "Errors.hpp"
#include "analyzer.hpp"
#include "common.hpp"

namespace octetos
{
namespace apidb
{
    bool symbols::Table::basicSymbols(octetos::db::postgresql::Connector& connect)
    {
        std::string str = "SELECT * from ";
        str += name;
        octetos::db::Datresult* dt = connect.execute(str);
		if(dt != NULL) 
		{
			//std::cout<<str<<std::endl;
			while (dt->getResult())
			{
				Symbol* attrribute = new Symbol();
				attrribute->classParent = this;
				attrribute->name = PQfname((const PGresult *)dt->getResult(),0);
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
				attrribute->inType = dt->getString(1);
				std::string requiered = dt->getString(2);
				if(requiered.compare("NO") == 0)//NULL permited in DB?
				{
					attrribute->required = true;
				}
				else if(requiered.compare("YES") == 0)//NULL permited in DB?
				{
					attrribute->required = false;
				}
				//std::string keyType = row[3];
				if(strcmp(dt->getString(3).c_str(),"PRI") == 0)
                {
                    attrribute->keyType = Symbol::KeyType::PRIMARY;
                }
                else if(strcmp(dt->getString(3).c_str(),"MUL") == 0)
                {
                    attrribute->keyType = Symbol::KeyType::FOREIGN_UNIQUE;
                }
				std::string extra = dt->getString(5);
                
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
			std::cout<<"Faill on basicSymbols  : "<< str <<std::endl;
			delete dt;//mysql_free_result(result);
			return false;
		}
	}
}
}
