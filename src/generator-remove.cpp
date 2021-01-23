
#include "analyzer.hpp"
#include "generator.hpp"

namespace octetos::apidb::generators
{
    Remove::Remove(const ConfigureProject& c,const apidb::symbols::Table& t,std::ofstream& o) : Operation(c,t,o)
    {
    }
    
    bool Remove::definite()
    {
        //for actual object
        if(configureProject.getInputLenguaje() == InputLenguajes::MySQL)
        {
            switch(configureProject.outputLenguaje)
            {
                case OutputLenguajes::CPP:
                    
                    break;
                case OutputLenguajes::JAVA:
                    
                    break;
                case OutputLenguajes::PHP:
                    
                    break;
                default:
                   throw BuildException("Lgenguaje no soportado",__FILE__,__LINE__);            
            }
        }
        else if(configureProject.getInputLenguaje() == InputLenguajes::MariaDB)
        {
            switch(configureProject.outputLenguaje)
            {
                case OutputLenguajes::CPP:
                    ofile << "\t\tbool remove(octetos::db::maria::Connector& connector);\n";                    
                    break;
                case OutputLenguajes::JAVA:
                    
                    break;
                case OutputLenguajes::PHP:
                    
                    break;
                default:
                   throw BuildException("Lgenguaje no soportado",__FILE__,__LINE__);            
            }
        }
        else if(configureProject.getInputLenguaje() == InputLenguajes::PostgreSQL)
        {
            switch(configureProject.outputLenguaje)
            {
                case OutputLenguajes::CPP:
                    
                    break;
                case OutputLenguajes::JAVA:
                    
                    break;
                case OutputLenguajes::PHP:
                    
                    break;
                default:
                   throw BuildException("Lgenguaje no soportado",__FILE__,__LINE__);            
            }
        }
        else
        {
            std::string msg = "Lenguaje no soportado " ;
            throw BuildException(msg,__FILE__,__LINE__);
        }
        
        return true;
    }
    
    
    bool Remove::implement()
    {
        //for actual object
        if(configureProject.getInputLenguaje() == InputLenguajes::MySQL)
        {
            switch(configureProject.outputLenguaje)
            {
                case OutputLenguajes::CPP:
                    
                    break;
                case OutputLenguajes::JAVA:
                    
                    break;
                case OutputLenguajes::PHP:
                    
                    break;
                default:
                   throw BuildException("Lgenguaje no soportado",__FILE__,__LINE__);            
            }
        }
        else if(configureProject.getInputLenguaje() == InputLenguajes::MariaDB)
        {
            switch(configureProject.outputLenguaje)
            {
                case OutputLenguajes::CPP:
                    ofile << "\tbool " << table.getName() << "::remove(octetos::db::maria::Connector& connector)\n";                    
                    break;
                case OutputLenguajes::JAVA:
                    
                    break;
                case OutputLenguajes::PHP:
                    
                    break;
                default:
                   throw BuildException("Lgenguaje no soportado",__FILE__,__LINE__);            
            }
        }
        else if(configureProject.getInputLenguaje() == InputLenguajes::PostgreSQL)
        {
            switch(configureProject.outputLenguaje)
            {
                case OutputLenguajes::CPP:
                    
                    break;
                case OutputLenguajes::JAVA:
                    
                    break;
                case OutputLenguajes::PHP:
                    
                    break;
                default:
                   throw BuildException("Lgenguaje no soportado",__FILE__,__LINE__);            
            }
        }
        else
        {
            std::string msg = "Lenguaje no soportado " ;
            throw BuildException(msg,__FILE__,__LINE__);
        }
        ofile << "\t{\n";
        ofile << "\t\t";
        ofile << stringType() << " " << getsqlString() << " = \"DELETE FROM " + table.getName() + " WHERE \";\n";
        ofile << "\t\t" << getsqlString() << " = " << getsqlString() << " + ";
        echoKey();
        ofile << ";\n";
        ofile << "\t\toctetos::db::maria::Datresult dt;"  << std::endl;
        ofile << "\t\treturn connector.execute(sqlString,dt);"  << std::endl;
        ofile << "\t}\n";
    }
    
    bool Remove::generate()
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
