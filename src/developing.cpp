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

#include "apidb.hpp"

#include <iostream>
#include <list>


int main()
{
	toolkit::clientdb::DatconectionMySQL mysqlConnector("192.168.0.101",3306,"business.alpha","root","k3yL0c41");  
	apidb::CG driver("nmp","test",mysqlConnector,apidb::InputLenguajes::MySQL_Server,apidb::OutputLenguajes::CPP);
	if(!driver.driving())
    {
        std::cerr<<"Fail parsin phase"<<std::endl;
        return -1;
    }
    
    if(!driver.saveConfig("test/nmp.apidb"))
    {
        std::cerr<<"Fail creation prject."<<std::endl;
        return -1;		
	}
    
    	
	return 0;	
}
