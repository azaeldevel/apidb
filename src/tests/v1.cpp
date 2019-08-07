
#include <CUnit/Basic.h>
#include <iostream>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <random>
#include <functional> //for std::function
#include <algorithm>  //for std::generate_n

#include "../apidb.hpp"
#include "../Errors.hpp"


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
static octetos::toolkit::clientdb::mysql::Datconnect mysqlSource("192.168.0.101",3306,"sysapp.alpha","develop","123456"); 

/* The suite initialization function.
 * Opens the temporary file used by the tests.
 * Returns zero on success, non-zero otherwise.
 */
int init_apidb(void)
{
        filename = random_string(10);
        filename_nlst = random_string(10);
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
        
	octetos::apidb::ConfigureProject config;
        config.name = "sysapp";
        config.builDirectory = "apidb";
        config.conectordb = &mysqlSource;
        config.version = version;
        config.inputLenguaje = octetos::apidb::InputLenguajes::MySQL;
        config.outputLenguaje = octetos::apidb::OutputLenguajes::CPP;	
        config.packing = octetos::apidb::PackingLenguajes::CMake;
        config.compiled = octetos::apidb::Compiled::STATIC;
        
        if(config.saveConfig(filename_nlst))
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
        octetos::apidb::ConfigureProject config;
        config.conectordb = &mysqlSource;
        config.inputLenguaje = octetos::apidb::InputLenguajes::MySQL;
        CU_ASSERT(config.testConexion());
}

void testCreateProject()
{
	octetos::toolkit::Version version;
	version.setNumbers(0,1,0);
        version.setStage(octetos::toolkit::Version::Stage::alpha);
        
	octetos::apidb::ConfigureProject config;
        config.name = "sysapp";
        config.builDirectory = "apidb";
        config.conectordb = &mysqlSource;
        config.version = version;
        config.inputLenguaje = octetos::apidb::InputLenguajes::MySQL;
        config.outputLenguaje = octetos::apidb::OutputLenguajes::CPP;	
        config.packing = octetos::apidb::PackingLenguajes::CMake;
        config.compiled = octetos::apidb::Compiled::STATIC;
        octetos::apidb::ConfigureProject::Table tbP("Persons");
        octetos::apidb::ConfigureProject::Function dwFullName("fullname",octetos::apidb::ConfigureProject::Function::DOWNLOAD);
        dwFullName.addParam("name1");
        dwFullName.addParam("name2");
        dwFullName.addParam("name3");
        dwFullName.addParam("name4");
        tbP.insert(std::make_pair(dwFullName.getName().c_str(), &dwFullName));
        octetos::apidb::ConfigureProject::Function dwShortName("shortname",octetos::apidb::ConfigureProject::Function::DOWNLOAD);
        dwShortName.addParam("name1");
        dwShortName.addParam("name3");
        tbP.insert(std::make_pair(dwShortName.getName().c_str(), &dwShortName));
        config.downloads.insert(std::make_pair(tbP.getName().c_str(),&tbP));
        config.selects.insert(std::make_pair(tbP.getName().c_str(),&tbP));
        octetos::apidb::ConfigureProject::Table tbUsers("Users");
        octetos::apidb::ConfigureProject::Function byUsername("byUsername",octetos::apidb::ConfigureProject::Function::SELECT);    
        byUsername.addParam("username");
        byUsername.addParam("person");
        tbUsers.insert(std::make_pair(byUsername.getName().c_str(), &byUsername));
        config.selects.insert(std::make_pair(tbP.getName().c_str(),&tbUsers));
    
        if(config.saveConfig(filename))
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
        octetos::apidb::ConfigureProject config;     
        //config.mvc =octetos::apidb::MVC::GTK3;
        if(!config.readConfig(filename_nlst))
        {
                if(octetos::toolkit::Error::check())
                {
                        std::cout << "Error  -> "<< octetos::toolkit::Error::get().describe() << std::endl;
                }
                CU_ASSERT(false);
                return;
        }
        octetos::apidb::Driver driver(config);
        if(!driver.driving(false))
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
        octetos::apidb::ConfigureProject config;        
        if(!config.readConfig(filename))
        {
                if(octetos::toolkit::Error::check())
                {
                        std::cout << "Error  -> "<< octetos::toolkit::Error::get().describe() << std::endl;
                }
                CU_ASSERT(false);
                return;
        }
        octetos::apidb::Driver driver(config);
        //std::cout <<"Objto construido 2." <<std::endl;
        if(!driver.driving(false))
        {
                if(octetos::toolkit::Error::check())
                {
                        std::cout << "Error  -> "<< octetos::toolkit::Error::get().describe() << std::endl;
                }
                CU_ASSERT(false);
        }
        
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
	
	if ((NULL == CU_add_test(pSuite, "Verificando la conectividad del componente.\n", testConecction)))
	{
		CU_cleanup_registry();
		return CU_get_error();
	}
	
	if ((NULL == CU_add_test(pSuite, "Creacion de proyeto a partir de descripcion statica.\n", testCreateProject)))
	{
		CU_cleanup_registry();
		return CU_get_error();
	}
	
	if ((NULL == CU_add_test(pSuite, "Verificando el proceso de contruccion.\n", testBuild)))
	{
		CU_cleanup_registry();
		return CU_get_error();
	}
	
	
	if ((NULL == CU_add_test(pSuite, "Creacion de proyeto a partir de descripcion statica para no-list.\n", testCreateProject_nlst)))
	{
		CU_cleanup_registry();
		return CU_get_error();
	}
	
	if ((NULL == CU_add_test(pSuite, "Verificando el proceso de contruccion para no-list.\n", testBuild_nlst)))
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
