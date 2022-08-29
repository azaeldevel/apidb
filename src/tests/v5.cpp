
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
#include <filesystem>
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
static octetos::db::mysql::Datconnect mysqlSource("localhost",3306,"muposys-0-alpha","muposys","123456");
#endif
#ifdef APIDB_POSTGRESQL
static octetos::db::postgresql::Datconnect postgresqlSource("localhost",5432,"sysapp_alpha","sysapp","123456"); 
#endif
#ifdef APIDB_MARIADB
static octetos::db::maria::Datconnect mariaSource("localhost",3306,"muposys-0-alpha","muposys","123456");
#endif
//static octetos::toolkit::clientdb::mysql::Datconnect mysqlSourcev2("192.168.0.101",3306,"sysappv2.alpha","develop","123456"); 
//static std::string sysappv1Filename = "sysappv1-alpha.apidb";
//static std::string sysappv20Filename = "sysappv20-alpha.apidb";
//static std::string sysappv30Filename = "sysappv30-alpha.apidb";
//static std::string sysappjava = "sysapp-java-alpha.apidb";

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
	//std::cout << "testCreateProject_nlst step 1\n";
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
	//std::cout << "testCreateProject_nlst step 2\n";
	octetos::core::Semver version;
	//std::cout << "testCreateProject_nlst step 3\n";
	version.setNumbers(0,1,0);
	//std::cout << "testCreateProject_nlst step 4\n";
    version.setPrerelease("alpha");  
	//std::cout << "testCreateProject_nlst step 5\n";
    configProject_nls.versionResult = version;
    //std::cout << "testCreateProject_nlst step 6\n";
    configProject_nls.outputLenguaje = octetos::apidb::OutputLenguajes::CPP;	
    configProject_nls.packing = octetos::apidb::PackingLenguajes::CMake;
    configProject_nls.compiled = octetos::apidb::Compiled::STATIC;
	configProject_nls.writeDatconnect = "conector";
    //std::cout << "testCreateProject_nlst step 7\n";
	
	
	try
	{
		configProject_nls.saveConfig(filename_nlst);
		CU_ASSERT(true);
	}
	catch (std::exception e)
	{
		CU_ASSERT(false);
	}
	//std::cout << "testCreateProject_nlst step 8\n";
}
void testCreateProject()
{
	//std::cout << "testCreateProject: Step 1\n";
	
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
	configProject.name = "muposys";
	configProject.builDirectory  = "muposys";
	configProject.versionResult = version;
	//std::cout << "testCreateProject: Step 2\n";
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
	catch (const std::exception& e)
	{
		CU_ASSERT(false);
		//std::cout << "Exception : " << e.what() << "\n";
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
    if(!driver.driving(&tracer))
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
	//std::cout << "Opening .. " << filename << "\n";
	try
	{
		configProject.readConfig(filename);
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
        system("cd ..");
	}
	else
	{
		CU_ASSERT(false);
	}
}
void testCompilen()
{
	//std::cout << "testCompilen Step 1\n";
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
	
	//std::cout << "testCompilen Step 2\n";

	if(configProject.packing == octetos::apidb::PackingLenguajes::CMake)
	{
		//std::cout << "testCompilen Step 2.1\n";
		
        int ret = 0;
		octetos::core::Semver ver = octetos::apidb::getPakageVersion();
		std::filesystem::path from = "../../../src/tests" ;
		std::filesystem::path to = "muposys/developing" + std::to_string(ver.getMajor()) + ".cpp";
		std::string filename = "developing" + std::to_string(ver.getMajor());
        if(configProject.getInputLenguaje() == octetos::apidb::InputLenguajes::MySQL)
        {
            filename += "-mysql.cpp";
        }
        else if(configProject.getInputLenguaje() == octetos::apidb::InputLenguajes::PostgreSQL)
        {
            filename += "-postgresql.cpp";
        }
        else if(configProject.getInputLenguaje() == octetos::apidb::InputLenguajes::MariaDB)
        {
            filename += "-mariadb.cpp";
        }
        else
		{
			throw octetos::apidb::BuildException("Lenguaje de entrada desconocido",__FILE__,__LINE__);
		}
		from /= filename;
		
		//std::cout << "testCompilen Step 2.2\n";
        
        if(not std::filesystem::exists(to))
		{
			if( not std::filesystem::copy_file(from,to))
			{
				CU_ASSERT(false);			
			}
		}

        std::string cmd  = " cd muposys && cmake . &> /dev/null";
        if(system(cmd.c_str()) < 0)
        {
            std::cout << "Fallo al realizar la compialcion.\n";
            CU_ASSERT(false);
        }
        cmd  = " cd muposys &&  make  &> /dev/null";
        if(system(cmd.c_str()) < 0)
        {
            std::cout << "Fallo al realizar la compilacion.\n";
            CU_ASSERT(false);
        }
        
		//std::cout << "testCompilen Step 2.3\n";
	}
	else
	{
		CU_ASSERT(false);
	}
	//std::cout << "testCompilen Step 3\n";
}


int main(int argc, char *argv[])
{
    bool runAll = false, enableMySQL = false,enablePostgreSQL = false,enableMariaDB = false;
    int runTest = 0;
	int serverscount = 0;
	
	//std::cout << "Test Step 1\n";
    
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
	
	//std::cout << "Test Step 2\n";
	
	/* initialize the CUnit test registry */
	if (CUE_SUCCESS != CU_initialize_registry()) return CU_get_error();

	//octetos::core::Semver ver = octetos::apidb::getPakageVersion();
	//std::string pkName = octetos::apidb::getPakageName();
	//std::string classVersionString = std::string("Probando ") + pkName + "  " + ver.toString();
	pSuite = CU_add_suite("APIDB", init_apidb, clean_apidb);
	if (NULL == pSuite) 
	{
		CU_cleanup_registry();
		return CU_get_error();
	}
    
    //std::cout << "Test Step 3\n";
	
	///////////////////////////////////////////////////////////CON LISTAS

	
	if(runTest == 1 or runAll)
	{
		if ((NULL == CU_add_test(pSuite, "Verificando la conectividad del componente.", testConecction)))
		{
			CU_cleanup_registry();
			return CU_get_error();
		}
	}   
	//std::cout << "Test Step 4\n";
	////////////////////////////////////////////////////////// SIN LISTAS
	if(runTest == 2 or runAll)
	{
		if ((NULL == CU_add_test(pSuite, "Creación de proyecto a partir de descripción statica para (no-lists).", testCreateProject_nlst)))
		{
			CU_cleanup_registry();
			return CU_get_error();
		}	
	}
	//std::cout << "Test Step 5\n";
	if(runTest == 2 or runAll)
	{
		if ((NULL == CU_add_test(pSuite, "Creación de proyecto a partir de descripción statica.", testCreateProject)))
		{
                CU_cleanup_registry();
                return CU_get_error();
		}
	}  
	//std::cout << "Test Step 6\n";
	if(runTest == 4 or runAll)
	{
		if ((NULL == CU_add_test(pSuite, "Verificando el proceso de contrucción.", testBuild)))
		{
			CU_cleanup_registry();
			return CU_get_error();
		}
	}
	//std::cout << "Test Step 7\n";
	if(runTest == 4 or runAll)
	{
		if ((NULL == CU_add_test(pSuite, "Verificando el proceso de contrucción(no-lists).", testBuild_nlst)))
		{
			CU_cleanup_registry();
			return CU_get_error();
		}
	}
	//std::cout << "Test Step 8\n";
	if(runTest == 5 or runAll)
	{
		if ((NULL == CU_add_test(pSuite, "Compilación de proyecto generado (no-lists).", testCompilen_nlst)))
		{
			CU_cleanup_registry();
			return CU_get_error();
		}
	}
	//std::cout << "Test Step 9\n";
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
	}
	*/
	
	//std::cout << "Test Step 10\n";
		
	/* Run all tests using the CUnit Basic interface */
	CU_basic_set_mode(CU_BRM_VERBOSE);
	CU_basic_run_tests();
	CU_cleanup_registry();
	return CU_get_error();	
}
