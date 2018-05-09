#ifndef __MCDRIVER_HPP__
#define __MCDRIVER_HPP__ 1

#include <string>
#include <cstddef>
#include <istream>

#include "scanner.hpp"
#include "parser.tab.hh"

namespace apibd
{
	class Driver
	{
	public:
	   Driver() = default;

	   virtual ~Driver();
	   std::string parse(const std::string& line);
	   /** 
		* parse - parse from a file
		* @param filename - valid string with input file
		*/
	   void parse( const char * const filename );
	   /** 
		* parse - parse from a c++ input stream
		* @param is - std::istream&, valid input stream
		*/
	   void parse( std::istream &iss );

	   std::ostream& print(std::ostream &stream);
	private:
	   void parse_helper( std::istream &stream );
	   apibd::Parser  *parser  = nullptr;
	   apibd::Scanner *scanner = nullptr;
	   std::string oneLine;
	};

} /* end namespace MC */
#endif /* END __MCDRIVER_HPP__ */
