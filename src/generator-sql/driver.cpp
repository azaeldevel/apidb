#include <cctype>
#include <fstream>
#include <cassert>
#include <sstream> 

#include "driver.hpp"

namespace apidb
{		
	const std::string& Driver::getNameProject()
	{
		return nameProject;
	}
	std::ofstream& Driver::getHeaderOutput()
	{
		return writeResults[0];
	}
	void Driver::setPramsProject(const std::string& name,const std::string& directory)
	{
		nameProject = name;
		directoryProject = directory;
		if((outputLenguaje.compare("C++") == 0) | (outputLenguaje.compare("C")  == 0))
		{//se requiere un archo para las cabezaras y otro para el codigo
		   writeResults = new std::ofstream[2];
		   std::string nameH = name + ".hpp";
		   writeResults[0].open(nameH);
		}
	}
	bool Driver::read()
	{
		return false;
	}
	Driver::Driver()
	{
	   //deafults
	   outputLenguaje = "C++";
	   inputLenguaje = "MySQL";
	   outputMessages = &std::cout;	   
	}
	void Driver::message(const std::string& msg)
	{
		(*outputMessages)<<msg<<std::endl;
	}
	const std::string& Driver::getOutputLenguaje()
	{
		return outputLenguaje;
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
		  std::cerr << "Failed to allocate scanner: (" <<
			 ba.what() << "), exiting!!\n";
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
