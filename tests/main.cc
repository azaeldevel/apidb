

#include <CUnit/Basic.h>

#ifdef OCTETOS_APIDB_ENABLE_TDD_V6
	#include "v6.hh"
#endif


int main(int argc, char *argv[])
{
	/* initialize the CUnit test registry */
	if (CUE_SUCCESS != CU_initialize_registry()) return CU_get_error();

#ifdef OCTETOS_APIDB_ENABLE_TDD_V6

	CU_pSuite pSuite_v6 = CU_add_suite("APIDB version 6..", v6_init, v6_clean);
	if (NULL == pSuite_v6)
	{
		CU_cleanup_registry();
		return CU_get_error();
	}
	
	if (NULL == CU_add_test(pSuite_v6, "Developing..", v6_develop))
	{
		CU_cleanup_registry();
		return CU_get_error();
	}
	
#endif

	/* Run all tests using the CUnit Basic interface */
	CU_basic_set_mode(CU_BRM_VERBOSE);
	CU_basic_run_tests();
	CU_cleanup_registry();
	return CU_get_error();
}
