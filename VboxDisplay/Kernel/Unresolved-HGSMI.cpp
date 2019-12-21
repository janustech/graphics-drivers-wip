#include "VBoxMPWddm.h"

int  HGSMIHeapSetup(HGSMIHEAP *pHeap, void *pvBase, HGSMISIZE cbArea, HGSMIOFFSET offBase, const HGSMIENV *pEnv)
{
    return -1;
}

void HGSMIHeapDestroy(HGSMIHEAP *pHeap)
{

}

void RT_UNTRUSTED_VOLATILE_HSTGST *HGSMIHeapBufferAlloc(HGSMIHEAP *pHeap, HGSMISIZE cbBuffer)
{
    return NULL;
}

DECLHIDDEN(void RT_UNTRUSTED_VOLATILE_HOST *) VBoxHGSMIBufferAlloc(PHGSMIGUESTCOMMANDCONTEXT pCtx, HGSMISIZE cbData,
    uint8_t u8Ch, uint16_t u16Op)
{
    return NULL;
}

DECLHIDDEN(void) VBoxHGSMIBufferFree(PHGSMIGUESTCOMMANDCONTEXT pCtx, void RT_UNTRUSTED_VOLATILE_HOST *pvBuffer)
{

}

void HGSMIHeapFree(HGSMIHEAP *pHeap, void RT_UNTRUSTED_VOLATILE_HSTGST *pvData)
{

}

void RT_UNTRUSTED_VOLATILE_HOST *HGSMIHeapAlloc(HGSMIHEAP *pHeap,
    HGSMISIZE cbData,
    uint8_t u8Channel,
    uint16_t u16ChannelInfo)
{
    return NULL;
}


void HGSMIHeapBufferFree(HGSMIHEAP *pHeap, void RT_UNTRUSTED_VOLATILE_HSTGST *pvBuf)
{

}

DECLHIDDEN(int)  VBoxHGSMIBufferSubmit(PHGSMIGUESTCOMMANDCONTEXT pCtx, void RT_UNTRUSTED_VOLATILE_HOST *pvBuffer)
{
    return -1;
}

DECLHIDDEN(int)      VBoxHGSMISetupGuestContext(PHGSMIGUESTCOMMANDCONTEXT pCtx,
    void *pvGuestHeapMemory,
    uint32_t cbGuestHeapMemory,
    uint32_t offVRAMGuestHeapMemory,
    const HGSMIENV *pEnv)
{
    return -1;
}

DECLHIDDEN(void)     VBoxHGSMISetupHostContext(PHGSMIHOSTCOMMANDCONTEXT pCtx,
    void *pvBaseMapping,
    uint32_t offHostFlags,
    void *pvHostAreaMapping,
    uint32_t offVRAMHostArea,
    uint32_t cbHostArea)
{

}
