#include "persmem_internal.h"



/*
 * NAME:        persmemFitToDepth
 * ACTION:      Rounds a value up to the nearest power of 2 and returns the 
 *              "depth" - ie. the power part.
 * PARAMETERS:  size_t i - the value to round.
 * RETURNS:     The nearest power of two greater than or equal to the value of i.
 */

unsigned persmemFitToLevel(size_t i)
{
    /* Find the most significant set bit. */
    int b = 0;
    size_t x = i;
    
    if (x > 0xFFFFFFFF) { b = b + 32; x = x >> 32; }
    if (x > 0x0000FFFF) { b = b + 16; x = x >> 16; }
    if (x > 0x000000FF) { b = b +  8; x = x >>  8; }
    if (x > 0x0000000F) { b = b +  4; x = x >>  4; }
    if (x > 0x00000003) { b = b +  2; x = x >>  2; }
    if (x > 0x00000001) { b = b +  1; }

    /* Return the nearest same or greater power of two. */
    unsigned bitPos = b;
    if ((((size_t)1)<<b) != i)
    {
        /* 
         * It's not an exact power of two - step up to the next power to 
         * cover it. 
         */
        bitPos++;
    }
    
    if (bitPos < PERSMEM_MIN_ALLOC_BITSIZE)
        return 0;
    else
        return bitPos - PERSMEM_MIN_ALLOC_BITSIZE;
}


/*
 * NAME:        persmemRoundUpPowerOf2
 * ACTION:      Rounds a value up to the nearest power of 2.
 * PARAMETERS:  size_t i - the value to round.
 * RETURNS:     The nearest power of two greater than or equal to the value of i.
 */

size_t persmemRoundUpPowerOf2(size_t i)
{
    return PERSMEM_LEVEL_BLOCK_BYTES(persmemFitToLevel(i));
}
