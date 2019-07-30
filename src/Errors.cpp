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

#include "Errors.hpp"

namespace octetos
{
namespace apidb
{
        octetos::toolkit::Error* error = NULL;

        octetos::toolkit::Error getError()
        {
                octetos::toolkit::Error e(*error);
                if(error != NULL)
                {
                        delete error;
                        error = NULL;
                }
                
                return e;
        }
        bool checkError()
        {
                if(error != NULL)return true;
                return false;
        }
        bool writeError(octetos::toolkit::Error& e)
        {
                
                if(error != NULL)
                {
                        return false;
                }
                error = &e;
                return true;
        }
} 
}
