
#include <CUnit/Basic.h>
#include <iostream>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <random>
#include <functional> //for std::function
#include <algorithm>  //for std::generate_n
#include <signal.h>



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
static octetos::apidb::ConfigureProject configProject_nls;
static octetos::apidb::ConfigureProject configProject;

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
        configProject.selects.insert(std::make_pair(tbP->getName().c_str(),tbP));
        configProject.selects.insert(std::make_pair(tbUsers->getName().c_str(),tbUsers));
        configProject.downloads.insert(std::make_pair(tbP->getName().c_str(),tbP));
        configProject.downloads.insert(std::make_pair(tbUsers->getName().c_str(),tbUsers));
        configProject.executable_target  = "developing";
        
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
        if(!configProject.readConfig(filename))
        {                
                if(octetos::toolkit::Error::check())
                {
                        std::cout << "Error  -> "<< octetos::toolkit::Error::get().describe() << std::endl;
                }
                CU_ASSERT(false);
                exit(EXIT_FAILURE);// hay pruebas que depende de esta.
        }
        octetos::apidb::Driver driver(configProject);
        octetos::apidb::Tracer tracer(0);
        if(!driver.driving((octetos::apidb::Tracer*)NULL))
        {
                if(octetos::toolkit::Error::check())
                {
                        std::cout << "Error  -> "<< octetos::toolkit::Error::get().describe() << std::endl;
                }
                CU_ASSERT(false);
                exit(EXIT_FAILURE);// hay pruebas que depende de esta.
        }
        CU_ASSERT(true);
}

void testCompile()
{
        if(configProject.packing == octetos::apidb::PackingLenguajes::CMake)
        {

                int ret = 0;

                std::string cmd = "cp ../tests/developing.cpp ";
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
                cmd  = " cd apidb && make   &> /dev/null";
                if(system(cmd.c_str()) < 0)
                {
                        std::cout << "Fallo al realizar la compialcion.\n";
                        CU_ASSERT(false);
                }
                cmd  = "./apidb/developing";
                ret = system(cmd.c_str()) ;
                if (WIFSIGNALED(ret) && (WTERMSIG(ret) == SIGINT || WTERMSIG(ret) == SIGQUIT) || ret < 0)
                {
                        std::cout << "Fallo al realizar la compialcion.\n";
                        CU_ASSERT(false);
                }

        }
}

void testNewAnalyzer()
{
        if(!configProject.readConfig(filename))
        {                
                if(octetos::toolkit::Error::check())
                {
                        std::cout << "Error  -> "<< octetos::toolkit::Error::get().describe() << std::endl;
                }
                CU_ASSERT(false);
                exit(EXIT_FAILURE);// hay pruebas que depende de esta.
        }
        octetos::apidb::Driver driver(configProject);
        octetos::apidb::Tracer tracer(0);
        if(!driver.driving(&tracer))
        {
                if(octetos::toolkit::Error::check())
                {
                        std::cout << "Error  -> "<< octetos::toolkit::Error::get().describe() << std::endl;
                }
                CU_ASSERT(false);
                exit(EXIT_FAILURE);// hay pruebas que depende de esta.
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
	
	
	if ((NULL == CU_add_test(pSuite, "Verificando la conectividad del componente.", testConecction)))
	{
		CU_cleanup_registry();
		return CU_get_error();
	}
	////////////////////////////////////////////////////////// SIN LISTAS
	if ((NULL == CU_add_test(pSuite, "Creacion de proyeto a partir de descripcion statica para no-list.", testCreateProject_nlst)))
	{
		CU_cleanup_registry();
		return CU_get_error();
	}	
	/*if ((NULL == CU_add_test(pSuite, "Verificando el proceso de contruccion para no-list.", testBuild_nlst)))
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
	
	/*if ((NULL == CU_add_test(pSuite, "Verificando el proceso de contruccion.", testBuild)))
	{
		CU_cleanup_registry();
		return CU_get_error();
	}*/
	
	/*if ((NULL == CU_add_test(pSuite, "Compilacion de proyecto generado.", testCompile)))
	{
		CU_cleanup_registry();
		return CU_get_error();
	}*/
		
	if ((NULL == CU_add_test(pSuite, "Analizer Nuevo.", testNewAnalyzer)))
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
