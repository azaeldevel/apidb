

#include "analyzer.hpp"
#include "generator.hpp"

namespace octetos::apidb::generators
{
    Select::Select(const ConfigureProject& c,const apidb::symbols::Table& t,std::ofstream& o) : Operation(c,t,o)
    {
    }
    
    bool Select::definite_static()
    {
        if(configureProject.getInputLenguaje() == InputLenguajes::MySQL)
        {
            ofile << "\t\tstatic std::vector<" << table.getName() << "*>* select(octetos::db::mysql::Connector& connector,const std::string& where, int leng = -1);"<<std::endl;
        }
        else if(configureProject.getInputLenguaje() == InputLenguajes::MariaDB)
        {
            ofile << "\t\tstatic std::vector<" << table.getName() << "*>* select(octetos::db::maria::Connector& connector,const std::string& where, int leng = -1,char order = 0);"<<std::endl;
        }
        else if(configureProject.getInputLenguaje() == InputLenguajes::PostgreSQL)
        {
            ofile << "\t\tstatic std::vector<" << table.getName() << "*>* select(octetos::db::postgresql::Connector& connector,const std::string& where, int leng = -1);"<<std::endl;
        }
        else
        {
            std::string msg = "Lenguaje no soportado " ;
            throw BuildException(msg,__FILE__,__LINE__);
        }
        
        const ConfigureProject::Table* tb = configureProject.findSelectTable(table.getName());
        if(tb != NULL) 
        {
            //std::cout << "Se encontro la tabla '" << table.getName() << std::endl;
            for(std::map<const char*, const apidb::ConfigureProject::Function*>::const_iterator itT = tb->begin(); itT != tb->end(); itT++)
            {
                if(configureProject.getInputLenguaje() == InputLenguajes::MySQL)
                {       
                    ofile << "\t\tstatic std::vector<" << table.getName() << "*>* " << itT->second->getName() << "(octetos::db::mysql::Connector& connector,";
                }
                else if(configureProject.getInputLenguaje() == InputLenguajes::MariaDB)
                {
                    ofile << "\t\tstatic std::vector<" << table.getName() << "*>* " << itT->second->getName() << "(octetos::db::maria::Connector& connector,";
                }
                else if(configureProject.getInputLenguaje() == InputLenguajes::PostgreSQL)
                {
                    ofile << "\t\tstatic std::vector<" << table.getName() << "*>* " << itT->second->getName() << "(octetos::db::postgresql::Connector& connector,";
                }
                else
                {
                    std::string msg = "Lenguaje no soportado " ;
                    throw BuildException(msg);
                }
                                
                //std::cout << "Generando codigo para  : " << itT->second->getName() << std::endl;
                const apidb::ConfigureProject::Parameters* params = itT->second->getParameters();                                
                apidb::ConfigureProject::Parameters::const_iterator itParamEnd = params->end();
                itParamEnd--;
                for(const std::string& param : *params)
                {
                    auto fl = table.find(param.c_str());
                    if(fl != table.end())
                    {
                        if((*fl).second->getOutType().compare("std::string") == 0)
                        {
                            ofile << "const std::string& ";
                        }
                        else if((*fl).second->getSymbolReferenced() != NULL)
                        {
                            ofile << "const " << (*fl).second->getSymbolReferenced()->getClassParent()->getName() << "& ";
                        }
                        else
                        {
                            ofile << (*fl).second->getOutType() << " ";                            
                        }
                    }
                    ofile << param; 
                    if(param != *itParamEnd)
                    {
                        ofile << ",";
                    }
                }
                ofile << ", int leng = 0);"<<std::endl;                                
            }
        }
        return true;
    }
    
    
    bool Select::implement_static()
    {
        if(configureProject.getInputLenguaje() == InputLenguajes::MySQL)
        {
            switch(configureProject.outputLenguaje)
            {
                case OutputLenguajes::CPP:
                    ofile << "\tstd::vector<"  << table.getName() << "*>* " << table.getName() << "::select(octetos::db::mysql::Connector& connector, const std::string& where, int leng)"<<std::endl;
                    break;
                case OutputLenguajes::JAVA:
                    ofile << "\tpublic static ArrayList<"  << table.getName() << "> " << "select(octetos.db.mysql.Connector connector,String where, int leng)  throws SQLException"<<std::endl;
                    break;
                case OutputLenguajes::PHP:
                    ofile << "\tpublic static function select($connector,$where,$leng = -1)"<<std::endl;
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
                    ofile << "\tstd::vector<"  << table.getName() << "*>* " << table.getName() << "::select(octetos::db::maria::Connector& connector, const std::string& where, int leng, char order)"<<std::endl;
                    break;
                case OutputLenguajes::JAVA:
                    ofile << "\tpublic static ArrayList<"  << table.getName() << "> " << "select(octetos.db.maria.Connector connector,String where, int leng, char order)  throws SQLException"<<std::endl;
                    break;
                case OutputLenguajes::PHP:
                    ofile << "\tpublic static function select($connector,$where,$leng = -1, $order = \"\")"<<std::endl;
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
                    ofile << "\tstd::vector<"  << table.getName() << "*>* " << table.getName() << "::select(octetos::db::postgresql::Connector& connector, const std::string& where, int leng)"<<std::endl;
                    break;
                case OutputLenguajes::JAVA:
                    ofile << "\tpublic static ArrayList<"  << table.getName() << "> " << "select(octetos.db.postgresql.Connector connector,String where, int leng)  throws SQLException"<<std::endl;
                    break;
                case OutputLenguajes::PHP:
                    ofile << "\tpublic static function select($connector,$where,$leng = -1)"<<std::endl;
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
        ofile << "\t{" <<std::endl;
        ofile << "\t\t";
        if(configureProject.outputLenguaje != OutputLenguajes::PHP) ofile << stringType() << " ";
        ofile << getsqlString() << " = \"SELECT ";
        //selecciona los campos de las llaves
        if(table.getKey().size() > 0)
        {
            auto endK = table.getKey().end();
            endK--;
            for(auto k : table.getKey())
            {
                ofile << k->getName();
                if(k != *endK)
                {
                    ofile << ",";
                }
            }
        }
        else
        {
            auto endK = table.end();
            endK--;
            for(auto k : table)
            {
                ofile << k.second->getName();
                if(k != *endK)
                {
                    ofile << ",";
                }
            }
        }
        if(configureProject.getInputLenguaje() == InputLenguajes::PostgreSQL)
		{
			ofile << " FROM \\\"" << table.getName() << " \";"<< std::endl;
		}
		else
		{
			ofile << " FROM " << table.getName() << " \";"<< std::endl;
		}
		ofile << "\t\t if(not where.empty())" << getsqlString();
        if(configureProject.outputLenguaje == OutputLenguajes::PHP) ofile << " .";
        if(configureProject.outputLenguaje == OutputLenguajes::CPP or configureProject.outputLenguaje == OutputLenguajes::JAVA) ofile << " +";
        ofile<< "= ";        
        if(configureProject.outputLenguaje == OutputLenguajes::PHP) ofile << "$";
        ofile << " + \" WHERE \" + where;\n";
        
        if(configureProject.outputLenguaje == OutputLenguajes::CPP or configureProject.outputLenguaje == OutputLenguajes::JAVA) ofile << "\t\tif(order == 'a' || order == 'A')\n";
        if(configureProject.outputLenguaje == OutputLenguajes::PHP) ofile << "\t\tif(strcmp($order,\"A\") == 0)\n";
        ofile << "\t\t{\n";
        ofile << "\t\t\t" << getsqlString() << " = " << getsqlString()  << " " << opConcat() << " " << "\" ORDER BY ";
        echoKeyListName();
        ofile << " ASC \";\n"; 
        ofile << "\t\t}\n";
        if(configureProject.outputLenguaje == OutputLenguajes::CPP or configureProject.outputLenguaje == OutputLenguajes::JAVA) ofile << "\t\telse if(order == 'd' || order == 'D')\n";
        if(configureProject.outputLenguaje == OutputLenguajes::PHP) ofile << "\t\tif(strcmp($order,\"D\") == 0)\n";
        ofile << "\t\t{\n";
        ofile << "\t\t\t" << getsqlString() << " = " << getsqlString() << " " << opConcat() << " " << "\" ORDER BY ";
        echoKeyListName();
        ofile << " DESC \";\n"; 
        ofile << "\t\t}\n";
        
        ofile << "\t\tif(";
        if(configureProject.outputLenguaje == OutputLenguajes::PHP) ofile << "$";
        ofile << "leng > 0)"  << std::endl;
        ofile << "\t\t{"  << std::endl;
        ofile << "\t\t\t" << getsqlString();        
        if(configureProject.outputLenguaje == OutputLenguajes::PHP) ofile << " .";
        if(configureProject.outputLenguaje == OutputLenguajes::CPP or configureProject.outputLenguaje == OutputLenguajes::JAVA) ofile << " +";
        ofile << "= \" LIMIT \";"  << std::endl;
		ofile << "\t\t\t" << getsqlString();        
        switch(configureProject.outputLenguaje)
        {
            case OutputLenguajes::CPP:
                ofile << " += std::to_string(leng);\n";
                break;
            case OutputLenguajes::JAVA:
                ofile << " += leng;\n";
                break;
            case OutputLenguajes::PHP:
                ofile << " .= $leng;\n";
                break;
            default:
                throw BuildException("Lgenguaje no soportado",__FILE__,__LINE__);            
        }
        ofile << "\t\t}"  << std::endl;
        
        if(configureProject.getInputLenguaje() == InputLenguajes::MySQL)
        {
            switch(configureProject.outputLenguaje)
            {
                case OutputLenguajes::CPP:
                    ofile << "\t\toctetos::db::mysql::Datresult dt;"  << std::endl;
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
        }
        else if(configureProject.getInputLenguaje() == InputLenguajes::MariaDB)
        {
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
        }
        else if(configureProject.getInputLenguaje() == InputLenguajes::PostgreSQL)
        {          
            switch(configureProject.outputLenguaje)
            {
                case OutputLenguajes::CPP:
                    ofile << "\t\toctetos::db::postgresql::Datresult dt;"  << std::endl;
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
        }
        else
        {
            std::string msg = "Lenguaje no soportado " ;
            throw BuildException(msg);
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
                throw BuildException("Lgenguaje no soportado",__FILE__,__LINE__);            
        }        
        ofile << "\t\t{" << std::endl;
        switch(configureProject.outputLenguaje)
        {
            case OutputLenguajes::CPP:
                ofile << "\t\t\tstd::vector<"<< table.getName() << "*>* tmpVc = new std::vector<" << table.getName() << "*>;" << std::endl;
                ofile << "\t\t\twhile(dt.nextRow())" << std::endl;
                break;
            case OutputLenguajes::JAVA:
                ofile << "\t\t\tArrayList<"<< table.getName() << "> tmpVc = new ArrayList<" << table.getName() << ">();" << std::endl;
                ofile << "\t\t\twhile(dt.next())" << std::endl;
                break;
            case OutputLenguajes::PHP:
                ofile << "\t\t\t$tmpVc = array();" << std::endl;
                ofile << "\t\t\twhile($dt->nextRow())" << std::endl;
                break;
            default:
                throw BuildException("Lenguaje no soportado",__FILE__,__LINE__);            
        }        
        ofile << "\t\t\t{"<< std::endl;
        //TODO: implemetar fuciones select en en caso de que no hay key
        switch(configureProject.outputLenguaje)
        {
            case OutputLenguajes::CPP:
                ofile << "\t\t\t\t"<< table.getName() << "* tmp = NULL;" << std::endl;
                break;
            case OutputLenguajes::JAVA:
                ofile << "\t\t\t\t"<< table.getName() << " tmp = null;" << std::endl;
                break;
            case OutputLenguajes::PHP:
                
                break;
            default:
                throw BuildException("Lenguaje no soportado",__FILE__,__LINE__);            
        }
        ofile << "\t\t\t\t";
        if(configureProject.outputLenguaje == OutputLenguajes::PHP)
        {
            ofile << "$tmp = new " << table.getName() << "();\n";
            ofile << "\t\t\t\t$tmp->create(";
        }
        else
        {
            ofile << "tmp = new " << table.getName() << "(";
        }
        
        auto endK2 = table.getKey().end();
        endK2--;
        int count2 = 0;
        if(configureProject.outputLenguaje == OutputLenguajes::JAVA or configureProject.outputLenguaje == OutputLenguajes::PHP) count2 = 1;
        for(auto k : table.getKey())
        {
            if(k->getOutType().compare(stringType()) == 0)
            {
                if(configureProject.outputLenguaje == OutputLenguajes::PHP) ofile << "$";
                ofile << "dt";
                if(configureProject.outputLenguaje == OutputLenguajes::CPP or configureProject.outputLenguaje == OutputLenguajes::JAVA) ofile << ".";
                if(configureProject.outputLenguaje == OutputLenguajes::PHP) ofile << "->";
                ofile << "getString(\"" << k->name << "\")";
            }
            else if(k->getOutType().compare(integerType()) == 0)
            {
                if(configureProject.outputLenguaje == OutputLenguajes::PHP) ofile << "$";
                ofile << "dt";
                if(configureProject.outputLenguaje == OutputLenguajes::CPP or configureProject.outputLenguaje == OutputLenguajes::JAVA) ofile << ".";
                if(configureProject.outputLenguaje == OutputLenguajes::PHP) ofile << "->";
                if(configureProject.outputLenguaje == OutputLenguajes::CPP) ofile << "getint(" << count2 << ")";
                if(configureProject.outputLenguaje == OutputLenguajes::JAVA) ofile << "getInt(" << count2 << ")";
                if(configureProject.outputLenguaje == OutputLenguajes::PHP) ofile << "getInteger(\"" << k->name << "\")";
            }
            else
            {
                if(configureProject.outputLenguaje == OutputLenguajes::PHP) ofile << "$";
                ofile << "dt";
                if(configureProject.outputLenguaje == OutputLenguajes::CPP or configureProject.outputLenguaje == OutputLenguajes::JAVA) ofile << ".";
                if(configureProject.outputLenguaje == OutputLenguajes::PHP) ofile << "->";
                ofile << "getString(\"" << k->name << "\")";                
            }
            if(k != *endK2)
            {
                ofile << ",";
            }
            count2++;
        }
        ofile << ")" << ";" << std::endl;
        switch(configureProject.outputLenguaje)
        {
            case OutputLenguajes::CPP:
                ofile << "\t\t\t\ttmpVc->push_back(tmp);" << std::endl;                
                break;
            case OutputLenguajes::JAVA:
                ofile << "\t\t\t\ttmpVc.add(tmp);" << std::endl;
                break;
            case OutputLenguajes::PHP:
                ofile << "\t\t\t\tarray_push($tmpVc,$tmp);" << std::endl;
                break;
            default:
                throw BuildException("Lgenguaje no soportado",__FILE__,__LINE__);            
        }
        ofile << "\t\t\t}"<< std::endl;
        ofile << "\t\t\treturn ";
        if(configureProject.outputLenguaje == OutputLenguajes::PHP) ofile << "$";
        ofile << "tmpVc;" << std::endl;
        ofile << "\t\t}" << std::endl;
        if(configureProject.outputLenguaje == OutputLenguajes::CPP) ofile << "\t\treturn NULL;\n";
        if(configureProject.outputLenguaje == OutputLenguajes::JAVA or configureProject.outputLenguaje == OutputLenguajes::PHP) ofile << "\t\treturn null;\n";
        ofile << "\t}" <<std::endl;
        
        
        //select from config
        //std::vector<apidb::ConfigureProject::Table> tbs = configureProject.selects;
        for( std::map<const char*,ConfigureProject::Table*>::const_iterator itT =  configureProject.selects.begin(); itT != configureProject.selects.end(); itT++)//std::vector<Table>
        {
            if(table.getName().compare(itT->second->getName()) != 0) 
            {
                continue;//buscar la configuracion de la tabla correspondiente
            }
            
            //for (auto const& [key, val] : *(itT->second))//class Table : public std::map<std::string,Function>
            for(ConfigureProject::Table::iterator itCfTb = itT->second->begin(); itCfTb != itT->second->end(); itCfTb++)
            {
                if(configureProject.getInputLenguaje() == InputLenguajes::MySQL)
                {        
                    ofile << "\tstd::vector<" << table.getName()<< "*>* " << table.getName() << "::" << itCfTb->second->getName() << "(octetos::db::mysql::Connector& connector,";
                }
                else if(configureProject.getInputLenguaje() == InputLenguajes::MariaDB)
                {
                    ofile << "\tstd::vector<" << table.getName()<< "*>* " << table.getName() << "::" << itCfTb->second->getName() << "(octetos::db::maria::Connector& connector,";
                }
                else if(configureProject.getInputLenguaje() == InputLenguajes::PostgreSQL)
                {
                    ofile << "\tstd::vector<" << table.getName()<< "*>* " << table.getName() << "::" << itCfTb->second->getName() << "(octetos::db::postgresql::Connector& connector,";
                }
                else
                {
                    std::string msg = "Lenguaje no soportado " ;
                    throw BuildException(msg);
                }
                
                const apidb::ConfigureProject::Parameters* params = itCfTb->second->getParameters();
                
                apidb::ConfigureProject::Parameters::const_iterator itParamEnd = params->end();
                itParamEnd--;
                for(const std::string& param : *params)
                {
                    auto fl = table.find(param.c_str());
                    if(fl == table.end())
                    {
                        std::string strmsg = "No se encontro el campo '";
                        strmsg = strmsg + param + "' en la tabla '" + table.getName() + "'" + "File : generator-c++";
                        throw BuildException(strmsg);
                    }
                    if((*fl).second->getOutType().compare("std::string") == 0)
                    {
                        ofile << "const std::string& " << param; 
                    }
                    else if((*fl).second->getSymbolReferenced() != NULL)
                    {
                                ofile << "const " << (*fl).second->getSymbolReferenced()->getClassParent()->getName() << "& " << param;
                    }
                    else
                    {
                            ofile << (*fl).second->getOutType() << " " << param;
                    }
                    if(param != *itParamEnd)
                    {
                        ofile << ",";
                    }
                }
                
                ofile << ", int leng)"<<std::endl;
                ofile << "\t{"<<std::endl;
                ofile << "\t\tstd::string sqlString = \"SELECT ";
                //const apidb::ConfigureProject::Parameters& params = val->getParameters();
                
                auto endK = table.getKey().end();
                endK--;
                for(auto k : table.getKey())
                {
                    ofile << k->getName();
                    if(k != *endK)
                    {
                        ofile << ",";
                    }
                }
				if(configureProject.getInputLenguaje() == InputLenguajes::PostgreSQL)
				{
                    ofile << " FROM \\\"" << table.getName() << "\\\" WHERE \";"<< std::endl;  
				}
				else
				{
					ofile << " FROM " << table.getName() << " WHERE \";"<< std::endl; 
				}
                itParamEnd = params->end();
                --itParamEnd;
                for(const std::string& param : *params)
                {
                    auto fl = table.find(param.c_str());
                    if(fl != table.end())
                    {
                        if((*fl).second->getClassReferenced() != NULL && (*fl).second->getOutType().compare("int") == 0)
                        {
                            //ofile << "\t\tsqlString = sqlString + \"" << param << " = \" + \"'\" + std::to_string(" << (*fl).second->getName() << ") + \"'\"";
                            ofile << "\t\tsqlString = sqlString + ";
                            echoKeyRawParam();
                        }
                        else if((*fl).second->getOutType().compare("int") == 0)
                        {
                            ofile << "\t\tsqlString = sqlString + \"" << param << " = \" + \"'\" + std::to_string(" << (*fl).second->getName() << ") + \"'\"";
                        }
                        else if((*fl).second->getOutType().compare("std::string") == 0)
                        {
                            ofile << "\t\tsqlString = sqlString + \"" << param << " = \" + \"'\" + " << (*fl).second->getName() << " + \"'\"";
                        }
                        else
                        {
                            std::string strmsg = "No se encontro el campo ";
                            strmsg = strmsg + "'" + (*fl).second->getName() + "' en la tabla '" + table.getName() + "'" + "File : generator-c++";
                            throw BuildException(strmsg);                                
                        }
                    }
                    else
                    {
                        std::string strmsg = "No se encontro el campo ";
                        strmsg = strmsg + "'" + param + "' en la tabla '" + table.getName() + "'";
                        throw BuildException(strmsg);
                    }
                    if(param != *itParamEnd)
                    {
                        ofile << " + \" and \";"<< std::endl;
                    }
                }
                ofile << ";" << std::endl;
                ofile << "\t\tif(leng > 0)"  << std::endl;
                ofile << "\t\t{"  << std::endl;
                ofile << "\t\t\tsqlString += \" LIMIT  \" + std::to_string(leng);"  << std::endl;
                ofile << "\t\t}"  << std::endl;
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
                ofile << "\t\t\tstd::vector<"<< table.getName() << "*>* tmpVc = new std::vector<" << table.getName() << "*>;" << std::endl;
                ofile << "\t\t\twhile(dt.nextRow()) "<< std::endl;
                ofile << "\t\t\t{"<< std::endl;     
                ofile << "\t\t\t\t"<< table.getName() << "* tmp = NULL;" << std::endl;             
                ofile << "\t\t\t\ttmp = new " << table.getName() << "(";              
                auto endK2 = table.getKey().end();
                endK2--;
                int count2 = 0;
                for(auto k : table.getKey())
                {
                    if(k->getOutType().compare("std::string") == 0)
                    {
                        ofile << "dt.getString(" << count2 << ")";
                    }
                    else if(k->getOutType().compare("int") == 0)
                    {
                        ofile << "dt.getint(" << count2 << ")";
                    }
                    else
                    {
                        ofile << "dt.getString(" << count2 << ")";               
                    }
                    if(k != *endK2)
                    {
                        ofile << ",";
                    }
                    count2++;
                }
                ofile << ")" << ";" << std::endl;
                ofile << "\t\t\t\ttmpVc->push_back(tmp);" << std::endl;
                ofile << "\t\t\t}"<< std::endl;
                ofile << "\t\t\treturn tmpVc;"<< std::endl;
                ofile << "\t\t}" << std::endl;
                ofile << "\t\telse" << std::endl;
                ofile << "\t\t{" << std::endl;
                ofile << "\t\t\treturn NULL;" << std::endl;
                ofile << "\t\t}" << std::endl;
                ofile << "\t}"<<std::endl;
            }
        }
        return true;
    }
    bool Select::definite_rawdata()
    {
        //constructor que toma key como parametro
        ofile << "\t\t"<< "bool ";
        if(configureProject.getInputLenguaje() == InputLenguajes::MySQL)
        {        
            ofile << "select(octetos::db::mysql::Connector& connector";
        }
        else if(configureProject.getInputLenguaje() == InputLenguajes::MariaDB)
        {
            ofile << "select(";
            switch(configureProject.outputLenguaje)
            {
                case OutputLenguajes::CPP:
                    ofile << "octetos::db::maria::Connector& connector";
                    break;
                case OutputLenguajes::JAVA:
                    ofile << "octetos.db.maria.Connector connector";
                    break;
                case OutputLenguajes::PHP:
                    ofile << "$connector";
                    break;
                default:
                    throw BuildException("Lgenguaje no soportado",__FILE__,__LINE__);            
            }
        }
        else if(configureProject.getInputLenguaje() == InputLenguajes::PostgreSQL)
        {
            ofile << "select(octetos::db::postgresql::Connector& connector";
        }
        else
        {
            std::string msg = "Lenguaje no soportado " ;
            throw BuildException(msg);
        }
        for(symbols::Symbol* k : table.getKey())
        {
			if(k->symbolReferenced != NULL)
			{
                if(configureProject.outputLenguaje == OutputLenguajes::CPP ) ofile << ",const ";
                if(configureProject.outputLenguaje == OutputLenguajes::PHP ) ofile << ",$";
				ofile << k->classReferenced->getName();
                if(configureProject.outputLenguaje == OutputLenguajes::CPP ) ofile << "& ";
			}
			else
			{
				ofile << "," << k->getOutType() << " " ;
			}
			
			ofile << k->getName();
		}
        ofile << ");"<<std::endl;
        
        return true;
    }
    
    bool Select::implement_rawdata()
    {
        if(table.getKey().size() == 0) return false;
        
        switch(configureProject.outputLenguaje)
        {
            case OutputLenguajes::CPP:
                ofile << "\t"<< "bool " << table.name << "::";               
                break;
            case OutputLenguajes::JAVA:
                ofile << "\t"<< "public boolean ";
                break;
            case OutputLenguajes::PHP:
                ofile << "\t"<< "public function ";
                break;
            default:
                throw BuildException("Lgenguaje no soportado",__FILE__,__LINE__);            
        }
        
        if(configureProject.getInputLenguaje() == InputLenguajes::MySQL)
        {        
            ofile << "select(octetos::db::mysql::Connector& connector";
        }
        else if(configureProject.getInputLenguaje() == InputLenguajes::MariaDB)
        {
            switch(configureProject.outputLenguaje)
            {
                case OutputLenguajes::CPP:
                    ofile << "select(octetos::db::maria::Connector& connector";
                    break;
                case OutputLenguajes::JAVA:
                    ofile << "select(octetos.db.maria.Connector connector";
                    break;
                case OutputLenguajes::PHP:
                    ofile << "selectRaw($connector";
                    break;
                default:
                    throw BuildException("Lgenguaje no soportado",__FILE__,__LINE__);            
            }
        }
        else if(configureProject.getInputLenguaje() == InputLenguajes::PostgreSQL)
        {
            ofile << "select(octetos::db::postgresql::Connector& connector";
        }
        else
        {
            std::string msg = "Lenguaje no soportado " ;
            throw BuildException(msg);
        }
        for(symbols::Symbol* k : table.getKey())
        {
			if(k->symbolReferenced != NULL)
			{
                ofile << ",";
                if(configureProject.outputLenguaje == OutputLenguajes::CPP ) ofile << "const ";
                if(configureProject.outputLenguaje == OutputLenguajes::PHP ) ofile << "$";
				if(configureProject.outputLenguaje != OutputLenguajes::PHP ) 
                {
                    ofile << k->classReferenced->getName();
                    ofile << " ";
                }
                if(configureProject.outputLenguaje == OutputLenguajes::CPP ) ofile << "&";
			}
			else if(configureProject.outputLenguaje == OutputLenguajes::PHP)
			{
				ofile << ",$" ;
			}
			else
			{
				ofile << "," << k->getOutType() << " " ;
			}
			ofile << k->getName();
		}
        ofile << ")";
        if(configureProject.outputLenguaje == OutputLenguajes::JAVA) ofile << " throws SQLException";
        ofile << "\n";
        ofile << "\t{\n";
        ofile << "\t\t";
        if(configureProject.outputLenguaje == OutputLenguajes::CPP or configureProject.outputLenguaje == OutputLenguajes::JAVA) ofile << stringType() << " ";
        if(configureProject.outputLenguaje == OutputLenguajes::PHP) ofile << "$";
        ofile << "sqlString = \"SELECT ";
        symbols::Key::const_iterator penultimo = --table.getKey().end();
        //symbols::Symbol* penultimo = (--(table.end()))->second;
        for(symbols::Symbol* k : table.getKey())
        {
			ofile << " " << k->getName();
            if(k != (*penultimo))
            {
                ofile << ",";
            }
		}
		ofile << "\";\n\t\t";
        if(configureProject.outputLenguaje == OutputLenguajes::PHP) ofile << "$";
        ofile << "sqlString = ";
        if(configureProject.outputLenguaje == OutputLenguajes::PHP) ofile << "$";
        ofile << "sqlString";
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
        ofile << "\" FROM \"";
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
        ofile << "TABLE_NAME";
        if(table.getKey().size() > 0)
        {
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
            echoKeyRawParam();
        }
		ofile << ";\n";
        
        if(configureProject.getInputLenguaje() == InputLenguajes::MySQL)
        {        
            ofile << "\t\toctetos::db::mysql::Datresult dat;\n";
        }
        else if(configureProject.getInputLenguaje() == InputLenguajes::MariaDB)
        {
            ofile << "\t\t";
            switch(configureProject.outputLenguaje)
            {
                case OutputLenguajes::CPP:
                    ofile << "octetos::db::maria::Datresult dat;\n";
                    break;
                case OutputLenguajes::JAVA:
                    ofile << "ResultSet dat = null;\n";
                    break;
                case OutputLenguajes::PHP:
                    ofile << "$dat = null;\n";
                    break;
                default:
                    throw BuildException("Lgenguaje no soportado",__FILE__,__LINE__);            
            }
        }
        else if(configureProject.getInputLenguaje() == InputLenguajes::PostgreSQL)
        {
            ofile << "\t\toctetos::db::postgresql::Datresult dat;\n";
        }
        else
        {
            std::string msg = "Lenguaje no soportado " ;
            throw BuildException(msg);
        }
        ofile << "\t\t";
        switch(configureProject.outputLenguaje)
        {
            case OutputLenguajes::CPP:
                ofile << "bool retflag = connector.select(sqlString,dat);\n";
                ofile << "\t\tif(retflag)\n";
                break;
            case OutputLenguajes::JAVA:
                ofile << "dat = connector.select(sqlString);\n";
                ofile << "\t\tif(dat != null)\n";
                break;
            case OutputLenguajes::PHP:
                ofile << "$dat = $connector->select($sqlString);\n";
                ofile << "\t\tif(!is_null($dat))\n";
                break;
            default:
                throw BuildException("Lgenguaje no soportado",__FILE__,__LINE__);            
        }
		
        ofile << "\t\t{\n";
        /*for(symbols::Symbol* k : table.getKey())
        {
            if(k->symbolReferenced != NULL)
            {
                switch(configureProject.outputLenguaje)
                {
                    case OutputLenguajes::CPP:
                        ofile << "\t\t\tthis->";
                        break;
                    case OutputLenguajes::JAVA:
                        ofile << "\t\t\tthis.";
                        break;
                    case OutputLenguajes::PHP:
                        ofile << "\t\t\t$this->";
                        break;
                    default:
                        throw BuildException("Lgenguaje no soportado",__FILE__,__LINE__);            
                }
                ofile << k->name << " = new " << k->classReferenced->name << "();\n";
                if(configureProject.outputLenguaje == OutputLenguajes::PHP)
                {
                    ofile << "\t\t\t$this->" << k->name << "->create($" << k->name << ");\n";
                }
            }
            else
            {
                switch(configureProject.outputLenguaje)
                {
                    case OutputLenguajes::CPP:
                        ofile << "\t\t\tthis->";
                        break;
                    case OutputLenguajes::JAVA:
                        ofile << "\t\t\tthis.";
                        break;
                    case OutputLenguajes::PHP:
                        ofile << "\t\t\t$this->";
                        break;
                    default:
                        throw BuildException("Lgenguaje no soportado",__FILE__,__LINE__);            
                }
                ofile << k->name << " = ";
                if(configureProject.outputLenguaje == OutputLenguajes::PHP) ofile << "$";
                ofile << k->name << ";\n";
            }
        }*/
        echoCopyParams();
        //ofile << "\t\t\t";
        ofile << "\t\t}\n";
        //ofile << "\t\t\n";
        
        switch(configureProject.outputLenguaje)
        {
            case OutputLenguajes::CPP:
                ofile << "\t\treturn retflag;\n";
                break;
            case OutputLenguajes::JAVA:
                ofile << "\t\treturn dat != null;\n";
                break;
            case OutputLenguajes::PHP:
                ofile << "\t\treturn !is_null($dat);\n";
                break;
            default:
                throw BuildException("Lgenguaje no soportado",__FILE__,__LINE__);            
        }
        ofile << "\t}\n";
        
        return true;
    }
    bool Select::generate()
    {
        if(definition)
        {
            return definite_static() and definite_rawdata();
        }
        
        if(implementation)
        {
            return implement_static() and implement_rawdata();
        }
        
        throw BuildException("Deve especificar si es definicion o implemtacion.",__FILE__,__LINE__);
    }
}
