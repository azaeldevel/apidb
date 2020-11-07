
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
#include "../common.hpp"

#ifdef APIDB_MYSQL
    #include <octetos/db/clientdb-mysql.hh>
#endif
#ifdef APIDB_POSTGRESQL
    #include <octetos/db/clientdb-postgresql.hh>
#endif
#ifdef APIDB_MARIADB
    #include <octetos/db/clientdb-maria.hh>
#endif

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
static std::string fnJava;
#ifdef APIDB_MYSQL
static octetos::db::mysql::Datconnect mysqlSource("localhost",3306,"sysapp.alpha","sysapp","123456");
#endif
#ifdef APIDB_POSTGRESQL
static octetos::db::postgresql::Datconnect postgresqlSource("localhost",5432,"sysapp_alpha","sysapp","123456"); 
#endif
#ifdef APIDB_MARIADB
static octetos::db::mariadb::Datconnect mariaSource("localhost",3306,"sysapp.alpha","sysapp","123456");
#endif
//static octetos::toolkit::clientdb::mysql::Datconnect mysqlSourcev2("192.168.0.101",3306,"sysappv2.alpha","develop","123456"); 
static std::string sysappv1Filename = "sysappv1-alpha.apidb";
static std::string sysappv20Filename = "sysappv20-alpha.apidb";
static std::string sysappv30Filename = "sysappv30-alpha.apidb";
static std::string sysappjava = "sysapp-java-alpha.apidb";

/* The suite initialization function.
 * Opens the temporary file used by the tests.
 * Returns zero on success, non-zero otherwise.
 */
int init_apidb(void)
{
        filename = random_string(50);
        filename_nlst = random_string(50);
		fnJava = random_string(50);
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
        remove(fnJava.c_str());
        return 0;
}




void testConecction()
{
        octetos::apidb::ConfigureProject configProject;
#ifdef APIDB_MYSQL
	configProject.setInputs(octetos::apidb::InputLenguajes::MySQL,mysqlSource);    
#endif
#ifdef APIDB_POSTGRESQL
	configProject.setInputs(octetos::apidb::InputLenguajes::PostgreSQL,postgresqlSource);    
#endif
#ifdef APIDB_MARIADB
	configProject.setInputs(octetos::apidb::InputLenguajes::MariaDB,mariaSource);    
#endif
    bool fl = configProject.testConexion();
    if(fl)
    {
        CU_ASSERT(true);        
    }
    else
    {
        if(octetos::core::Error::check())
        {
            std::cout << octetos::core::Error::get().describe() << std::endl;
        } 
        CU_ASSERT(false); 
    }
}

void testCreateProject_nlst()
{
	octetos::apidb::ConfigureProject configProject_nls;
    configProject_nls.name = "sysapp";
    configProject_nls.builDirectory = "apidb-nlst";
#ifdef APIDB_MYSQL
	configProject_nls.setInputs(octetos::apidb::InputLenguajes::MySQL,mysqlSource);    
#endif
#ifdef APIDB_POSTGRESQL
	configProject_nls.setInputs(octetos::apidb::InputLenguajes::PostgreSQL,postgresqlSource);    
#endif
#ifdef APIDB_MARIADB
	configProject_nls.setInputs(octetos::apidb::InputLenguajes::MariaDB,mariaSource);    
#endif
	octetos::core::Semver version;
	version.setNumbers(0,1,0);
    version.setPrerelease("alpha");    
    configProject_nls.versionResult = version;
    configProject_nls.outputLenguaje = octetos::apidb::OutputLenguajes::CPP;	
    configProject_nls.packing = octetos::apidb::PackingLenguajes::CMake;
    configProject_nls.compiled = octetos::apidb::Compiled::STATIC;
	configProject_nls.writeDatconnect = "conector";
    
	try
	{
		configProject_nls.saveConfig(filename_nlst);
		CU_ASSERT(true);
	}
	catch (std::exception e)
	{
		CU_ASSERT(false);
	}
}
void testCreateProject()
{
	if(octetos::core::Error::check())
	{
		std::cout << octetos::core::Error::get().describe() << std::endl;
		CU_ASSERT(false)
		return;
	}
	octetos::core::Semver version;
	version.setNumbers(0,1,0);
	version.setPrerelease("alpha");
	octetos::apidb::ConfigureProject configProject;
	configProject.name = "sysapp";
	configProject.builDirectory  = "apidb";
	configProject.versionResult = version;
#ifdef APIDB_MYSQL
	configProject.setInputs(octetos::apidb::InputLenguajes::MySQL,mysqlSource);    
#endif
#ifdef APIDB_POSTGRESQL
	configProject.setInputs(octetos::apidb::InputLenguajes::PostgreSQL,postgresqlSource);    
#endif
#ifdef APIDB_MARIADB
	configProject.setInputs(octetos::apidb::InputLenguajes::MariaDB,mariaSource);    
#endif
	configProject.outputLenguaje = octetos::apidb::OutputLenguajes::CPP;	
	configProject.packing = octetos::apidb::PackingLenguajes::CMake;
	configProject.compiled = octetos::apidb::Compiled::STATIC;
	octetos::apidb::ConfigureProject::Table* tbP = new octetos::apidb::ConfigureProject::Table("Persons");
	octetos::apidb::ConfigureProject::Function* dwFullName = new octetos::apidb::ConfigureProject::Function("fullname");
	dwFullName->addParam(std::string("name1"));
	dwFullName->addParam(std::string("name2"));
	dwFullName->addParam(std::string("name3"));
	dwFullName->addParam(std::string("name4"));
	tbP->insert(std::make_pair(dwFullName->getName().c_str(), dwFullName));
	octetos::apidb::ConfigureProject::Function* dwShortName = new octetos::apidb::ConfigureProject::Function("shortname");
	dwShortName->addParam(std::string("name1"));
	dwShortName->addParam(std::string("name3"));
	tbP->insert(std::make_pair(dwShortName->getName().c_str(), dwShortName));
	configProject.downloads.insert(std::make_pair(tbP->getName().c_str(),tbP));
	configProject.selects.insert(std::make_pair(tbP->getName().c_str(),tbP));
	octetos::apidb::ConfigureProject::Table* tbUsers = new octetos::apidb::ConfigureProject::Table("Users");
	octetos::apidb::ConfigureProject::Function* byUsername = new octetos::apidb::ConfigureProject::Function("byUsername");    
	byUsername->addParam(std::string("name"));
	byUsername->addParam(std::string("person"));
	tbUsers->insert(std::make_pair(byUsername->getName().c_str(), byUsername));
	configProject.selects.insert(std::make_pair(tbUsers->getName().c_str(),tbUsers));
	configProject.downloads.insert(std::make_pair(tbUsers->getName().c_str(),tbUsers));
    octetos::core::Semver ver = octetos::apidb::getPakageVersion();
    std::string strdevtaget = "developing";
	configProject.executable_target  = strdevtaget + std::to_string(ver.getMajor());
	configProject.namespace_detect = "emulate";
	configProject.writeDatconnect = "conector";
	//std::cout << std::endl << "Testing 1" << std::endl;
	if(octetos::core::Error::check())
	{
		std::cout << octetos::core::Error::get().describe() << std::endl;
		CU_ASSERT(false)
		return;
	}
	try
	{
		configProject.saveConfig(filename);
		CU_ASSERT(true);
	}
	catch (std::exception e)
	{
		CU_ASSERT(false);
	}
	
	
	
	
	
	//>>>>>>>>>>>>>>>>>>>>>for java
	configProject.outputLenguaje = octetos::apidb::OutputLenguajes::JAVA;	
	//configProject.packing = octetos::apidb::PackingLenguajes::CMake;
	
	if(octetos::core::Error::check())
	{
		std::cout << octetos::core::Error::get().describe() << std::endl;
		CU_ASSERT(false)
		return;
	}
	try
	{
		configProject.saveConfig(fnJava);
		CU_ASSERT(true);
	}
	catch (std::exception& e)
	{
		CU_ASSERT(false);
		std::cout << e.what() << "\n";
	}
}

void testBuild_nlst()
{
    //std::cout << "testBuild_nlst: Step 1\n";
	octetos::apidb::ConfigureProject configProject_nls;
    //std::cout << "testBuild_nlst: Step 1.1\n";
	try
	{
		configProject_nls.readConfig(filename_nlst);
		CU_ASSERT(true);
	}
	catch(octetos::core::Error e)
	{
		CU_ASSERT(false);
		return;
	}

    //std::cout << "testBuild_nlst: Step 2\n";
    octetos::apidb::Driver driver(configProject_nls);
    if(octetos::core::Error::check())
    {
        std::cout << "Error -> " << octetos::core::Error::get().describe() << std::endl;
        CU_ASSERT(false);
    }
    octetos::apidb::Tracer tracer(0);
    //std::cout << "testBuild_nlst: Step 3\n";
    if(!driver.driving(NULL))
    {
        if(octetos::core::Error::check())
        {
            std::cout << "Error  -> " << octetos::core::Error::get().describe() << std::endl;
        }
        CU_ASSERT(false);
        return;
    }
    
    //std::cout << "testBuild_nlst: Step 4\n";
        
    CU_ASSERT(true);
}

void testBuild()
{
    //std::cout << "testBuild: Step 1\n";
	octetos::apidb::ConfigureProject configProject;
    //std::cout << "testBuild: Step 1.1\n";
	try
	{
		configProject.readConfig(fnJava);
		CU_ASSERT(true);
	}
	catch(octetos::core::Error& e)
	{
        CU_ASSERT(false);
		std::cout << e.what() << "\n";
        return;		
	}

    //std::cout << "testBuild: Step 2\n";
    octetos::apidb::Driver driver(configProject);
    if(octetos::core::Error::check())
    {
        std::cout << "Error -> " << octetos::core::Error::get().describe() << std::endl;
        CU_ASSERT(false);
    }
    octetos::apidb::Tracer tracer(0);
    //std::cout << "testBuild: Step 3\n";
    if(!driver.driving(&tracer))
    {
        if(octetos::core::Error::check())
        {
            std::cout << "Error  -> " << octetos::core::Error::get().describe() << std::endl;
        }
        CU_ASSERT(false);
        return;
    }
    
    //std::cout << "testBuild: Step 4\n";
        
    CU_ASSERT(true);
}




void testCompilen_nlst()
{
	octetos::apidb::ConfigureProject configProject;
	try
	{
		configProject.readConfig(filename_nlst);
		CU_ASSERT(true);
	}
	catch(octetos::core::Error e)
	{
		CU_ASSERT(false);
		return;
	}

	if(configProject.packing == octetos::apidb::PackingLenguajes::CMake)
	{
        //int ret = 0;
		//octetos::core::Semver ver = octetos::apidb::getPakageVersion();
        std::string cmd ;

        cmd  = " cd apidb-nlst && cmake . &> /dev/null";
        if(system(cmd.c_str()) < 0)
        {
            std::cout << "Fallo al realizar la compialcion.\n";
            CU_ASSERT(false);
        }
        cmd  = " cd apidb-nlst &&  make &> /dev/null";
        if(system(cmd.c_str()) < 0)
        {
            std::cout << "Fallo al realizar la compialcion.\n";
            CU_ASSERT(false);
        }
	}
	else
	{
		CU_ASSERT(false);
	}
}
void testCompilen()
{
	octetos::apidb::ConfigureProject configProject;
	try
	{
		configProject.readConfig(filename);
		CU_ASSERT(true);
	}
	catch(octetos::core::Error e)
	{
		CU_ASSERT(false);
		return;
	}

	if(configProject.packing == octetos::apidb::PackingLenguajes::CMake)
	{
        int ret = 0;
		octetos::core::Semver ver = octetos::apidb::getPakageVersion();
        std::string cmd = "cp ../../src/tests/developing";
        cmd += std::to_string(ver.getMajor());
        if(configProject.getInputLenguaje() == octetos::apidb::InputLenguajes::MySQL)
        {
            cmd += "-mysql";
        }
        else if(configProject.getInputLenguaje() == octetos::apidb::InputLenguajes::PostgreSQL)
        {
            cmd += "-postgresql";
        }
        else if(configProject.getInputLenguaje() == octetos::apidb::InputLenguajes::MariaDB)
        {
            cmd += "-mariadb";
        }
        else
		{
			throw octetos::apidb::BuildException("Lenguaje de entrada desconocido",__FILE__,__LINE__);
		}
        
        cmd += ".cpp ";
        cmd += " apidb/developing";
        cmd += std::to_string(ver.getMajor());
        cmd += ".cpp ";
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
	}
	else
	{
		CU_ASSERT(false);
	}
}


int main(int argc, char *argv[])
{
    bool runAll = false, enableMySQL = false,enablePostgreSQL = false,enableMariaDB = false;
    int runTest = 0;
	int serverscount = 0;
    
    for(int i = 1; i < argc; i++)
    {
        if(strcmp(argv[i],"--run") == 0)
        {
            //std::cout << argv[i] << ";\n";
            runTest = std::stoi(argv[i + 1]);
			i++;
        }
        else if(strcmp(argv[i],"--run-all") == 0)
        {
            //std::cout << argv[i] << ";\n";
            runAll = true;
        }
        
        if(strcmp(argv[i],"--enable-mysql") == 0)
        {
            //std::cout << argv[i] << ";\n";
#ifndef APIDB_MYSQL
            std::cerr << "No hay soporta para MySQL.\n";
            return EXIT_FAILURE;
#endif
            enableMySQL = true;
			serverscount++;
        }
        if(strcmp(argv[i],"--enable-postgresql") == 0)
        {
            //std::cout << argv[i] << ";\n";
#ifndef APIDB_POSTGRESQL
            std::cerr << "No hay soporta para PostgreSQL.\n";
            return EXIT_FAILURE;
#endif
            enablePostgreSQL = true;
			serverscount++;
        }
        if(strcmp(argv[i],"--enable-mariadb") == 0)
        {
            //std::cout << argv[i] << ";\n";
#ifndef APIDB_MARIADB
            std::cerr << "No hay soporta para MariaDB.\n";
            return EXIT_FAILURE;
#endif
            enableMariaDB = true;
			serverscount++;
        } 
    }
    
    if(serverscount > 1)
	{
		std::cerr << "Solo se puer ejecutar un servidor a la vez.";
		return EXIT_FAILURE;
	}
	
	CU_pSuite pSuite = NULL;
	
	/* initialize the CUnit test registry */
	if (CUE_SUCCESS != CU_initialize_registry()) return CU_get_error();

	octetos::core::Semver ver = octetos::apidb::getPakageVersion();
	std::string pkName = octetos::apidb::getPakageName();
	std::string classVersionString = std::string("Probando ") + pkName + " " + ver.toString();
	pSuite = CU_add_suite(classVersionString.c_str(), init_apidb, clean_apidb);
	if (NULL == pSuite) 
	{
		CU_cleanup_registry();
		return CU_get_error();
	}
    
	
	///////////////////////////////////////////////////////////CON LISTAS

     
	if(runTest == 1 or runAll)
	{
		if ((NULL == CU_add_test(pSuite, "Verificando la conectividad del componente.", testConecction)))
		{
			CU_cleanup_registry();
			return CU_get_error();
		}
	}   
	////////////////////////////////////////////////////////// SIN LISTAS
	if(runTest == 2 or runAll)
	{
		if ((NULL == CU_add_test(pSuite, "Creación de proyecto a partir de descripción statica para (no-lists).", testCreateProject_nlst)))
		{
			CU_cleanup_registry();
			return CU_get_error();
		}	
	}  
	if(runTest == 2 or runAll)
	{
		if ((NULL == CU_add_test(pSuite, "Creación de proyecto a partir de descripción statica.", testCreateProject)))
		{
                CU_cleanup_registry();
                return CU_get_error();
		}	
	}   
	/*if(runTest == 4 or runAll)
	{
		if ((NULL == CU_add_test(pSuite, "Verificando el proceso de contrucción.", testBuild)))
		{
			CU_cleanup_registry();
			return CU_get_error();
		}
	}*/
	if(runTest == 4 or runAll)
	{
		if ((NULL == CU_add_test(pSuite, "Verificando el proceso de contrucción(no-lists).", testBuild_nlst)))
		{
			CU_cleanup_registry();
			return CU_get_error();
		}
	}
	if(runTest == 4 or runAll)
	{
		if ((NULL == CU_add_test(pSuite, "Verificando el proceso de contrucción.", testBuild)))
		{
			CU_cleanup_registry();
			return CU_get_error();
		}
	}
	if(runTest == 5 or runAll)
	{
		if ((NULL == CU_add_test(pSuite, "Compilación de proyecto generado (no-lists).", testCompilen_nlst)))
		{
			CU_cleanup_registry();
			return CU_get_error();
		}
	}
	if(runTest == 5 or runAll)
	{
		if ((NULL == CU_add_test(pSuite, "Compilación de proyecto generado.", testCompilen)))
		{
			CU_cleanup_registry();
			return CU_get_error();
		}
	}
	/*
	if(runTest == 6 or runAll)
	{
		if ((NULL == CU_add_test(pSuite, "Pruebas temporales.", testTemp)))
		{
			CU_cleanup_registry();
			return CU_get_error();
		}
	}*/
		
	/* Run all tests using the CUnit Basic interface */
	CU_basic_set_mode(CU_BRM_VERBOSE);
	CU_basic_run_tests();
	CU_cleanup_registry();
	return CU_get_error();	
}
