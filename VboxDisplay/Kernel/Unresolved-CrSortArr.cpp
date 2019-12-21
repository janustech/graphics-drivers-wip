#include "cr_sortarray.h"

VBOXSADECL(int) CrSaInit(CR_SORTARRAY *pArray, uint32_t cInitBuffer)
{
    return -1;
}

VBOXSADECL(void) CrSaCleanup(CR_SORTARRAY *pArray)
{

}
/*
* @return true if element is found */
VBOXSADECL(bool) CrSaContains(const CR_SORTARRAY *pArray, uint64_t element)
{
    return false;
}

/*
* @return VINF_SUCCESS  if element is added
* VINF_ALREADY_INITIALIZED if element was in array already
* VERR_NO_MEMORY - no memory
*  */
VBOXSADECL(int) CrSaAdd(CR_SORTARRAY *pArray, uint64_t element)
{
    return -1;
}

/*
* @return VINF_SUCCESS  if element is removed
* VINF_ALREADY_INITIALIZED if element was NOT in array
*  */
VBOXSADECL(int) CrSaRemove(CR_SORTARRAY *pArray, uint64_t element)
{
    return -1;
}

VBOXSADECL(void) CrSaIntersect(CR_SORTARRAY *pArray1, const CR_SORTARRAY *pArray2)
{

}

VBOXSADECL(int) CrSaClone(const CR_SORTARRAY *pArray1, CR_SORTARRAY *pResult)
{
    return -1;
}
