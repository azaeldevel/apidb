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

#include <mysql/my_global.h>
#include <mysql/mysql.h>
#include <iostream>

#include "clientdb.hpp"


namespace clientdb
{	
    bool Connector::rollback()
    {
        if (serverConnector != NULL)
        {
            if(mysql_rollback((MYSQL*)serverConnector) == 0)
            {
                return true;
            }
        }
        
        return false; 
    }
	bool Connector::query(const std::string& sql,Rows& rows)
	{
		if(query(sql.c_str()))
		{
			MYSQL_RES *result = mysql_store_result((MYSQL*)serverConnector);
			MYSQL_ROW row;
			while((row = mysql_fetch_row(result)))
			{
				rows.import(&row);
			}
			
			return true;
		}
		
		return false;
	}
	
    const Datconection* Connector::getDatconection() const
    {
        return datconection;
    }
    bool Connector::commit()
    {
        if (serverConnector != NULL)
        {
            if(mysql_commit((MYSQL*)serverConnector))
            {
                return true;
            }
        }
        
        return false; 
    }
    ID Connector::insert(const std::string& str)
    {
		if (mysql_query((MYSQL*)serverConnector, str.c_str()) == 0) 
		{
			return mysql_insert_id((MYSQL*)serverConnector);
		}
		else
        {   
            return 0; 
        }		
    }
    
    const char* Connector::serverDescription()
    {
        return mysql_get_client_info();
    }

    Connector::Connector()
    {
    }

    bool Connector::connect(DatconectionMySQL& conection)
    {
        serverConnector = (void*)mysql_init(NULL);
        if (serverConnector == NULL)
        {
            std::string msg = "";
            msg = msg + " MySQL server return error number '";
            msg = msg + std::to_string(mysql_errno((MYSQL*)serverConnector));
            msg = msg + "' ";
            msg = msg + mysql_error((MYSQL*)serverConnector);
            return false;//return toolkit::Exception(toolkit::Message::FAIL_SERVER_DATABASE,msg.c_str());            
        }

        if (mysql_real_connect((MYSQL*)serverConnector, conection.host, conection.usuario, conection.password,conection.database,conection.port, NULL, 0) == NULL)
        {
            std::string msg = "";
            msg = msg + " MySQL server return error number '";
            msg = msg + std::to_string(mysql_errno((MYSQL*)serverConnector));
            msg = msg + "' ";
            msg = msg + mysql_error((MYSQL*)serverConnector);
            return false;//return toolkit::Exception(toolkit::Message::FAIL_SERVER_DATABASE,msg.c_str());
        }
        
        if(mysql_autocommit((MYSQL*)serverConnector,0) != 0)
        {
            return false;//return toolkit::Exception(toolkit::Message::FAIL_SERVER_DATABASE,"Fail on disable commit.");
        }
        
        datconection = &conection;
        
        return true;//return toolkit::Confirmation(toolkit::Message::SUCCEED,"Conexion completa");
    }
    
    void* Connector::getServerConnector()
    {
        return this->serverConnector;
    }
	
    bool Connector::query(const std::string& str)
    {
		if (mysql_query((MYSQL*)serverConnector, str.c_str()) == 0) 
		{
			return true;
		}
		
		return false;
    }
}
