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
#ifndef APIDB_GENERATOR_HPP
#define APIDB_GENERATOR_HPP

#include <fstream>

 namespace apidb
{
	namespace generators
	{
		class Generator
		{
		public:			
			virtual bool generate() = 0;
			OutputLenguajes getOutputLenguaje() const;
			std::string getOutputLenguajeString()const;	
			Generator(const ConfigureProject& config);
			
		protected:		
			//OutputLenguajes outputLenguaje;//se 
			const ConfigureProject& configureProject;
		};
		
		class CPP : public Generator
		{
		public:
			virtual bool generate();
			CPP(apidb::Analyzer&,const ConfigureProject&);
			const std::string& getHeaderName() const;
			std::ofstream& getSourceOutput();
			std::ofstream& getHeaderOutput();
			virtual ~CPP();
			
		private:
			void createSpaceH(std::ofstream& file,const ConfigureProject&);
			void createClassH(const apidb::symbols::Table&,std::ofstream&,const std::string&,const ConfigureProject&);
			void createClassPrivateH(std::ofstream&);
			void createClassAttributesH(const apidb::symbols::Table&,std::ofstream&);
			void createClassPublicH(std::ofstream&);
			void createClassMethodesH(const apidb::symbols::Table&,std::ofstream&,const ConfigureProject&);
			void createSpaceCPP(std::ofstream& file,const ConfigureProject&);
			void createClassCPP(const apidb::symbols::Table&,std::ofstream&,const std::string&,const ConfigureProject&);
			void createClassPrivateCPP(std::ofstream&);
			void createClassAttributesCPP(const apidb::symbols::Table&,std::ofstream&);
			void createClassPublicCPP(std::ofstream&);
			void createClassMethodesCPP(const apidb::symbols::Table&,std::ofstream&,const ConfigureProject&);
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
            void writeDownloadsH(const apidb::symbols::Table& table, std::ofstream& ofile,const ConfigureProject&);
            void writeDownloadsCPP(const apidb::symbols::Table&,std::ofstream&,const ConfigureProject&);
            void writeSelectsH(const apidb::symbols::Table& table, std::ofstream& ofile,const ConfigureProject&);
            void writeSelectsCPP(const apidb::symbols::Table&,std::ofstream&,const ConfigureProject&);
			
			apidb::Analyzer& analyzer;
			std::ofstream* writeResults;//erreglo de writeoutput files
			std::string projectH;
			std::string projectCPP;
		};
		
		
		class CMake : public Generator
		{
		public:			
			virtual bool generate();
			virtual ~CMake();
			CMake(apidb::Analyzer&,const ConfigureProject&);
			
		private:
			apidb::Analyzer& analyzer;
			std::ofstream cmakelists;
			std::ofstream toolkitcommonconifg;
			std::ofstream toolkitclientdbConfig;
			std::ofstream config;
			std::ofstream developing;
			//Options options;
			const ConfigureProject& configureProject;
		};	
	}
		
}
#endif
