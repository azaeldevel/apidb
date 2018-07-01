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
 * */

#include "apidb.hpp"
#include "toolkit.hpp"

#include <iostream>

namespace apidb
{
		
	bool CG::driving()
	{
		if(analyze())
		{
			if(generate()) return true;
		}
		
		return false;
	}
	
	bool CG::generate()
	{		
		if(this->analyzer->getOutputLenguaje() == apidb::OutputLenguajes::CPP)
		{
			apidb::CPPGenerator cpp(*analyzer);
			return cpp.generate();
		}
		else
		{
			return false;
		}
	}
	
	bool CG::analyze()
	{
		analyzer->getOutputMessage() << "Analisis de codigo..." << std::endl;
		analyzer->getOutputMessage() << "\tLenguaje de entrada: " << analyzer->getInputLenguajeString() << std::endl;
		
		apidb::mysql::Analyzer* analyzer = NULL;
		if(this->analyzer->getInputLenguaje() == apidb::InputLenguajes::MySQL_Server)
		{
			analyzer = (apidb::mysql::Analyzer*)(this->analyzer);
		}
		else
		{
			return false;
		}
		if(analyzer->listing(*connector)) //reading tables
        {
            for(internal::Table* table: analyzer->getListTable()) //reading attrubtes by table
            {
				analyzer->getOutputMessage() << "\tCreating basic simbols for " << table->name  << "." << std::endl;
                //simbolos basicos 
                if(!table->basicSymbols(*connector))
                {
					//std::cerr<<"Faill on basicSymbols"<<std::endl;
					return false;
				}				
				//foreign key's
				if(!table->fillKeyType(*connector,analyzer->getListTable()))
                {
					//std::cerr<<"Faill on fillKeyType"<<std::endl;
					return false;
				}//parsing imput types
				
				for(internal::Symbol* attribute: *table)
				{
					//std::cout<<attribute->inType<<std::endl;
					attribute->outType = analyzer->parse(attribute->inType);
				}				
			}				
        }  
          
		return true;
	}
		
	CG::CG(const std::string& name,const std::string& directory,const toolkit::clientdb::Datconection& datconection,InputLenguajes inputLenguaje, OutputLenguajes outputLenguaje)
	{		
		connector = new toolkit::clientdb::Connector();
		analyzer = new mysql::Analyzer(inputLenguaje,outputLenguaje);		
		try
		{
			bool flag = connector->connect(datconection);
			if(flag)
			{
				analyzer->setPramsProject(name,directory);				
			}
		}
		catch(toolkit::clientdb::SQLException ex)
		{
			analyzer->getErrorMessage() <<"Fallo la conexion el servidor de datos el cual respondio; "<<std::endl;
		}		
	}
} 
