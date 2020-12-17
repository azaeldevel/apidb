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

#ifndef APIDB_GENERATOR_HPP
#define APIDB_GENERATOR_HPP

#include <fstream>
#if defined WINDOWS_MINGW
    #include <shell.hh>
#else
    #include <octetos/coreutils/shell.hh>
#endif


namespace octetos
{
namespace apidb
{
	namespace generators
	{
        /**
         * @brief genera una llada recursiva para el componete indicado
         * @condition se debe llamar con k->symbolReferenced
         * @condition en caso de que el valopr sea no string se deve realizar la comversion en la funcion llamadora
         */
        void getKey(std::ofstream& ofile, const symbols::Symbol* k);
        /**
         * @brief genera una llada recursiva para el componete indicado
         * @condition se debe llamar con k->symbolReferenced
         * @condition en caso de que el valopr sea no string se deve realizar la comversion en la funcion llamadora
         */
        void getKey2(std::ofstream& ofile, const symbols::Symbol* k);
        
        void insertParamsRaw(std::ofstream& ofile,symbols::Symbol* k,symbols::Symbol* parent);
        void insertValueRaw(std::ofstream& ofile,symbols::Symbol* k,symbols::Symbol* parent);
        symbols::Symbol* getRootSymbol(symbols::Symbol* k);
        
        /**
        * \private No es parte del API
        * */
		class Generator
		{
		public:
			virtual bool generate(bool log) = 0;
			OutputLenguajes getOutputLenguaje() const;
			std::string getOutputLenguajeString()const;
			PackingLenguajes getPackingLenguaje() const;
			std::string getPackingLenguajeString()const;
			Generator(const ConfigureProject& config,apidb::Analyzer& analyzer);

		protected:
			//OutputLenguajes outputLenguaje;//se
			const ConfigureProject& configureProject;
			apidb::Analyzer& analyzer;
			const symbols::SymbolsTable& getSymbolsTable()const;
			coreutils::Shell shell;
		};

		/**
		* \private
		* */
		class CPP : public Generator
		{
		public:
			virtual bool generate(bool log);
			CPP(apidb::Analyzer&,const ConfigureProject&);
			const std::string& getHeaderName() const;
			std::ofstream& getSourceOutput();
			std::ofstream& getHeaderOutput();
			virtual ~CPP();

		private:
            //contructor
			void writeDefaultContructorH(const apidb::symbols::Table&,std::ofstream&);
			void writeDefaultContructorCPP(const apidb::symbols::Table&,std::ofstream&);
			void writeKeyRawDataContructorH(const apidb::symbols::Table&,std::ofstream&);
			void writeKeyRawDataContructorCPP(const apidb::symbols::Table&,std::ofstream&);
			void writeCopyContructorH(const apidb::symbols::Table&,std::ofstream&);
			void writeCopyContructorCPP(const apidb::symbols::Table&,std::ofstream&);
            
            //selects
			void writeSelectStaticH(const apidb::symbols::Table& table, std::ofstream& ofile);
			void writeSelectStaticCPP(const apidb::symbols::Table&,std::ofstream&);
            void writeSelectInstancetObjectDataH(const apidb::symbols::Table&,std::ofstream&);
            void writeSelectInstancetObjectDataCPP(const apidb::symbols::Table&,std::ofstream&);
            
            //downloads
			void writeDownloadsH(const apidb::symbols::Table& table, std::ofstream& ofile);
			void writeDownloadsCPP(const apidb::symbols::Table&,std::ofstream&);
            
            //getter
            void writeGettersH(const apidb::symbols::Table& table, std::ofstream& ofile);
            void writeGettersCPP(const apidb::symbols::Table& table, std::ofstream& ofile);
            
            //updatter
            void writeUppdatersH(const apidb::symbols::Table& table, std::ofstream& ofile);
            void writeUppdatersCPP(const apidb::symbols::Table& table, std::ofstream& ofile);
            
            //inserts
			void writeInsertH(const apidb::symbols::Table&,std::ofstream&);
			void writeInsertCPP(const apidb::symbols::Table&,std::ofstream&);            
            
            //methodes
			void createClassMethodesH(const apidb::symbols::Table&,std::ofstream&);
			void createClassMethodesCPP(const apidb::symbols::Table&,std::ofstream&);            
            
			//varias
			bool createH(std::ofstream& file,bool log,const symbols::SymbolsTable&);
			bool createH(std::ofstream& file,bool log,const symbols::ISpace*);
			void createClassH(const apidb::symbols::Table&,std::ofstream&,const std::string&,bool log);
			void createClassPrivateH(std::ofstream&);
			void createClassAttributesH(const apidb::symbols::Table&,std::ofstream&);
			void createClassPublicH(std::ofstream&);
			bool createCPP(std::ofstream& file,bool log,const symbols::SymbolsTable&);
			bool createCPP(std::ofstream& file,bool log,const symbols::ISpace*);
			void createClassCPP(const apidb::symbols::Table&,std::ofstream&,const std::string&);
			void createClassPrivateCPP(std::ofstream&);
			void createClassAttributesCPP(const apidb::symbols::Table&,std::ofstream&);
			void createClassPublicCPP(std::ofstream&);
			void writeKeyValueH(const apidb::symbols::Table&,std::ofstream&);
			void writeKeyValueCPP(const apidb::symbols::Table&,std::ofstream&);
			bool createDatconnectHPP(std::ofstream& file,bool log);
            bool createDatconnectCPP(std::ofstream& file,bool log);

			//apidb::Analyzer& analyzer;
			std::ofstream* writeResults;//erreglo de writeoutput files
			std::string projectH;
			std::string projectCPP;
		};

		/**
		* \private
		* */
		class Java : public Generator
		{
		public:
			virtual bool generate(bool log);
			Java(apidb::Analyzer&,const ConfigureProject&);
			const std::string& getHeaderName() const;
			std::ofstream& getSourceOutput();
			virtual ~Java();

		private:
			bool create(bool log,const symbols::SymbolsTable&);
			bool create(std::ofstream& file,bool log,const symbols::ISpace*);
			void createClass(const apidb::symbols::Table&,std::ofstream&,const std::string&);
			void createClassPrivate(std::ofstream&);
			void createClassAttributes(const apidb::symbols::Table&,std::ofstream&);
			void createClassPublic(std::ofstream&);
			void createClassMethodes(const apidb::symbols::Table&,std::ofstream&);
			void writeDefaultContructor(const apidb::symbols::Table&,std::ofstream&);
			void writeKeyValue(const apidb::symbols::Table&,std::ofstream&);
			void writeCopyContructor(const apidb::symbols::Table&,std::ofstream&);
			void writeKeyContructor(const apidb::symbols::Table&,std::ofstream&);
			void writeInsert(const apidb::symbols::Table&,std::ofstream&);
			void writeDownloads(const apidb::symbols::Table&,std::ofstream&);
			void writeSelects(const apidb::symbols::Table&,std::ofstream&);
			bool createDatconnect(std::ofstream& file,bool log);

			std::ofstream* writeResult;//erreglo de writeoutput files
 			std::string filename;
		};

		/**
		* \private No es parte del API
		* */
		class CMake : public Generator
		{
		public:
			virtual bool generate(bool log);
			virtual ~CMake();
			CMake(apidb::Analyzer&,const ConfigureProject&);

		private:
			//apidb::Analyzer& analyzer;
			std::ofstream cmakelists;
			std::ofstream toolkitcommonconifg;
			std::ofstream toolkitclientdbConfig;
			std::ofstream config;
			std::ofstream developing;
			//Options options;
			//const ConfigureProject& configureProject;
		};


		/**
		* \private No es parte del API
		* */
		class Maven : public Generator
		{
		public:
			virtual bool generate(bool log);
			virtual ~Maven();
			Maven(apidb::Analyzer&,const ConfigureProject&);

		private:
			std::string filename;
		};
	}

}
}
#endif
