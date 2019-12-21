#include "iprt\alloc.h"
#include "iprt\log.h"
#include "iprt\initterm.h"


RTDECL(void *)  RTMemAllocTag(size_t cb, const char *pszTag)
{
    return NULL;
}

RTDECL(void) RTMemFree(void *pv)
{

}

RTDECL(void *)  RTMemAllocZTag(size_t cb, const char *pszTag)
{
    return NULL;
}

RTDECL(PRTLOGGER) RTLogRelGetDefaultInstanceEx(uint32_t fFlagsAndGroup)
{
    return NULL;
}

RTR3DECL(int) RTR3InitDll(uint32_t fFlags)
{
    return 0;
}
