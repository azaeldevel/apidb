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
        class ConfigureProject
        {
        private:
                bool processNode(xmlTextReaderPtr);
                bool getProjectNodes(xmlTextReaderPtr);
                
        public:      
                class Parameters : public std::vector<const char*>
                {
                public:
                        ~Parameters();
                };        
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
                        void setHeader(Parameters*);
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
                class Table : public std::map<const char*, const Function*>
                {
                private:
                        std::string name;
                public:
                        const std::string& getName() const;
                        Table(const std::string&);
                        Table();
                };
        
                std::string name; 
                std::string directory;
                octetos::toolkit::Version version;
                octetos::toolkit::clientdb::mysql::Datconnect* conectordb;
                InputLenguajes inputLenguaje;
                OutputLenguajes outputLenguaje;
                PackingLenguajes packing;
                Compiled compiled;
                MVC mvc;
                std::map<const char*,Table*> downloads;//to fix: convertir el tipo en Table* ya que la myoria de las operaciones usa sub elementos creados dinamin¡camente
                std::map<const char*,Table*> selects;//to fix: convertir el tipo en Table* ya que la myoria de las operaciones usa sub elementos creados dinamin¡camente
                //
                octetos::toolkit::Version projectVersion;
                
                //ConfigureProject(std::string filename);
                //ConfigureProject(const ConfigureProject& configProy);
                ConfigureProject();
                bool saveConfig();
                bool checkXML(xmlTextReaderPtr);
                bool readConfig(std::string filename);
                bool isError();
                toolkit::Error getError();
                        
                const std::string& getName()const;
                const std::string& getDirectory()const;
                const octetos::toolkit::Version& getVersion()const;
                const octetos::toolkit::clientdb::mysql::Datconnect& getConector()const;
    };
}
}

#endif
