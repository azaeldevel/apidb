#include <cctype>
#include <fstream>
#include <cassert>
#include <sstream> 

#include "analyzer.hpp"
namespace octetos
{
namespace apidb
{	
	

	BuildException::~BuildException() throw()
	{
		;
	}
	const char* BuildException::what() const throw()
	{
		return description.c_str();
	}
        BuildException::BuildException(const std::string &description) throw()
        {
		this->description = description;
	}
	

	
	std::ostream& Analyzer::getErrorMessage()
	{
		return *errorMessages;
	}		
	std::ostream& Analyzer::getOutputMessage()
	{
		return *outputMessages;
	}
	Analyzer::~Analyzer()
	{
	}
	Analyzer::Analyzer(const ConfigureProject& config,octetos::toolkit::clientdb::Connector* conn) : configureProject(config), connector(conn)
	{
	}
	OutputLenguajes Analyzer::getOutputLenguaje() const
	{
		return configureProject.outputLenguaje;
	}
	std::map<const char*,symbols::Tables*,symbols::cmp_str> Analyzer::copyListTable() const
	{
		return spacies;
	}	
	const std::map<const char*,symbols::Tables*,symbols::cmp_str>& Analyzer::getListTableConst() const
	{
		return spacies;
	}
	std::map<const char*,symbols::Tables*,symbols::cmp_str>& Analyzer::getListTable() 
	{
		return spacies;
	}		
	const std::string& Analyzer::getNameProject()
	{
		return configureProject.name;
	}		
	const std::string& Analyzer::getDirectoryProject()
	{
		return configureProject.directory;
	}	
	InputLenguajes Analyzer::getInputLenguaje()const
	{
		return configureProject.inputLenguaje;
	}
	
	
	
namespace mysql
{
	bool Analyzer::analyze(bool log)
	{
		bool flag = symbols::listing(*(octetos::toolkit::clientdb::mysql::Connector*)connector,spacies);
                
                for(auto const& [keySpace, AttSpace]  : spacies)
                {
                        for(apidb::symbols::Table* table : *AttSpace) //reading attrubtes by table
                        {
                                //for (auto const& [key, attribute] : *table)
                                if(log)  *outputMessages << "\tCreating basic simbols for " << table->name  << "." << std::endl;
                                //simbolos basicos 
                                if(!table->basicSymbols(*connector))
                                {
                                        //std::cerr<<"Faill on basicSymbols"<<std::endl;
                                        return false;
                                }
                        }
                }
                
                for(auto const& [keySpace, AttSpace]  : spacies)
                {
                for(auto table: *AttSpace) //reading attrubtes by table
                {
                        //foreign key's
                        if(!table->fillKeyType(*connector,getListTable()))
                        {
                                //std::cerr<<"Faill on fillKeyType"<<std::endl;
                                return false;
                        }
                }
                }
                //std::cout<<"Step 3."<<std::endl;
                for(auto const& [keySpace, AttSpace]  : spacies)
                {
                for(auto table: *AttSpace) //reading attrubtes by table
                {
                        for (auto const& [key, attribute] : *table)
                        {
                                //std::cout<<"\t"<<attribute->inType<<std::endl;
                                attribute->outType = parse(attribute->inType);
                        }
                }
                }
                //std::cout<<"Step 4."<<std::endl;
                return flag;
	}
	Analyzer::Analyzer(const ConfigureProject& config,octetos::toolkit::clientdb::Connector* conn) : apidb::Analyzer(config,conn)
	{
		outputMessages = &std::cout;	  
		errorMessages = &std::cerr; 
	}
	

	
	/*Analyzer::Analyzer()
	{
	   //deafults
	   outputLenguaje = OutputLenguajes::CPP;
	   inputLenguaje = InputLenguajes::MySQL_Server;
	   outputMessages = &std::cout;	  
	   errorMessages = &std::cerr; 
	}*/
	
	void Analyzer::message(const std::string& msg)
	{
		(*outputMessages)<<msg<<std::endl;
	}
	std::string Analyzer::parse(const std::string& line)
	{
		std::istringstream text(line);
		parse(text);
		return oneLine;
	}
	Analyzer::~Analyzer()
	{
		delete(scanner);
		scanner = nullptr;
		delete(parser);
		parser = nullptr;	   
	}

	void Analyzer::parse( const char * const filename )
	{
	   assert( filename != nullptr );
	   std::ifstream in_file( filename );
	   if( ! in_file.good() )
	   {
		   exit( EXIT_FAILURE );
	   }
	   parse_helper( in_file );
	   return;
	}
	
	void Analyzer::parse( std::istream &stream )
	{
	   if( ! stream.good()  && stream.eof() )
	   {
		   return;
	   }
	   //else
	   parse_helper( stream ); 
	   return;
	}


	void Analyzer::parse_helper( std::istream &stream )
	{
	   delete(scanner);
	   try
	   {
		  scanner = new Scanner( &stream );
	   }
	   catch( std::bad_alloc &ba )
	   {
		  std::cerr << "Failed to allocate scanner: (" << ba.what() << "), exiting!!\n";
		  exit( EXIT_FAILURE );
	   }
	   
	   delete(parser); 
	   try
	   {
		  parser = new Parser(*scanner,*this);
	   }
	   catch( std::bad_alloc &ba )
	   {
		  std::cerr << "Failed to allocate parser: (" <<  ba.what() << "), exiting!!\n";
		  exit( EXIT_FAILURE );
	   }
	   const int accept(0);
	   if( parser->parse() != accept )
	   {
		  std::cerr << "Parse failed!!\n";
	   }
	   return;
	}


	std::ostream& Analyzer::print( std::ostream &stream )
	{
	   return(stream);
	}
}
}
}
