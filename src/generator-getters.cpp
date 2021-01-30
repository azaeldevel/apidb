
#include "analyzer.hpp"
#include "generator.hpp"

namespace octetos::apidb::generators
{
    Getter::Getter(const ConfigureProject& c,const apidb::symbols::Table& t,std::ofstream& o) : Operation(c,t,o)
    {
    }
    
    bool Getter::definite(const symbols::Symbol* s)
    {
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
                    if(s->outType.compare("std::string") == 0)
                    {
                        ofile <<"\t\tconst "<< s->getOutType() << "& ";
                    }
                    else if(s->getSymbolReferenced())
                    {
                        ofile <<"\t\tconst " << s->getClassReferenced()->getName() << "& ";
                    }
                    else 
                    {
                        ofile <<"\t\t"<< s->getOutType() << " ";
                    }
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
        ofile << "get" << s->getUpperName() << "() const; \n";
        
        return true;
    }    
    bool Getter::definiteKey(const symbols::Symbol* s)
    {
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
                    if(s->outType.compare("std::string") == 0)
                    {
                        ofile <<"\t\tconst "<< s->getOutType() << "& ";
                    }
                    else if(s->getSymbolReferenced())
                    {
                        ofile <<"\t\t" << s->getOutType() << " ";
                    }
                    else 
                    {
                        ofile <<"\t\t"<< s->getOutType() << " ";
                    }
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
        ofile << "get" << s->getUpperName() << "Value() const; \n";
        
        return true;
    }
    
    
    
    
    
    
    bool Getter::implement(const symbols::Symbol* s)
    {
        if(s->outType.compare("std::string") == 0)
        {
            switch(configureProject.outputLenguaje)
            {
                case OutputLenguajes::CPP:
                    ofile <<"\tconst std::string& ";
                    break;
                case OutputLenguajes::JAVA:
                    ofile <<"\tpublic String ";
                    break;
                case OutputLenguajes::PHP:
                    ofile <<"\tpublic function ";
                    break;
                default:
                   throw BuildException("Lgenguaje no soportado",__FILE__,__LINE__);            
            }
        }
        else if(s->symbolReferenced)
        {
            switch(configureProject.outputLenguaje)
            {
                case OutputLenguajes::CPP:
                    ofile <<"\tconst " << s->classReferenced->name << "& ";
                    break;
                case OutputLenguajes::JAVA:
                    ofile <<"\tpublic " << s->classReferenced->name << " ";
                    break;
                case OutputLenguajes::PHP:
                    ofile <<"\tpublic function ";
                    break;
                default:
                   throw BuildException("Lgenguaje no soportado",__FILE__,__LINE__);            
            }
        }
        else 
        {
            switch(configureProject.outputLenguaje)
            {
                case OutputLenguajes::CPP:
                    ofile <<"\t"<< s->getOutType() << " ";
                    break;
                case OutputLenguajes::JAVA:
                    ofile <<"\tpublic "<< s->getOutType() << " ";
                    break;
                case OutputLenguajes::PHP:
                    ofile <<"\tpublic function ";
                    break;
                default:
                   throw BuildException("Lgenguaje no soportado",__FILE__,__LINE__);            
            }
        }
        
        if(configureProject.outputLenguaje == OutputLenguajes::CPP) ofile << table.getName() << "::";
        ofile << "get" << s->getUpperName() << "()";	
        if(configureProject.outputLenguaje == OutputLenguajes::CPP)  ofile << " const";
        
        ofile << "\n";
        ofile << "\t{"<<std::endl;
        if(s->outType.compare("std::string") == 0)
        {
            
            switch(configureProject.outputLenguaje)
            {
                case OutputLenguajes::CPP:
                    ofile <<"\t\treturn "<< s->getName()  <<";"<< std::endl;
                    break;
                case OutputLenguajes::JAVA:
                    ofile <<"\t\treturn "<< s->getName()  <<";"<< std::endl;
                    break;
                case OutputLenguajes::PHP:
                    ofile <<"\t\treturn $this->"<< s->getName()  <<";"<< std::endl;
                    break;
                default:
                   throw BuildException("Lgenguaje no soportado",__FILE__,__LINE__);            
            }
        }
        else if(s->symbolReferenced)
        {
            ofile <<"\t\treturn ";
            switch(configureProject.outputLenguaje)
            {
                case OutputLenguajes::CPP:
                    ofile << "*";
                    break;
                case OutputLenguajes::JAVA:
                    
                    break;
                case OutputLenguajes::PHP:
                    ofile << "$this->";
                    break;
                default:
                   throw BuildException("Lgenguaje no soportado",__FILE__,__LINE__);            
            }
            ofile << s->getName()  <<";"<< std::endl;
        }
        else 
        {
            switch(configureProject.outputLenguaje)
            {
                case OutputLenguajes::CPP:
                    ofile <<"\t\treturn "<< s->getName() <<";"<< std::endl;
                    break;
                case OutputLenguajes::JAVA:
                    ofile <<"\t\treturn "<< s->getName() <<";"<< std::endl;
                    break;
                case OutputLenguajes::PHP:
                    ofile <<"\t\treturn $this->"<< s->getName() <<";"<< std::endl;
                    break;
                default:
                   throw BuildException("Lgenguaje no soportado",__FILE__,__LINE__);            
            }
        }
        ofile << "\t}\n"<<std::endl; 
        
        return true;
    }
    bool Getter::implementKey(const symbols::Symbol* s)
    {
        if(s->outType.compare("std::string") == 0)
        {
            switch(configureProject.outputLenguaje)
            {
                case OutputLenguajes::CPP:
                    ofile <<"\t"<< s->getOutType() << " ";
                    break;
                case OutputLenguajes::JAVA:
                    ofile <<"\tpublic "<< s->getOutType() << " ";
                    break;
                case OutputLenguajes::PHP:
                    ofile <<"\tpublic function ";
                    break;
                default:
                   throw BuildException("Lgenguaje no soportado",__FILE__,__LINE__);            
            }
        }
        else if(s->getSymbolReferenced())
        {
            switch(configureProject.outputLenguaje)
            {
                case OutputLenguajes::CPP:
                    ofile <<"\t"<< s->getOutType() << " ";
                    break;
                case OutputLenguajes::JAVA:
                    ofile <<"\tpublic "<< s->getOutType() << " ";
                    break;
                case OutputLenguajes::PHP:
                    ofile <<"\tpublic function ";
                    break;
                default:
                   throw BuildException("Lgenguaje no soportado",__FILE__,__LINE__);            
            }
        }
        else 
        {
            switch(configureProject.outputLenguaje)
            {
                case OutputLenguajes::CPP:
                    ofile <<"\t"<< s->getOutType() << " ";
                    break;
                case OutputLenguajes::JAVA:
                    ofile <<"\tpublic "<< s->getOutType() << " ";
                    break;
                case OutputLenguajes::PHP:
                    ofile <<"\tpublic function ";
                    break;
                default:
                   throw BuildException("Lgenguaje no soportado",__FILE__,__LINE__);            
            }
        }
        if(configureProject.outputLenguaje == OutputLenguajes::CPP) ofile << table.getName() << "::" ;
        ofile << "get" << s->getUpperName() << "Value()";
        if(configureProject.outputLenguaje == OutputLenguajes::CPP) ofile << " const";
        ofile << "\n";
        ofile << "\t{"<<std::endl;
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
                    if(s->symbolReferenced != NULL)
                    {
                        ofile <<"\t\treturn (*"<< s->name << ")";
                        inheritField(ofile,s->symbolReferenced,".");
                    }
                    else
                    {
                        ofile <<"\t\treturn " << s->name;
                    }
                    break;
                case OutputLenguajes::JAVA:
                    if(s->symbolReferenced != NULL)
                    {
                        ofile <<"\t\treturn " << s->name;
                        inheritField(ofile,s->symbolReferenced,".");
                    }
                    else
                    {
                        ofile <<"\t\treturn " << s->name;
                    }
                    break;
                case OutputLenguajes::PHP:
                    if(s->symbolReferenced != NULL)
                    {
                        ofile <<"\t\treturn $this->" << s->name;
                        inheritField(ofile,s->symbolReferenced,"->");
                    }
                    else
                    {
                        ofile <<"\t\treturn $this->" << s->name;
                    }
                    break;
                default:
                   throw BuildException("Lgenguaje no soportado",__FILE__,__LINE__);            
            }
            ofile << ";" << std::endl;
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
        ofile << "\t}"<<std::endl;
        
        return true;
    }
    
    
    
    
    
    
    bool Getter::definite()
    {
        for(std::map<const char*,symbols::Symbol*,symbols::cmp_str>::const_iterator it = table.begin(); it != table.end(); it++)
        {
            if(definite(it->second) == false) return false;
        }
        
        for(symbols::Symbol* k : table.getKey())
        {
            if(definiteKey(k) == false) return false;
        }
        
        return true;
    }
    bool Getter::implement()
    {
        for(std::map<const char*,symbols::Symbol*,symbols::cmp_str>::const_iterator it = table.begin(); it != table.end(); it++)
        {
            if(implement(it->second) == false) return false;
        }
        
        for(symbols::Symbol* k : table.getKey())
        {
            if(implementKey(k) == false) return false;
        }
        
        return true;
    }
    bool Getter::generate()
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
