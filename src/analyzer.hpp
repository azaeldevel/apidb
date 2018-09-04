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

#ifndef APIDB_ANALYZER_HPP
#define APIDB_ANALYZER_HPP

#include "common.hpp"
#include "scanner.hpp"
#include "parser.tab.hh"


namespace apidb
{
    
	class Analyzer
	{
	public:
		virtual std::ostream& getOutputMessage() = 0;
		virtual std::ostream& getErrorMessage() = 0;
		virtual std::string parse(const std::string& line) = 0;
		
		symbols::Tables& getListTable();
		
		std::string getInputLenguajeString()const;
		
		const std::string& getNameProject();
		const std::string& getDirectoryProject();
		//void setPramsProject(const std::string& name,const std::string& directory);
		//void setPramsLenguajes(InputLenguajes inputLenguaje, OutputLenguajes outputLenguaje);
				
		InputLenguajes getInputLenguaje() const;
		OutputLenguajes getOutputLenguaje() const;
		Analyzer(const ConfigureProject&);
		
    protected:
		symbols::Tables symbolsTables;		
		toolkit::clientdb::Connector* connector;
		std::ostream* outputMessages;//out stream
		std::ostream* errorMessages;//out stream
		
		//std::string nameProject;
		//std::string directoryProject;
		//InputLenguajes inputLenguaje;
		//OutputLenguajes outputLenguaje;
		const ConfigureProject& configureProject;
	};


    namespace mysql
    {
        class Analyzer : public apidb::Analyzer
        {
        public:
            bool listing(toolkit::clientdb::Connector& connect);
            
            virtual std::ostream& getOutputMessage();
            virtual std::ostream& getErrorMessage();
            
            //Analyzer();
            Analyzer(const ConfigureProject&);		
            virtual ~Analyzer();
            
            /**
            * Parse desde una std::string
            **/
            virtual std::string parse(const std::string& line);
            /** 
            * parse - parse from a file
            * @param filename - valid string with input file
            */
            void parse(const char * const filename);
            /** 
            * parse - parse from a c++ input stream
            * @param is - std::istream&, valid input stream
            */
            void parse(std::istream &iss);
            
            void message(const std::string&);		
            std::ostream& print(std::ostream &stream);
            
            //Don't use, is temporal: usada por parse para retorna sui resultado
            std::string oneLine;
        private:
            void parse_helper(std::istream &stream);
            Parser  *parser  = nullptr;
            Scanner *scanner = nullptr;   
        };
    }
}
#endif
