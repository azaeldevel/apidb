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
                
	/*std::ostream& Analyzer::getErrorMessage()
	{
		return *errorMessages;
	}		
	std::ostream& Analyzer::getOutputMessage()
	{
		return *outputMessages;
	}*/
	Analyzer::~Analyzer()
	{
                /*if(outputMessages != NULL)
                {
                        outputMessages = NULL;                        
                }
                if(errorMessages != NULL)
                {
                        errorMessages = NULL;                        
                }*/
	}
	Analyzer::Analyzer(const ConfigureProject& config,octetos::toolkit::clientdb::Connector* conn,toolkit::ActivityProgress* p) : configureProject(config), connector(conn),progress(p)
	{
	}
	/*OutputLenguajes Analyzer::getOutputLenguaje() const
	{
		return configureProject.outputLenguaje;
	}*/
	std::map<const char*,symbols::Space*,symbols::cmp_str> Analyzer::copyListTable() const
	{
		return spacies;
	}	
	const std::map<const char*,symbols::Space*,symbols::cmp_str>& Analyzer::getListTableConst() const
	{
		return spacies;
	}
	/*std::map<const char*,symbols::Space*,symbols::cmp_str>& Analyzer::getListTable() 
	{
                std::cout <<"\u001b[31;1m" << "Advertencia:  El uso de Analyzer::getListTable se considera peligroso ya que retorna una referencia no modificable de la tabla de simbols, use  getListTableConst o copyListTable, esta funcion será removida en v2.\n" << "\u001b[0m";
		return spacies;
	}*/		
	/*const std::string& Analyzer::getNameProject()
	{
		return configureProject.name;
	}*/		
	/*const std::string& Analyzer::getDirectoryProject()
	{
		return configureProject.directory;
	}*/	
	/*InputLenguajes Analyzer::getInputLenguaje()const
	{
		return configureProject.inputLenguaje;
	}*/
}
}
