
#include <CUnit/Basic.h>
#include <iostream>
#include <stdlib.h>

#include "../apidb.hpp"


//static std::string  rootDir;

/* Pointer to the file used by the tests. */
//static FILE* temp_file = NULL;
static const char* filenameAPIDB = "apidb/apidb";

/* The suite initialization function.
 * Opens the temporary file used by the tests.
 * Returns zero on success, non-zero otherwise.
 */
int init_toolkit_common(void)
{
        return 0;
}

/* The suite cleanup function.
 * Closes the temporary file used by the tests.
 * Returns zero on success, non-zero otherwise.
 */
int clean_toolkit_common(void)
{
        return 0;
}



void testGeneration()
{        
        FILE *file = fopen(filenameAPIDB, "r");
        if (file != NULL)
        {
                fclose(file);
        }
        else
        {
                CU_ASSERT(false);                
        }
    
        octetos::apidb::ConfigureProject config;        
        CU_ASSERT(config.readConfig(filenameAPIDB));  
        config.directory = "doc/test/ret/";
        octetos::apidb::Driver driver(config);
        if(!driver.driving(false))
        {
                CU_ASSERT(false);
        }
        else
        {
                CU_ASSERT(true);
        }
}

/*void testRQ0001001()
{	
	toolkit::Version ver = toolkit::getVersion();
	std::string strMessge = "Valid RQ 0001-001..";
	CU_ASSERT(ver.getMajor() > -1)
}*/


int main(int argc, char *argv[])
{
        /*if(argc > 0)
        {
                rootDir = argv[1];
        }
        else
        {
                std::cerr << "Indique el directorio root.";
                return EXIT_FAILURE;
        }*/
	CU_pSuite pSuite = NULL;
	
	/* initialize the CUnit test registry */
	if (CUE_SUCCESS != CU_initialize_registry()) return CU_get_error();

	octetos::toolkit::Version ver = octetos::apidb::getPakageVersion();
        std::string pkName = octetos::apidb::getPakageName();
	std::string classVersionString = std::string("Probando ") + pkName + " " + ver.toString();
	pSuite = CU_add_suite(classVersionString.c_str(), init_toolkit_common, clean_toolkit_common);
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
	/*std::string classMessage = "Messages class.";
	if ((NULL == CU_add_test(pSuite, classMessage.c_str(), testRQ0001001)))
	{
		CU_cleanup_registry();
		return CU_get_error();
	}*/
	
	/* Run all tests using the CUnit Basic interface */
	CU_basic_set_mode(CU_BRM_VERBOSE);
	CU_basic_run_tests();
	CU_cleanup_registry();
	return CU_get_error();	
}
