
#include "analyzer.hpp"
#include "generator.hpp"

namespace octetos::apidb::generators
{

  

    Insert::Insert(const ConfigureProject& c,const apidb::symbols::Table& t,std::ofstream& o) : Operation(c,t,o)
    {
    }
    
    bool Insert::definite()
    {        
        //inser Raw data
        ofile << "\t\t"<< "bool insert(";
        if(configureProject.getInputLenguaje() == InputLenguajes::MySQL)
        {
            switch(configureProject.outputLenguaje)
            {
                case OutputLenguajes::CPP:
                    ofile << "octetos::db::mysql::Connector& connector";
                    break;
                case OutputLenguajes::JAVA:
                    ofile << "octetos.db.mysql.Connector connector";
                    break;
                case OutputLenguajes::PHP:
                    ofile << "$connector";
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
            switch(configureProject.outputLenguaje)
            {
                case OutputLenguajes::CPP:
                    ofile << "octetos::db::posgresql::Connector& connector";
                    break;
                case OutputLenguajes::JAVA:
                    ofile << "octetos.db.posgresql.Connector connector";
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
            throw BuildException("Lgenguaje no soportado",__FILE__,__LINE__);            
        }
        for(symbols::Symbol* k : table.getRequired())
        {
            if(k->symbolReferenced!= NULL)
            {
                for(symbols::Symbol* l : k->classReferenced->getRequired())
                {
                    symbols::Symbol* rootS = getRootSymbol(l);
                    if(rootS->isPrimaryKey() and rootS->isAutoIncrement()) continue;
                    
                    if(l->symbolReferenced!= NULL)
                    {                        
                        ofile << ",";
                        insertParamsRaw(ofile,l,k);
                    }
                    else if(l->outType.compare(stringType()) == 0)
                    {
                        if(configureProject.outputLenguaje == OutputLenguajes::CPP) ofile << ",const ";
                        ofile << stringType();
                        if(configureProject.outputLenguaje == OutputLenguajes::CPP) ofile << "& " ;
                        ofile << k->name << l->upperName;
                    }
                    else
                    {
                        ofile << "," << k->getOutType() << " " << k->name << l->upperName;
                    }
                }
            }
			else if(k->outType.compare(stringType()) == 0)
            {
                if(configureProject.outputLenguaje == OutputLenguajes::CPP) ofile << ",const ";
                ofile << stringType();
                if(configureProject.outputLenguaje == OutputLenguajes::CPP) ofile << "& " ;
                ofile << k->name;
            }
            else
            {
                if(not k->isAutoIncrement() and not k->isPrimaryKey()) ofile << "," << k->getOutType() << " " << k->name;
            }
		}
        ofile << ");"<<std::endl;    
        return true;
    }
    
    
    bool Insert::implement()
    {
        auto penultimoReq = --table.getRequired().end();
        // Methodo insert Raw datas
        ofile << "\t";
        if(configureProject.getInputLenguaje() == InputLenguajes::MySQL)
        {
            switch(configureProject.outputLenguaje)
            {
                case OutputLenguajes::CPP:
                    ofile << "bool " << table.getName() << "::insert(octetos::db::mysql::Connector& connector";
                    break;
                case OutputLenguajes::JAVA:
                    ofile << "public boolean insert(octetos.db.mysql.Connector connector";
                    break;
                case OutputLenguajes::PHP:
                    ofile << "public function insert($connector";
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
                    ofile << "bool " << table.getName() << "::insert(octetos::db::maria::Connector& connector";
                    break;
                case OutputLenguajes::JAVA:
                    ofile << "public boolean insert(octetos.db.maria.Connector connector";
                    break;
                case OutputLenguajes::PHP:
                    ofile << "public function insert($connector";
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
                    ofile << "bool " << table.getName() << "::insert(octetos::db::postgresql::Connector& connector";
                    break;
                case OutputLenguajes::JAVA:
                    ofile << "public boolean insert(octetos.db.postgresqls.Connector connector";
                    break;
                case OutputLenguajes::PHP:
                    ofile << "public function insert($connector";
                    break;
                default:
                   throw BuildException("Lgenguaje no soportado",__FILE__,__LINE__);            
            }
        }
        else
        {
            throw BuildException("Lgenguaje no soportado",__FILE__,__LINE__);            
        }
        for(symbols::Symbol* k : table.getRequired())
        {
            if(k->symbolReferenced!= NULL)
            {
                for(symbols::Symbol* l : k->classReferenced->getRequired())
                {
                    symbols::Symbol* rootS = getRootSymbol(l);
                    if(rootS->isPrimaryKey() and rootS->isAutoIncrement()) continue;
                    
                    if(l->symbolReferenced!= NULL)
                    {                        
                        ofile << ",";
                        insertParamsRaw(ofile,l,k);
                    }
                    else if(l->outType.compare(stringType()) == 0)
                    {
                        ofile << ",";
                        if(configureProject.outputLenguaje == OutputLenguajes::CPP) ofile << "const " << stringType() << "& ";                        
                        if(configureProject.outputLenguaje == OutputLenguajes::JAVA) ofile << stringType() << " ";
                        if(configureProject.outputLenguaje == OutputLenguajes::PHP) ofile << "$";
                        ofile << k->name << l->upperName;
                    }
                    else
                    {
                        
                        switch(configureProject.outputLenguaje)
                        {
                            case OutputLenguajes::CPP:
                                ofile << "," << k->getOutType() << " " << k->name << l->upperName;
                                break;
                            case OutputLenguajes::JAVA:
                                ofile << "," << k->getOutType() << " " << k->name << l->upperName;
                                break;
                            case OutputLenguajes::PHP:
                                ofile << ", $" << k->name << l->upperName;
                                break;
                            default:
                                throw BuildException("Lgenguaje no soportado",__FILE__,__LINE__);            
                        }
                    }
                }
            }
			else if(k->outType.compare(stringType()) == 0)
            {
                ofile << ",";
                if(configureProject.outputLenguaje == OutputLenguajes::CPP) ofile << "const " << stringType() << "& ";                        
                if(configureProject.outputLenguaje == OutputLenguajes::JAVA) ofile << stringType() << " ";
                if(configureProject.outputLenguaje == OutputLenguajes::PHP) ofile << "$" ;
                ofile << k->name;
            }
            else
            {
                if(not k->isAutoIncrement() and not k->isPrimaryKey()) 
                {
                    ofile << "," << k->getOutType() << " " << k->name;
                }
            }
		}
        ofile << ")";
        if(configureProject.outputLenguaje == OutputLenguajes::JAVA) ofile << " throws SQLException";
        ofile << "\n\t{\n";
        
        //para cada campo foraneo
        for(symbols::Symbol* k : table.getRequired())
        {
            if(k->isPrimaryKey() && k->isAutoIncrement()) continue;
            
			if(k->symbolReferenced != NULL)
            {
                ofile << "\t\t";
                if(configureProject.outputLenguaje == OutputLenguajes::PHP) ofile << "$this->";
                ofile << k->name << " = new " << k->classReferenced->name << "();\n";
                ofile << "\t\tif(";
                if(configureProject.outputLenguaje == OutputLenguajes::PHP) ofile << "$this->";
                ofile << k->name;
                switch(configureProject.outputLenguaje)
                {
                    case OutputLenguajes::CPP:
                        ofile << "->";
                        break;
                    case OutputLenguajes::JAVA:
                        ofile << ".";
                        break;
                    case OutputLenguajes::PHP:
                        ofile << "->";
                        break;
                    default:
                    throw BuildException("Lgenguaje no soportado",__FILE__,__LINE__);            
                }
                ofile << "insert(";
                switch(configureProject.outputLenguaje)
                {
                    case OutputLenguajes::CPP:
                        ofile << "connector";
                        break;
                    case OutputLenguajes::JAVA:
                        ofile << "connector";
                        break;
                    case OutputLenguajes::PHP:
                        ofile << "$connector";
                        break;
                    default:
                        throw BuildException("Lgenguaje no soportado",__FILE__,__LINE__);            
                }
                for(symbols::Symbol* l : k->symbolReferenced->classParent->getRequired())
                {
                    if(l->symbolReferenced != NULL)
                    {
                        for(symbols::Symbol* m : l->symbolReferenced->classParent->getRequired())
                        {                            
                            symbols::Symbol* rootS = getRootSymbol(m);
                            if(rootS->isPrimaryKey() and rootS->isAutoIncrement()) continue;
                            
                            ofile << ",";
                            if(m->symbolReferenced!= NULL)
                            {
                                insertValueRaw(ofile,m,l);
                            }
                            else if(m->outType.compare(stringType()) == 0)
                            {
                                ofile << k->name << m->upperName;
                            }
                            else
                            {
                                ofile << k->name << m->upperName;
                            }
                        }
                    }
                    else if(l->outType.compare(stringType()) == 0)
                    {
                        symbols::Symbol* rootS = getRootSymbol(l);
                        if(rootS->isPrimaryKey() and rootS->isAutoIncrement()) continue;
                        
                        ofile << ",";
                        if(configureProject.outputLenguaje == OutputLenguajes::PHP) ofile << "$" ;
                        ofile << k->name << l->upperName;
                    }
                    else
                    {
                        symbols::Symbol* rootS = getRootSymbol(l);
                        if(rootS->isPrimaryKey() and rootS->isAutoIncrement()) continue;
                        
                        ofile << ",";                        
                        if(configureProject.outputLenguaje == OutputLenguajes::PHP) ofile << "$" ;
                        ofile << k->name << l->upperName;
                    }
                }
                ofile << ") == false) return false;\n";
            }            
		}

                
        ofile << "\t\t";
        switch(configureProject.outputLenguaje)
        {
            case OutputLenguajes::CPP:
                ofile << stringType() << " " ;
                break;
            case OutputLenguajes::JAVA:
                ofile << stringType()<< " " ;
                break;
            case OutputLenguajes::PHP:
                
                break;
            default:
                throw BuildException("Lgenguaje no soportado",__FILE__,__LINE__);            
        }
        ofile << getsqlString() << " = \"\";"<<std::endl;
		if(configureProject.getInputLenguaje() == InputLenguajes::PostgreSQL)
		{
			ofile << "\t\t" <<  getsqlString() << " = " << getsqlString() << " " << opConcat() << " \"INSERT INTO \\\"\"  ";
            if(configureProject.outputLenguaje == OutputLenguajes::CPP or configureProject.outputLenguaje == OutputLenguajes::JAVA) ofile << " + ";
            if(configureProject.outputLenguaje == OutputLenguajes::PHP) ofile << " . $this->";
			ofile << "TABLE_NAME + \"\\\"\" ; "<<std::endl;
		}
		else
		{
			ofile << "\t\t" <<  getsqlString() << " = " << getsqlString() << " " << opConcat() << " \"INSERT INTO \" ";
            if(configureProject.outputLenguaje == OutputLenguajes::CPP or configureProject.outputLenguaje == OutputLenguajes::JAVA) ofile << " + ";
            if(configureProject.outputLenguaje == OutputLenguajes::PHP) ofile << " . $this->";
			ofile << "TABLE_NAME ; "<<std::endl;
		}
        ofile << "\t\t" <<  getsqlString() << " = " << getsqlString() << " " << opConcat() << " \"(";
        for(symbols::Symbol* k : table.getRequired())
        {
            if(k->isPrimaryKey() and k->isAutoIncrement()) continue;
            
            ofile << k->name;
            if(*penultimoReq != k)
            {
                ofile << ",";
            }
        }
		ofile << ")\";" << std::endl;
		ofile << "\t\t" <<  getsqlString() << " = " << getsqlString() << " " << opConcat() << " \" VALUES(\"";
		for(symbols::Symbol* k : table.getRequired())
        {
            if(k->isPrimaryKey() and k->isAutoIncrement()) continue;
            
            if(k->symbolReferenced != NULL)
            {                
                if(getRootSymbol(k->symbolReferenced)->outType.compare(stringType()) == 0)
                {
                    ofile << " " << opConcat() << " \"'\" ";
                    insertValueRaw(ofile,k->symbolReferenced,k);
                    ofile << " " << opConcat() << " \"'\" ";
                }
                else
                {
                    switch(configureProject.outputLenguaje)
                    {
                        case OutputLenguajes::CPP:
                            ofile << " + std::to_string((*";
                            break;
                        case OutputLenguajes::JAVA:
                            ofile << " + ";
                            break;
                        case OutputLenguajes::PHP:
                            ofile << " " << opConcat() << " ";
                            break;
                        default:
                            throw BuildException("Lgenguaje no soportado",__FILE__,__LINE__);            
                    }
                    
                    if(configureProject.outputLenguaje == OutputLenguajes::PHP) ofile << "$this->";
                    ofile << k->name;
                    if(configureProject.outputLenguaje == OutputLenguajes::CPP) ofile << ")";
                    inheritField(ofile,k->symbolReferenced,opReference());
                    if(configureProject.outputLenguaje == OutputLenguajes::CPP) ofile << ")";                                       
                }
            }
            else if(k->outType.compare(stringType()) == 0)
            {
                ofile << " " << opConcat() << " " << " \"'\" ";
                if(configureProject.outputLenguaje == OutputLenguajes::CPP or configureProject.outputLenguaje == OutputLenguajes::JAVA) ofile << " + ";
                if(configureProject.outputLenguaje == OutputLenguajes::PHP) ofile << " . ";
                if(configureProject.outputLenguaje == OutputLenguajes::PHP) ofile << "$";
                ofile << k->name;
                if(configureProject.outputLenguaje == OutputLenguajes::CPP or configureProject.outputLenguaje == OutputLenguajes::JAVA) ofile << " + ";
                if(configureProject.outputLenguaje == OutputLenguajes::PHP) ofile << " . ";
                ofile << "\"'\"";
            }
            else
            {                
                switch(configureProject.outputLenguaje)
                {
                    case OutputLenguajes::CPP:
                        ofile << " + std::to_string(" << k->name << ")";
                        break;
                    case OutputLenguajes::JAVA:
                        ofile << " + " << k->name;
                        break;
                    case OutputLenguajes::PHP:
                        ofile << " . " << k->name;
                        break;
                    default:
                        throw BuildException("Lgenguaje no soportado",__FILE__,__LINE__);            
                }
            }
            if(*penultimoReq != k)
            {
                ofile << " " << opConcat() << " \",\" ";
            }
        }
		ofile << " " << opConcat() << "  \")\";"<< std::endl;
		
        
        //ofile << "\t\tstd::cout << sqlString << std::endl;\n";
        if(configureProject.getInputLenguaje() == InputLenguajes::MySQL)
        {        
            ofile << "\t\toctetos::db::mysql::Datresult dt;" << std::endl;
            //iniciar llave?
            if(table.getKey().size() > 1)
            {//es llave compuesta?
                std::string msg = "No hay soporte aun para llaves compuestas" ;
                throw BuildException(msg,__FILE__,__LINE__);
            }
            else if(table.getKey().size() == 1 and (*table.getKey().begin())->symbolReferenced == NULL and (*(table.getKey().begin()))->outType.compare("int") == 0)
            {//es llave simple y no tiene campos foraneos?
                ofile << "\t\tif(connector.insert(sqlStrings,dt))\n";
                ofile << "\t\t{\n";
                ofile << "\t\t\t" << (*(table.getKey().begin()))->name;
                ofile << " = connector.last_inserted_id();\n";
                ofile << "\t\t\tif(" << (*(table.getKey().begin()))->name << " > 0) return true;\n";
                ofile << "\t\t}\n";
                ofile << "\t\treturn false;\n";
            }
            else if(table.getKey().size() == 1 and (*table.getKey().begin())->symbolReferenced != NULL )
            {//es llave simple y no tiene campos foraneos?
                ofile << "\t\treturn connector.insert(sqlStrings,dt);\n";
            }
            else if(table.getKey().size() < 1 )
            {
                std::string msg = "No hay llave para esta table " ;
                throw BuildException(msg,__FILE__,__LINE__);
            }  
        }
        else if(configureProject.getInputLenguaje() == InputLenguajes::MariaDB)
        {
            ofile << "\t\t";
            switch(configureProject.outputLenguaje)
            {
                case OutputLenguajes::CPP:
                    ofile << "octetos::db::maria::Datresult ";
                    break;
                case OutputLenguajes::JAVA:
                    ofile << "ResultSet ";
                    break;
                case OutputLenguajes::PHP:
                    ofile << "$";
                    break;
                default:
                        throw BuildException("Lgenguaje no soportado",__FILE__,__LINE__);            
            }
            ofile << "dt";
             switch(configureProject.outputLenguaje)
            {
                case OutputLenguajes::CPP:
                    
                    break;
                case OutputLenguajes::JAVA:
                    ofile << " = null";
                    break;
                case OutputLenguajes::PHP:
                    ofile << " = null";
                    break;
                default:
                        throw BuildException("Lgenguaje no soportado",__FILE__,__LINE__);            
            }
            ofile << ";\n";
            //iniciar llave?
            if(table.getKey().size() > 1)
            {//es llave compuesta?
                std::string msg = "No hay soporte aun para llaves compuestas" ;
                throw BuildException(msg,__FILE__,__LINE__);
            }
            else if(table.getKey().size() == 1 and (*table.getKey().begin())->symbolReferenced == NULL and (*(table.getKey().begin()))->outType.compare(integerType()) == 0)
            {//es llave simple y no tiene campos foraneos?
                switch(configureProject.outputLenguaje)
                {
                    case OutputLenguajes::CPP:
                        ofile << "\t\tif(connector.insert(sqlString,dt))\n";
                        break;
                    case OutputLenguajes::JAVA:
                        ofile << "\t\tif(connector.insert(sqlString,dt))\n";
                        break;
                    case OutputLenguajes::PHP:
                        ofile << "\t\tif($connector->insert($sqlString,$dt))\n";
                        break;
                    default:
                            throw BuildException("Lgenguaje no soportado",__FILE__,__LINE__);            
                }
                ofile << "\t\t{\n";
                ofile << "\t\t\t";
                if(configureProject.outputLenguaje == OutputLenguajes::PHP) ofile << "$this->";
                ofile << (*(table.getKey().begin()))->name;                
                switch(configureProject.outputLenguaje)
                {
                    case OutputLenguajes::CPP:
                        ofile << " = connector.last_inserted_id();\n";
                        break;
                    case OutputLenguajes::JAVA:
                        ofile << " = connector.last_inserted_id();\n";
                        break;
                    case OutputLenguajes::PHP:
                        ofile << " = $connector->last_inserted_id();\n";
                        break;
                    default:
                            throw BuildException("Lgenguaje no soportado",__FILE__,__LINE__);            
                }
                ofile << "\t\t\tif(";
                if(configureProject.outputLenguaje == OutputLenguajes::PHP) ofile << "$this->";
                ofile << (*(table.getKey().begin()))->name << " > 0) return true;\n";
                ofile << "\t\t}\n";
                ofile << "\t\treturn false;\n";
            }
            else if(table.getKey().size() == 1 and (*table.getKey().begin())->symbolReferenced != NULL )
            {//es llave simple y no tiene campos foraneos?
                
                switch(configureProject.outputLenguaje)
                {
                    case OutputLenguajes::CPP:
                        ofile << "\t\treturn connector.insert(sqlString,dt);\n";
                        break;
                    case OutputLenguajes::JAVA:
                        ofile << "\t\treturn connector.insert(sqlString,dt);\n";
                        break;
                    case OutputLenguajes::PHP:
                        ofile << "\t\treturn $connector->insert($sqlString,$dt);\n";
                        break;
                    default:
                            throw BuildException("Lgenguaje no soportado",__FILE__,__LINE__);            
                }
            }
            else if(table.getKey().size() < 1 )
            {
                std::string msg = "No hay llave para esta table " ;
                throw BuildException(msg,__FILE__,__LINE__);
            }  
        }
        else if(configureProject.getInputLenguaje() == InputLenguajes::PostgreSQL)
        {
            ofile << "\t\toctetos::db::postgresql::Datresult dt;" << std::endl;
            throw BuildException("No se ha emplemetnedo el insert en postgresql",__FILE__,__LINE__);
        }
        else
        {
            std::string msg = "Lenguaje no soportado " ;
            throw BuildException(msg,__FILE__,__LINE__);
        }   
        
        ofile << "\t}"<<std::endl;
        
        return true;
    }    
    
    bool Insert::generate()
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
