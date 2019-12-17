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

#ifndef APIDB_HPP
#define APIDB_HPP

#include "common.hpp"
#include "driver.hpp"

namespace octetos
{
namespace apidb
{
	octetos::core::Version getPakageVersion();
	std::string getPakageName();	
        static const octetos::core::Version ver100(1,0,0);
        static const octetos::core::Version ver110(1,1,0);
	static const octetos::core::Version ver200(2,0,0);
	static const octetos::core::Version ver220(2,2,0);
}
}
#endif
