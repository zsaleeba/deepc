#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <CUnit/Basic.h>

#include "persmem.h"


PersMem *pm = NULL;

struct MasterStruct
{
    int a;
    void *b;
};


int initSystemSuite()
{
    unlink("test.pm");
    return 0;
}


int cleanSystemSuite()
{
    unlink("test.pm");
    return 0;
}


void testPmopen()
{
    pm = pmopen("test.pm", true, true, sizeof(struct MasterStruct));
    CU_ASSERT(pm->wasCreated);
    CU_ASSERT(pm->masterStruct != NULL);
}


void testPmmalloc()
{
    void *mem = pmmalloc(pm, 6);
    CU_ASSERT(mem != NULL);
    memcpy(mem, "hello", 6);
}


void testPmclose()
{
    pmclose(pm);
}


int main()
{
    CU_pSuite pSuite = NULL;
 
    /* initialize the CUnit test registry */
    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();
 
    /* add a suite to the registry */
    pSuite = CU_add_suite("persmem_system", initSystemSuite, cleanSystemSuite);
    if (NULL == pSuite) 
    {
        CU_cleanup_registry();
        return CU_get_error();
    }
 
    /* add the tests to the suite */
    /* NOTE - ORDER IS IMPORTANT - MUST TEST fread() AFTER fprintf() */
    if ((CU_add_test(pSuite, "test of pmopen()", testPmopen) == NULL) ||
            (CU_add_test(pSuite, "test of pmmalloc()", testPmmalloc) == NULL) ||
            (CU_add_test(pSuite, "test of pmclose()", testPmclose) == NULL))
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
