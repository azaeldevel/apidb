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
    std::string symbols::TablePostgreSQL::primaryName(octetos::db::Connector& connect) const 
    {
        return "";
    }
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
        //std::cout << "symbols::TablePostgreSQL::basicSymbols : Step 1\n";
        std::string str = "SELECT table_name, column_name, data_type,is_nullable,column_default FROM information_schema.columns WHERE table_name   = ";
        str += "'" + name + "'";
        octetos::db::postgresql::Datresult dt;
        bool fl = connect.execute(str,dt);
        //std::cout << "symbols::TablePostgreSQL::basicSymbols : Step 2\n";
		if(fl) 
		{
            //std::cout << "symbols::TablePostgreSQL::basicSymbols : Step 2.1\n";
            while(dt.nextRow())
			{
                //std::cout << "symbols::TablePostgreSQL::basicSymbols : Step 2.1.1\n";
				Symbol* attrribute = new Symbol();
				attrribute->classParent = this;                
                //std::cout << "symbols::TablePostgreSQL::basicSymbols : Step 2.1.1.1\n";
				attrribute->name = dt.getString(1);
                //std::cout << "symbols::TablePostgreSQL::basicSymbols : Step 2.1.1.2\n";
                //std::cout << attrribute->name << std::endl;
				std::string strName = attrribute->name;
                //std::cout << "symbols::TablePostgreSQL::basicSymbols : Step 2.1.2\n";
				if(strName.compare("id") == 0)
				{
					strName = "ID";
				}
				else
				{
					strName[0] = toupper(strName[0]);	
				}
                //std::cout << "symbols::TablePostgreSQL::basicSymbols : Step 2.1.3\n";
				attrribute->get = "get"; attrribute->get += strName; attrribute->get += "()";
				attrribute->upperName = strName;
				attrribute->inType = dt.getString(2);
				std::string requiered = dt.getString(3);
				if(requiered.compare("NO") == 0)//NULL permited in DB?
				{
					attrribute->required = true;
                    required.push_back(attrribute);//si attrribute->required tambie se agrega a un lista especial
				}
				else if(requiered.compare("YES") == 0)//NULL permited in DB?
				{
					attrribute->required = false;
				}
								
				
                //std::cout << "symbols::TablePostgreSQL::basicSymbols : Step 2.1.4\n";
                
                insert(std::make_pair(attrribute->name.c_str(),attrribute));
				
                //std::cout << "symbols::TablePostgreSQL::basicSymbols : Step 2.1.5\n";
			}
			//delete dt;//mysql_free_result(result);
			//return true;			
		}
		else
		{
			std::string msg = "";
			msg = msg + " Fail on basicSymbols : '";
            msg = msg +str;
			core::Error::write(core::Error(msg,ErrorCodes::ANALYZER_FAIL,__FILE__,__LINE__));
			return false;
		}
		
		//second stage::buscando llaves primarias
		//std::cout << "symbols::TablePostgreSQL::basicSymbols : Step 3\n";
        str = "select kcu.table_schema, kcu.table_name, tco.constraint_name, kcu.ordinal_position as position,kcu.column_name as key_column from information_schema.table_constraints tco join information_schema.key_column_usage kcu on kcu.constraint_name = tco.constraint_name and kcu.constraint_schema = tco.constraint_schema and kcu.constraint_name = tco.constraint_name where tco.constraint_type = 'PRIMARY KEY' and kcu.table_name = '" + name + "' order by kcu.table_schema, kcu.table_name,position";
        //std::cout << "\n" << str << "\n";
        octetos::db::postgresql::Datresult dt2;
        fl = connect.execute(str,dt2);
        //std::cout << "symbols::TablePostgreSQL::basicSymbols : Step 4\n";
		if(fl) 
		{
            //std::cout << "symbols::TablePostgreSQL::basicSymbols : Step 4.1\n";
            while(dt2.nextRow())
			{
                //std::cout << "symbols::TablePostgreSQL::basicSymbols : Step 4.1.1\n";
                std::map<const char*,Symbol*,cmp_str>::iterator it = find(dt2.getString(4).c_str());
                if(it == end())
                {
                    std::string msg = "";
                    msg = msg + " No se encontró el campo : '";
                    msg = msg + dt2.getString(4) + "'\n";
                    core::Error::write(core::Error(msg,ErrorCodes::ANALYZER_FAIL,__FILE__,__LINE__));
                    return false;
                }
                
                it->second->keyType = Symbol::KeyType::PRIMARY;
                if(it->second->required && it->second->keyType == Symbol::KeyType::PRIMARY)//primary key
				{
					key.push_back(it->second);
					it->second->isPK = true;//attrribute->keyType = symbols::Symbol::KeyType::PRIMARY;
                    it->second->isAutoInc = true;//TODO:Hay que averiguar si realmente es auto incremnto.
				}
			}
			return true;			
		}
		else
		{
			std::string msg = "";
			msg = msg + " Fail on basicSymbols : '";
            msg = msg +str;
			core::Error::write(core::Error(msg,ErrorCodes::ANALYZER_FAIL,__FILE__,__LINE__));
			return false;
		}
		
		//third stage::buscando llaves foraneas
		str = "SELECT conname, pg_catalog.pg_get_constraintdef(r.oid, true) as condef FROM pg_catalog.pg_constraint r WHERE r.conrelid = '";
        str += name + "'::regclass AND r.contype = 'f' ORDER BY 1";
        octetos::db::postgresql::Datresult dt3;
        fl = connect.execute(str,dt3);
        if(fl) 
		{
            //std::cout << "symbols::TablePostgreSQL::basicSymbols : Step 4.1\n";
            while(dt3.nextRow())
			{
                //std::cout << "symbols::TablePostgreSQL::basicSymbols : Step 4.1.1\n";
                std::map<const char*,Symbol*,cmp_str>::iterator it = find(dt3.getString(1).c_str());
                if(it == end())
                {
                    std::string msg = "";
                    msg = msg + " No se encontró el campo : '";
                    msg = msg + dt3.getString(1) + "'\n";
                    core::Error::write(core::Error(msg,ErrorCodes::ANALYZER_FAIL,__FILE__,__LINE__));
                    return false;
                }

                it->second->keyType = Symbol::KeyType::FOREIGN_UNIQUE;                
                
			}
			
			return true;			
		}
		else
		{
			std::string msg = "";
			msg = msg + " Fail on basicSymbols : '";
            msg = msg +str;
			core::Error::write(core::Error(msg,ErrorCodes::ANALYZER_FAIL,__FILE__,__LINE__));
			return false;
		}
            
        return true;
	}
 /**
    * Rellena los campos 'classReferenced' y 'symbolReferenced' de la tabla
    */
    bool symbols::TablePostgreSQL::fillKeyType(octetos::db::Connector& connect,const SymbolsTable& symbolsTable)
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
			
                
		delete dt;  */              
		return true;
    }
}
}
