#include "iprt\alloc.h"
#include "iprt\semaphore.h"
#include "iprt\assert.h"
#include "iprt\log.h"

RTDECL(void) RTMemTmpFree(void *pv)
{

}

RTDECL(void) RTMemFree(void *pv)
{

}

RTDECL(void *)  RTMemAllocTag(size_t cb, const char *pszTag)
{
    return NULL;
}

RTDECL(int)  RTSemEventCreate(PRTSEMEVENT phEventSem)
{
    return -1;
}

RTDECL(int)  RTSemEventDestroy(RTSEMEVENT hEventSem)
{
    return -1;
}

RTDECL(int)  RTSemEventSignal(RTSEMEVENT hEventSem)
{
    return -1;
}

RTDECL(int)  RTSemEventWait(RTSEMEVENT hEventSem, RTMSINTERVAL cMillies)
{
    return -1;
}

RTDECL(void *)  RTMemTmpAllocTag(size_t cb, const char *pszTag)
{
    return NULL;
}

RTDECL(bool)    RTAssertShouldPanic(void)
{
    return true;
}

RTDECL(void)    RTAssertMsg1Weak(const char *pszExpr, unsigned uLine, const char *pszFile, const char *pszFunction)
{

}

RTDECL(PRTLOGGER) RTLogRelGetDefaultInstanceEx(uint32_t fFlagsAndGroup)
{
    return NULL;
}

RTDECL(PRTLOGGER) RTLogRelSetDefaultInstance(PRTLOGGER pLogger)
{
    return NULL;
}

RTDECL(int) RTLogDestroy(PRTLOGGER pLogger)
{
    return -1;
}

RTDECL(PRTLOGGER)   RTLogSetDefaultInstance(PRTLOGGER pLogger)
{
    return NULL;
}

RT_ASM_DECL_PRAGMA_WATCOM(bool) ASMAtomicCmpXchgU8(volatile uint8_t RT_FAR *pu8, const uint8_t u8New, const uint8_t u8Old)
{
    return false;
}
