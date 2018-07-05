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
			
		protected:		
			OutputLenguajes outputLenguaje;//se 
		};
		
		class CPP : public Generator
		{
		public:
			virtual bool generate();
			CPP(apidb::Analyzer&);
			const std::string& getHeaderName() const;
			std::ofstream& getSourceOutput();
			std::ofstream& getHeaderOutput();
			virtual ~CPP();
			
		private:
			void createSpaceH(std::ofstream& file);
			void createClassH(const apidb::internal::Table&,std::ofstream&,const std::string&);
			void createClassPrivateH(std::ofstream&);
			void createClassAttributesH(const apidb::internal::Table&,std::ofstream&);
			void createClassPublicH(std::ofstream&);
			void createClassMethodesH(const apidb::internal::Table&,std::ofstream&);
			void createSpaceCPP(std::ofstream& file);
			void createClassCPP(const apidb::internal::Table&,std::ofstream&,const std::string&);
			void createClassPrivateCPP(std::ofstream&);
			void createClassAttributesCPP(const apidb::internal::Table&,std::ofstream&);
			void createClassPublicCPP(std::ofstream&);
			void createClassMethodesCPP(const apidb::internal::Table&,std::ofstream&);
			void writeDefaultContructorH(const apidb::internal::Table&,std::ofstream&);
			void writeDefaultContructorCPP(const apidb::internal::Table&,std::ofstream&);
			void writeKeyValueH(const apidb::internal::Table&,std::ofstream&);
			void writeKeyValueCPP(const apidb::internal::Table&,std::ofstream&);
			void writeCopyContructorH(const apidb::internal::Table&,std::ofstream&);
			void writeCopyContructorCPP(const apidb::internal::Table&,std::ofstream&);
			void writeKeyContructorH(const apidb::internal::Table&,std::ofstream&);
			void writeKeyContructorCPP(const apidb::internal::Table&,std::ofstream&);
			void writeInsertH(const apidb::internal::Table&,std::ofstream&);
			void writeInsertCPP(const apidb::internal::Table&,std::ofstream&);
			
			apidb::Analyzer* analyzer;
			std::ofstream* writeResults;//erreglo de writeoutput files
			std::string projectH;
			std::string projectCPP;
		};
		
		
		class CMake : public Generator
		{
		public:
			struct Options
			{
				toolkit::Version cmake_minimun_requiered;
				struct Project
				{
					std::string name;
					toolkit::Version version;
					OutputLenguajes lenguague;
					std::string directory;
				};
				Project project;
			};
			
			virtual bool generate();
			virtual ~CMake();
			CMake(apidb::Analyzer&);
			
		private:
			apidb::Analyzer* analyzer;
			std::ofstream cmakelists;
			std::ofstream toolkitcommonconifg;
			std::ofstream toolkitclientdbConfig;
			std::ofstream config;
			std::ofstream developing;
			Options options;
		};
	
	}
		
}
