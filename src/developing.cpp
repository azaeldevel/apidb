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

	std::string name = "business1";		
	std::string dir = "../business1";
	std::string strProject;	
	if(dir.compare(".") == 0 | dir.empty())
	{
		strProject = name + ".apidb";
	}
	else
	{
		strProject = dir + "/" + name + ".apidb";
	}
	std::ifstream fin(strProject);
	if(fin) 
	{
		std::cout<<"Cargando '" << strProject << "' ..." <<std::endl;
		apidb::CG driver;
		if(!driver.loadConfig(strProject))
		{
			std::cerr<<"Fallo la configuracion."<<std::endl;
			return EXIT_FAILURE;
		}
		else
		{
			std::cout<<"'"<<strProject<<"' cargado.";
		}		
		
		
		if(!driver.driving())
		{
			std::cerr<<"Fail parsin phase"<<std::endl;
			return EXIT_FAILURE;
		}
		
		return EXIT_SUCCESS;
	}
	   
	
	return 0;	
}
