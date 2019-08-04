#include <cctype>
#include <fstream>
#include <cassert>
#include <sstream> 

#include "analyzer.hpp"

namespace octetos
{
namespace apidb
{	
namespace mysql
{
	bool Analyzer::analyze(bool log)
	{
		bool flag = symbols::listing(*(octetos::toolkit::clientdb::mysql::Connector*)connector,spacies);
                
                //for(auto const& [keySpace, AttSpace]  : spacies)
                for(std::map<const char*,symbols::Tables*,symbols::cmp_str>::iterator it = spacies.begin(); it != spacies.end(); it++)
                {
                        //for(apidb::symbols::Table* table : *AttSpace) //reading attrubtes by table
                        for(std::list<symbols::Table*>::iterator itTb = it->second->begin(); itTb != it->second->end(); itTb++)
                        {
                                if(log)  *outputMessages << "\tCreating basic simbols for " << (*itTb)->name  << "." << std::endl;
                                //simbolos basicos 
                                if(!(*itTb)->basicSymbols(*connector))
                                {
                                        //std::cerr<<"Faill on basicSymbols"<<std::endl;
                                        return false;
                                }
                        }
                }
                
                //for(auto const& [keySpace, AttSpace]  : spacies)
                for(std::map<const char*,symbols::Tables*,symbols::cmp_str>::iterator it = spacies.begin(); it != spacies.end(); it++)
                {
                        //for(auto table: *AttSpace) //reading attrubtes by table
                        for(std::list<symbols::Table*>::iterator itTb = it->second->begin(); itTb != it->second->end(); itTb++)
                        {
                                //foreign key's
                                if(!(*itTb)->fillKeyType(*connector,getListTable()))
                                {
                                        //std::cerr<<"Faill on fillKeyType"<<std::endl;
                                        return false;
                                }
                        }
                }
                //std::cout<<"Step 3."<<std::endl;
                //for(auto const& [keySpace, AttSpace]  : spacies)
                for(std::map<const char*,symbols::Tables*,symbols::cmp_str>::iterator it = spacies.begin(); it != spacies.end(); it++)
                {
                        //for(auto table: *AttSpace) //reading attrubtes by table
                        for(std::list<symbols::Table*>::iterator itTb = it->second->begin(); itTb != it->second->end(); itTb++)
                        {
                                //for (auto const& [key, attribute] : *table)
                                for(std::map<const char*,symbols::Symbol*,symbols::cmp_str>::iterator itFl = (*itTb)->begin(); itFl != (*itTb)->end(); itFl++)
                                {
                                        //std::cout<<"\t"<<attribute->inType<<std::endl;
                                        itFl->second->outType = parse(itFl->second->inType);
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
