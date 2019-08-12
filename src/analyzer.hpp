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


#ifndef APIDB_ANALYZER_HPP
#define APIDB_ANALYZER_HPP

#include "common.hpp"
#include "ConfigureProject.hpp"

namespace octetos
{
namespace apidb
{    
        /**
         * \brief No hay muchas funciones dispoblies aquí quiza le sea utíl si desea conocer la tabla de simbolos.
         **/
	class Analyzer
	{
	public:
                /**
                 * \brief Realiza una llamada al parser interno que realiza el analisis de cadana pasada
                 * \private no llame directame solo es para el uso del parser interno
                 * */
		virtual std::string parse(const std::string& line) = 0;
                /**
                 * \brief Simplemete analiza la base de datos y genera la informacion de la tabla de simbolos
                 * \param progress Use NULL para especificar que no desea log o una instacia valida de toolkit::ActivityProgress para generarlos.
                 * */
                virtual bool analyze(toolkit::ActivityProgress* progress) = 0;
                
                /**
                 * \brief Retorna un referancia a la tabla de simbolos, no es modificable si necesita poder modicar 
                 * \details Ya que la tabla de simbolos es un estructura de datos compleja y muy delicada no se permite acceso de escritura, si lo necesita puede considerar hacer una copia de la tabal mediante la  funcion copyListTable, la caul le construira una identica. Quiza quiera reconsiderar el modelo de su programa o la estructura del mismo tambien.
                 **/
                const std::map<const char*,symbols::ISpace*,symbols::cmp_str>& getListTableConst() const;
                /**
                 * \brief Crea una copia de la tabla de simbolos las cual puede ser manipulada
                 * */
                std::map<const char*,symbols::ISpace*,symbols::cmp_str> copyListTable()const;
			
                /**
                 * \brief Requiere la configuracion del proyecto y un conector la base de datos valido.
                 * */
		Analyzer(const ConfigureProject&, octetos::toolkit::clientdb::Connector*,toolkit::ActivityProgress* progress);
                /**
                 * \brief Destructor
                 * */
                virtual ~Analyzer(); 
                /**
                 *\brief Nuevo sistema de recuperacion de Mensajes. 
                 **/
                toolkit::ActivityProgress& getOutput();
                /**
                 * \brief Retorna una referencia al la estructura de configuracion de Proyecto.
                 **/
                const ConfigureProject& getConfigureProject()const;
        protected:
                /**
                 * \brief Tabla de simbolos completa.
                 * \details Consiste de un conjuto de symbols::Tables que determina un espacio, asu vez cada Tables es un constu de std::list<Table*>  que representa las tabla del espacio. Cada Table a su vez contiene std::map<const char*,Symbol*,cmp_str> que representa los campos de la tabla.
                 * */
                symbols::SymbolsTable symbolsTable;
                /**
                 * \brief COnector a la base de datos.
                 * */
		octetos::toolkit::clientdb::Connector* connector;
		/**
                 * \brief Esta funcion es la encarga de resolver cada simbolo encontrado en la base de datos.
                 * */
		bool listing();  
                /**
                 * \brief Indica la configuracion del proyecto.
                 * */
                const ConfigureProject& configureProject;
                /**
                 * \brief Para envio de mensaje al usuario.
                 * */
                toolkit::ActivityProgress* progress;
				
		bool basicSymbols(symbols::ISpace* ispace,toolkit::ActivityProgress* progress);
	};
}
}
#endif
