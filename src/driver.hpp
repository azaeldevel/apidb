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

#ifndef APIDB_DRIVER_HPP
#define APIDB_DRIVER_HPP

#include "analyzer.hpp"
#include "generator.hpp"

//On INFORMATION_SCHEMA
//select TABLE_SCHEMA,TABLE_NAME,COLUMN_NAME,ORDINAL_POSITION,REFERENCED_TABLE_SCHEMA from KEY_COLUMN_USAGE WHERE TABLE_SCHEMA='muposys-0-alpha';
//select TABLE_CATALOG,TABLE_SCHEMA,TABLE_NAME,COLUMN_NAME,ORDINAL_POSITION,DATA_TYPE,COLUMN_KEY  from COLUMNS WHERE TABLE_SCHEMA='muposys-0-alpha';

namespace octetos
{
namespace apidb
{

struct Field
{
        const char* name;        
};
struct Table : public std::vector<Field>
{
        const char* name;
};
struct Space : public std::vector<Table>
{
        const char* name;
};

class Input
{
public:
        virtual void read(const char* space) = 0;
        
protected:
        std::vector<Space> space;
};
/**
* \brief SQL estandar
* */
class InputSQL: public Input
{
public:
        InputSQL(octetos::db::Connector&);
        InputSQL(const octetos::db::Datconnect&);
        InputSQL(const char* server,unsigned int port, const char* user,const char* password);        
        virtual ~InputSQL();
        virtual void read(const char* space);
        virtual void listing_tables(const char* space);
        
        static const char* schema_name;
        
protected:
        octetos::db::Connector* conn;
        bool conn_free;
        
};
/**
* \brief Es un analizador compatible en lo posible con MariaDB/MySQL, su funcion es servir como base para los analizadores especificos correspondientes.
* */
class InputMM: public InputSQL
{
public:
        InputMM(octetos::db::Connector&);
        InputMM(const octetos::db::Datconnect&);
        InputMM(const char* server,unsigned int port, const char* user,const char* password);
        virtual void read(const char* space);
};
class InputMySQL: public InputMM
{
public:
        InputMySQL(octetos::db::Connector&);
        virtual void read(const char* space);
};
class InputMaria: public InputMM
{
public:
        InputMaria(octetos::db::Connector&);
        InputMaria(const octetos::db::Datconnect&);
        InputMaria(const char* server,unsigned int port, const char* user,const char* password);
        virtual void read(const char* space);
};


class Output
{
public:
        Output(const Input&);
        virtual void write(std::ofstream&) = 0;
protected:
        const Input& in;
};
class OutputAPIAcces : public Output
{
public:
        OutputAPIAcces(const Input&);
        virtual void write(std::ofstream&);
protected:
};
class OutputGtkmm : public Output
{
public:
        OutputGtkmm(const Input&);
        virtual void write(std::ofstream&);
protected:
};

        
        /**
         * \brief Responsable de manejar todos el proceso desde el analisis hasta la generacion de los archivos fuentes
         * */
	class Driver
	{
	public:
                /**
                 * \deprecated Lee este valor directamente de ConfigureProject
                 * */
		OutputLenguajes getOutputLenguaje() const;
                
                /**
                 * \deprecated Ya que esta funcion genera log  mediante la salida estandar sera removido en favor de toolkit::ActivityProgress en v2 
                 * \brief Lee el servidor
                 * */
		virtual bool analyze(core::ActivityProgress* progress);
                /**
                 * \deprecated Ya que esta funcion genera log  mediante la salida estandar sera removido en favor de toolkit::ActivityProgress en v2 
                 * \brief Genera los archivos de codigo
                 **/
		virtual bool generate(core::ActivityProgress* progress);
                /**
                 * \deprecated Ya que esta funcion genera log  mediante la salida estandar sera removido en favor de toolkit::ActivityProgress en v2 
                 * \brief Se puede considar como una llamada a 'analyze' seguida de una llamda a 'generate'
                 * */
		bool driving(core::ActivityProgress* progress);
                
                /**
                 * \deprecated Ya que esta funcion genera log  mediante la salida estandar sera removido en favor de toolkit::ActivityProgress en v2 
                 * \brief Lee el servidor
                 * */
		//virtual bool analyze(bool log);
                /**
                 * \deprecated Ya que esta funcion genera log  mediante la salida estandar sera removido en favor de toolkit::ActivityProgress en v2 
                 * \brief Genera los archivos de codigo
                 **/
		//virtual bool generate(bool log);
                /**
                 * \deprecated Ya que esta funcion genera log  mediante la salida estandar sera removido en favor de toolkit::ActivityProgress en v2 
                 * \brief Se puede considar como una llamada a 'analyze' seguida de una llamda a 'generate'
                 * */
		//bool driving(bool log);
                
                /**
                 * \brief Unico contructor 
                 * */
        [[deprecated("Working in new modele")]]Driver(ConfigureProject&);
        Driver(const ConfigureProject&,const octetos::db::Datconnect&,apidb::Input&,apidb::Output&);
        /**
        * \brief Retorna el obejeto Analyzer utilizado.
        * */
        const Analyzer& getAnalyzer() const;
        virtual ~Driver();                
        bool getTablesName(std::list<std::string>& retList,symbols::Space* space = NULL)const;
        bool getFiledsName(std::list<std::string>& retList,const std::string& table)const;
        
        bool loadLibrary();
        
	private:
		octetos::db::Connector* connector;		
        apidb::Analyzer* analyzer;
		const ConfigureProject& configureProject;
        //void* handle;
        octetos::db::Connector* (*createConnector)();
        
        //>>>>
        void (*destroyAnalyzer)(octetos::apidb::Analyzer*);
        apidb::Analyzer* (*createAnalyzer)(const octetos::apidb::ConfigureProject*,octetos::db::Connector*,octetos::core::ActivityProgress*);
        
        //new model
        bool backward;
        apidb::Input* in;
        apidb::Output* out;
	};
}
}

#endif

