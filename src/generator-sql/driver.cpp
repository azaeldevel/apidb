#include <cctype>
#include <fstream>
#include <cassert>
#include <sstream> 

#include "driver.hpp"

namespace apidb
{		
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
			//delete symbol;
		}	
		clear();
	}
	internal::Tables::~Tables()
	{
		for (apidb::internal::Table* table : *this)
		{
			//delete table;
		}
		clear();
	}
	const internal::Table* internal::Tables::search(const std::string& tableName)const
	{
		std::list<internal::Table*>::const_iterator first = this->begin();
		std::list<internal::Table*>::const_iterator last = this->end();
		
		while (first != last) 
		{
			if ((*first)->name.compare(tableName) == 0) break;
			++first;
		}
		
		if(first != last) return (const Table*)(*first);
		return NULL;
	}
	std::ostream& Driver::getErrorMessage()
	{
		return *errorMessages;
	}
	
	std::string Driver::getOutputLenguajeString()const
	{
		switch(getOutputLenguaje())
		{
			case OutputLenguajes::CPP:
				return "C++";
			default:
				return "Unknow";
		}
	}	
	std::string Driver::getInputLenguajeString()const
	{
		switch(getInputLenguaje())
		{
			case InputLenguajes::MySQL_Server:
				return "Servidor MySQL";
			default:
				return "Unknow";
		}
	}
		
	std::ostream& Driver::getOutputMessage()
	{
		return *outputMessages;
	}
	bool Driver::generate()
	{
		return false;
	}
	Driver::InputLenguajes Driver::getInputLenguaje()const
	{
		return inputLenguaje;
	}
	Driver::OutputLenguajes Driver::getOutputLenguaje()const
	{
		return outputLenguaje;
	}
	const std::string& Driver::getHeaderName() const
	{
		if(outputLenguaje == OutputLenguajes::CPP)
		{
			return projectH;
		}
		else
		{
			throw std::invalid_argument("OutputLenguaje is unknow.");
		}
	}
	std::ofstream& Driver::getSourceOutput()
	{
		return writeResults[1];
	}
	std::ofstream& Driver::getHeaderOutput()
	{
		return writeResults[0];
	}
	const std::string& Driver::getNameProject()
	{
		return nameProject;
	}
	void Driver::setPramsProject(const std::string& name,const std::string& directory)
	{
		nameProject = name;
		directoryProject = directory;
		if(outputLenguaje == OutputLenguajes::CPP)
		{//se requiere un archivo para las cabezaras y otro para el codigo
		   writeResults = new std::ofstream[2];
		   if((directory.empty()) | (directory.compare(".") == 0)) 
		   {
			   projectH = nameProject + ".hpp";
			   writeResults[0].open(projectH);
			   projectCPP = nameProject + ".cpp";
			   writeResults[1].open(projectCPP);
		   }
		   else
		   {
			   projectH = nameProject + ".hpp";
			   projectCPP = nameProject + ".cpp";
			   writeResults[0].open(directoryProject + "/" + projectH);
			   writeResults[1].open(directoryProject + "/" + projectCPP);
		   }
		}
		else
		{
			message("OutputLenguaje is unknow.");
		}
	}
	bool Driver::analyze()
	{
		return false;
	}
	Driver::Driver()
	{
	   //deafults
	   outputLenguaje = OutputLenguajes::CPP;
	   inputLenguaje = InputLenguajes::MySQL_Server;
	   outputMessages = &std::cout;	  
	   errorMessages = &std::cerr; 
	}
	void Driver::message(const std::string& msg)
	{
		(*outputMessages)<<msg<<std::endl;
	}
	std::string Driver::parse(const std::string& line)
	{
		std::istringstream text(line);
		parse(text);
		return oneLine;
	}
	Driver::~Driver()
	{
		delete(scanner);
		scanner = nullptr;
		delete(parser);
		parser = nullptr;
		if(outputLenguaje == OutputLenguajes::CPP)
		{
		   //delete &writeResults[0];
		   //delete &writeResults[1];
		}	   
	}

	void Driver::parse( const char * const filename )
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
	
	void Driver::parse( std::istream &stream )
	{
	   if( ! stream.good()  && stream.eof() )
	   {
		   return;
	   }
	   //else
	   parse_helper( stream ); 
	   return;
	}


	void Driver::parse_helper( std::istream &stream )
	{
	   delete(scanner);
	   try
	   {
		  scanner = new apidb::Scanner( &stream );
	   }
	   catch( std::bad_alloc &ba )
	   {
		  std::cerr << "Failed to allocate scanner: (" << ba.what() << "), exiting!!\n";
		  exit( EXIT_FAILURE );
	   }
	   
	   delete(parser); 
	   try
	   {
		  parser = new apidb::Parser(*scanner,*this);
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


	std::ostream& Driver::print( std::ostream &stream )
	{
	   return(stream);
	}
}
