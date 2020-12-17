/**
 * 
 *  This file is part of apidb.
 *  APIDB do Make easy to connect your Database
 *  Copyright (C) 2018  Azael Reyes
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

#ifndef APIDB_DRIVER_HPP
#define APIDB_DRIVER_HPP

#include "analyzer.hpp"
#include "generator.hpp"

namespace octetos
{
namespace apidb
{
        /**
         * \brief Responsable de manejar todos el proceso desde el analisis hasta la generacion de los archivos fuentes
         * */
	class Driver
	{
	public:
                /**
                 * \deprecated Lee este valor directamente de ConfigureProject
                 * */
		OutputLenguajes getOutputLenguaje() const;
                
                /**
                 * \deprecated Ya que esta funcion genera log  mediante la salida estandar sera removido en favor de toolkit::ActivityProgress en v2 
                 * \brief Lee el servidor
                 * */
		virtual bool analyze(core::ActivityProgress* progress);
                /**
                 * \deprecated Ya que esta funcion genera log  mediante la salida estandar sera removido en favor de toolkit::ActivityProgress en v2 
                 * \brief Genera los archivos de codigo
                 **/
		virtual bool generate(core::ActivityProgress* progress);
                /**
                 * \deprecated Ya que esta funcion genera log  mediante la salida estandar sera removido en favor de toolkit::ActivityProgress en v2 
                 * \brief Se puede considar como una llamada a 'analyze' seguida de una llamda a 'generate'
                 * */
		bool driving(core::ActivityProgress* progress);
                
                /**
                 * \deprecated Ya que esta funcion genera log  mediante la salida estandar sera removido en favor de toolkit::ActivityProgress en v2 
                 * \brief Lee el servidor
                 * */
		//virtual bool analyze(bool log);
                /**
                 * \deprecated Ya que esta funcion genera log  mediante la salida estandar sera removido en favor de toolkit::ActivityProgress en v2 
                 * \brief Genera los archivos de codigo
                 **/
		//virtual bool generate(bool log);
                /**
                 * \deprecated Ya que esta funcion genera log  mediante la salida estandar sera removido en favor de toolkit::ActivityProgress en v2 
                 * \brief Se puede considar como una llamada a 'analyze' seguida de una llamda a 'generate'
                 * */
		//bool driving(bool log);
                
                /**
                 * \brief Unico contructor 
                 * */
        Driver(ConfigureProject&);
        /**
        * \brief Retorna el obejeto Analyzer utilizado.
        * */
        const Analyzer& getAnalyzer() const;
        virtual ~Driver();                
        bool getTablesName(std::list<std::string>& retList,symbols::Space* space = NULL)const;
        bool getFiledsName(std::list<std::string>& retList,const std::string& table)const;
        
        bool loadLibrary();
        
	private:
		octetos::db::Connector* connector;		
        apidb::Analyzer* analyzer;
		ConfigureProject& configureProject;
        //void* handle;
        octetos::db::Connector* (*createConnector)();
        
        //>>>>
        void (*destroyAnalyzer)(octetos::apidb::Analyzer*);
        apidb::Analyzer* (*createAnalyzer)(const octetos::apidb::ConfigureProject*,octetos::db::Connector*,octetos::core::ActivityProgress*);
	};
}
}

#endif

