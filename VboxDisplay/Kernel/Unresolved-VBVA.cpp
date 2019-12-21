#include "VBoxVideoGuest.h"

DECLHIDDEN(bool) VBoxVBVAEnable(PVBVABUFFERCONTEXT pCtx,
    PHGSMIGUESTCOMMANDCONTEXT pHGSMICtx,
    struct VBVABUFFER *pVBVA, int32_t cScreen)
{
    return false;
}

DECLHIDDEN(void) VBoxVBVADisable(PVBVABUFFERCONTEXT pCtx,
    PHGSMIGUESTCOMMANDCONTEXT pHGSMICtx,
    int32_t cScreen)
{

}

DECLHIDDEN(bool) VBoxVBVABufferBeginUpdate(PVBVABUFFERCONTEXT pCtx,
    PHGSMIGUESTCOMMANDCONTEXT pHGSMICtx)
{
    return false;
}

DECLHIDDEN(void) VBoxVBVABufferEndUpdate(PVBVABUFFERCONTEXT pCtx)
{

}

DECLHIDDEN(bool) VBoxVBVAWrite(PVBVABUFFERCONTEXT pCtx,
    PHGSMIGUESTCOMMANDCONTEXT pHGSMICtx,
    const void *pv, uint32_t cb)
{
    return false;
}

DECLHIDDEN(void) VBoxVBVASetupBufferContext(PVBVABUFFERCONTEXT pCtx,
    uint32_t offVRAMBuffer,
    uint32_t cbBuffer)
{

}

DECLHIDDEN(bool)     VBoxHGSMIIsSupported(void)
{
    return false;
}

DECLHIDDEN(void)     VBoxHGSMIGetBaseMappingInfo(uint32_t cbVRAM,
    uint32_t *poffVRAMBaseMapping,
    uint32_t *pcbMapping,
    uint32_t *poffGuestHeapMemory,
    uint32_t *pcbGuestHeapMemory,
    uint32_t *poffHostFlags)
{

}

DECLHIDDEN(uint32_t) VBoxHGSMIGetMonitorCount(PHGSMIGUESTCOMMANDCONTEXT pCtx)
{
    return 0;
}

DECLHIDDEN(uint16_t) VBoxHGSMIGetScreenFlags(PHGSMIGUESTCOMMANDCONTEXT pCtx)
{
    return 0;
}

DECLHIDDEN(bool)     VBoxVideoAnyWidthAllowed(void)
{
    return false;
}

DECLHIDDEN(bool)     VBoxVGACfgAvailable(void)
{
    return false;
}

DECLHIDDEN(bool)     VBoxVGACfgQuery(uint16_t u16Id, uint32_t *pu32Value, uint32_t u32DefValue)
{
    return false;
}

DECLHIDDEN(int)      VBoxHGSMIUpdatePointerShape(PHGSMIGUESTCOMMANDCONTEXT pCtx,
    uint32_t fFlags,
    uint32_t cHotX,
    uint32_t cHotY,
    uint32_t cWidth,
    uint32_t cHeight,
    uint8_t *pPixels,
    uint32_t cbLength)
{
    return -1;
}

DECLHIDDEN(void)     VBoxVideoSetModeRegisters(uint16_t cWidth, uint16_t cHeight,
    uint16_t cVirtWidth, uint16_t cBPP,
    uint16_t fFlags,
    uint16_t cx, uint16_t cy)
{

}

DECLHIDDEN(int)      VBoxHGSMISendHostCtxInfo(PHGSMIGUESTCOMMANDCONTEXT pCtx,
    HGSMIOFFSET offVRAMFlagsLocation,
    uint32_t fCaps,
    uint32_t offVRAMHostArea,
    uint32_t cbHostArea)
{
    return -1;
}

DECLHIDDEN(void)     VBoxHGSMIGetHostAreaMapping(PHGSMIGUESTCOMMANDCONTEXT pCtx,
    uint32_t cbVRAM,
    uint32_t offVRAMBaseMapping,
    uint32_t *poffVRAMHostArea,
    uint32_t *pcbHostArea)
{

}
