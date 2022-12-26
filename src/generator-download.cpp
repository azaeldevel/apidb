
#include "analyzer.hpp"
#include "generator.hpp"

namespace octetos::apidb::generators
{
    
    Download::Download(const ConfigureProject& c,const apidb::symbols::Table& t,std::ofstream& o) : Select(c,t,o)
    {
        
    }
        
    void Download::definite()
    {
        /*const ConfigureProject::Table* tb = configureProject.findDownloadTable(table.getName());
        if(tb != NULL)
        {
            for(std::map<const char*, const apidb::ConfigureProject::Function*>::const_iterator itF = tb->begin(); itF != tb->end(); itF++)
            {
                if(configureProject.getInputLenguaje() == InputLenguajes::MySQL)
                {        
                    ofile << "\t\tbool " << itF->first << "(octetos::db::mysql::Connector& connector);"<<std::endl;
                }
                else if(configureProject.getInputLenguaje() == InputLenguajes::MariaDB)
                {
                    ofile << "\t\tbool " << itF->first << "(octetos::db::maria::Connector& connector);"<<std::endl;
                }
                else if(configureProject.getInputLenguaje() == InputLenguajes::PostgreSQL)
                {
                    ofile << "\t\tbool " << itF->first << "(octetos::db::postgresql::Connector& connector);"<<std::endl;
                }
                else
                {
                    std::string msg = "Lenguaje no soportado " ;
                    throw BuildException(msg);
                }
            }
        }*/
        
        for(auto symbol : table)
        {
            if(symbol.second->isPK) continue;
                
            if(configureProject.getInputLenguaje() == InputLenguajes::MySQL)
            {        
                ofile << "\t\tbool down" << symbol.second->upperName << "(octetos::db::mysql::Connector& connector);\n";
            }
            else if(configureProject.getInputLenguaje() == InputLenguajes::MariaDB)
            {        
                ofile << "\t\tbool down" << symbol.second->upperName << "(octetos::db::maria::Connector& connector);\n";
            }
            else if(configureProject.getInputLenguaje() == InputLenguajes::PostgreSQL)
            {
                ofile << "\t\tbool down" << symbol.second->upperName << "(octetos::db::postgresql::Connector& connector);\n";
            }
            else
            {
                std::string msg = "Lenguaje no soportado " ;
                throw BuildException(msg);
            }
        }
    }
    
    
    void Download::implement()
    {
        if(table.getKey().size() == 0) return;
        
        /*for( std::map<const char*,ConfigureProject::Table*>::const_iterator itT = configureProject.downloads.begin(); itT != configureProject.downloads.end(); itT++)//std::vector<Table>
        {
            if(table.getName().compare(itT->second->getName()) != 0) 
            {
                continue;//buscar la configuracion de la tabla correspondiente
            }
                        
            //for (auto const& [key, val] : *(itT->second))//class Table : public std::map<const char*, const Function*>
            for(ConfigureProject::Table::iterator itCfTb = itT->second->begin(); itCfTb != itT->second->end(); itCfTb++)
            {
                if(configureProject.getInputLenguaje() == InputLenguajes::MySQL)
                {       
                    ofile << "\tbool " << table.getName() << "::" << itCfTb->first << "(octetos::db::mysql::Connector& connector)"<<std::endl;
                }
                else if(configureProject.getInputLenguaje() == InputLenguajes::MariaDB)
                {
                    ofile << "\tbool " << table.getName() << "::" << itCfTb->first << "(octetos::db::maria::Connector& connector)"<<std::endl;
                }
                else if(configureProject.getInputLenguaje() == InputLenguajes::PostgreSQL)
                {
                    ofile << "\tbool " << table.getName() << "::" << itCfTb->first << "(octetos::db::postgresql::Connector& connector)"<<std::endl;
                }
                else
                {
                    std::string msg = "Lenguaje no soportado " ;
                    throw BuildException(msg);
                }
                
                ofile << "\t{ " << std::endl;
                ofile << "\t\tstd::string sqlString = \"SELECT ";
                const apidb::ConfigureProject::Parameters* params = itCfTb->second->getParameters();

                {
                apidb::ConfigureProject::Parameters::const_iterator itParamEnd = params->end();
                itParamEnd--;
                for(const std::string& param : *params)
                {
					ofile << param; 
					if(param != *itParamEnd)
					{
						ofile << ",";
					}
                }  
                if(configureProject.getInputLenguaje() == InputLenguajes::PostgreSQL)
				{
					ofile << " FROM \\\"" << table.getName() << "\\\" WHERE \";\n" ;
				}
				else
				{
					ofile << " FROM " << table.getName() << " WHERE \";\n" ;
				}
				
                ofile << "\t\tsqlString = sqlString + ";
                echoKeyRawParam();
                ofile << ";\n";
                
                if(configureProject.getInputLenguaje() == InputLenguajes::MySQL)
                {        
                    ofile << "\t\toctetos::db::mysql::Datresult dt;"  << std::endl;
                }
                else if(configureProject.getInputLenguaje() == InputLenguajes::MariaDB)
                {
                    ofile << "\t\toctetos::db::maria::Datresult dt;"  << std::endl;
                }
                else if(configureProject.getInputLenguaje() == InputLenguajes::PostgreSQL)
                {
                    ofile << "\t\toctetos::db::postgresql::Datresult dt;"  << std::endl;
                }
                else
                {
                    std::string msg = "Lenguaje no soportado " ;
                                    throw BuildException(msg);
                }
                
                ofile << "\t\tbool flag = connector.execute(sqlString,dt);"  << std::endl;
                ofile << "\t\tif(flag)"  << std::endl;
                ofile << "\t\t{" << std::endl;
                ofile << ""<< std::endl;
                ofile << "\t\t\twhile (dt.nextRow()) "<< std::endl;
                ofile << "\t\t\t{"<< std::endl;
                //ofile << "\t\t\t\tfor(int i = 0; i < num_fields; i++)"<< std::endl;
                //ofile << "\t\t\t\t{"<< std::endl;
                itParamEnd = params->end();
                int countparam = 0;
                for(const std::string& param : *params)
                {
                    //ofile << param; 
                    if(param != *itParamEnd)
                    {
                        //ofile << "\t\t\t\t\tthis->" << param << " = (row[i] ? row[i] : NULL);"<< std::endl;
                        ofile << "\t\t\t\t\tthis->" << param << " = ";
                        auto fl = table.find(param.c_str());
                        if(fl != table.end())
                        {
							if((*fl).second->getClassReferenced() != NULL)
							{
								if((*fl).second->outType.compare("int"))
								{
									ofile << " new " << (*fl).second->getClassReferenced()->getName() << "(dt.getint(" << countparam << "));" << std::endl ;
								}
								else if((*fl).second->outType.compare("long") or (*fl).second->outType.compare("long int"))
								{
									ofile << " new " << (*fl).second->getClassReferenced()->getName() << "(dt.getl(" << countparam << "));" << std::endl ;
								}
								else if((*fl).second->outType.compare("std::String"))
								{
									ofile << " new " << (*fl).second->getClassReferenced()->getName() << "(dt.getString(" << countparam << "));" << std::endl ;
								}
								else
								{
									ofile << " new " << (*fl).second->getClassReferenced()->getName() << "(dt.getString(" << countparam << "));" << std::endl ;
								}
							}
							else if((*fl).second->getOutType().compare("int") == 0)
							{
								ofile << " dt.getint(" << countparam << ")" << ";"<< std::endl ;
							}
							else if((*fl).second->getOutType().compare("long") == 0)
							{
								ofile << " dt.getl(" << countparam << ")" << ";"<< std::endl ;
							}
							else if((*fl).second->getOutType().compare("std::string") == 0 || (*fl).second->getOutType().compare("const char*") == 0)
							{
								ofile << " dt.getString(" << countparam << ")" << ";" << std::endl ;
							}
							else
							{
								ofile << " dt.getString(" << countparam << ")" << ";" << std::endl ;
							}
                        }
                        else
                        {
                            std::string strmsg = "No se encontro el campo ";
                            strmsg = strmsg + "'" + param + "' en la tabla '" + table.getName() + "' File : generator-c++";
                            throw BuildException(strmsg);
                        }
					}
					countparam++;
				}
                                
				//ofile << "\t\t\t;"<< std::endl;
				//ofile << "\t\t\t\t}"<< std::endl;
				ofile << "\t\t\t}"<< std::endl;
				ofile << "\t\t\treturn true;" << std::endl;
				ofile << "\t\t}" << std::endl;
				ofile << "\t\telse" << std::endl;
				ofile << "\t\t{" << std::endl;
				ofile << "\t\t\treturn false;" << std::endl;
				ofile << "\t\t}" << std::endl;
				}
			ofile << "\t} " << std::endl;
			}         
		}    */
          
		
		
        for(auto symbol : table)
        {
            if(symbol.second->isPK) continue;
            
            if(configureProject.getInputLenguaje() == InputLenguajes::MySQL)
            {
                ofile << "\tbool " << table.name << "::down" << symbol.second->upperName << "(octetos::db::mysql::Connector& connector)";
            }
            else if(configureProject.getInputLenguaje() == InputLenguajes::MariaDB)
            {
                switch(configureProject.outputLenguaje)
                {
                    case OutputLenguajes::CPP:
                        ofile << "\tbool " << table.getName() << "::down" << symbol.second->upperName << "(octetos::db::maria::Connector& connector)";
                        break;
                    case OutputLenguajes::JAVA:
                        ofile << "\tpublic boolean down" << symbol.second->upperName << "(octetos.db.maria.Connector connector)";
                        break;
                    case OutputLenguajes::PHP:
                        ofile << "\tpublic function down" << symbol.second->upperName << "($connector)";
                        break;
                    default:
                        throw BuildException("Lenguaje no soportado",__FILE__,__LINE__);            
                }
            }
            else if(configureProject.getInputLenguaje() == InputLenguajes::PostgreSQL)
            {
                ofile << "\tbool " << table.name << "::down" << symbol.second->upperName << "(octetos::db::postgresql::Connector& connector)";
            }
            else
            {
                std::string msg = "Lenguaje no soportado " ;
                throw BuildException(msg);
            }
            if(configureProject.outputLenguaje == OutputLenguajes::JAVA) ofile << " throws SQLException";
            ofile << "\n\t{\n\t\t";
            if(configureProject.outputLenguaje != OutputLenguajes::PHP) ofile << stringType() << " ";
            ofile << getsqlString() << " = \"SELECT " << symbol.second->name << " ";
             
            if(configureProject.getInputLenguaje() == InputLenguajes::PostgreSQL)
            {
                ofile << " FROM \\\"" << table.getName() << "\\\" WHERE \";"<< std::endl;
            }
            else
            {
                ofile << " FROM " << table.getName() << " WHERE \";"<< std::endl;
            }
            
            ofile << "\t\t" << getsqlString() << " = " << getsqlString();
            if(configureProject.outputLenguaje == OutputLenguajes::PHP) ofile << " .";
            if(configureProject.outputLenguaje == OutputLenguajes::CPP or configureProject.outputLenguaje == OutputLenguajes::JAVA) ofile << " + ";
            echoKey();
            ofile << ";\n";
                        
            
            switch(configureProject.outputLenguaje)
            {
                case OutputLenguajes::CPP:
                    if(configureProject.getInputLenguaje() == InputLenguajes::MySQL)
                    {        
                        ofile << "\t\toctetos::db::mysql::Datresult dt;"  << std::endl;
                    }
                    else if(configureProject.getInputLenguaje() == InputLenguajes::MariaDB)
                    {
                        ofile << "\t\toctetos::db::maria::Datresult dt;"  << std::endl;
                    }
                    else if(configureProject.getInputLenguaje() == InputLenguajes::PostgreSQL)
                    {
                        ofile << "\t\toctetos::db::postgresql::Datresult dt;"  << std::endl;
                    }
                    else
                    {
                        std::string msg = "Lenguaje no soportado " ;
                        throw BuildException(msg);
                    }
                    break;
                case OutputLenguajes::JAVA:
                    if(configureProject.getInputLenguaje() == InputLenguajes::MySQL)
                    {        
                        ofile << "\t\tResultSet dt = null;"  << std::endl;
                    }
                    else if(configureProject.getInputLenguaje() == InputLenguajes::MariaDB)
                    {
                        ofile << "\t\tResultSet dt = null;"  << std::endl;
                    }
                    else if(configureProject.getInputLenguaje() == InputLenguajes::PostgreSQL)
                    {
                        ofile << "\t\tResultSet dt = null;"  << std::endl;
                    }
                    else
                    {
                        std::string msg = "Lenguaje no soportado " ;
                        throw BuildException(msg);
                    }
                    break;
                case OutputLenguajes::PHP:
                    if(configureProject.getInputLenguaje() == InputLenguajes::MySQL)
                    {        
                        ofile << "\t\t$dt = null;"  << std::endl;
                    }
                    else if(configureProject.getInputLenguaje() == InputLenguajes::MariaDB)
                    {
                        ofile << "\t\t$dt = null;"  << std::endl;
                    }
                    else if(configureProject.getInputLenguaje() == InputLenguajes::PostgreSQL)
                    {
                        ofile << "\t\t$dt = null;"  << std::endl;
                    }
                    else
                    {
                        std::string msg = "Lenguaje no soportado " ;
                        throw BuildException(msg);
                    }
                    break;
                    break;
                default:
                    throw BuildException("Lgenguaje no soportado",__FILE__,__LINE__);            
            }
            
            switch(configureProject.outputLenguaje)
            {
                case OutputLenguajes::CPP:
                    ofile << "\t\tbool flag = connector.select(sqlString,dt);"  << std::endl;
                    ofile << "\t\tif(flag)"  << std::endl;
                    break;
                case OutputLenguajes::JAVA:
                    ofile << "\t\tdt = connector.select(sqlString);"  << std::endl;
                    ofile << "\t\tif(dt != null)"  << std::endl;
                    break;
                case OutputLenguajes::PHP:
                    ofile << "\t\t$dt = $connector->select($sqlString);"  << std::endl;
                    ofile << "\t\tif(!is_null($dt))"  << std::endl;
                    break;
                default:
                    throw BuildException("Lenguaje no soportado",__FILE__,__LINE__);            
            }
            ofile << "\t\t{\n"; 
            switch(configureProject.outputLenguaje)
            {
                case OutputLenguajes::CPP:
                    ofile << "\t\t\tif(!dt.nextRow()) return false;\n";
                    break;
                case OutputLenguajes::JAVA:
                    ofile << "\t\t\tif(!dt.next()) return false;\n";
                    break;
                case OutputLenguajes::PHP:
                    ofile << "\t\t\tif(!$dt->nextRow()) return false;\n";
                    break;
                default:
                    throw BuildException("Lenguaje no soportado",__FILE__,__LINE__);            
            }
            if(symbol.second->symbolReferenced != NULL)
            {
                ofile << "\t\t\t" << symbol.second->name << " = new " << symbol.second->classReferenced->name << "(";
                if(symbol.second->outType.compare("std::string") == 0)
                {
                    ofile << symbol.second->getName();
                }
                else if(symbol.second->outType.compare("int") == 0)
                {
                    ofile << "dt.getint(0)";
                }
                else if(symbol.second->outType.compare("long") == 0)
                {
                    ofile << "dt.getl(0)";
                }
                else
                {
                    if((*table.key.begin())->symbolReferenced)
                    {
                        ofile << "(*" << (*table.key.begin())->name << ")";
                        inheritField(ofile,(*table.key.begin())->symbolReferenced,opReference());
                    }
                    else
                    {
                        ofile << "(" << (*table.key.begin())->name << ")";                        
                    }
                }
                ofile << ");\n";
            }
            else
            {
                short fielNumber = 0;
                switch(configureProject.outputLenguaje)
                {
                    case OutputLenguajes::CPP:
                        fielNumber = 0;
                        break;
                    case OutputLenguajes::JAVA:
                        fielNumber = 1;
                        break;
                    case OutputLenguajes::PHP:
                            
                        break;
                    default:
                            throw BuildException("Lenguaje no soportado",__FILE__,__LINE__);            
                }
                ofile << "\t\t\t";
                if(configureProject.outputLenguaje == OutputLenguajes::PHP) ofile << "$this->";
                ofile << symbol.second->name << " = ";
                if(configureProject.outputLenguaje == OutputLenguajes::PHP) ofile << "$";
                ofile << "dt" << opReference();
                if(symbol.second->outType.compare(stringType()) == 0)
                {
                    switch(configureProject.outputLenguaje)
                    {
                        case OutputLenguajes::CPP:
                            ofile << "getString(" << fielNumber << ")";
                            break;
                        case OutputLenguajes::JAVA:
                            ofile << "getString(" << fielNumber << ")";
                            break;
                        case OutputLenguajes::PHP:
                            ofile << "getString(\"" << symbol.second->name << "\")";
                            break;
                        default:
                            throw BuildException("Lenguaje no soportado",__FILE__,__LINE__);            
                    }
                }
                else if(symbol.second->outType.compare(integerType()) == 0)
                {
                    switch(configureProject.outputLenguaje)
                    {
                        case OutputLenguajes::CPP:
                            ofile << "getint(" << fielNumber << ")";
                            break;
                        case OutputLenguajes::JAVA:
                            ofile << "getInt(" << fielNumber << ")";
                            break;
                        case OutputLenguajes::PHP:
                            ofile << "getString(\"" << symbol.second->name << "\")";
                            break;
                        default:
                            throw BuildException("Lgenguaje no soportado",__FILE__,__LINE__);            
                    }
                }
                else if(symbol.second->outType.compare("long") == 0)
                {
                    switch(configureProject.outputLenguaje)
                    {
                        case OutputLenguajes::CPP:
                            ofile << "getl(" << fielNumber << ")";
                            break;
                        case OutputLenguajes::JAVA:
                            ofile << "getInt(" << fielNumber << ")";
                            break;
                        case OutputLenguajes::PHP:
                            ofile << "getString(\"" << symbol.second->name << "\")";
                            break;
                        default:
                            throw BuildException("Lgenguaje no soportado",__FILE__,__LINE__);            
                    }
                }
                else if(symbol.second->outType.compare("long") == 0)
                {
                    ofile << "getl(0)";
                }
                else if(symbol.second->outType.compare("long long") == 0)
                {
                    ofile << "getll(0)";
                }
                else if(symbol.second->outType.compare(floatType()) == 0)
                {      
                    switch(configureProject.outputLenguaje)
                    {
                        case OutputLenguajes::CPP:
                            ofile << "getfloat(" << fielNumber << ")"; 
                            break;
                        case OutputLenguajes::JAVA:
                            ofile << "getFloat(" << fielNumber << ")"; 
                            break;
                        case OutputLenguajes::PHP:
                            ofile << "getString(\"" << symbol.second->name << "\")"; 
                            break;
                        default:
                            throw BuildException("Lgenguaje no soportado",__FILE__,__LINE__);            
                    }
                }
                else if(symbol.second->outType.compare(doubleType()) == 0)
                {
                    switch(configureProject.outputLenguaje)
                    {
                        case OutputLenguajes::CPP:
                            ofile << "getdouble(" << fielNumber << ")";
                            break;
                        case OutputLenguajes::JAVA:
                            ofile << "getDouble(" << fielNumber << ")";
                            break;
                        case OutputLenguajes::PHP:
                            ofile << "getString(\"" << symbol.second->name << "\")";
                            break;
                        default:
                            throw BuildException("Lgenguaje no soportado",__FILE__,__LINE__);            
                    }
                }
                else if(symbol.second->outType.compare("char") == 0)
                {
                    ofile << "getchar(0)";
                }
                else if(symbol.second->outType.compare(byteType()) == 0)
                {            
                    switch(configureProject.outputLenguaje)
                    {
                        case OutputLenguajes::CPP:
                            ofile << "getchar(0)"; 
                            break;
                        case OutputLenguajes::JAVA:
                            ofile << "getByte(0)"; 
                            break;
                        case OutputLenguajes::PHP:
                            ofile << "getString(\"" << symbol.second->name << "\")";
                            break;
                        default:
                            throw BuildException("Lenguaje no soportado",__FILE__,__LINE__);            
                    }
                }
                else if(symbol.second->outType.compare("signed char") == 0)
                {
                    ofile << "getschar(0)";
                }
                else if(symbol.second->outType.compare("unsigned char") == 0)
                {
                    ofile << "getuchar(0)";
                }
                else if(symbol.second->outType.compare("short") == 0)
                {
                    ofile << "getshort(0)";
                }
                else if(symbol.second->outType.compare("signed short") == 0)
                {
                    ofile << "getshort(0)";
                }
                else if(symbol.second->outType.compare("unsigned short") == 0)
                {
                    ofile << "getushort(0)";
                }
                else
                {
                    std::string msg = "El tipo de dato '";
                    msg += symbol.second->outType + "' no tiene conversion en el metodo de descarga.";
                    throw BuildException(msg,__FILE__,__LINE__);
                }
            }
            ofile << ";\n";
            ofile << "\t\t\treturn true;\n";
            ofile << "\t\t}\n";
            ofile << "\t\treturn false;\n";
            ofile << "\t}\n";            
        }
    }


    bool Download::generate()
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
        
        throw BuildException("Deve especificar si es definicion o implemtacion.",__FILE__,__LINE__);
    }
}
