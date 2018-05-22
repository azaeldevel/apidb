/**
 * 
 *  This file is part of apidb.
 * 
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * 
 * */

#include "apidb.hpp"
#include "toolkit.hpp"

#include <iostream>

namespace apidb
{
	
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
			throw BuildException("La tabla no tiene llave que es necesaria para el constructor de la clase");
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
			else
			{
				throw BuildException("EL tipo de dato correspondiente a la llave es inmanejable para este esquema este esquema");
			}
		}
		else
		{
			throw BuildException("La tabla no tiene llave que es necesaria para el constructor de la clase");
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
			else
			{
				throw BuildException("EL tipo de dato correspondiente a la llave es inmanejable para este esquema este esquema");
			}
		}
		else
		{
			throw BuildException("La tabla no tiene llave que es necesaria para el constructor de la clase");
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
    void CPPGenerator::createClassMethodesCPP(apidb::Driver& driver,const apidb::internal::Table& table,std::ofstream& ofile)
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
    
    void CPPGenerator::createSpaceCPP(apidb::Driver& driver,std::ofstream& file)
    {
        file <<"namespace "<<driver.getNameProject()<<std::endl;
        file <<"{"<<std::endl;
        const internal::Tables& tables = driver.getListTable();
        for (apidb::internal::Table* table : tables) 
        {
            createClassCPP(driver,*table,file,table->name);       
        }
        file <<"}"<<std::endl;
    }
	void CPPGenerator::createClassCPP(apidb::Driver& driver,const apidb::internal::Table& cl,std::ofstream& file,const std::string& nameClass)
    {
		file << "\tconst std::string " <<  nameClass << "::TABLE_NAME = \""<<  nameClass << "\";" << std::endl;
        createClassMethodesCPP(driver,cl,file);        
        file<< std::endl<< std::endl;
    }
    
    void CPPGenerator::createClassMethodesH(apidb::Driver& driver,const apidb::internal::Table& table,std::ofstream& ofile)
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
    
    void CPPGenerator::createClassAttributesH(apidb::Driver& driver,const apidb::internal::Table& table,std::ofstream& ofile)
    {
        for(internal::Symbol* attr : table)
        {
			if(driver.getOutputLenguaje() == Driver::OutputLenguajes::CPP)
			{
				if((attr->outType.compare("char") == 0) | (attr->outType.compare("short") == 0) | (attr->outType.compare("int") == 0) | (attr->outType.compare("long") == 0) | (attr->outType.compare("float") == 0) | (attr->outType.compare("double") == 0))
				{
					if(attr->classReferenced == NULL)//si es foreing key
					{
						ofile <<"\t\t"<< attr->outType << " "<< attr->name<<";"<< std::endl;						
					}
					else
					{
						ofile <<"\t\t"<< attr->classReferenced->name << "* "<< attr->name<<";"<< std::endl;
					}						
				}
				else
				{
					ofile <<"\t\t" << attr->outType <<" "<< attr->name <<";"<< std::endl;
				}
			}
			else
			{
				driver.message("OutputLenguaje is unknow.");
			}             
        }        
    }
    void CPPGenerator::createSpaceH(apidb::Driver& driver,std::ofstream& file)
    {
        file <<"namespace "<<driver.getNameProject()<<std::endl;
        file <<"{"<<std::endl;
        const internal::Tables& tables = driver.getListTable();
        for (const apidb::internal::Table* table : tables) 
        {
			file <<"\tclass " <<table->name << ";"<<std::endl;
		}
		file<<std::endl;
        for (const apidb::internal::Table* table : tables) 
        {
            createClassH(driver,*table,file,table->name);       
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
    void CPPGenerator::createClassH(apidb::Driver& driver,const apidb::internal::Table& cl,std::ofstream& file,const std::string& nameClass)
    {
        file <<"\tclass "<<nameClass<<std::endl;
        file <<"\t{"<<std::endl;
        createClassPrivateH(file);
        file << "\t\tstatic const std::string TABLE_NAME;" <<std::endl;
        createClassAttributesH(driver,cl,file);
        createClassPublicH(file);
        createClassMethodesH(driver,cl,file);
        file <<"\t};"<<std::endl;
    }
    
    bool CPPGenerator::generate(apidb::Driver& driver)
    {		
		driver.getOutputMessage() << "Generando codigo... " << std::endl;
		driver.getOutputMessage() << "\tLenguaje resultado: " << driver.getOutputLenguajeString() << std::endl;
		//includes in header file
        std::string headers = "";
        bool stringFlag = false;
        const apidb::internal::Tables& tables = driver.getListTable();
		for(internal::Table* table: tables)
		{
			driver.getOutputMessage() << "\tCoding " << table->name << "." << std::endl;
			for(internal::Symbol* attr : *table)
			{
				if(attr->outType.compare("std::string")==0 && stringFlag == false)
				{
					driver.getHeaderOutput()<< "#include <string>" <<std::endl;
					stringFlag = true;
				}					
			}
		}
			
			
		//inlcudes in source file
        driver.getSourceOutput()<< "#include \"" <<driver.getHeaderName() <<"\""<<std::endl<<std::endl; 
		driver.getHeaderOutput()<< "#include <clientdb.hpp>"<<std::endl;
			
		//writing code
		if(!driver.getNameProject().empty())
		{			
			createSpaceH(driver,driver.getHeaderOutput());    
			createSpaceCPP(driver,driver.getSourceOutput()); 
		}   
		else
		{
			for (apidb::internal::Table* table : tables) 
			{
				createClassH(driver,*table,driver.getHeaderOutput(),table->name);  
				createClassCPP(driver,*table,driver.getSourceOutput(),table->name);      
			}
		}            
        return true;    
    }
    

	
	
	
	
	
	
	
	
	
	
	bool CG::generate()
	{
		apidb::CPPGenerator cpp;
		cpp.generate(*this);
		return false;
	}
	
	bool CG::analyze()
	{
		getOutputMessage() << "Analisis de codigo..." << std::endl;
		getOutputMessage() << "\tLenguaje de entrada: " << getInputLenguajeString() << std::endl;
		//rows = new apidb::internal::Tables();
		
		if(symbolsTables.listing(*connector)) //reading tables
        {
            for(internal::Table* table: symbolsTables) //reading attrubtes by table
            {
				getOutputMessage() << "\tCreating simbols for " << table->name  << "." << std::endl;
                if(!table->basicSymbols(*connector))
                {
					//std::cerr<<"Faill on basicSymbols"<<std::endl;
					return false;
				}
				if(!table->fillKeyType(*connector,symbolsTables))
                {
					//std::cerr<<"Faill on fillKeyType"<<std::endl;
					return false;
				}
				
				//parsing imput types
				for(internal::Symbol* attribute: *table)
				{
					attribute->outType = parse(attribute->inType);
				}
            }            
        }  
        		        
		return true;
	}
		
	CG::CG(const std::string& name,const std::string& directory,const toolkit::clientdb::Datconection& datconection,InputLenguajes inputLenguaje, OutputLenguajes outputLenguaje):Driver(inputLenguaje,outputLenguaje)
	{		
		connector = new toolkit::clientdb::Connector();
		try
		{
			bool flag = connector->connect(datconection);
			if(flag)
			{
				setPramsProject(name,directory);				
			}
		}
		catch(toolkit::clientdb::SQLException ex)
		{
			getErrorMessage() <<"Fallo la conexion el servidor de datos el cual respondio; "<<std::endl;
		}
	}
} 
