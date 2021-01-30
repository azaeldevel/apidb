
#include "analyzer.hpp"
#include "generator.hpp"

namespace octetos::apidb::generators
{
    Remove::Remove(const ConfigureProject& c,const apidb::symbols::Table& t,std::ofstream& o) : Operation(c,t,o)
    {
    }
    
    bool Remove::definite()
    {
        if(table.getKey().size() == 0) return false;
        
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
        if(table.getKey().size() == 0) return false;
        
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
                    ofile << "\tpublic boolean remove(octetos.db.maria.Connector connector) throws SQLException\n";
                    break;
                case OutputLenguajes::PHP:
                    ofile << "\tpublic function remove($connector)\n";
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
        if(configureProject.outputLenguaje == OutputLenguajes::CPP or configureProject.outputLenguaje == OutputLenguajes::JAVA) ofile << stringType() << " ";
        ofile << getsqlString() << " = \"DELETE FROM " + table.getName() + " WHERE \";\n";
        ofile << "\t\t" << getsqlString() << " = " << getsqlString();
        if(configureProject.outputLenguaje == OutputLenguajes::CPP or configureProject.outputLenguaje == OutputLenguajes::JAVA) ofile << " + ";
        if(configureProject.outputLenguaje == OutputLenguajes::PHP) ofile << " . ";
        echoKey();
        ofile << ";\n";
        switch(configureProject.outputLenguaje)
        {
            case OutputLenguajes::CPP:
                ofile << "\t\toctetos::db::maria::Datresult dt;"  << std::endl;
                break;
            case OutputLenguajes::JAVA:
                ofile << "\t\tResultSet dt = null;"  << std::endl;
                break;
            case OutputLenguajes::PHP:
                ofile << "\t\t$dt = null;"  << std::endl;                    
                break;
            default:
                   throw BuildException("Lgenguaje no soportado",__FILE__,__LINE__);            
        }
        if(configureProject.outputLenguaje == OutputLenguajes::CPP or configureProject.outputLenguaje == OutputLenguajes::JAVA ) ofile << "\t\treturn connector.remove(sqlString,dt);\n";
        if(configureProject.outputLenguaje == OutputLenguajes::PHP) ofile << "\t\treturn $connector->remove($sqlString,$dt);\n";
        ofile << "\t}\n";
        
        return true;
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
