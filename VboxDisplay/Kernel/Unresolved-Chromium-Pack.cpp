#include "cr_pack.h"
#include "cr_packfunctions.h"

DECLEXPORT(void) crPackInitBuffer(CRPackBuffer *buffer, void *buf, int size, int mtu
#ifdef IN_RING0
    , unsigned int num_opcodes
#endif
)
{

}

DECLEXPORT(void) crPackSetBuffer(CRPackContext *pc, CRPackBuffer *buffer)
{

}

DECLEXPORT(void) crPackReleaseBuffer(CRPackContext *pc)
{

}

void PACK_APIENTRY crPackWindowPosition(CR_PACKER_CONTEXT_ARGDECL GLint window, GLint x, GLint y)
{

}

void PACK_APIENTRY crPackWindowShow(CR_PACKER_CONTEXT_ARGDECL GLint window, GLint flag)
{

}

void PACK_APIENTRY crPackWindowSize(CR_PACKER_CONTEXT_ARGDECL GLint window, GLint w, GLint h)
{

}

void PACK_APIENTRY crPackWindowVisibleRegion(CR_PACKER_CONTEXT_ARGDECL GLint window, GLint cRects, const GLint * pRects)
{

}

void PACK_APIENTRY crPackVBoxTexPresent(CR_PACKER_CONTEXT_ARGDECL GLuint texture, GLuint cfg, GLint xPos, GLint yPos, GLint cRects, const GLint * pRects)
{

}

void PACK_APIENTRY crPackChromiumParameteriCR(CR_PACKER_CONTEXT_ARGDECL GLenum target, GLint value)
{

}
