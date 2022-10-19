
#include "analyzer.hpp"
#include "generator.hpp"

namespace octetos::apidb::generators
{
    Getter::Getter(const ConfigureProject& c,const apidb::symbols::Table& t,std::ofstream& o) : Operation(c,t,o)
    {
    }
    
    
    void Getter::cpp_return(const symbols::Symbol* s,bool cpp_conts)
    {
        switch(configureProject.outputLenguaje)
        {
        case OutputLenguajes::CPP:
            if(s->symbolReferenced)
            {
                if(cpp_conts) ofile <<"const ";
                ofile << s->getClassReferenced()->getName() << "&";
            }
            else if(s->outType.compare("std::string") == 0)
            {
                if(cpp_conts) ofile <<"const ";
                ofile << s->getOutType() << "&";
            }
            else if(s->isPK)
            {
                if(cpp_conts) ofile <<"const ";
                ofile << s->getOutType();
            }
            else
            {
                ofile << s->getOutType();
            }
            break;
        case OutputLenguajes::JAVA:
                     
                    break;
        case OutputLenguajes::PHP:
                    
                    break;
                default:
                   throw BuildException("Lenguaje no soportado",__FILE__,__LINE__);            
        }
    }
    bool Getter::definite_cpp(const symbols::Symbol* s,bool cpp_conts)
    {        
        ofile <<"\t\t";
        cpp_return(s,cpp_conts);
        ofile << " get" << s->getUpperName() << "()";
        if(cpp_conts) ofile <<"const";
        ofile << ";\n";
        
        return true;
    }
    bool Getter::implement_cpp(const symbols::Symbol* s,bool cpp_conts)
    {
        ofile <<"\t";
        cpp_return(s,cpp_conts);    
        
        ofile << " " << table.getName() << "::get" << s->getUpperName() << "()";
        if(cpp_conts) ofile <<" const";
        ofile << "\n";        
        ofile << "\t{\n";
            {
                if(s->symbolReferenced)
                {
                    ofile <<"\t\treturn *";
                }
                else 
                {
                    ofile <<"\t\treturn ";
                }
                ofile << s->getName()  <<";\n";
            }        
        ofile << "\t}\n";
        return true;
    }
    /*bool Getter::definite(const symbols::Symbol* s)
    {
        //const section
        switch(configureProject.outputLenguaje)
        {
        case OutputLenguajes::CPP:
                    if(s->symbolReferenced)
                    {
                        ofile <<"\t\tconst " << s->getClassReferenced()->getName() << "& ";
                    }
                    else if(s->outType.compare("std::string") == 0)
                    {
                        ofile <<"\t\tconst "<< s->getOutType() << "& ";
                    }
                    else if(s->isPK)
                    {
                        ofile <<"\t\tconst "<< s->getOutType() << " ";
                    }
                    else 
                    {
                        goto const_section;
                    }
                    break;
        case OutputLenguajes::JAVA:
                     
                    break;
        case OutputLenguajes::PHP:
                    
                    break;
                default:
                   throw BuildException("Lenguaje no soportado",__FILE__,__LINE__);            
        }
        ofile << "get" << s->getUpperName() << "() const; \n";
        
        const_section:
        if(not s->symbolReferenced or not s->isPK) return true;
        
        //not const 
        switch(configureProject.outputLenguaje)
        {
        case OutputLenguajes::CPP:
            if(s->symbolReferenced)
                    {
                        ofile <<"\t\t" << s->getClassReferenced()->getName() << "& ";
                    }
                    else if(s->outType.compare("std::string") == 0)
                    {
                        ofile <<"\t\t"<< s->getOutType() << "& ";
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
                   throw BuildException("Lenguaje no soportado",__FILE__,__LINE__);            
        }
        ofile << "get" << s->getUpperName() << "(); \n";
        
        return true;
    }*/
    /*bool Getter::definiteKey(const symbols::Symbol* s)
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
                   throw BuildException("Lenguaje no soportado",__FILE__,__LINE__);            
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
                   throw BuildException("Lenguaje no soportado",__FILE__,__LINE__);            
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
    }*/
    
    
    
    
    
    
    void Getter::implement(const symbols::Symbol* s)
    {
        //omitir si no es un cmapo referido y es un campo basico
         if((configureProject.getInputLenguaje() == InputLenguajes::MariaDB or configureProject.getInputLenguaje() == InputLenguajes::MySQL) and configureProject.outputLenguaje == OutputLenguajes::CPP and not s->symbolReferenced and s->outType.compare("std::string") != 0 and not s->isPK and not s->isAutoInc) return;
         
        //
        if(s->symbolReferenced)
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
        else if(s->outType.compare("std::string") == 0)
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
        else  
        {
            switch(configureProject.outputLenguaje)
            {
                case OutputLenguajes::CPP:
                    ofile <<"\tconst "<< s->getOutType() << " ";
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
        if(s->symbolReferenced)
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
        else if(s->outType.compare("std::string") == 0)
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
                   throw BuildException("Lenguaje no soportado",__FILE__,__LINE__);            
            }
        }
        ofile << "\t}\n"<<std::endl; 
        
        
        //not consti
        if(not s->symbolReferenced) return;
        
        if(s->symbolReferenced)
        {
            switch(configureProject.outputLenguaje)
            {
                case OutputLenguajes::CPP:
                    ofile <<"\t" << s->classReferenced->name << "& ";
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
        else if(s->outType.compare("std::string") == 0)
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
        if(configureProject.outputLenguaje == OutputLenguajes::CPP)  ofile;
        
        ofile << "\n";
        ofile << "\t{"<<std::endl;
        if(s->symbolReferenced)
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
        else if(s->outType.compare("std::string") == 0)
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
                   throw BuildException("Lenguaje no soportado",__FILE__,__LINE__);            
            }
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
                   throw BuildException("Lenguaje no soportado",__FILE__,__LINE__);            
            }
        }
        ofile << "\t}\n"<<std::endl;
    }
    /*bool Getter::implementKey(const symbols::Symbol* s)
    {
        if(s->outType.compare("std::string") == 0)
        {
            switch(configureProject.outputLenguaje)
            {
                case OutputLenguajes::CPP:
                    ofile <<"\tconst "<< s->getOutType() << "& ";
                    break;
                case OutputLenguajes::JAVA:
                    ofile <<"\tpublic "<< s->getOutType() << " ";
                    break;
                case OutputLenguajes::PHP:
                    ofile <<"\tpublic function ";
                    break;
                default:
                   throw BuildException("Lenguaje no soportado",__FILE__,__LINE__);            
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
                   throw BuildException("Lenguaje no soportado",__FILE__,__LINE__);            
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
    }*/
    
    
    
    
    
    
    void Getter::definite()
    {
        switch(configureProject.outputLenguaje)
        {
        case OutputLenguajes::CPP:
            for(std::map<const char*,symbols::Symbol*,symbols::cmp_str>::const_iterator it = table.begin(); it != table.end(); it++)
            {
                definite_cpp(it->second,true);
                if(it->second->symbolReferenced) definite_cpp(it->second,false);
            }
            break;
        case OutputLenguajes::JAVA:
            break;
        case OutputLenguajes::PHP:
            break;
        default:
            throw BuildException("Lenguaje no soportado",__FILE__,__LINE__);            
        }
    }
    void Getter::implement()
    {        
        switch(configureProject.outputLenguaje)
        {
        case OutputLenguajes::CPP:
            for(std::map<const char*,symbols::Symbol*,symbols::cmp_str>::const_iterator it = table.begin(); it != table.end(); it++)
            {
                if(implement_cpp(it->second,true) == false) return;
                if(it->second->symbolReferenced)if(implement_cpp(it->second,false) == false) return;
            }
            break;
        case OutputLenguajes::JAVA:
            throw BuildException("No soportado aun",__FILE__,__LINE__);
            break;
        case OutputLenguajes::PHP:
            throw BuildException("No soportado aun",__FILE__,__LINE__);
        default:          
            throw BuildException("Lenguaje no soportado",__FILE__,__LINE__);  
        }        
    }
    bool Getter::generate()
    {
        if(definition)
        {
            definite();
            return true;
        }
        
        if(implementation)
        {
            implement();
            return true;
        }
    }
    
}
