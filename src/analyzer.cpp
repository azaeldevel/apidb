#include <cctype>
#include <fstream>
#include <cassert>
#include <sstream> 

#include "analyzer.hpp"

namespace octetos
{
namespace apidb
{	
	toolkit::ActivityProgress& Analyzer::getOutput()
        {
                return *progress;
        }
        const ConfigureProject& Analyzer::getConfigureProject()const
        {
                return configureProject;
        }
                
	Analyzer::~Analyzer()
	{
	}
	Analyzer::Analyzer(const ConfigureProject& config,octetos::toolkit::clientdb::Connector* conn,toolkit::ActivityProgress* p) : configureProject(config), connector(conn),progress(p)
	{
	}
	std::map<const char*,symbols::Space*,symbols::cmp_str> Analyzer::copyListTable() const
	{
		return spacies;
	}	
	const std::map<const char*,symbols::Space*,symbols::cmp_str>& Analyzer::getListTableConst() const
	{
		return spacies;
	}
}
}
