
#include "analyzer.hpp"
#include "generator.hpp"

namespace octetos::apidb::generators
{
    
    Constructor::Constructor(const ConfigureProject& c,const apidb::symbols::Table& t,std::ofstream& o) : Operation(c,t,o)
    {
        
    }
          
    void Constructor::definite()
    {

    }
    void Constructor::definite_default()
    {
        ofile <<"\t\t"<<table.getName()<<"();"<<std::endl;
    }
    
    void Constructor::implement()
    {
        ofile <<"\t"<<table.getName()<< "::" <<table.getName()<<"()";
        if(not table.references.empty())
        {
            ofile << " : ";
            for(unsigned int i = 0; i < table.references.size(); i++)
            {
                ofile << table.references[i]->name << "(NULL)";
                if(i < table.references.size() - 1) ofile << ",";
            }
        }
        ofile << "\n";
		ofile <<"\t{\n";
        /*for(const std::pair<const char*,symbols::Symbol*>& it : table)
        {
            if(it.second->symbolReferenced)
            {
                ofile << "\t\t" << it.second->name << " = NULL;\n";
            }
        }*/
		ofile <<"\t}\n";
    }
    
    bool Constructor::generate()
    {
        if(definition) definite();
        
        if(implementation) implement();
        
        return true;
    }
}
