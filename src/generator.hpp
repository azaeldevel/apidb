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
namespace octetos
{
 namespace apidb
{
	namespace generators
	{
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
		};
		
                /**
                 * \private No es parte del API
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
			void createSpaceH(std::ofstream& file,bool log);
			void createClassH(const apidb::symbols::Table&,std::ofstream&,const std::string&,bool log);
			void createClassPrivateH(std::ofstream&);
			void createClassAttributesH(const apidb::symbols::Table&,std::ofstream&);
			void createClassPublicH(std::ofstream&);
			void createClassMethodesH(const apidb::symbols::Table&,std::ofstream&);
			void createSpaceCPP(std::ofstream& file);
			void createClassCPP(const apidb::symbols::Table&,std::ofstream&,const std::string&);
			void createClassPrivateCPP(std::ofstream&);
			void createClassAttributesCPP(const apidb::symbols::Table&,std::ofstream&);
			void createClassPublicCPP(std::ofstream&);
			void createClassMethodesCPP(const apidb::symbols::Table&,std::ofstream&);
			void writeDefaultContructorH(const apidb::symbols::Table&,std::ofstream&);
			void writeDefaultContructorCPP(const apidb::symbols::Table&,std::ofstream&);
			void writeKeyValueH(const apidb::symbols::Table&,std::ofstream&);
			void writeKeyValueCPP(const apidb::symbols::Table&,std::ofstream&);
			void writeCopyContructorH(const apidb::symbols::Table&,std::ofstream&);
			void writeCopyContructorCPP(const apidb::symbols::Table&,std::ofstream&);
			void writeKeyContructorH(const apidb::symbols::Table&,std::ofstream&);
			void writeKeyContructorCPP(const apidb::symbols::Table&,std::ofstream&);
			void writeInsertH(const apidb::symbols::Table&,std::ofstream&);
			void writeInsertCPP(const apidb::symbols::Table&,std::ofstream&);
                        //void writeSelectH(const apidb::symbols::Table& table, std::ofstream& ofile);
                        //void writeSelectCPP(const apidb::symbols::Table&,std::ofstream&);
                        void writeDownloadsH(const apidb::symbols::Table& table, std::ofstream& ofile);
                        void writeDownloadsCPP(const apidb::symbols::Table&,std::ofstream&);
                        void writeSelectsH(const apidb::symbols::Table& table, std::ofstream& ofile);
                        void writeSelectsCPP(const apidb::symbols::Table&,std::ofstream&);
			
			//apidb::Analyzer& analyzer;
			std::ofstream* writeResults;//erreglo de writeoutput files
			std::string projectH;
			std::string projectCPP;
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
	}
		
}
}
#endif
