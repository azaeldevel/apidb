#include <cctype>
#include <fstream>
#include <cassert>
#include <sstream> 

#include "driver.hpp"

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
	
	int internal::Symbol::getID()const
	{
		return id;
	}
	internal::Symbol::Symbol()
	{
		counter++;
		id = counter;
	}
	
	int internal::Symbol::counter = 0;	
	internal::Table::~Table()
	{
		for (apidb::internal::Symbol* symbol : *this)
		{
			delete symbol;
		}	
		clear();
	}
	internal::Tables::~Tables()
	{
		for (apidb::internal::Table* table : *this)
		{
			delete table;
		}
		clear();
	}
	const internal::Table* internal::Tables::search(const std::string& tableName)const
	{
		std::list<internal::Table*>::const_iterator actual = this->begin();
		std::list<internal::Table*>::const_iterator last = this->end();
		
		while (actual != last) 
		{
			if ((*actual)->name.compare(tableName) == 0) return (const internal::Table*)(*actual);
			++actual;
		}
		return NULL;
	}
	
	
	
	const internal::Tables& Analyzer::getListTable() const
	{
		return symbolsTables;
	}
	std::string Analyzer::getOutputLenguajeString()const
	{
		switch(getOutputLenguaje())
		{
			case OutputLenguajes::CPP:
				return "C++";
			default:
				return "Unknow";
		}
	}
	std::string Analyzer::getInputLenguajeString()const
	{
		switch(getInputLenguaje())
		{
			case InputLenguajes::MySQL_Server:
				return "Servidor MySQL";
			default:
				return "Unknow";
		}
	}
	const std::string& Analyzer::getNameProject()
	{
		return nameProject;
	}
	void Analyzer::setPramsProject(const std::string& name,const std::string& directory)
	{
		nameProject = name;
		directoryProject = directory;
	}
	const std::string& Analyzer::getDirectoryProject()
	{
		return directoryProject;
	}
	InputLenguajes Analyzer::getInputLenguaje()const
	{
		return inputLenguaje;
	}
	OutputLenguajes Analyzer::getOutputLenguaje()const
	{
		return outputLenguaje;
	}
	
	
	
namespace mysql
{
	bool Analyzer::listing(toolkit::clientdb::Connector& connect)
	{
		return symbolsTables.listing(connect);
	}
	Analyzer::Analyzer(InputLenguajes inputLenguaje, OutputLenguajes outputLenguaje)
	{
		this->inputLenguaje = inputLenguaje;
		this->outputLenguaje = outputLenguaje;
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
	Analyzer::Analyzer()
	{
	   //deafults
	   outputLenguaje = OutputLenguajes::CPP;
	   inputLenguaje = InputLenguajes::MySQL_Server;
	   outputMessages = &std::cout;	  
	   errorMessages = &std::cerr; 
	}
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
