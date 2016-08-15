#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <CUnit/Basic.h>

#include "persmem.h"
#include "persmem_internal.h"

#define TEST_PATH "/tmp/pmtest.persmem"
#define MULTI_PMMALLOC_NUM_ALLOCS 10
#define MULTI_PMMALLOC_ALLOC_SIZE 256
#define RANDOM_ALLOC_ITERATIONS 100000
#define RANDOM_ALLOC_MAX_BLOCKS 1000
#define MAX_BLOCK_INFO 1024

PersMem *pm = NULL;
int expectedAllocs = 0;


struct MasterStruct
{
    int a;
    char *b;
    void *multi[MULTI_PMMALLOC_NUM_ALLOCS];
};


int initSystemSuite()
{
    unlink(TEST_PATH);
    return 0;
}


int cleanSystemSuite()
{
    unlink(TEST_PATH);
    return 0;
}


void showAllocMap()
{
    PersMemBlockInfo blocks[MAX_BLOCK_INFO];
    size_t blockMax = MAX_BLOCK_INFO;
    persmemAllocMapBlockInfo(pm, blocks, &blockMax);
    size_t block;
    for (block = 0; block < blockMax; block++)
    {
        printf("block %d: %p %d\n", (int)block, blocks[block].mem, (int)blocks[block].size);
    }
}


int getAllocMapSize()
{
    PersMemBlockInfo blocks[MAX_BLOCK_INFO];
    size_t blockMax = MAX_BLOCK_INFO;
    persmemAllocMapBlockInfo(pm, blocks, &blockMax);
    return blockMax;
}


void testPmopen()
{
    pm = pmopen(TEST_PATH, true, true, sizeof(struct MasterStruct));
    CU_ASSERT_FATAL(pm != NULL);
    CU_ASSERT(pm->wasCreated);
    CU_ASSERT(pm->masterStruct != NULL);
    
    expectedAllocs = 3;
    CU_ASSERT(getAllocMapSize() == expectedAllocs);
}


void testPmmalloc()
{
    CU_ASSERT_FATAL(pm != NULL);

    void *mem = pmmalloc(pm, 6);
    CU_ASSERT(mem != NULL);
    memcpy(mem, "hello", 6);

    struct MasterStruct *ms = pm->masterStruct;
    ms->a = 42;
    ms->b = mem;
    CU_ASSERT(getAllocMapSize() == 4);
    
    expectedAllocs++;
    CU_ASSERT(getAllocMapSize() == expectedAllocs);
}


void testMultiPmmalloc()
{
    int i;
    CU_ASSERT_FATAL(pm != NULL);

    struct MasterStruct *ms = pm->masterStruct;
    for (i = 0; i < MULTI_PMMALLOC_NUM_ALLOCS; i++)
    {
        ms->multi[i] = pmmalloc(pm, MULTI_PMMALLOC_ALLOC_SIZE);
        CU_ASSERT(ms->multi[i] != NULL);
    }
    
    expectedAllocs += MULTI_PMMALLOC_NUM_ALLOCS;
    CU_ASSERT(getAllocMapSize() == expectedAllocs);
}


void testPmclose()
{
    CU_ASSERT_FATAL(pm != NULL);

    pmclose(pm);
}


void testReopen()
{
    pm = pmopen(TEST_PATH, true, true, sizeof(struct MasterStruct));
    CU_ASSERT_FATAL(pm != NULL);
    CU_ASSERT(!pm->wasCreated);
    CU_ASSERT_FATAL(pm->masterStruct != NULL);
    
    struct MasterStruct *ms = pm->masterStruct;
    CU_ASSERT(ms->a == 42);
    CU_ASSERT(strcmp(ms->b, "hello") == 0);
    
    CU_ASSERT(getAllocMapSize() == expectedAllocs);
}


void testPmfree()
{
    struct MasterStruct *ms = pm->masterStruct;
    pmfree(pm, ms->b);
    ms->b = NULL;
    
    expectedAllocs--;
    CU_ASSERT(getAllocMapSize() == expectedAllocs);
}


void testMultiPmfree()
{
    struct MasterStruct *ms = pm->masterStruct;

    int i;
    for (i = 0; i < MULTI_PMMALLOC_NUM_ALLOCS; i++)
    {
        pmfree(pm, ms->multi[i]);
        ms->multi[i] = NULL;
    }
    
    expectedAllocs -= MULTI_PMMALLOC_NUM_ALLOCS;
    CU_ASSERT(getAllocMapSize() == expectedAllocs);
}


void testRandomAlloc()
{
    int i;
    void *block[RANDOM_ALLOC_MAX_BLOCKS*2];

    /* Clear the blocks for a start. */
    for (i = 0; i < RANDOM_ALLOC_MAX_BLOCKS * 2; i++)
    {
        block[i] = NULL;
    }

    /* Allocate and free blocks at random. */
    for (i = 0; i < RANDOM_ALLOC_ITERATIONS; i++)
    {
        /* Find a free block slot. */
        int blockPos;
        do
        {
            blockPos = rand() % (RANDOM_ALLOC_MAX_BLOCKS*2);
        } while (block[blockPos] != NULL);

        /* Allocate a block here. */
        int blockSize = 1 << (rand() % 6 + 5);
        block[blockPos] = pmmalloc(pm, blockSize);

        if (i >= RANDOM_ALLOC_MAX_BLOCKS)
        {
            /* Find a used block slot. */
            int blockPos;
            do
            {
                blockPos = rand() % (RANDOM_ALLOC_MAX_BLOCKS*2);
            } while (block[blockPos] == NULL);

            /* Free the block here. */
            pmfree(pm, block[blockPos]);
            block[blockPos] = NULL;
        }
    }

    CU_ASSERT(getAllocMapSize() == expectedAllocs + RANDOM_ALLOC_MAX_BLOCKS);
    
    /* Free the remaining blocks. */
    for (i = 0; i < RANDOM_ALLOC_MAX_BLOCKS*2; i++)
    {
        if (block[i] != NULL)
        {
            pmfree(pm, block[i]);
            block[i] = NULL;
        }
    }

    CU_ASSERT(getAllocMapSize() == expectedAllocs);
}


void testExpand()
{
    unsigned origLevel = pm->c->mapLevel;
    size_t bigBlock = PERSMEM_LEVEL_BLOCK_BYTES(origLevel);
    void *bigMem = pmmalloc(pm, bigBlock);
    CU_ASSERT(bigMem != NULL);
    CU_ASSERT(pm->c->mapLevel == origLevel+1);

    pmfree(pm, bigMem);
}


void testRealloc()
{
    struct MasterStruct *ms = pm->masterStruct;
    ms->b = pmrealloc(pm, ms->b, 64);
    CU_ASSERT(ms->b != NULL);
    ms->b = pmrealloc(pm, ms->b, 32);
    CU_ASSERT(ms->b == mem);
    ms->b = pmrealloc(pm, ms->b, 4096);
    CU_ASSERT(ms->b != mem2);
}


void testReclose()
{
    CU_ASSERT_FATAL(pm != NULL);

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
    if ((CU_add_test(pSuite, "test of pmopen()", testPmopen) == NULL) ||
            (CU_add_test(pSuite, "test of pmmalloc()", testPmmalloc) == NULL) ||
            (CU_add_test(pSuite, "test of multiple pmmalloc()", testMultiPmmalloc) == NULL) ||
            (CU_add_test(pSuite, "test of random alloc/free", testRandomAlloc) == NULL) ||
            (CU_add_test(pSuite, "test of pmclose()", testPmclose) == NULL) ||
            (CU_add_test(pSuite, "test of re-open", testReopen) == NULL) ||
            (CU_add_test(pSuite, "test of pmfree()", testPmfree) == NULL) ||
            (CU_add_test(pSuite, "test of expand", testExpand) == NULL) ||
            (CU_add_test(pSuite, "test of pmrealloc()", testRealloc) == NULL) ||
            (CU_add_test(pSuite, "test of multi pmfree()", testMultiPmfree) == NULL) ||
            (CU_add_test(pSuite, "test of re-close", testReclose) == NULL))
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
