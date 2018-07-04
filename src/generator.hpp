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
		};
	
	}
		
}
