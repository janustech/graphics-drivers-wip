#include "VBox/VBoxGuestLib.h"

DECLR0VBGL(int)     VbglR0InitClient(void)
{
    return -1;
}

DECLR0VBGL(void)    VbglR0TerminateClient(void)
{

}

DECLR0VBGL(int) VbglR0GRAlloc(struct VMMDevRequestHeader **ppReq, size_t cbReq, VMMDevRequestType enmReqType)
{
    return -1;
}

DECLR0VBGL(void) VbglR0GRFree(struct VMMDevRequestHeader *pReq)
{

}

DECLR0VBGL(int) VbglR0GRPerform(struct VMMDevRequestHeader *pReq)
{
    return -1;
}

DECLR0VBGL(int) VbglR0CrCtlCreate(VBGLCRCTLHANDLE *phCtl)
{
    return -1;
}

DECLR0VBGL(int) VbglR0CrCtlDestroy(VBGLCRCTLHANDLE hCtl)
{
    return -1;
}

DECLR0VBGL(int) VbglR0CrCtlConConnect(VBGLCRCTLHANDLE hCtl, HGCMCLIENTID *pidClient)
{
    return -1;
}

DECLR0VBGL(int) VbglR0CrCtlConDisconnect(VBGLCRCTLHANDLE hCtl, HGCMCLIENTID idClient)
{
    return -1;
}

DECLR0VBGL(int) VbglR0CrCtlConCallRaw(VBGLCRCTLHANDLE hCtl, struct VBGLIOCHGCMCALL *pCallInfo, int cbCallInfo)
{
    return -1;
}

DECLR0VBGL(int) VbglR0CrCtlConCall(VBGLCRCTLHANDLE hCtl, struct VBGLIOCHGCMCALL *pCallInfo, int cbCallInfo)
{
    return -1;
}

DECLR0VBGL(int) VbglR0CrCtlConCallUserDataRaw(VBGLCRCTLHANDLE hCtl, struct VBGLIOCHGCMCALL *pCallInfo, int cbCallInfo)
{
    return -1;
}
