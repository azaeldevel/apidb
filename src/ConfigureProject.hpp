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

#ifndef APIDB_CONFIGUREPROJECT_HPP
#define APIDB_CONFIGUREPROJECT_HPP

 
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
    public:
        /**
        * \brief Almacena la lista de parametros que le corresponde a una Funcion
        */
        class Parameters : public std::vector<std::string>
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
			/**
			* \brief Nombre de la funcion
			* */
			const std::string& getName() const;
			/**
			* \brief Simplementa contrulle la funcion con su nombre
			* */
			Function(const std::string&);
			/**
			* \brief Simplemente limpia los parametros
			* */
			Function();
			/**
			* \brief Libera la memoria requerid por el objeto
			* */
			~Function();
			/**
			* \private Funcion en desarrollo aun.
			* */
			void setBody(const std::string&);
			/**
			* \brief Retorna el obejto Parameters
			* */
			const Parameters* getParameters() const;
			/**
			* \brief Lista de paramatros en formato correcto.
			* */
			std::string listParams()const;
			/**
			* \brief Agrega un nuevo parametro a la funcion.
			* */
			void addParam(const std::string& p);
        private:
			/**
			* \brief Nombre de la funcion
			* */
			std::string name;
			/**
			* \deprecated Funcion ahora redundante ya que hay una estructra de datos para cada skeleton. El analizaro o el generador no usan esta informacion sera removido en v2
			* */
			//Skeleton skeleton;
			/**
			* \brief Parametros interno
			* */
			Parameters* header; 
			/**
			* \private en desarrollo.
			* */
			std::string body;
		};
		
		/**
		* \brief Almacena la descripcion de una tabla
		* */
		class Table : public std::map<const char*, const Function*,symbols::cmp_str>
		{
		private:
			/**
			* \brief Nombre completo de la tabla
			* */
			std::string name;
		public:
			/**
			* \brief Retorna el nombre completo de la tabla.
			* */
			const std::string& getName() const;
			/**
			* \brief Crea el objeto inicializando el nombre de la tabla.
			**/
			Table(const std::string&);                        
			~Table();
		};
        
        
        enum Platform
        {
            NotPlatform,
            Linux_Debian,
            Linux_Arch,
            Linux_Gentoo,
            Windows_Mingw
        };
    ///>>>>>>>>>>>>>>>>>>>>>>>>>Owner mebers    
    private:
        bool processNode(xmlTextReaderPtr);
        bool getProjectNodes(xmlTextReaderPtr);
        /**
        * \brief la version del archivo de proyecto.
        * */
        core::Semver projectVersion;
        
        void* handle;
        octetos::db::Datconnect* (*createDatConnection)();
        void (*destroyDatConnection)(octetos::db::Datconnect*);
        octetos::db::Connector* (*createConnector)();
        void (*destroyConnector)(octetos::db::Connector*);
        bool loadLibrary();
        bool checkLibrary(InputLenguajes in);
        /**
        * \brief Identifica el tipo del Servidor de base de datos
        * */
        InputLenguajes inputLenguaje;
        /**
        * \brief Información de conexión a la base de datos, esta opcion sera removida en v5
        * \deprecated Sera removido de la interface publica en v5, usar getDatconnection() en su lugar.
        **/
        octetos::db::Datconnect* conectordb;
        
        void* getfnDatConection();
        void* getfnCreateConector();
        //bool enabledPostgreSQl;
        //bool enabledMariaDB;
        //bool enabledMySQL;
        bool failLoadDat;
        Platform platform;
        
    public:
    	bool checkFailLoadDat()const;
        void* getLibraryHandle() const;
        octetos::db::Datconnect* newDatConnection();
        void deleteDatConnection(octetos::db::Datconnect*);
        octetos::db::Connector* newConnector()const;
        void deleteConnector(octetos::db::Connector*)const;
        void setInputLenguaje(InputLenguajes);
        void setInputs(InputLenguajes,octetos::db::Datconnect&);
        InputLenguajes getInputLenguaje()const;
        ~ConfigureProject();
        
        const Table* findSelectTable(const std::string&)const;
        const Table* findDownloadTable(const std::string&)const;
		bool build();
		
		ConfigureProject();
		/**
		* \brief Guarda el proyecto en el archivo especificado
		* */
		void saveConfig(const std::string& filename);
		/**
		* \brief Lee el proyecto desde el archivo especificado
		* */
		void readConfig(const std::string& filename);
		/**
		* \brief Verica los datos de conexion al servidor
		**/
		bool testConexion();
      	
      	bool unmcompress(const std::string& filename,char* tmp_apidbDir);
      	bool compress(const std::string& tarf, char * tmp_apidbDir,const std::string& filenameProj);
        /**
        * \brief Version del archivo de proyecto.
        * \details Contiene la version del paquete apidb usado paara generar/guardar dicho archivo.
        **/
        const core::Semver& getVersionProject()const;
        
        octetos::db::Datconnect*  getDatconnection() const;
        
        Platform getPlatform()const;
		
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
		core::Semver versionResult;
		/**
		* \brief Identifica El lenguaje en que se genera el codigo fuente.
		* */
		OutputLenguajes outputLenguaje;
		/**
		* \brief Identica el tipo de gestos de paquetes que usara para contruir el proyecto resultado.
		* */
		PackingLenguajes packing;
		/**
		* \brief Edentifica el tipo de libreria que generara(statica o dinamica).
		* */
		Compiled compiled;
		/**
		* \brief Lista las tablas para las que se generara funciones de descarga
		* */
		std::map<const char*,Table*,symbols::cmp_str> downloads;//to fix: convertir el tipo en Table* ya que la myoria de las operaciones usa sub elementos creados dinamin¡camente
		/**
		* \brief Lista las tablas para las que se generara funciones de Selects
		* */
		std::map<const char*,Table*,symbols::cmp_str> selects;//to fix: convertir el tipo en Table* ya que la myoria de las operaciones usa sub elementos creados dinamin¡camente                
		/**
		* \brief Agrega la creacion de un Ejecutalbe
		* \details Si se asigna esta variable se creara el ejecutble con el mismo nobre a apartir de un archivo con el mimos nombre pero extencion del lenguaje inicado en  'outputLenguaje'.
		* */
		std::string executable_target;
		/**
		* \brief Esta opción indica el comportamiento de APIDB cuan las tablas tiene formato de espacios
		* \details En MySQL se aceptan puntos en los nombre de las tablas, sin embargo, en C++ no se puede crear clases cuyo nombre contenga puntos, APIDB le ofrece la opción algunas opciones para menejar este inconbeniente. Use 'emulate' para apegarce a la definicion de espacion segun el lenguaje de salida selecionado. Use 'reject' o deje en blanco para no aceptar.
		* */
		std::string namespace_detect;
		/**
		* \brief Sí es true genera una variable statica global con los datos de conexión.
		* */
		std::string writeDatconnect;
    };
}
}

#endif
