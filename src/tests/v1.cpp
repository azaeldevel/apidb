
#include <CUnit/Basic.h>
#include <iostream>
#include <stdlib.h>

#include "../apidb.hpp"
#include "../Errors.hpp"


//static std::string  rootDir;

/* Pointer to the file used by the tests. */
//static FILE* temp_file = NULL;
static const char* filenameAPIDB = "apidb/apidb";

/* The suite initialization function.
 * Opens the temporary file used by the tests.
 * Returns zero on success, non-zero otherwise.
 */
int init_apidb(void)
{
        return 0;
}

/* The suite cleanup function.
 * Closes the temporary file used by the tests.
 * Returns zero on success, non-zero otherwise.
 */
int clean_apidb(void)
{
        return 0;
}



void testGeneration()
{            
        octetos::apidb::ConfigureProject config;        
        if(config.readConfig(filenameAPIDB) == false)
        {
                //std::cout << "Error  -> "<< octetos::apidb::getError().what() << std::endl;//std::cout << "Error  -> " << octetos::apidb::getError().what() << std::endl;
                CU_ASSERT(false);
                return;
        }
        config.directory = "doc/test/ret/";
        octetos::apidb::Driver driver(config);
        if(!driver.driving(false))
        {
                //std::cout << "Error  -> "<< octetos::apidb::getError().what() << std::endl;//std::cout << "Error  -> " << octetos::apidb::getError().what() << std::endl;
                CU_ASSERT(false);
        }
        
        CU_ASSERT(true);
}

/*void testRQ0001001()
{	
	toolkit::Version ver = toolkit::getVersion();
	std::string strMessge = "Valid RQ 0001-001..";
	CU_ASSERT(ver.getMajor() > -1)
}*/


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
	
	if ((NULL == CU_add_test(pSuite, "Verificando la lectura de configuracion y generacion de proyecto.\n", testGeneration)))
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
