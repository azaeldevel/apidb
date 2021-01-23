
#include "analyzer.hpp"
#include "generator.hpp"

namespace octetos::apidb::generators
{
    Update::Update(const ConfigureProject& c,const apidb::symbols::Table& t,std::ofstream& o) : Operation(c,t,o)
    {
    }
    
    bool Update::definite()
    {
        for(std::map<const char*,symbols::Symbol*,symbols::cmp_str>::const_iterator it = table.begin(); it != table.end(); it++)
        {
            if(not it->second->isPrimaryKey())
            {
                if(configureProject.getInputLenguaje() == InputLenguajes::MySQL)
                {        
                    ofile << "\t\tbool up" << it->second->getUpperName() << "(octetos::db::mysql::Connector& connector,";
                }
                else if(configureProject.getInputLenguaje() == InputLenguajes::MariaDB)
                {
                    ofile << "\t\tbool up" << it->second->getUpperName() << "(octetos::db::maria::Connector& connector,";
                }
                else if(configureProject.getInputLenguaje() == InputLenguajes::PostgreSQL)
                {
                    ofile << "\t\tbool up" << it->second->getUpperName() << "(octetos::db::postgresql::Connector& connector,";
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
        for(std::map<const char*,symbols::Symbol*,symbols::cmp_str>::const_iterator it = table.begin(); it != table.end(); it++)
        {
            if(not it->second->isPrimaryKey())
			{
		        if(configureProject.getInputLenguaje() == InputLenguajes::MySQL)
		        {
		            ofile << "\tbool " << table.getName() <<"::up" << it->second->getUpperName() << "(octetos::db::mysql::Connector& connector,";
		        }
		        else if(configureProject.getInputLenguaje() == InputLenguajes::MariaDB)
		        {
		            ofile << "\tbool " << table.getName() <<"::up" << it->second->getUpperName() << "(octetos::db::maria::Connector& connector,";
		        }
		        else if(configureProject.getInputLenguaje() == InputLenguajes::PostgreSQL)
		        {
		            ofile << "\tbool " << table.getName() <<"::up" << it->second->getUpperName() << "(octetos::db::postgresql::Connector& connector,";
		        }
		        else
		        {
		            std::string msg = "Lenguaje no soportado " ;
		            throw BuildException(msg);
		        }
				if((it->second->getOutType().compare("std::string") == 0 || it->second->getOutType().compare("int") == 0) && it->second->getClassReferenced() != NULL)
		        {
		            ofile << "const " << it->second->getClassReferenced()->getName()  << "& " << it->second->getName() ;
		        }
		        else if(it->second->getOutType().compare("std::string") == 0)
		        {
		            ofile << "const " << it->second->getOutType() << "& " << it->second->getName() ;
		        }
		        else
		        {
		            ofile << it->second->getOutType() << " " << it->second->getName() ;
		        }
				ofile <<")"<< std::endl;
				ofile << "\t{"<<std::endl;
				ofile << "\t\tstd::string sqlString = \"\";"<<std::endl;
				if(configureProject.getInputLenguaje() == InputLenguajes::PostgreSQL)
				{
					ofile << "\t\tsqlString = \"UPDATE \\\"\" ;+ TABLE_NAME + \"\\\"\";"<<std::endl;
				}
				else
				{
					ofile << "\t\tsqlString = \"UPDATE \" + TABLE_NAME;"<<std::endl;
					//ofile << "\t\tsqlString += \" FROM \";"<<std::endl;
				}
				ofile << "\t\tsqlString = sqlString + \" SET " ;
		        
				ofile << it->second->getName()  << " = " ;
                if( it->second->getOutType().compare("int") == 0 && it->second->getSymbolReferenced() != NULL)
                {
                    ofile << "'\" +  std::to_string(" << it->second->getName()   << ".getKey" << it->second->getSymbolReferenced()->getUpperName() << "())+ \"'\";" << std::endl;                                    
                }
                else if( it->second->getOutType().compare("int") == 0 && it->second->getSymbolReferenced() == NULL)
                {
                    ofile << "'\" +  std::to_string(" << it->second->getName()   << ")+ \"'\";" << std::endl;                            
                }
                else if(it->second->getOutType().compare("std::string") == 0 && it->second->getSymbolReferenced() != NULL)
                {
                    ofile << "'\" + " << it->second->getName()  << " + \"'\";" << std::endl;
                }
                else if(it->second->getOutType().compare("std::string") == 0  && it->second->getSymbolReferenced() == NULL)
                {
                    ofile << "'\" + " << it->second->getName()  << " + \"'\";" << std::endl;
                }
                else
                {
                    ofile << "\" + std::to_string(" << it->second->getName()  << ");" << std::endl;
                }
				
				ofile << "\t\tsqlString = sqlString + \" WHERE ";
                if(table.getKey().size() > 0)
                {
                    auto kEnd = table.getKey().end();
                    kEnd--;
                    for(auto k : table.getKey())
                    {
                        ofile << k->getName()  << " = \" + ";  
                        if(k->classReferenced != NULL) //es un objeto
                        {
                            if(k->outType.compare("std::string") == 0)
                            {
                                    ofile << k->getName() << ";\n";
                            }
                            else
                            {
                                ofile << "std::to_string((*" << k->getName() << ")";
                                inheritField(ofile,k->symbolReferenced,opReference());
                                ofile<< ");\n";
                            }
                        }
                        else if(k->outType.compare("std::string") == 0)
                        {
                            ofile << " '\" + " <<  k->getName()  << " + \"'\";\n";
                        }
                        else
                        {
                            ofile << "std::to_string(" << k->getName()  <<");\n";
                        }
                        if(k != *kEnd)
                        {
                            ofile << " + \" and \" ";
                        }
                    }
                }
                else
                {
                    throw BuildException("No hay soporte para table sin llave",__FILE__,__LINE__);
                }
				
                if(configureProject.getInputLenguaje() == InputLenguajes::MySQL)
		        {
                    ofile <<"\t\toctetos::db::mysql::Datresult dat;\n";
		        }
		        else if(configureProject.getInputLenguaje() == InputLenguajes::MariaDB)
		        {
		            ofile <<"\t\toctetos::db::maria::Datresult dat;\n";
		        }
		        else if(configureProject.getInputLenguaje() == InputLenguajes::PostgreSQL)
		        {
		            ofile <<"\t\toctetos::db::postgresql::Datresult dat;\n";
		        }
		        else
		        {
		            std::string msg = "Lenguaje no soportado " ;
		            throw BuildException(msg);
		        }
                
				ofile <<"\t\treturn connector.update(sqlString,dat);\n";
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
