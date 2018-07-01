namespace apidb
{
	class CPPGenerator
	{
    public:
		virtual bool generate();
		CPPGenerator(apidb::Analyzer&);
		const std::string& getHeaderName() const;
		std::ofstream& getSourceOutput();
		std::ofstream& getHeaderOutput();
		~CPPGenerator();
		
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
}
