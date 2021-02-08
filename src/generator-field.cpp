
#include "analyzer.hpp"
#include "generator.hpp"

namespace octetos::apidb::generators
{
    
    Field::Field(const ConfigureProject& c,const apidb::symbols::Table& t,std::ofstream& o) : Operation(c,t,o)
    {
        
    }
          
    bool Field::definite()
    {
        for(std::map<const char*,symbols::Symbol*,symbols::cmp_str>::const_iterator it = table.begin(); it != table.end(); it++)
        {
            if(it->second->symbolReferenced != NULL)
            {
                ofile << "\t\t" << it->second->classReferenced->name << "* "<< it->second->name <<";\n";
            }
            else
            {
                ofile << "\t\t" << it->second->getOutType() << " " << it->second->name <<";\n";
            }
        }
        
        return true;
    }
    
    bool Field::implement()
    {
        for(std::map<const char*,symbols::Symbol*,symbols::cmp_str>::const_iterator it = table.begin(); it != table.end(); it++)
        {
            if(it->second->symbolReferenced != NULL)
            {
                ofile << "\t";
                if(configureProject.outputLenguaje == OutputLenguajes::PHP) ofile << "private $";
                if(configureProject.outputLenguaje == OutputLenguajes::JAVA) ofile << it->second->classReferenced->name;
                if(configureProject.outputLenguaje != OutputLenguajes::PHP) ofile << " ";
                ofile << it->second->name<<";\n";
            }
            else
            {
                ofile << "\t";
                if(configureProject.outputLenguaje == OutputLenguajes::PHP) ofile << "private $";
                if(configureProject.outputLenguaje == OutputLenguajes::JAVA) ofile << it->second->getOutType();
                if(configureProject.outputLenguaje != OutputLenguajes::PHP) ofile << " ";
                ofile << it->second->name<<";\n";
            }
        }
        
        return true;
    }
    
    bool Field::generate()
    {
        if(definition)
        {
            return definite();
        }
        
        if(implementation)
        {
            return implement();
        }
        
        throw BuildException("Deve especificar si es definicion o implemtacion.",__FILE__,__LINE__);
    }
}
