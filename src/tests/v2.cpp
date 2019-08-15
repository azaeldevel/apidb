
#include <CUnit/Basic.h>
#include <iostream>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <random>
#include <functional> //for std::function
#include <algorithm>  //for std::generate_n
#include <signal.h>
#include <map>


#include "../apidb.hpp"
#include "../Errors.hpp"
#include <toolkit/common/common.hpp>
#include "../common.hpp"

std::string random_string( size_t length )
{
    auto randchar = []() -> char
    {
        const char charset[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";
        const size_t max_index = (sizeof(charset) - 1);
        return charset[ rand() % max_index ];
    };
    std::string str(length,0);
    std::generate_n( str.begin(), length, randchar );
    return str;
}

static std::string filename;
static std::string filename_nlst;
static octetos::toolkit::clientdb::mysql::Datconnect mysqlSource("192.168.0.101",3306,"sysappv2.alpha","develop","123456"); 


/* The suite initialization function.
 * Opens the temporary file used by the tests.
 * Returns zero on success, non-zero otherwise.
 */
int init_apidb(void)
{
        filename = random_string(50);
        filename_nlst = random_string(50);
        return 0;
}

/* The suite cleanup function.
 * Closes the temporary file used by the tests.
 * Returns zero on success, non-zero otherwise.
 */
int clean_apidb(void)
{
        remove(filename.c_str());
        remove(filename_nlst.c_str());
        return 0;
}

void testCreateProject_nlst()
{
	octetos::toolkit::Version version;
	version.setNumbers(0,1,0);
        version.setStage(octetos::toolkit::Version::Stage::alpha);
        
	octetos::apidb::ConfigureProject configProject_nls;
        configProject_nls.name = "sysapp";
        configProject_nls.builDirectory = "apidb";
        configProject_nls.conectordb = &mysqlSource;
        configProject_nls.versionResult = version;
        configProject_nls.inputLenguaje = octetos::apidb::InputLenguajes::MySQL;
        configProject_nls.outputLenguaje = octetos::apidb::OutputLenguajes::CPP;	
        configProject_nls.packing = octetos::apidb::PackingLenguajes::CMake;
        configProject_nls.compiled = octetos::apidb::Compiled::STATIC;
        
        if(configProject_nls.saveConfig(filename_nlst))
        {
                CU_ASSERT(true);
        }
        else
        {
                CU_ASSERT(false);
        }
}

void testConecction()
{
        octetos::apidb::ConfigureProject configProject;
        configProject.conectordb = &mysqlSource;
        configProject.inputLenguaje = octetos::apidb::InputLenguajes::MySQL;
        CU_ASSERT(configProject.testConexion());
}

void testCreateProject()
{
	octetos::toolkit::Version version;
	version.setNumbers(0,1,0);
        version.setStage(octetos::toolkit::Version::Stage::alpha);
	octetos::apidb::ConfigureProject configProject;
        configProject.name = "sysapp";
        configProject.builDirectory  = "apidb";
        configProject.conectordb = &mysqlSource;
        configProject.versionResult = version;
        configProject.inputLenguaje = octetos::apidb::InputLenguajes::MySQL;
        configProject.outputLenguaje = octetos::apidb::OutputLenguajes::CPP;	
        configProject.packing = octetos::apidb::PackingLenguajes::CMake;
        configProject.compiled = octetos::apidb::Compiled::STATIC;
        octetos::apidb::ConfigureProject::Table* tbP = new octetos::apidb::ConfigureProject::Table("Persons");
        octetos::apidb::ConfigureProject::Function* dwFullName = new octetos::apidb::ConfigureProject::Function("fullname");
        dwFullName->addParam("name1");
        dwFullName->addParam("name2");
        dwFullName->addParam("name3");
        dwFullName->addParam("name4");
        tbP->insert(std::make_pair(dwFullName->getName().c_str(), dwFullName));
        octetos::apidb::ConfigureProject::Function* dwShortName = new octetos::apidb::ConfigureProject::Function("shortname");
        dwShortName->addParam("name1");
        dwShortName->addParam("name3");
        tbP->insert(std::make_pair(dwShortName->getName().c_str(), dwShortName));
        configProject.downloads.insert(std::make_pair(tbP->getName().c_str(),tbP));
        configProject.selects.insert(std::make_pair(tbP->getName().c_str(),tbP));
        octetos::apidb::ConfigureProject::Table* tbUsers = new octetos::apidb::ConfigureProject::Table("Users");
        octetos::apidb::ConfigureProject::Function* byUsername = new octetos::apidb::ConfigureProject::Function("byUsername");    
        byUsername->addParam("username");
        byUsername->addParam("person");
        tbUsers->insert(std::make_pair(byUsername->getName().c_str(), byUsername));
        configProject.selects.insert(std::make_pair(tbUsers->getName().c_str(),tbUsers));
        configProject.downloads.insert(std::make_pair(tbUsers->getName().c_str(),tbUsers));
        //configProject.executable_target  = "¿?";
		//configProject.namespace_detect = "emulate";
		
        if(configProject.saveConfig(filename))
        {
                CU_ASSERT(true);
        }
        else
        {
                CU_ASSERT(false);
        }
}

void testBuild_nlst()
{
	octetos::apidb::ConfigureProject configProject_nls;
        if(!configProject_nls.readConfig(filename_nlst))
        {
                if(octetos::toolkit::Error::check())
                {
                        std::cout << "Error  -> "<< octetos::toolkit::Error::get().describe() << std::endl;
                }
                CU_ASSERT(false);
                return;
        }
        octetos::apidb::Driver driver(configProject_nls);
        octetos::apidb::Tracer tracer(0);
        if(!driver.driving((octetos::apidb::Tracer*)NULL))
        {
                if(octetos::toolkit::Error::check())
                {
                        std::cout << "Error  -> "<< octetos::toolkit::Error::get().describe() << std::endl;
                }
                CU_ASSERT(false);
        }
        
        CU_ASSERT(true);
}

void testBuild()
{   
	octetos::apidb::ConfigureProject configProject;
	//octetos::toolkit::Error::write(octetos::toolkit::Error("Teste error",1,__FILE__,__LINE__));
        if(!configProject.readConfig(filename))
        {                
                if(octetos::toolkit::Error::check())
                {
                        std::cout << "Error  -> "<< octetos::toolkit::Error::get().describe() << std::endl;
                }
                CU_ASSERT(false);
                exit(EXIT_FAILURE);// hay pruebas que depende de esta.
        }
	//configProject.executable_target = "developing2";
	configProject.conectordb = &mysqlSource;
	configProject.namespace_detect = "emulate";
        octetos::apidb::Driver driver(configProject);
        octetos::apidb::Tracer tracer(0);
        if(driver.driving(NULL) == false)
        {
			std::cout << "Fail  -> "<< std::endl;
                if(octetos::toolkit::Error::check())
                {
                        std::cout << "Error  -> "<< octetos::toolkit::Error::get().describe() << std::endl;
                }
                CU_ASSERT(false);
                exit(EXIT_FAILURE);// hay pruebas que depende de esta.
        }
        CU_ASSERT(true);
			
	std::list<std::string> listName;
	if(driver.getTablesName(listName) == false)
	{
		if(octetos::toolkit::Error::check())
		{
			std::cout << "\nError  -> "<< octetos::toolkit::Error::get().describe() << std::endl;
		}
		CU_ASSERT(false);
	}
	/*for(std::list<std::string>::iterator it = listName.begin(); it != listName.end(); it++)
	{
		std::cout << "Tabla : " << *it << std::endl;
	}*/
	//std::cout << "Total de tablas : " << listName.size() << std::endl;
	std::list<std::string> listFields;
	if(driver.getFiledsName(listFields,"Persons") == false)
	{
		if(octetos::toolkit::Error::check())
		{
			std::cout << "\nError  -> "<< octetos::toolkit::Error::get().describe() << std::endl;
		}
		CU_ASSERT(false);
	}
	/*for(std::list<std::string>::iterator it = listFields.begin(); it != listFields.end(); it++)
	{
		std::cout << "\nField : " << *it;
	}*/
}

void testCompile()
{
	octetos::apidb::ConfigureProject configProject;
	if(!configProject.readConfig(filename))
	{                
		if(octetos::toolkit::Error::check())
		{
			std::cout << "Error  -> "<< octetos::toolkit::Error::get().describe() << std::endl;
		}
		CU_ASSERT(false);
		exit(EXIT_FAILURE);// hay pruebas que depende de esta.
	}
	else
	{
		CU_ASSERT(true);
	}
	if(configProject.packing == octetos::apidb::PackingLenguajes::CMake)
	{

                int ret = 0;
		octetos::toolkit::Version ver = octetos::apidb::getPakageVersion();
                std::string cmd = "cp ../tests/developing";
				cmd += std::to_string(ver.getMajor());
				cmd += ".cpp ";
                cmd += " apidb/ ";
                if(system(cmd.c_str()) < 0)
                {
                        std::cout << "Fallo al copiar el archivo developing.cpp\n";
                        CU_ASSERT(false);
                }

                cmd  = " cd apidb && cmake . &> /dev/null";
                if(system(cmd.c_str()) < 0)
                {
                        std::cout << "Fallo al realizar la compialcion.\n";
                        CU_ASSERT(false);
                }
                cmd  = " cd apidb &&  make &> /dev/null";
                if(system(cmd.c_str()) < 0)
                {
                        std::cout << "Fallo al realizar la compialcion.\n";
                        CU_ASSERT(false);
                }
                /*cmd  = "./apidb/developing";
                ret = system(cmd.c_str()) ;
                if (WIFSIGNALED(ret) && (WTERMSIG(ret) == SIGINT || WTERMSIG(ret) == SIGQUIT) || ret < 0)
                {
                        std::cout << "Fallo al realizar la compialcion.\n";
                        CU_ASSERT(false);
                }*/

	}
	else
	{
		CU_ASSERT(false);
	}
}

void testTemp()
{
	std::map<const char*,octetos::apidb::symbols::ISpace*,octetos::apidb::symbols::cmp_str> lst;
	octetos::apidb::symbols::Table t1("t1");
	std::pair<const char*, octetos::apidb::symbols::ISpace*> newInser1(t1.getName().c_str(),&t1);
	lst.insert(newInser1);
	octetos::apidb::symbols::Table t2("t2");
	std::pair<const char*, octetos::apidb::symbols::ISpace*> newInser2(t2.getName().c_str(),&t2);
	lst.insert(newInser2);
	octetos::apidb::symbols::Table t3("t3");
	std::pair<const char*, octetos::apidb::symbols::ISpace*> newInser3(t3.getName().c_str(),&t3);
	lst.insert(newInser3);
	octetos::apidb::symbols::Table user("Users");
	std::pair<const char*, octetos::apidb::symbols::ISpace*> newInser4(user.getName().c_str(),&user);
	lst.insert(newInser4);
	lst.insert(newInser4);
	
	std::map<const char*,octetos::apidb::symbols::ISpace*,octetos::apidb::symbols::cmp_str>::iterator it;
	it = lst.find("t45");
	if(it != lst.end()) CU_ASSERT(false);
	std::string str = "Users";
	it = lst.find(str.c_str());
	if(it == lst.end()) CU_ASSERT(false);
	it = lst.find("t1");
	if(it == lst.end()) CU_ASSERT(false);
	CU_ASSERT(true);		
}

int main(int argc, char *argv[])
{
	CU_pSuite pSuite = NULL;
	
	/* initialize the CUnit test registry */
	if (CUE_SUCCESS != CU_initialize_registry()) return CU_get_error();

	octetos::toolkit::Version ver = octetos::apidb::getPakageVersion();
	std::string pkName = octetos::apidb::getPakageName();
	std::string classVersionString = std::string("Probando ") + pkName + " " + ver.toString();
	pSuite = CU_add_suite(classVersionString.c_str(), init_apidb, clean_apidb);
	if (NULL == pSuite) 
	{
		CU_cleanup_registry();
		return CU_get_error();
	}
	
	
	if ((NULL == CU_add_test(pSuite, "Pruebas temporales.", testTemp)))
	{
		CU_cleanup_registry();
		return CU_get_error();
	}
	
	if ((NULL == CU_add_test(pSuite, "Verificando la conectividad del componente.", testConecction)))
	{
		CU_cleanup_registry();
		return CU_get_error();
	}
	////////////////////////////////////////////////////////// SIN LISTAS
	/*if ((NULL == CU_add_test(pSuite, "Creacion de proyeto a partir de descripcion statica para no-list.", testCreateProject_nlst)))
	{
		CU_cleanup_registry();
		return CU_get_error();
	}	
	if ((NULL == CU_add_test(pSuite, "Verificando el proceso de contruccion para no-list.", testBuild_nlst)))
	{
		CU_cleanup_registry();
		return CU_get_error();
	}*/

	
	
	
	///////////////////////////////////////////////////////////CON LISTAS
	if ((NULL == CU_add_test(pSuite, "Creacion de proyeto a partir de descripcion statica.", testCreateProject)))
	{
		CU_cleanup_registry();
		return CU_get_error();
	}
	
	if ((NULL == CU_add_test(pSuite, "Verificando el proceso de contruccion.", testBuild)))
	{
		CU_cleanup_registry();
		return CU_get_error();
	}
	
	if ((NULL == CU_add_test(pSuite, "Compilacion de proyecto generado.", testCompile)))
	{
		CU_cleanup_registry();
		return CU_get_error();
	}
		
	/* Run all tests using the CUnit Basic interface */
	CU_basic_set_mode(CU_BRM_VERBOSE);
	CU_basic_run_tests();
	CU_cleanup_registry();
	return CU_get_error();	
}
