#include <list>
#include <vector>
#include <iostream>
#include <fstream>

#include "analyzer.hpp"
#include "generator.hpp"


namespace apidb
{

	CPPGenerator::~CPPGenerator()
	{
		delete[] writeResults;
	}
	const std::string& CPPGenerator::getHeaderName() const
	{
		return projectH;
	}
	std::ofstream& CPPGenerator::getSourceOutput()
	{
		return writeResults[1];
	}
	std::ofstream& CPPGenerator::getHeaderOutput()
	{
		return writeResults[0];
	}
	
	CPPGenerator::CPPGenerator(apidb::Analyzer& d):analyzer(&d)
	{
		writeResults = new std::ofstream[2];
		if((d.getDirectoryProject().empty()) | (d.getDirectoryProject().compare(".") == 0)) 
		{
			projectH = d.getNameProject() + ".hpp";
			writeResults[0].open(projectH);
			projectCPP = d.getNameProject() + ".cpp";
			writeResults[1].open(projectCPP);
		}
		else
		{
			projectH = d.getNameProject() + ".hpp";
			projectCPP = d.getNameProject() + ".cpp";
			writeResults[0].open(d.getDirectoryProject() + "/" + projectH);
			writeResults[1].open(d.getDirectoryProject() + "/" + projectCPP);
		}
	}
	void CPPGenerator::writeInsertH(const apidb::internal::Table& table,std::ofstream& ofile)
	{
		// creando insert
        ofile << "\t\t"<< "bool ";
        ofile << "insert(toolkit::clientdb::Connector& connector";
        for(std::list<internal::Symbol*>::const_iterator i = table.required.begin(); i != table.required.end(); i++)
        {
			if((*i)->keyType != internal::Symbol::KeyType::PRIMARY)//la primary key es auto incremento no se agrega
			{
				if(i != table.required.end())
				{
					ofile << ","; //se agrega la coma si hay un segundo parametro
				}
				
				//
				if(((*i)->outType.compare("char") == 0) | ((*i)->outType.compare("short") == 0) | ((*i)->outType.compare("int") == 0) | ((*i)->outType.compare("long") == 0) | ((*i)->outType.compare("float") == 0) | ((*i)->outType.compare("double") == 0))
				{
					if((*i)->classReferenced == NULL)//si es foreing key
					{
						ofile << (*i)->outType << " ";						
					}
					else
					{
						ofile << "const " << (*i)->classReferenced->name << "& ";
					}
				}
				else
				{
					ofile << "const " << (*i)->outType <<"& ";
				}
				ofile << (*i)->name;
			}
		}
        ofile << ");"<<std::endl;
	}
	void CPPGenerator::writeInsertCPP(const apidb::internal::Table& table,std::ofstream& ofile)	
	{	
		// Methodo insert
        ofile << "\t"<< "bool ";
        ofile <<table.name<< "::insert(toolkit::clientdb::Connector& connector";
        for(std::list<internal::Symbol*>::const_iterator i = table.required.begin(); i != table.required.end(); i++)
        {
			if((*i)->keyType != internal::Symbol::KeyType::PRIMARY)//la primary key es auto incremento no se agrega
			{
				if(i != table.required.end())
				{
					ofile << ","; //se agrega la coma si hay un segundo parametro
				}
				
				//
				if(((*i)->outType.compare("char") == 0) | ((*i)->outType.compare("short") == 0) | ((*i)->outType.compare("int") == 0) | ((*i)->outType.compare("long") == 0) | ((*i)->outType.compare("float") == 0) | ((*i)->outType.compare("double") == 0))
				{
					if((*i)->classReferenced == NULL)//si es foreing key
					{
						ofile << (*i)->outType << " ";						
					}
					else
					{
						ofile << "const " << (*i)->classReferenced->name << "& ";
					}
				}
				else
				{
					ofile << "const " << (*i)->outType <<"& ";
				}
				ofile << (*i)->name;
			}
		}
        ofile << ")"<<std::endl;
        ofile << "\t{"<<std::endl;
        ofile << "\t\t"<<"std::string sqlString = \"\";"<<std::endl;
        ofile << "\t\t"<<"sqlString = sqlString + \"INSERT INTO \" + TABLE_NAME; "<<std::endl;
        ofile << "\t\t"<<"sqlString = sqlString + \"(\";"<<std::endl;
        for(std::list<internal::Symbol*>::const_iterator i = table.required.begin(); i != table.required.end(); i++)
        {
			if((*i)->keyType != internal::Symbol::KeyType::PRIMARY)//la primary key es auto incremento no se agrega
			{
				if(i != table.required.begin())
				{
					ofile << ","; //se agrega la coma si hay un segundo parametro
				}				
				ofile <<"\t\tsqlString = sqlString + \"" << (*i)->name <<"\";"<< std::endl;
			}
		}
		ofile << "\t\tsqlString = sqlString + \") VALUES(\";"<<std::endl;
        for(std::list<internal::Symbol*>::const_iterator i = table.required.begin(); i != table.required.end(); i++)
        {
			if((*i)->keyType != internal::Symbol::KeyType::PRIMARY)//la primary key es auto incremento no se agrega
			{
				if(i != table.required.begin())
				{
					ofile << ","; //se agrega la coma si hay un segundo parametro
				}				
				//ofile <<"\t\tsqlString = sqlString + \"" << (*i)->name;
				if(((*i)->outType.compare("short") == 0) | ((*i)->outType.compare("int") == 0) | ((*i)->outType.compare("long") == 0) | ((*i)->outType.compare("float") == 0) | ((*i)->outType.compare("double") == 0))
				{
					if((*i)->classReferenced == NULL)//si es foreing key
					{
						ofile << "\t\tsqlString = sqlString + \"'\" + std::to_string(" << (*i)->name << ") + \"'\";"<< std::endl;						
					}
					else
					{
						ofile << "\t\tsqlString = sqlString + \"'\" + " << (*i)->name << ".toStringKey() + \"'\";"<< std::endl;
					}
				}
				else
				{
					ofile << "\t\tsqlString = sqlString + \"'\" + " << (*i)->name << " + \"'\";" << std::endl;
				}
			}			
		}
		ofile << "\t\tsqlString = sqlString + \")\";"<< std::endl;
		//asegurar que tiene key
		if(table.key != NULL)
		{
			ofile << "\t\tthis->"  << table.key->name;
			if((table.key->outType.compare("int") == 0))
			{
				if(table.key->classReferenced == NULL)//si es foreing key
				{
					ofile << " = connector.insert(sqlString);"<< std::endl;		
					ofile << "\t\tif(this->" << table.key->name << " > 0) return true;"<< std::endl;
					ofile << "\t\telse return false;"<< std::endl;				
				}
				else
				{
					ofile << " = new " << table.key->classReferenced->name << "((int)connector.insert(sqlString));"<< std::endl;
					ofile << "\t\tif(this->" << table.key->name << " != NULL) return true;"<< std::endl;
					ofile << "\t\telse return false;"<< std::endl;
				}
			}
			else
			{
				ofile << " = " << table.key->classReferenced->name << "(connector.insert(sqlString));"<< std::endl;
				ofile << "\t\tif(this->" << table.key->name << " != NULL) return true;"<< std::endl;
				ofile << "\t\telse return false;"<< std::endl;
			}			
		}
		else //no tiene key
		{
			throw BuildException("La tabla no tiene llave que es necesaria para el constructor de la clase in writeInsertCPP");
		}
        ofile << "\t}"<<std::endl;
	}
        
	void CPPGenerator::writeKeyContructorH(const apidb::internal::Table& table,std::ofstream& ofile)
	{
        //constructor que toma key como parametro
        if(table.key != NULL)//tiene key
        {
			if(table.key->outType.compare("int") == 0)
			{
				ofile << "\t\t" <<table.name;
				if(table.key->classReferenced == NULL)
				{
					ofile << "(int id);"<<std::endl;
				}
				else
				{
					ofile << "(const " << table.key->classReferenced->name<< "& obj);"<<std::endl;
				}
			}
			else if(table.key->outType.compare("std::string") == 0)
			{
				ofile << "\t\t" <<table.name;
				ofile << "(std::string id);"<<std::endl;
			}
			else
			{
				throw BuildException("EL tipo de dato correspondiente a la llave es inmanejable para este esquema este esquema'" + table.key->outType + "'");
			}
		}
		else
		{
			std::string msg = "La tabla no tiene llave que es necesaria para el constructor de la clase in writeKeyContructorH ";
			msg = msg + table.name;
			throw BuildException(msg);
		}
	}
	void CPPGenerator::writeKeyContructorCPP(const apidb::internal::Table& table ,std::ofstream& ofile)
	{
		//constructor que toma key como parametro
        if(table.key != NULL)//tiene key
        {
			if(table.key->outType.compare("int") == 0)
			{
				ofile << "\t" <<table.name << "::" << table.name;
				if(table.key->classReferenced == NULL)
				{
					ofile << "(int id)"<<std::endl;
				}
				else
				{
					ofile << "(const " << table.key->classReferenced->name << "& obj)"<<std::endl;
				}
			}
			else if(table.key->outType.compare("std::string") == 0)
			{
				ofile << "(std::string id)"<<std::endl;
			}
			else
			{
				throw BuildException("EL tipo de dato correspondiente a la llave es inmanejable para este esquema este esquema");
			}
		}
		else
		{
			throw BuildException("La tabla no tiene llave que es necesaria para el constructor de la clase in writeKeyContructorCPP" );
		}
		ofile << "\t{" <<std::endl;
		if(table.key->outType.compare("int") == 0)
		{
			if(table.key->classReferenced == NULL)
			{
					ofile << "\t\tthis->" << table.key->name << "=id;" <<std::endl;
			}
			else
			{
					ofile << "\t\tthis->" << table.key->name<< " = new " << table.key->classReferenced->name << "(obj);"<<std::endl;
			}
		}
		else
		{
				throw BuildException("EL tipo de dato correspondiente a la llave es inmanejable para este esquema este esquema");
		}
		ofile << "\t}" <<std::endl;
	}
        
	void CPPGenerator::writeCopyContructorH(const apidb::internal::Table& table,std::ofstream& ofile)
	{
		//constructor de copias 
		ofile << "\t\t" <<table.name<<"(const " << table.name <<"&);"<<std::endl;
	}
	void CPPGenerator::writeCopyContructorCPP(const apidb::internal::Table& table,std::ofstream& ofile)
	{
		//constructor de copias 
		ofile << "\t" << table.name << "::" << table.name <<"(const " << table.name <<"& obj)"<<std::endl;
		ofile << "\t{"<<std::endl;
		for(const internal::Symbol* attr : table)
		{
			ofile << "\t\tthis->"<< attr->name << " = obj." << attr->name<<";"<<std::endl;
		}
		ofile << "\t}"<<std::endl;
				
	}
	void CPPGenerator::writeKeyValueH(const apidb::internal::Table& table ,std::ofstream& ofile)
	{
        //si la table tiene key
        if(table.key != NULL) 
        {
			ofile << "\t\tstd::string toStringKey()const;" <<std::endl;			
		}
	}
	void CPPGenerator::writeKeyValueCPP(const apidb::internal::Table& table,std::ofstream& ofile)
	{
        if(table.key != NULL)
        {
			ofile << "\tstd::string " <<table.name << "::toStringKey()const" <<std::endl;	
			ofile << "\t{" << std::endl;
			if((table.key->outType.compare("int") == 0))
			{
				if(table.key->classReferenced == NULL)//si es foreing key
				{
					ofile <<"\t\treturn std::to_string(" << table.key->name;
					ofile <<");"<<std::endl;						
				}
				else
				{
					ofile <<"\t\treturn std::to_string(" << table.key->name;
					internal::Symbol* actual = table.key;
					do
					{
						ofile << "->" << actual->classReferenced->key->get;
						actual = actual->classReferenced->key;						
					}
					while((actual->outType.compare("int") == 0) && (actual->classReferenced != NULL));
					ofile <<");"<<std::endl;
				}
			}
			else if((table.key->outType.compare("std::string") == 0))
			{
				ofile <<"\t\treturn " << table.key->name << ";" <<std::endl;
			}
			else
			{
				ofile <<"\t\treturn std::to_string(" << table.key->name <<");"<<std::endl;
			}
			ofile << "\t}" << std::endl;
		}
	}
        
	void CPPGenerator::writeDefaultContructorH(const apidb::internal::Table& table,std::ofstream& ofile)
	{
		ofile <<"\t\t"<<table.name<<"();"<<std::endl;
	}
	void CPPGenerator::writeDefaultContructorCPP(const apidb::internal::Table& table,std::ofstream& ofile)
    {
		ofile <<"\t"<<table.name<< "::" <<table.name<<"()"<<std::endl;
		ofile <<"\t{"<<std::endl;
		ofile <<"\t}"<<std::endl;
	}
    void CPPGenerator::createClassMethodesCPP(const apidb::internal::Table& table,std::ofstream& ofile)
    {		
        for(const internal::Symbol* attr : table)
        {
			//gets
			if((attr->outType.compare("char") == 0) | (attr->outType.compare("short") == 0) | (attr->outType.compare("int") == 0) | (attr->outType.compare("long") == 0) | (attr->outType.compare("float") == 0) | (attr->outType.compare("double") == 0))
			{
				if(attr->classReferenced == NULL)//si es foreing key
				{
					ofile <<"\t"<< attr->outType << " ";						
				}
				else
				{
					ofile <<"\t"<< "const " << attr->classReferenced->name << "& ";
				}
			}
			else
			{
				ofile <<"\t" << "const " << attr->outType <<"& ";
			}				
			ofile << table.name <<"::get" << attr->name << "()const"<< std::endl;
			ofile << "\t{"<<std::endl;	
			if((attr->outType.compare("char") == 0) | (attr->outType.compare("short") == 0) | (attr->outType.compare("int") == 0) | (attr->outType.compare("long") == 0) | (attr->outType.compare("float") == 0) | (attr->outType.compare("double") == 0))
			{
				if(attr->classReferenced == NULL)//si es foreing key
				{
					ofile <<"\t\treturn "<< attr->name<<";"<< std::endl;						
				}
				else
				{
					ofile <<"\t\treturn *"<< attr->name <<";"<< std::endl;
				}						
			}
			else
			{
				ofile <<"\t\treturn " << attr->name <<";"<< std::endl;
			}								
			ofile << "\t}"<<std::endl;
			
			
			if(attr->keyType == internal::Symbol::KeyType::PRIMARY || attr->keyType == internal::Symbol::KeyType::FOREIGN_UNIQUE)
			{
				continue;
			}			
			//updates
			ofile << "\tbool " << table.name <<"::update" << attr->name << "(toolkit::clientdb::Connector& connector,";
			if((attr->outType.compare("int") == 0) | (attr->outType.compare("long") == 0))
			{
				if(attr->classReferenced == NULL)//si es foreing key
				{
					ofile << attr->outType << " " << attr->name;						
				}
				else
				{
					ofile <<"const "<< attr->classReferenced->name <<"& " << attr->name;
				}						
			}
			else
			{
				ofile << attr->outType << " " << attr->name;
			}
			ofile <<")"<< std::endl;
			ofile << "\t{"<<std::endl;
			ofile <<"\t\tstd::string sqlString = \"\";"<<std::endl;
			ofile <<"\t\tsqlString = \"UPDATE \" + TABLE_NAME;"<<std::endl;
			ofile <<"\t\tsqlString = sqlString + \" SET " << attr->name << " = \" + " ;
			if((attr->outType.compare("int") == 0) | (attr->outType.compare("long") == 0))
			{
				if(attr->classReferenced == NULL)//si es foreing key
				{
					ofile << "\"'\" + std::to_string(" << attr->name <<") + \"'\";";						
				}
				else
				{
					ofile << "\"'\" + std::to_string(" << attr->name ;	
					const internal::Symbol* actual = attr;
					do
					{
						ofile << "." << actual->classReferenced->key->get;
						actual = actual->classReferenced->key;						
					}
					while((actual->outType.compare("int") == 0) && (actual->classReferenced != NULL));				
					ofile <<") + \"'\";"<<std::endl;						
				}
			}
			else if((attr->outType.compare("std::string") == 0))
			{
				ofile << " \"'\" + " << attr->name <<" + \"'\";"<<std::endl;	
			}
			else
			{
				ofile <<" + std::to_string(" << attr->name <<");"<<std::endl;		
			}
			ofile <<"\t\tsqlString = sqlString + \" WHERE " << table.key->name << "  = \" ";			
			if((table.key->outType.compare("int") == 0) | (table.key->outType.compare("long") == 0))
			{
				if(table.key->classReferenced == NULL)//si es foreing key
				{
					ofile << " + std::to_string(" << table.key->get <<");"<<std::endl; ;	
				}
				else
				{
					ofile << " + std::to_string(" << table.key->name ;	
					const internal::Symbol* actual = table.key;
					do
					{
						ofile << "->" << actual->classReferenced->key->get;
						actual = actual->classReferenced->key;						
					}
					while((actual->outType.compare("int") == 0) && (actual->classReferenced != NULL));	
					ofile <<");"<<std::endl;		
				}
			}
			else
			{
				
			}
			
			ofile <<"\t\treturn connector.query(sqlString);"<<std::endl;
			ofile << "\t}"<<std::endl;						
        }
		
		writeKeyContructorCPP(table,ofile);
		writeCopyContructorCPP(table,ofile);
		writeDefaultContructorCPP(table,ofile);
			
		writeInsertCPP(table,ofile);		
		writeKeyValueCPP(table,ofile);
		ofile << std::endl; 
    }
    
    void CPPGenerator::createSpaceCPP(std::ofstream& file)
    {
        file <<"namespace "<<analyzer->getNameProject()<<std::endl;
        file <<"{"<<std::endl;
        const internal::Tables& tables = analyzer->getListTable();
        for (apidb::internal::Table* table : tables) 
        {
            createClassCPP(*table,file,table->name);       
        }
        file <<"}"<<std::endl;
    }
	void CPPGenerator::createClassCPP(const apidb::internal::Table& cl,std::ofstream& file,const std::string& nameClass)
    {
		file << "\tconst std::string " <<  nameClass << "::TABLE_NAME = \""<<  nameClass << "\";" << std::endl;
        createClassMethodesCPP(cl,file);        
        file<< std::endl<< std::endl;
    }
    
    void CPPGenerator::createClassMethodesH(const apidb::internal::Table& table,std::ofstream& ofile)
    {
		std::string insertMethode = "";
        for(internal::Symbol* attr : table)
        {
			//get
			if((attr->outType.compare("char") == 0) | (attr->outType.compare("short") == 0) | (attr->outType.compare("int") == 0) | (attr->outType.compare("long") == 0) | (attr->outType.compare("float") == 0) | (attr->outType.compare("double") == 0))
			{
				if(attr->classReferenced == NULL)//si es foreing key
				{
					ofile <<"\t\t"<< attr->outType << " ";						
				}
				else
				{
					ofile <<"\t\t"<< "const " << attr->classReferenced->name << "& ";
				}
			}
			else
			{
				ofile <<"\t\t" << "const " << attr->outType <<"& ";
			}				
			ofile << "get" << attr->name << "() const;"<< std::endl;
			
			
			if(attr->keyType == internal::Symbol::KeyType::PRIMARY)
			{
				continue;
			}			
			//update
			ofile << "\t\tbool " << "update" << attr->name << "(toolkit::clientdb::Connector& connector,";
			if((attr->outType.compare("int") == 0) | (attr->outType.compare("long") == 0))
			{
				if(attr->classReferenced == NULL)//si es foreing key
				{
					ofile << attr->outType << " " << attr->name;						
				}
				else
				{
					ofile <<"const "<< attr->classReferenced->name <<"& ";
				}						
			}
			else
			{
				ofile << attr->outType << " " << attr->name;
			}
			ofile << ");"<< std::endl;					
        }  
                 
          
		writeKeyContructorH(table,ofile);		
		writeCopyContructorH(table,ofile);
		writeDefaultContructorH(table,ofile);
		ofile << std::endl; 	
		writeInsertH(table,ofile);	
		writeKeyValueH(table,ofile);
		ofile << std::endl; 		
		  
    }
    
    void CPPGenerator::createClassAttributesH(const apidb::internal::Table& table,std::ofstream& ofile)
    {
        for(internal::Symbol* attr : table)
        {
			//ofile <<"["<<attr->outType<<"]"<<std::endl;
			if(analyzer->getOutputLenguaje() == OutputLenguajes::CPP)
			{
				if((attr->outType.compare("char") == 0) | (attr->outType.compare("short") == 0) | (attr->outType.compare("int") == 0) | (attr->outType.compare("long") == 0) | (attr->outType.compare("float") == 0) | (attr->outType.compare("double") == 0))
				{
					if(attr->classReferenced == NULL)//si es foreing key
					{
						//ofile <<"[1]"<<std::endl;
						ofile <<"\t\t"<< attr->outType << " "<< attr->name<<";"<< std::endl;						
					}
					else
					{
						//ofile <<"[2]"<<std::endl;
						ofile <<"\t\t"<< attr->classReferenced->name << "* "<< attr->name<<";"<< std::endl;
					}
				}
				else
				{
					//ofile <<"[3]"<<std::endl;
					ofile <<"\t\t" << attr->outType <<" "<< attr->name <<";"<< std::endl;
				}
			}
			else
			{
				analyzer->getErrorMessage()<<"OutputLenguaje is unknow.";
			}             
        }        
    }
    void CPPGenerator::createSpaceH(std::ofstream& file)
    {
        file <<"namespace "<<analyzer->getNameProject()<<std::endl;
        file <<"{"<<std::endl;
        //file <<"Listing tables" << std::endl;
        const internal::Tables& tables = analyzer->getListTable();
        for (const apidb::internal::Table* table : tables) 
        {
			//file <<"Backward Table " << table->name << std::endl;
			file <<"\tclass " <<table->name << ";"<<std::endl;
		}
		file<<std::endl;
        for (const apidb::internal::Table* table : tables) 
        {
			//file <<"Declare Table " << table->name << std::endl;
            createClassH(*table,file,table->name);       
        }
        file <<"}"<<std::endl;
    }
    void CPPGenerator::createClassPublicH(std::ofstream& file)
    {
        file << "\tpublic:" <<std::endl;
    }
    void CPPGenerator::createClassPrivateH(std::ofstream& file)
    {
        file << "\tprivate:" <<std::endl;
    }
    void CPPGenerator::createClassH(const apidb::internal::Table& cl,std::ofstream& file,const std::string& nameClass)
    {
		//file <<"keyword"<<std::endl;
		analyzer->getOutputMessage() <<"\tHeading class " << cl.name<<std::endl;
        file <<"\tclass "<<nameClass<<std::endl;        
        file <<"\t{"<<std::endl;
        //file <<"private"<<std::endl;
        createClassPrivateH(file);
        file << "\t\tstatic const std::string TABLE_NAME;" <<std::endl;
        //file <<"atributes"<<std::endl;
        createClassAttributesH(cl,file);
        createClassPublicH(file);
        //file <<"methodes"<<std::endl;
        createClassMethodesH(cl,file);
        file <<"\t};"<<std::endl;
    }
    
    bool CPPGenerator::generate()
    {		
		analyzer->getOutputMessage() << "Generando codigo... " << std::endl;
		analyzer->getOutputMessage() << "\tLenguaje resultado: " << analyzer->getOutputLenguajeString() << std::endl;
		//includes in header file
        std::string headers = "";
        bool stringFlag = false;
        const apidb::internal::Tables& tables = analyzer->getListTable();
		for(internal::Table* table: tables)
		{
			for(internal::Symbol* attr : *table)
			{
				if(attr->outType.compare("std::string")==0 && stringFlag == false)
				{
					getHeaderOutput()<< "#include <string>" <<std::endl;
					stringFlag = true;
				}
			}
		}
			
			
		//inlcudes in source file
        getSourceOutput()<< "#include \"" <<getHeaderName() <<"\""<<std::endl<<std::endl; 
		getHeaderOutput()<< "#include <clientdb.hpp>"<<std::endl;
			
		//writing code
		if(!analyzer->getNameProject().empty())
		{			
			createSpaceH(getHeaderOutput());  
			createSpaceCPP(getSourceOutput()); 
		}   
		else
		{
			for (apidb::internal::Table* table : tables) 
			{
				createClassH(*table,getHeaderOutput(),table->name);  
				createClassCPP(*table,getSourceOutput(),table->name);     
			}
		}            
        return true;    
    }
    

	
	
}
