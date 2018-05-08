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

#ifndef CLIENTDB_HPP
#define CLIENTDB_HPP

#include "toolkit.hpp"

namespace clientdb
{
	toolkit::Version getPakageVersion();
	const char* getPakageName();	
	typedef unsigned int ID;
    
    struct Datconection    
    {
        
    };
    
    struct DatconectionMySQL : public Datconection
    {
        const char *host;
        const char *usuario;
        const char *password;
        const char *database;
        unsigned int port;
        const char *unix_socket;
        unsigned long client_flag;

        int last_errono;
        const char * last_errmsg;
    };
	
	class Rows
	{
	public:
		virtual void import(void* row) = 0;
	};
    
    class Connector
    {
    private:
        void* serverConnector;
        Datconection* datconection;
    public:
        Connector();
        Connector(DatconectionMySQL& connector);
        bool connect(DatconectionMySQL& connector);
        const char* serverDescription();
        bool query(const std::string&);
        bool query(const std::string&,Rows&);
        ID insert(const std::string&);
        bool commit();
        bool rollback();
        void* getServerConnector();
        const Datconection* getDatconection() const;  
    };    
}

#endif
