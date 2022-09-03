
#include "analyzer.hpp"
#include "generator.hpp"

namespace octetos::apidb::generators
{
    void Insert::insertCall(std::ofstream& ofile)
    {
        if(mode == Mode::CreateParent)
        {
            //para cada campo foraneo
            for(symbols::Symbol* k : table.getRequired())
            {
                if(k->isPrimaryKey() && k->isAutoIncrement()) continue;
                
                if(k->symbolReferenced != NULL)
                {
                    ofile << "\t\t";
                    if(configureProject.outputLenguaje == OutputLenguajes::PHP) ofile << "$";
                    ofile << "this";
                    if(configureProject.outputLenguaje == OutputLenguajes::PHP or configureProject.outputLenguaje == OutputLenguajes::CPP) ofile << "->";
                    if(configureProject.outputLenguaje == OutputLenguajes::JAVA) ofile << ".";
                    ofile << k->name << " = new " << k->classReferenced->name << "();\n";
                    ofile << "\t\tif(";
                    if(configureProject.outputLenguaje == OutputLenguajes::PHP) ofile << "$";
                    ofile << "this";
                    if(configureProject.outputLenguaje == OutputLenguajes::PHP or configureProject.outputLenguaje == OutputLenguajes::CPP) ofile << "->";
                    if(configureProject.outputLenguaje == OutputLenguajes::JAVA) ofile << ".";
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
                    ofile << "insert";
                    if(configureProject.outputLenguaje == OutputLenguajes::PHP)
                    {
                        if(mode == Mode::CreateParent)
                        {
                            ofile << "Raw";
                        }
                        else if(mode == Mode::ReferencedParent)
                        {
                            ofile << "Object";
                        }
                    }
                    ofile << "(";
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
                            throw BuildException("Lenguaje no soportado",__FILE__,__LINE__);            
                    }
                    if(mode == Mode::CreateParent)
                    {
                        for(symbols::Symbol* l : k->symbolReferenced->classParent->getRequired())
                        {
                            if(l->symbolReferenced != NULL)
                            {
                                for(symbols::Symbol* m : l->symbolReferenced->classParent->getRequired())
                                {                            
                                    const symbols::Symbol* rootS = getRootSymbol(m);
                                    if(rootS->isPrimaryKey() and rootS->isAutoIncrement()) continue;
                                    
                                    ofile << ",";
                                    if(m->symbolReferenced!= NULL)
                                    {
                                        insertParamsRaw(ofile,m,l);
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
                                const symbols::Symbol* rootS = getRootSymbol(l);
                                if(rootS->isPrimaryKey() and rootS->isAutoIncrement()) continue;
                                
                                ofile << ",";
                                if(configureProject.outputLenguaje == OutputLenguajes::PHP) ofile << "$" ;
                                ofile << k->name << l->upperName;
                            }
                            else
                            {
                                const symbols::Symbol* rootS = getRootSymbol(l);
                                if(rootS->isPrimaryKey() and rootS->isAutoIncrement()) continue;
                                
                                ofile << ",";                        
                                if(configureProject.outputLenguaje == OutputLenguajes::PHP) ofile << "$" ;
                                ofile << k->name << l->upperName;
                            }
                        }
                    }
                    else if(mode == Mode::ReferencedParent)
                    {
                        for(symbols::Symbol* l : k->symbolReferenced->classParent->getRequired())
                        {
                            if(l->symbolReferenced != NULL)
                            {
                                
                                ofile << ","<< k->name;
                            }
                            else if(l->outType.compare(stringType()) == 0)
                            {
                                const symbols::Symbol* rootS = getRootSymbol(l);
                                if(rootS->isPrimaryKey() and rootS->isAutoIncrement()) continue;
                                
                                ofile << ",";
                                if(configureProject.outputLenguaje == OutputLenguajes::PHP) ofile << "$" ;
                                ofile << k->name << l->upperName;
                            }
                            else
                            {
                                const symbols::Symbol* rootS = getRootSymbol(l);
                                if(rootS->isPrimaryKey() and rootS->isAutoIncrement()) continue;
                                
                                ofile << ",";                        
                                if(configureProject.outputLenguaje == OutputLenguajes::PHP) ofile << "$" ;
                                ofile << k->name << l->upperName;
                            }
                        }
                    }
                        
                    ofile << ") == false) return false;\n";
                }            
            }
        }        
    }
    
	void Insert::insertParamsRaw(std::ofstream& ofile,const symbols::Symbol* k,const symbols::Symbol* parent)
    {
        if(k->symbolReferenced != NULL)
        {
            if(k->symbolReferenced->symbolReferenced != NULL)
            {
                insertParamsRaw(ofile,k->symbolReferenced,parent);
            }     
            else
            {
                auto penultimo = k->symbolReferenced->classParent->getRequired().begin();
                penultimo--;
                penultimo--;
                for(symbols::Symbol* l : k->symbolReferenced->classParent->getRequired())
                {
                    switch(configureProject.outputLenguaje)
                    {
                        case OutputLenguajes::CPP:
                            ofile << l->outType << " " << parent->name << l->upperName;
                            break;
                        case OutputLenguajes::JAVA:
                            ofile << l->outType << " " << parent->name << l->upperName;
                            break;
                        case OutputLenguajes::PHP:
                            ofile << parent->name << l->upperName;
                            break;
                        default:
                        throw BuildException("Lenguaje no soportado",__FILE__,__LINE__);            
                    }
                    if(*penultimo != l)
                    {
                        ofile << ",";
                    }
                }
            }
        }
    }  
	void Insert::insertValueRaw(std::ofstream& ofile,const symbols::Symbol* k,const symbols::Symbol* parent)
    {
        if(k->symbolReferenced != NULL)
        {
            if(k->symbolReferenced->symbolReferenced != NULL)
            {
                insertValueRaw(ofile,k->symbolReferenced,parent);
            }     
            else
            {
                auto penultimo = k->symbolReferenced->classParent->getRequired().begin();
                penultimo--;
                penultimo--;
                for(symbols::Symbol* l : k->symbolReferenced->classParent->getRequired())
                {
                    ofile << parent->name << l->upperName;
                    if(*penultimo != l)
                    {
                        ofile << ",";
                    }
                }
            }
        }
    }
    Insert::Insert(const ConfigureProject& c,const apidb::symbols::Table& t,std::ofstream& o,Mode m) : Operation(c,t,o),mode(m),countFuns(0)
    {
    }
    
    bool Insert::definite()
    {
        short countRef = 0;
        /*if(mode == Mode::ReferencedParent)
        {
            for(symbols::Symbol* k : table.getRequired())
            {
                if(k->symbolReferenced != NULL) countRef++;
            }
            if(countRef == 0) return false;
        }*/        
        
        //inser Raw data
        ofile << "\t\t" << "bool insert(";
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
            throw BuildException("Lenguaje no soportado",__FILE__,__LINE__);            
        }
        if(mode == Mode::CreateParent)
        {
            for(const symbols::Symbol* k : table.getRequired())
            {
                if(k->symbolReferenced != NULL)
                {
                    for(symbols::Symbol* l : k->classReferenced->getRequired())
                    {
                        const symbols::Symbol* rootS = getRootSymbol(l);
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
        }
        else if(mode == Mode::ReferencedParent)
        {
            for(symbols::Symbol* k : table.getRequired())
            {
                //const symbols::Symbol* rootS = getRootSymbol(k);
                if(k->isPrimaryKey() and k->isAutoIncrement()) continue;
                        
                ofile << ",";
                if(k->symbolReferenced!= NULL)
                {
                    ofile << "const " << k->symbolReferenced->classParent->name << "& " << k->name;
                }
                else if(k->outType.compare(stringType()) == 0)
                {
                    if(configureProject.outputLenguaje == OutputLenguajes::CPP) ofile << "const ";
                    ofile << stringType();
                    if(configureProject.outputLenguaje == OutputLenguajes::CPP) ofile << "& " ;
                    ofile << k->name;
                }
                else if(k->outType.compare(integerType()) == 0)
                {
                    
                    if(configureProject.outputLenguaje == OutputLenguajes::CPP or configureProject.outputLenguaje == OutputLenguajes::JAVA) ofile << integerType();
                    ofile << " " << k->name;
                }
                else
                {
                    if(not k->isAutoIncrement() and not k->isPrimaryKey()) ofile << k->getOutType() << " " << k->name;
                }
            }
        }
        ofile << ");"<<std::endl;    
        return true;
    }
    
    
    bool Insert::implement()
    {
        short countRef = 0;
        /*if(mode == Mode::ReferencedParent)
        {
            for(const symbols::Symbol* k : table.getRequired())
            {
                if(k->symbolReferenced != NULL) countRef++;
            }
            if(countRef == 0) return false;
        }  */
        
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
                   throw BuildException("Lenguaje no soportado",__FILE__,__LINE__);            
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
                    ofile << "public function insert";
                    if(mode == Mode::CreateParent)
                    {
                        ofile << "Raw";
                    }
                    else if(mode == Mode::ReferencedParent)
                    {
                        ofile << "Object";
                    }
                    ofile <<  "($connector";
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
            throw BuildException("Lenguaje no soportado",__FILE__,__LINE__);            
        }
        
        if(mode == Mode::CreateParent)
        {
            for(const symbols::Symbol* k : table.getRequired())
            {
                if(k->symbolReferenced!= NULL)
                {
                    for(const symbols::Symbol* l : k->classReferenced->getRequired())
                    {
                        //const symbols::Symbol* rootS = getRootSymbol(l);
                        if(l->isPrimaryKey() and l->isAutoIncrement()) continue;
                        
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
                                    throw BuildException("Lenguaje no soportado",__FILE__,__LINE__);            
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
                        if(configureProject.outputLenguaje == OutputLenguajes::CPP or configureProject.outputLenguaje == OutputLenguajes::JAVA) ofile << "," << k->getOutType() << " " << k->name;
                        if(configureProject.outputLenguaje == OutputLenguajes::PHP) ofile << ",$" << k->name;
                    }
                }
            }
        }
        else if(mode == Mode::ReferencedParent)
        {
            for(const symbols::Symbol* k : table.getRequired())
            {
                //const symbols::Symbol* rootS = getRootSymbol(k);
                if(k->isPrimaryKey() and k->isAutoIncrement()) continue;
                
                ofile << ",";
                if(k->symbolReferenced != NULL)
                {
                    if(configureProject.outputLenguaje == OutputLenguajes::CPP) ofile << "const ";
                    if(configureProject.outputLenguaje != OutputLenguajes::PHP) ofile << k->symbolReferenced->classParent->name;
                    if(configureProject.outputLenguaje == OutputLenguajes::CPP) ofile << "& ";
                    ofile << " ";
                    if(configureProject.outputLenguaje == OutputLenguajes::PHP) ofile << "$";
                    ofile << k->name;
                }
                else if(k->outType.compare(stringType()) == 0)
                {
                    
                    if(configureProject.outputLenguaje == OutputLenguajes::CPP) ofile << "const " << stringType() << "& ";                        
                    if(configureProject.outputLenguaje == OutputLenguajes::JAVA) ofile << stringType() << " ";
                    ofile << " ";
                    if(configureProject.outputLenguaje == OutputLenguajes::PHP) ofile << "$";
                    ofile << k->name;
                }
                else if(k->outType.compare(integerType()) == 0)
                {
                    
                    if(configureProject.outputLenguaje == OutputLenguajes::CPP or configureProject.outputLenguaje == OutputLenguajes::JAVA) ofile << integerType();
                    ofile << " ";
                    if(configureProject.outputLenguaje == OutputLenguajes::PHP) ofile << "$";
                    ofile << k->name;
                }
                else
                {
                    if(not k->isAutoIncrement() and not k->isPrimaryKey()) 
                    {
                        if(configureProject.outputLenguaje == OutputLenguajes::CPP or configureProject.outputLenguaje == OutputLenguajes::JAVA) ofile << k->getOutType() << " " << k->name;
                        if(configureProject.outputLenguaje == OutputLenguajes::PHP) ofile << ",$" << k->name;
                    }
                }
            }
        }
        ofile << ")";
        if(configureProject.outputLenguaje == OutputLenguajes::JAVA) ofile << " throws SQLException";
        ofile << "\n\t{\n";
        
        if(mode == Mode::CreateParent)
        {
            insertCall(ofile);
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
                throw BuildException("Lenguaje no soportado",__FILE__,__LINE__);            
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
        if(mode == Mode::CreateParent)
        {
            for(const symbols::Symbol* k : table.getRequired())
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
                                throw BuildException("Lenguaje no soportado",__FILE__,__LINE__);            
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
        }
        else if(mode == Mode::ReferencedParent)
        {
            for(const symbols::Symbol* k : table.getRequired())
            {
                if(k->isPrimaryKey() and k->isAutoIncrement()) continue;
                
                if(k->symbolReferenced != NULL)
                {    
                    switch(configureProject.outputLenguaje)
                    {
                        case OutputLenguajes::CPP:
                            ofile << " + std::to_string(";
                            break;
                        case OutputLenguajes::JAVA:
                            ofile << " + ";
                            break;
                        case OutputLenguajes::PHP:
                            ofile << " " << opConcat() << " ";
                            break;
                        default:
                            throw BuildException("Lenguaje no soportado",__FILE__,__LINE__);            
                    }
                        
                    if(configureProject.outputLenguaje == OutputLenguajes::PHP) ofile << "$";
                    ofile << k->name;                    
                    inheritField(ofile,k->symbolReferenced,opReference());
                    if(configureProject.outputLenguaje == OutputLenguajes::CPP) ofile << ")";                                       
                    
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
                            ofile << " . $" << k->name;
                            break;
                        default:
                            throw BuildException("Lenguaje no soportado",__FILE__,__LINE__);            
                    }
                }
                if(*penultimoReq != k)
                {
                    ofile << " " << opConcat() << " \",\" ";
                }
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
                        throw BuildException("Lenguaje no soportado",__FILE__,__LINE__);            
            }
            ofile << ";\n";
            //std::cout << "Table : " << table.getName() << " : " << table.getKey().size() << "\n";
            //iniciar llave?
            if(table.getKey().size() > 0)
            {//es llave compuesta?
                switch(configureProject.outputLenguaje)
                {
                    case OutputLenguajes::CPP:
                        ofile << "\t\tif(connector.insert(sqlString,dt))\n";
                        ofile << "\t\t{\n";
                        if(mode == Mode::CreateParent)
                        {
                            echoCopyParamsRaw();
                        }
                        else if(mode == Mode::ReferencedParent)
                        {
                            echoCopyParams();
                        }                        
                        ofile << "\t\t\treturn true;\n";
                        ofile << "\t\t}\n";
                        break;
                    case OutputLenguajes::JAVA:
                        ofile << "\t\tif(connector.insert(sqlString,dt))\n";
                        ofile << "\t\t{\n";
                        if(mode == Mode::CreateParent)
                        {
                            echoCopyParamsRaw();
                        }
                        else if(mode == Mode::ReferencedParent)
                        {
                            echoCopyParams();
                        }                        
                        ofile << "\t\t\treturn true;\n";
                        ofile << "\t\t}\n";
                        break;
                    case OutputLenguajes::PHP:                        
                        ofile << "\t\tif($connector->insert($sqlString,$dt))\n";
                        ofile << "\t\t{\n";
                        if(mode == Mode::CreateParent)
                        {
                            echoCopyParamsRaw();
                        }
                        else if(mode == Mode::ReferencedParent)
                        {
                            echoCopyParams();
                        }                        
                        ofile << "\t\t\treturn true;\n";
                        ofile << "\t\t}\n";
                        break;
                    default:
                            throw BuildException("Lgenguaje no soportado",__FILE__,__LINE__);            
                }                
                ofile << "\t\treturn false;\n";
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
                            throw BuildException("Lenguaje no soportado",__FILE__,__LINE__);            
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
                        ofile << "\t\tif(connector.insert(sqlString,dt))\n";
                        ofile << "\t\t{\n";
                        if(mode == Mode::CreateParent)
                        {
                            
                        }
                        else if(mode == Mode::ReferencedParent)
                        {
                            echoCopyParams();
                        }                        
                        ofile << "\t\t\treturn true;\n";
                        ofile << "\t\t}\n";
                        ofile << "\t\treturn false;\n";
                        break;
                    case OutputLenguajes::JAVA:
                        ofile << "\t\tif(connector.insert(sqlString,dt))\n";
                        ofile << "\t\t{\n";
                        if(mode == Mode::CreateParent)
                        {
                            
                        }
                        else if(mode == Mode::ReferencedParent)
                        {
                            echoCopyParamsJava();
                        }                        
                        ofile << "\t\t\treturn true;\n";
                        ofile << "\t\t}\n";
                        ofile << "\t\treturn false;\n";
                        break;
                        break;
                    case OutputLenguajes::PHP:;
                        ofile << "\t\tif($connector->insert($sqlString,$dt))\n";
                        ofile << "\t\t{\n";
                        if(mode == Mode::CreateParent)
                        {
                            
                        }
                        else if(mode == Mode::ReferencedParent)
                        {
                            echoCopyParams();
                        }                        
                        ofile << "\t\t\treturn true;\n";
                        ofile << "\t\t}\n";
                        ofile << "\t\treturn false;\n";
                        break;
                    default:
                            throw BuildException("Lgenguaje no soportado",__FILE__,__LINE__);            
                }
            }
            else if(table.getKey().size() < 1 )
            {
                switch(configureProject.outputLenguaje)
                {
                    case OutputLenguajes::CPP:
                        ofile << "\t\tif(connector.insert(sqlString,dt)) ";
                        break;
                    case OutputLenguajes::JAVA:
                        ofile << "\t\tif(connector.insert(sqlString,dt)) ";
                        break;
                    case OutputLenguajes::PHP:
                        ofile << "\t\tif($connector->insert($sqlString,$dt)) ";
                        break;
                    default:
                            throw BuildException("Lgenguaje no soportado",__FILE__,__LINE__);            
                }
                
                ofile << "return true;\n";
                ofile << "\t\treturn false;\n";
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
