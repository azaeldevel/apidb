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

#ifndef APIDB_READER_ANALYZER_HPP
#define APIDB_READER_ANALYZER_HPP

#include "../analyzer.hpp"
#include "scanner.hpp"
#include "parser.tab.hh"

namespace octetos
{
namespace apidb
{    
namespace mysql
{
        class Analyzer : public apidb::Analyzer
        {
        public:            
            virtual bool analyze(bool log);
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
            
            //Analyzer();
            Analyzer(const ConfigureProject&,octetos::toolkit::clientdb::Connector*);		
            virtual ~Analyzer();         
            
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
}
#endif
