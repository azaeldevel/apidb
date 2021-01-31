
#include "analyzer.hpp"
#include "generator.hpp"

namespace octetos::apidb::generators
{
    Update::Update(const ConfigureProject& c,const apidb::symbols::Table& t,std::ofstream& o) : Operation(c,t,o)
    {
    }
    
    bool Update::definite()
    {
        if(table.getKey().size() == 0) return false;
        
        for(std::map<const char*,symbols::Symbol*,symbols::cmp_str>::const_iterator it = table.begin(); it != table.end(); it++)
        {
            if(not it->second->isPrimaryKey())
            {
                ofile << "\t\t";
                if(configureProject.getInputLenguaje() == InputLenguajes::MySQL)
                {        
                    ofile << "bool up" << it->second->getUpperName() << "(octetos::db::mysql::Connector& connector,";
                }
                else if(configureProject.getInputLenguaje() == InputLenguajes::MariaDB)
                {
                    ofile << "bool up" << it->second->getUpperName() << "(octetos::db::maria::Connector& connector,";
                }
                else if(configureProject.getInputLenguaje() == InputLenguajes::PostgreSQL)
                {
                    ofile << "bool up" << it->second->getUpperName() << "(octetos::db::postgresql::Connector& connector,";
                }
                else
                {
                    std::string msg = "Lenguaje no soportado " ;
                    throw BuildException(msg);
                }
                if(it->second->getClassReferenced() != 0)
                {
                    ofile << " const " << it->second->getClassReferenced()->getName() << "& " << it->second->getName();
                }
                else if((it->second->getOutType().compare("std::string") == 0))
                {
                    ofile << "const std::string& " << it->second->getName();
                }
                else if((it->second->getOutType().compare("int") == 0) | (it->second->getOutType().compare("long") == 0))
                {
                    ofile << it->second->getOutType() << " " << it->second->getName();						
                }
                else
                {
                    ofile << it->second->getOutType() << " " << it->second->getName();
                }
                ofile << ");"<< std::endl;
            }
        }
        return true;
    }
    
    
    bool Update::implement()
    {
        if(table.getKey().size() == 0) return false;
        
        for(std::map<const char*,symbols::Symbol*,symbols::cmp_str>::const_iterator it = table.begin(); it != table.end(); it++)
        {
            if(not it->second->isPrimaryKey())
			{
                ofile << "\t";
		        if(configureProject.getInputLenguaje() == InputLenguajes::MySQL)
		        {
                    switch(configureProject.outputLenguaje)
                    {
                        case OutputLenguajes::CPP:
                            ofile << "bool " << table.getName() <<"::up" << it->second->getUpperName() << "(octetos::db::mysql::Connector& connector,";
                            break;
                        case OutputLenguajes::JAVA:
                            ofile << "public boolean up" << it->second->getUpperName() << "(octetos.db.mysql.Connector connector,";
                            break;
                        case OutputLenguajes::PHP:
                            ofile << "public function up" << it->second->getUpperName() << "$connector";
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
                            ofile << "bool " << table.getName() <<"::up" << it->second->getUpperName() << "(octetos::db::maria::Connector& connector,";
                            break;
                        case OutputLenguajes::JAVA:
                            ofile << "public boolean up" << it->second->getUpperName() << "(octetos.db.maria.Connector connector,";
                            break;
                        case OutputLenguajes::PHP:
                            ofile << "public function up" << it->second->getUpperName() << "($connector";
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
                            ofile << "bool " << table.getName() <<"::up" << it->second->getUpperName() << "(octetos::db::postgresql::Connector& connector,";
                            break;
                        case OutputLenguajes::JAVA:
                            ofile << "public boolean up" << it->second->getUpperName() << "(octetos.db.postgresql.Connector connector,";
                            break;
                        case OutputLenguajes::PHP:
                            ofile << "$connector";
                            break;
                        default:
                        throw BuildException("Lgenguaje no soportado",__FILE__,__LINE__);            
                    }
		        }
		        else
		        {
		            std::string msg = "Lenguaje no soportado " ;
		            throw BuildException(msg);
		        }
		        //ofile << ",";
				if(it->second->symbolReferenced != NULL)
		        {
                    switch(configureProject.outputLenguaje)
                    {
                        case OutputLenguajes::CPP:
                            ofile << "const " << it->second->getClassReferenced()->getName()  << "& " << it->second->getName() ;
                            break;
                        case OutputLenguajes::JAVA:
                            ofile << it->second->getClassReferenced()->getName()  << " " << it->second->getName() ;
                            break;
                        case OutputLenguajes::PHP:
                            ofile << " $" << it->second->getName() ;
                            break;
                        default:
                        throw BuildException("Lgenguaje no soportado",__FILE__,__LINE__);            
                    }
		        }
		        else if(it->second->getOutType().compare(stringType()) == 0)
                {
                    switch(configureProject.outputLenguaje)
                    {
                        case OutputLenguajes::CPP:
                            ofile << "const " << stringType() << "& " << it->second->getName() ;
                            break;
                        case OutputLenguajes::JAVA:
                            ofile << stringType() << " " << it->second->getName() ;
                            break;
                        case OutputLenguajes::PHP:
                            ofile << " $" << it->second->getName() ;
                            break;
                        default:
                        throw BuildException("Lgenguaje no soportado",__FILE__,__LINE__);            
                    }
                }
		        else
		        {
		            ofile << it->second->getOutType() << " " << it->second->getName() ;
		        }
				ofile <<")";
                if(configureProject.outputLenguaje == OutputLenguajes::JAVA) ofile <<" throws SQLException";
				ofile << "\n\t{"<<std::endl;
                ofile << "\t\t";
                switch(configureProject.outputLenguaje)
                {
                    case OutputLenguajes::CPP:
                        ofile << "std::string sqlString ";
                        break;
                    case OutputLenguajes::JAVA:
                        ofile << "String sqlString";
                        break;
                    case OutputLenguajes::PHP:
                        ofile << "$sqlString";
                        break;
                    default:
                        throw BuildException("Lgenguaje no soportado",__FILE__,__LINE__);            
                }
                ofile << " = \"\";\n";
				if(configureProject.getInputLenguaje() == InputLenguajes::PostgreSQL)
				{
					ofile << "\t\t" << getsqlString() << " = \"UPDATE \\\"\" ;";
                    switch(configureProject.outputLenguaje)
                    {
                        case OutputLenguajes::CPP:
                            ofile << "+";
                            break;
                        case OutputLenguajes::JAVA:
                            ofile << "+";
                            break;
                        case OutputLenguajes::PHP:
                            ofile << ".";
                            break;
                        default:
                            throw BuildException("Lgenguaje no soportado",__FILE__,__LINE__);            
                    }
                    if(configureProject.outputLenguaje == OutputLenguajes::PHP) ofile << "$this->";
                    ofile << "TABLE_NAME ";
                    switch(configureProject.outputLenguaje)
                    {
                        case OutputLenguajes::CPP:
                            ofile << "+";
                            break;
                        case OutputLenguajes::JAVA:
                            ofile << "+";
                            break;
                        case OutputLenguajes::PHP:
                            ofile << ".";
                            break;
                        default:
                            throw BuildException("Lgenguaje no soportado",__FILE__,__LINE__);            
                    }                    
                    ofile << " \"\\\"\";\n";
				}
				else
				{
					ofile << "\t\t" << getsqlString() << " = \"UPDATE \" ";                    
                    switch(configureProject.outputLenguaje)
                    {
                        case OutputLenguajes::CPP:
                            ofile << "+ ";
                            break;
                        case OutputLenguajes::JAVA:
                            ofile << "+ ";
                            break;
                        case OutputLenguajes::PHP:
                            ofile << ". ";
                            break;
                        default:
                            throw BuildException("Lgenguaje no soportado",__FILE__,__LINE__);            
                    }
                    if(configureProject.outputLenguaje == OutputLenguajes::PHP) ofile << "$this->";
                    ofile << "TABLE_NAME;"<<std::endl;
				}
				ofile << "\t\t";
                ofile << getsqlString() << " = " << getsqlString();
                switch(configureProject.outputLenguaje)
                {
                    case OutputLenguajes::CPP:
                        ofile << " + ";
                        break;
                    case OutputLenguajes::JAVA:
                        ofile << " + ";
                        break;
                    case OutputLenguajes::PHP:
                        ofile << " . ";
                        break;
                    default:
                        throw BuildException("Lgenguaje no soportado",__FILE__,__LINE__);            
                }
                ofile << " \" SET \"" ;                
                switch(configureProject.outputLenguaje)
                {
                    case OutputLenguajes::CPP:
                        ofile << " + ";
                        break;
                    case OutputLenguajes::JAVA:
                        ofile << " + ";
                        break;
                    case OutputLenguajes::PHP:
                        ofile << " . ";
                        break;
                    default:
                        throw BuildException("Lgenguaje no soportado",__FILE__,__LINE__);            
                }
		        echoDataRawParam(it->second);
                ofile << ";\n";
                if(table.getKey().size() > 0)
                {
                    ofile << "\t\t";
                    ofile << getsqlString() << " = " << getsqlString();
                    switch(configureProject.outputLenguaje)
                    {
                        case OutputLenguajes::CPP:
                            ofile << " + ";
                            break;
                        case OutputLenguajes::JAVA:
                            ofile << " + ";
                            break;
                        case OutputLenguajes::PHP:
                            ofile << " . ";
                            break;
                        default:
                            throw BuildException("Lgenguaje no soportado",__FILE__,__LINE__);            
                    }
                    ofile << "\" WHERE \"";                    
                    switch(configureProject.outputLenguaje)
                    {
                        case OutputLenguajes::CPP:
                            ofile << " + ";
                            break;
                        case OutputLenguajes::JAVA:
                            ofile << " + ";
                            break;
                        case OutputLenguajes::PHP:
                            ofile << " . ";
                            break;
                        default:
                            throw BuildException("Lgenguaje no soportado",__FILE__,__LINE__);            
                    } 
                    echoKey();
                    ofile << ";\n";
                }
                else
                {
                    //no tiene contructor con llaves
                }
				
                if(configureProject.getInputLenguaje() == InputLenguajes::MySQL)
		        {
                    switch(configureProject.outputLenguaje)
                    {
                        case OutputLenguajes::CPP:
                            ofile <<"\t\toctetos::db::mysql::Datresult dt;\n";
                            break;
                        case OutputLenguajes::JAVA:
                            ofile <<"\t\tResultSet dt = null;\n";
                            break;
                        case OutputLenguajes::PHP:
                            ofile <<"\t\t$dt = null;\n";
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
                            ofile <<"\t\toctetos::db::maria::Datresult dt;\n";
                            break;
                        case OutputLenguajes::JAVA:
                            ofile <<"\t\tResultSet dt = null;\n";
                            break;
                        case OutputLenguajes::PHP:
                            ofile <<"\t\t$dt = null;\n";
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
                            ofile <<"\t\toctetos::db::posgresql::Datresult dt;\n";
                            break;
                        case OutputLenguajes::JAVA:
                            ofile <<"\t\tResultSet dt = null;\n";
                            break;
                        case OutputLenguajes::PHP:
                            ofile <<"\t\t$dt = null;\n";
                            break;
                        default:
                            throw BuildException("Lgenguaje no soportado",__FILE__,__LINE__);            
                    }
		        }
		        else
		        {
		            std::string msg = "Lenguaje no soportado " ;
		            throw BuildException(msg);
		        }
                
				
                switch(configureProject.outputLenguaje)
                {
                    case OutputLenguajes::CPP:
                        ofile <<"\t\treturn connector.update(sqlString,dt);\n";
                        break;
                    case OutputLenguajes::JAVA:
                        ofile <<"\t\treturn connector.update(sqlString,dt);\n";
                        break;
                    case OutputLenguajes::PHP:
                        ofile <<"\t\treturn $connector->update(sqlString,dt);\n";
                        break;
                    default:
                        throw BuildException("Lgenguaje no soportado",__FILE__,__LINE__);            
                }
				ofile << "\t}"<<std::endl;	
            } 
        }
        return true;
    }    
    
    bool Update::generate()
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
