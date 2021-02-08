 /** 
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

#ifndef APIDB_COMMON_MARIADB_HPP
#define APIDB_COMMON_MARIADB_HPP
 
#include "common.hpp"



namespace octetos
{
namespace apidb
{ 
namespace symbols
{
    class TableMariaDB : public Table
    {
    public:
        virtual bool basicSymbols(octetos::db::Connector& connect);
		virtual bool fillKeyType(octetos::db::Connector& connect, const SymbolsTable&);
        std::string primaryName(octetos::db::Connector& connect) const;
        TableMariaDB(const std::string& );
    };  
}
}
} 

#endif
