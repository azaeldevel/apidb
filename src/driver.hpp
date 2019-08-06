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

#ifndef APIDB_DRIVER_HPP
#define APIDB_DRIVER_HPP

#include "../analyzer.hpp"
#include "../generator.hpp"

namespace octetos
{
namespace apidb
{
        /**
         * \brief Responsable manejar todos el proceso desde el analisis hasta la generacion de los archivos fuentes
         * */
	class Driver
	{
	public:
		OutputLenguajes getOutputLenguaje() const;
                /**
                 * \brief Lee el servidor
                 * */
		virtual bool analyze(bool log);
                /**
                 * \brief Genera los archivos de codigo
                 **/
		virtual bool generate(bool log);
                /**
                 * \brief Se puede considar como una llamada a 'analyze' seguida de una llamda a 'generate'
                 * */
		bool driving(bool log);
                /**
                 * \brief Unico contructor 
                 * */
                Driver(const ConfigureProject&);
                const Analyzer& getAnalyzer() const;
                virtual ~Driver();
                
	private:
		octetos::toolkit::clientdb::Connector* connector;		
                apidb::Analyzer* analyzer;
		const ConfigureProject& configureProject;
	};
}
}

#endif

