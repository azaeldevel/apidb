#include <cctype>
#include <fstream>
#include <cassert>
#include <sstream> 

#include "analyzer.hpp"

namespace apidb
{	
	
	Analyzer::Analyzer(const ConfigureProject& config) : configureProject(config)
	{
		
	}

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
	

	
	
	/*void Analyzer::setPramsLenguajes(InputLenguajes inputLenguaje, OutputLenguajes outputLenguaje)
	{
		//this->inputLenguaje = inputLenguaje;
		//this->outputLenguaje = outputLenguaje;		
	}*/
	
	OutputLenguajes Analyzer::getOutputLenguaje() const
	{
		return configureProject.outputLenguaje;
	}
	
	symbols::Tables& Analyzer::getListTable() 
	{
		return symbolsTables;
	}
		
	std::string Analyzer::getInputLenguajeString() const
	{
		switch(configureProject.inputLenguaje)
		{
			case InputLenguajes::MySQL:
				return "Servidor MySQL";
			default:
				return "Unknow";
		}
	}
	
	const std::string& Analyzer::getNameProject()
	{
		return configureProject.name;
	}
	
	/*void Analyzer::setPramsProject(const std::string& name,const std::string& directory)
	{
		//nameProject = name;
		//directoryProject = directory;
	}*/
	
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
	bool Analyzer::listing(toolkit::clientdb::Connector& connect)
	{
		return symbolsTables.listing(connect);
	}
	
	Analyzer::Analyzer(const ConfigureProject& config) : apidb::Analyzer(config)
	{
		//this->inputLenguaje = inputLenguaje;
		//this->outputLenguaje = outputLenguaje;
		outputMessages = &std::cout;	  
		errorMessages = &std::cerr; 
	}
	
	std::ostream& Analyzer::getErrorMessage()
	{
		return *errorMessages;
	}

		
	std::ostream& Analyzer::getOutputMessage()
	{
		return *outputMessages;
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
