#include <stdio.h>
#include <libpq-fe.h>
#include <octetos/db/clientdb-postgresql.hh>

#include "common-postgresql.hpp"
#include "apidb.hpp"
#include "Errors.hpp"
#include "analyzer.hpp"

namespace octetos
{
namespace apidb
{
    symbols::TablePostgreSQL::TablePostgreSQL(const std::string& s) : Table(s)
    {
        
        
    }
    /*bool createDatconnect(const std::string& host, unsigned int port,const std::string& database,const std::string& usuario,const std::string& password, octetos::db::Datconnect** dat)
    {
        (*dat) = new octetos::db::postgresql::Datconnect(host,port,database,usuario,password);
        return true;
    }
    
    bool createDatconnect(octetos::db::Datconnect** dat)
    {
        (*dat) = new octetos::db::postgresql::Datconnect();
        return true;
    }
    
    bool createConnector(octetos::db::Connector** conn)
    {
        (*conn) = new octetos::db::postgresql::Connector();
        return true;
    }*/
    

    bool symbols::TablePostgreSQL::basicSymbols(octetos::db::Connector& connect)
    {
        std::string str = "SELECT * from ";
        str += name;
        octetos::db::postgresql::Datresult dt;
        bool fl = connect.execute(str,dt);
		if(fl) 
		{
            for(int i = 0; i < PQntuples((const PGresult *)dt.getResult()); i++)
			{
				Symbol* attrribute = new Symbol();
				attrribute->classParent = this;
				attrribute->name = PQfname((const PGresult *)dt.getResult(),i);
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
                Oid type = PQparamtype((const PGresult*)dt.getResult(),i);//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
				attrribute->inType = dt.getString(i);//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
				std::string requiered = dt.getString(i);//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
				if(requiered.compare("NO") == 0)//NULL permited in DB?
				{
					attrribute->required = true;
				}
				else if(requiered.compare("YES") == 0)//NULL permited in DB?
				{
					attrribute->required = false;
				}
				//std::string keyType = row[3];
				if(strcmp(dt.getString(i).c_str(),"PRI") == 0)
                {
                    attrribute->keyType = Symbol::KeyType::PRIMARY;
                }
                else if(strcmp(dt.getString(i).c_str(),"MUL") == 0)
                {
                    attrribute->keyType = Symbol::KeyType::FOREIGN_UNIQUE;
                }
				std::string extra = dt.getString(i);
                
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
			//delete dt;//mysql_free_result(result);
			return true;			
		}
		else
		{
			std::cout<<"Faill on basicSymbols  : "<< str <<std::endl;
			//delete dt;//mysql_free_result(result);
			return false;
		}
	}
 /**
    * Rellena los campos 'classReferenced' y 'symbolReferenced' de la tabla
    */
    bool symbols::Table::fillKeyType(octetos::db::Connector& connect,const SymbolsTable& symbolsTable)
	{
        /**
        * Lista las relaciones de llaves foraneas para la tabla actual
        */
		/*std::string fks = "SELECT k.COLUMN_NAME, k.REFERENCED_TABLE_NAME, k.REFERENCED_COLUMN_NAME FROM information_schema.TABLE_CONSTRAINTS i,information_schema.KEY_COLUMN_USAGE k WHERE i.CONSTRAINT_NAME = k.CONSTRAINT_NAME  AND i.CONSTRAINT_TYPE = 'FOREIGN KEY' AND i.TABLE_SCHEMA =k.TABLE_SCHEMA AND i.TABLE_NAME = "; 
        fks += "'";
		fks += fullname;
        fks += "' AND i.CONSTRAINT_SCHEMA =  '" ;
		fks += ((octetos::db::Datconnect*)(connect.getDatconection()))->getDatabase();
		fks += "'";
		//std::cout<<fks<<std::endl;
		//std::cout<< "In table: " <<fullname<<std::endl;
        octetos::db::Datresult* dt = connect.execute(fks.c_str());
        if(dt != NULL)
        {                      
			symbols::SymbolsTable::const_iterator itGlobal = symbolsTable.find(symbolsTable.getConfigureProject().name.c_str());
			symbols::Space* global = ((symbols::Space*)itGlobal->second);
            MYSQL_ROW row;
            while ((row = mysql_fetch_row((MYSQL_RES*)(dt->getResult()))))
            {
				//std::cout<<"Buscando tabla '" << row[1] << "' symbols::Table::fillKeyType Find" << std::endl;			
				symbols::Table* table = global->findTable(row[1]);
				//std::cout<<"Buscando tabla '" << row[1] << "' symbols::Table::fillKeyType return" << std::endl;	
				if(table == NULL)
				{
					std::string msg = "No se encontro la tabla '";
					msg += row[1];
					msg += "'";
					core::Error::write(core::Error(msg,ErrorCodes::ANALYZER_FAIL,__FILE__,__LINE__));
					return false;
				}
				//std::cout<<"Se encontró tabla '" << table->getName() << "'" << std::endl;
				Symbol* referenceSymbol = ((Table*)table)->findSymbol(row[2]);
				if(referenceSymbol == NULL)
				{
					std::string msg = "No se encontro el campo '";
					msg += row[2];
					msg += "'";
					core::Error::write(core::Error(msg,ErrorCodes::ANALYZER_FAIL,__FILE__,__LINE__));
					return false;
				}
				//std::cout<<"Se encontró campo '" << referenceSymbol->getName() << "'" << std::endl;
				Symbol* targetSymbol = findSymbol(row[0]);
				if(targetSymbol == NULL)
				{
					std::string msg = "No se encontro el campo '";
					msg += row[0];
					msg += "'";
					core::Error::write(core::Error(msg,ErrorCodes::ANALYZER_FAIL,__FILE__,__LINE__));
					return false;
				}
				//std::cout<< targetSymbol->getName() << "-->" << referenceTable->getName()  << ":" << referenceSymbol->getName() << "'" << std::endl;
			}	
		}
		else
		{
			//std::cout << "No retorno resultado la consulta" << std::endl;
		}
			
                
		delete dt;                
		return true;*/
    }
}
}
