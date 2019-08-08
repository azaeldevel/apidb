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

#ifndef APIDB_ERRORS_HPP
#define APIDB_ERRORS_HPP

#include <toolkit/common/common.hpp>

namespace octetos
{
namespace apidb
{
        //extern toolkit::Error* error;
        /**
         * \brief Codigo de Errores de APIDB.
         * */
        enum ErrorCodes
        {                        
                /**
                 * \brief Para complier con el requisito de toolkit::Error
                 * */
                READFILE_TEMPUNPACKFAIL = toolkit::Error::Codes::ROOFCODE,//No se puede crear el directorio tempora para desempauqetar el archivo de proyecto.
                Read_FileFailParseNode,
                Read_UncomConfigFile,
                READFILE_OPENXMLFILE,
                READFILE_OPENDVERFILE,
                READFILE_INVALIDPATHVER,
                READFILE_FAILPARSERVER,
                READFILE_INVALIDPATH,//La direecion especificada  del archivo especificada no es valida
                CONFIGUREPROJECT_FAIL_ON_MOVE_FILE,
                CONFIGUREPROJECT_PARSE_XML,
                CONFIGUREPROJECT_NOCOMPATIBLE_PROJECT,
                CONFIGUREPROJECT_NOFULL_PATCH_PROJECT,
                
                APPLICATION_GTK3_ROWACTIVE_NOTFOUND_TABLE,
                APPLICATION_GTK3_CONFIGPROJECT_NULL,
                
                BD_BAT_NAMETABLE
                
        };
        
        /*toolkit::Error getError();
        bool checkError();
        bool writeError(toolkit::Error* e);*/
}
}
#endif
