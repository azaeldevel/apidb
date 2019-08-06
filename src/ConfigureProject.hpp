#ifndef APIDB_CONFIGUREPROJECT_HPP
#define APIDB_CONFIGUREPROJECT_HPP
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
 
#include "common.hpp"

namespace octetos
{
namespace apidb
{
        /**
         * \brief Encargada de interactuar con el archivo de proyecto
         * */
        class ConfigureProject
        {
        private:
                bool processNode(xmlTextReaderPtr);
                bool getProjectNodes(xmlTextReaderPtr);
                /**
                 * \brief la version del archivo de proyecto.
                 * */
                octetos::toolkit::Version projectVersion;
                
        public:      
                /**
                 * \brief Almacena la lista de parametros que le corresponde a una funcion
                 * \see Funtion
                 */
                class Parameters : public std::vector<const char*>
                {
                public:
                        ~Parameters();
                };        
                /**
                 * \brief Almacena de descripcion de una funcion
                 * */
                class Function
                {
                public:
                        enum Skeleton
                        {
                                DOWNLOAD,
                                SELECT,
                                INSERT,
                                UPDATE,
                                COSTUM
                        };
                        const std::string& getName() const;
                        Function(const std::string&,Skeleton skeleton);
                        Function(const std::string&);
                        Function();
                        ~Function();
                        Skeleton getSkeleton() const;
                        //void setHeader(Parameters*);
                        void setBody(const std::string&);
                        const Parameters* getParameters() const;
                        std::string listParams()const;
                        void addParam(const char* p);
                private:
                        std::string name;
                        Skeleton skeleton;
                        Parameters* header; 
                        std::string body;
                };
                /**
                 * \brief Almacena la descripcion de una tabla
                 * */
                class Table : public std::map<const char*, const Function*>
                {
                private:
                        std::string name;
                public:
                        const std::string& getName() const;
                        Table(const std::string&);
                        Table();
                };
        
                /**
                 * \brief El nombre del proyecto.
                 */
                std::string name; 
                /**
                 * \brief El directorio donde se colocará los archivos fuentes de la API generada.
                 * */
                std::string builDirectory;
                /**
                 * \brief La version del proyecto resultado.
                 * */
                octetos::toolkit::Version version;
                /**
                 * \brief Información de conexión a la base de datos
                 * */
                octetos::toolkit::clientdb::mysql::Datconnect* conectordb;
                /**
                 * \brief Identifica el tipo del Servidor de base de datos
                 * */
                InputLenguajes inputLenguaje;
                /**
                 * \biref Identifica El lenguaje en que se genera el codigo fuente.
                 * */
                OutputLenguajes outputLenguaje;
                /**
                 * \brief Identica el tipo de gestos de paquetes que usara para contruir el proyecto resultado.
                 * */
                PackingLenguajes packing;
                /**
                 * \biref Edentifica el tipo de libreria que generara(statica o dinamica).
                 * */
                Compiled compiled;
                MVC mvc;
                /**
                 * \brief Lista las tablas para las que se generara funciones de descarga
                 * */
                std::map<const char*,Table*> downloads;//to fix: convertir el tipo en Table* ya que la myoria de las operaciones usa sub elementos creados dinamin¡camente
                /**
                 * \brief Lista las tablas para las que se generara funciones de Selects
                 * */
                std::map<const char*,Table*> selects;//to fix: convertir el tipo en Table* ya que la myoria de las operaciones usa sub elementos creados dinamin¡camente

                
                //ConfigureProject(std::string filename);
                //ConfigureProject(const ConfigureProject& configProy);
                ConfigureProject();
                /**
                 * \brief Guarda el proyecto en el archivo especificado
                 * */
                bool saveConfig(const std::string& filename = "");
                /**
                 * \brief Lee el proyecto desde el archivo especificado
                 * */
                bool readConfig(std::string filename);
                /**
                 * \brief Verica los datos de conexion al servidor
                 **/
                bool testConexion();
                        
                const std::string& getName()const;
                const octetos::toolkit::Version& getVersion()const;
                const octetos::toolkit::clientdb::mysql::Datconnect& getConector()const;
    };
}
}

#endif
