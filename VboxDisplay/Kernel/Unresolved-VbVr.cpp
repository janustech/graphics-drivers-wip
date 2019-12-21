#include "cr_vreg.h"

VBOXVREGDECL(int) VBoxVrInit(void)
{
    return -1;
}

VBOXVREGDECL(void) VBoxVrTerm(void)
{

}

VBOXVREGDECL(int) VBoxVrListRectsAdd(PVBOXVR_LIST pList, uint32_t cRects, PCRTRECT paRects, bool *pfChanged)
{
    return -1;
}

VBOXVREGDECL(int) VBoxVrListRectsSubst(PVBOXVR_LIST pList, uint32_t cRects, PCRTRECT paRects, bool *pfChanged)
{
    return -1;
}

VBOXVREGDECL(int) VBoxVrListRectsGet(PVBOXVR_LIST pList, uint32_t cRects, PRTRECT paRects)
{
    return -1;
}

VBOXVREGDECL(void) VBoxVrListClear(PVBOXVR_LIST pList)
{

}
