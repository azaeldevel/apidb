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

#include "driver.hpp"
#include "versionInfo.h"
#include <string>
#include <iostream>

namespace octetos
{
namespace apidb
{
	std::string getPakageName()
	{
		return std::string(PAKAGENAME);
	}
	octetos::toolkit::Version getPakageVersion()
	{
                octetos::toolkit::Version v;
                v.setNumbers(VERSION_MAJOR,VERSION_MINOR,VERSION_PATCH);
                v.setStage(VERSION_STAGE);
                v.setBuild(std::stoul(VERSION_BUILD));
		return v;		
	}
	
} 
}
