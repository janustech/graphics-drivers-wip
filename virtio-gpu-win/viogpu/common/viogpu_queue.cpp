#include "helper.h"
#include "baseobj.h"

static BOOLEAN BuildSGElement(VirtIOBufferDescriptor* sg, PVOID buf, ULONG size)
{
    if (size != 0 && MmIsAddressValid(buf))
    {
        sg->length = min(size, PAGE_SIZE);
        sg->physAddr = MmGetPhysicalAddress(buf);
        return TRUE;
    }
    return FALSE;
}


VioGpuQueue::VioGpuQueue()
{
    KeInitializeSpinLock(&m_SpinLock);
}

VioGpuQueue::~VioGpuQueue()
{
    Close();
}

void VioGpuQueue::Lock(KIRQL* Irql)
{
    PAGED_CODE();

    DbgPrint(TRACE_LEVEL_VERBOSE, ("---> %s\n", __FUNCTION__));

    KIRQL SavedIrql;
    KeAcquireSpinLock(&m_SpinLock, &SavedIrql);
    *Irql = SavedIrql;

    DbgPrint(TRACE_LEVEL_VERBOSE, ("<--- %s\n", __FUNCTION__));
}

void VioGpuQueue::Unlock(KIRQL Irql)
{
    PAGED_CODE();

    DbgPrint(TRACE_LEVEL_VERBOSE, ("---> %s\n", __FUNCTION__));

    KeReleaseSpinLock(&m_SpinLock, Irql);

    DbgPrint(TRACE_LEVEL_VERBOSE, ("<--- %s\n", __FUNCTION__));
}

void VioGpuQueue::Close(void)
{
    m_pVirtQueue = NULL;
}

UINT VioGpuQueue::QueryAllocation()
{
    PAGED_CODE();

    DbgPrint(TRACE_LEVEL_VERBOSE, ("---> %s\n", __FUNCTION__));

    USHORT NumEntries;
    ULONG RingSize, HeapSize;

    NTSTATUS status = virtio_query_queue_allocation(
        m_pVIODevice,
        m_Index,
        &NumEntries,
        &RingSize,
        &HeapSize);
    if (!NT_SUCCESS(status))
    {
        DbgPrint(TRACE_LEVEL_FATAL, ("[%s] virtio_query_queue_allocation(%d) failed with error %x\n", __FUNCTION__, m_Index, status));
        return 0;
    }

    DbgPrint(TRACE_LEVEL_VERBOSE, ("<--- %s\n", __FUNCTION__));

    return NumEntries;
}


//    void AllocCmd();
//    void AllocCmdResp();

PVOID CtrlQueue::AllocCmd(PGPU_VBUFFER* buf, int sz)
{
    PAGED_CODE();

    DbgPrint(TRACE_LEVEL_VERBOSE, ("---> %s\n", __FUNCTION__));

    PGPU_VBUFFER vbuf;
    vbuf = m_pBuf->GetBuf(sz, sizeof(GPU_CTRL_HDR), NULL);
    ASSERT(vbuf);
    *buf = vbuf;

    DbgPrint(TRACE_LEVEL_VERBOSE, ("<--- %s  vbuf = %p\n", __FUNCTION__, vbuf));

    return vbuf ? vbuf->buf : NULL;
}

PVOID CtrlQueue::AllocCmdResp(PGPU_VBUFFER* buf, int cmd_sz, PVOID resp_buf, int resp_sz)
{
    PAGED_CODE();

    DbgPrint(TRACE_LEVEL_VERBOSE, ("---> %s\n", __FUNCTION__));

    PGPU_VBUFFER vbuf;
    vbuf = m_pBuf->GetBuf(cmd_sz, resp_sz, resp_buf);
    ASSERT(vbuf);
    *buf = vbuf;

    DbgPrint(TRACE_LEVEL_VERBOSE, ("<--- %s\n", __FUNCTION__));

    return vbuf ? vbuf->buf : NULL;
}

BOOLEAN CtrlQueue::GetDisplayInfo(PGPU_VBUFFER buf, UINT id, PULONG xres, PULONG yres)
{
    PAGED_CODE();

    DbgPrint(TRACE_LEVEL_VERBOSE, ("---> %s\n", __FUNCTION__));

    PGPU_RESP_DISP_INFO resp = (PGPU_RESP_DISP_INFO)buf->resp_buf;
    if (resp->hdr.type != VIRTIO_GPU_RESP_OK_DISPLAY_INFO)
    {
        DbgPrint(TRACE_LEVEL_VERBOSE, (" %s type = %x: disabled\n", __FUNCTION__, resp->hdr.type));
        return FALSE;
    }
    if (resp->pmodes[id].enabled) {
        DbgPrint(TRACE_LEVEL_VERBOSE, ("output %d: %dx%d+%d+%d\n", id,
              resp->pmodes[id].r.width,
              resp->pmodes[id].r.height,
              resp->pmodes[id].r.x,
              resp->pmodes[id].r.y));
        *xres = resp->pmodes[id].r.width;
        *yres = resp->pmodes[id].r.height;
    } else {
        DbgPrint(TRACE_LEVEL_VERBOSE, ("output %d: disabled\n", id));
        return FALSE;
    }

    DbgPrint(TRACE_LEVEL_VERBOSE, ("<--- %s\n", __FUNCTION__));

    return TRUE;
}

BOOLEAN CtrlQueue::AskDisplayInfo(PGPU_VBUFFER* buf)
{
    PAGED_CODE();

    DbgPrint(TRACE_LEVEL_VERBOSE, ("---> %s\n", __FUNCTION__));

    PGPU_CTRL_HDR cmd;
    PGPU_VBUFFER vbuf;
    PGPU_RESP_DISP_INFO resp_buf;
    KEVENT   event;
    NTSTATUS status;

    resp_buf = reinterpret_cast<PGPU_RESP_DISP_INFO>
        (new (NonPagedPoolNx) BYTE[sizeof(GPU_RESP_DISP_INFO)]);

    if (!resp_buf)
    {
        DbgPrint(TRACE_LEVEL_ERROR, ("---> %s Failed allocate %d bytes\n", __FUNCTION__, sizeof(GPU_RESP_DISP_INFO)));
        return FALSE;
    }

    cmd = (PGPU_CTRL_HDR)AllocCmdResp(&vbuf, sizeof(GPU_CTRL_HDR), resp_buf, sizeof(GPU_RESP_DISP_INFO));
    RtlZeroMemory(cmd, sizeof(GPU_CTRL_HDR));

//	vgdev->display_info_pending = true;
    cmd->type = VIRTIO_GPU_CMD_GET_DISPLAY_INFO;

    KeInitializeEvent(&event, NotificationEvent, FALSE);
    vbuf->event = &event;

//FIXME if 
    QueueBuffer(vbuf);     
    status = KeWaitForSingleObject(&event,
        Executive,
        KernelMode,
        FALSE,
        NULL
    );
    ASSERT(NT_SUCCESS(status));
    *buf = vbuf;

    DbgPrint(TRACE_LEVEL_VERBOSE, ("<--- %s\n", __FUNCTION__));

    return TRUE;
}

UINT CtrlQueue::QueueBufferLocked(PGPU_VBUFFER buf)
{
    PAGED_CODE();

    DbgPrint(TRACE_LEVEL_VERBOSE, ("---> %s\n", __FUNCTION__));

    VirtIOBufferDescriptor  sg[16];
    int outcnt = 0, incnt = 0;
    UINT ret = 0;

    ASSERT(buf->size <= PAGE_SIZE);
    if (BuildSGElement(&sg[outcnt + incnt], (PVOID)buf->buf, buf->size))
    {
        outcnt++;
    }

    if (buf->data_size)
    {
        ULONG data_size = buf->data_size;
        PVOID data_buf = (PVOID)buf->data_buf;
        while (data_size)
        {
            if (BuildSGElement(&sg[outcnt + incnt], data_buf, data_size))
            {
                data_buf = (PVOID)((LONG_PTR)(data_buf) + PAGE_SIZE);
                data_size -= min(data_size, PAGE_SIZE);
                outcnt++;
            }
        }
    }

    ASSERT(buf->resp_size <= PAGE_SIZE);
    if (buf->resp_size)
    {
        if (BuildSGElement(&sg[outcnt + incnt], (PVOID)buf->resp_buf, buf->resp_size))
        {
            incnt++;
        }
    }

    ASSERT(outcnt);
    ASSERT((outcnt + incnt) <= 16);
    ret = AddBuf(&sg[0], outcnt, incnt, buf, NULL, 0);
    Kick();
    if (!ret)
    {
        ret = NumFree();
    }

    DbgPrint(TRACE_LEVEL_VERBOSE, ("<--- %s ret = %d\n", __FUNCTION__, ret));

    return ret;
}

UINT CtrlQueue::QueueBuffer(PGPU_VBUFFER buf)
{
    PAGED_CODE();

    DbgPrint(TRACE_LEVEL_VERBOSE, ("---> %s\n", __FUNCTION__));

    UINT res = 0;
    KIRQL SavedIrql;
    Lock(&SavedIrql);
    res = QueueBufferLocked(buf);
    Unlock(SavedIrql);

    DbgPrint(TRACE_LEVEL_VERBOSE, ("<--- %s\n", __FUNCTION__));

    return res;
}

PGPU_VBUFFER CtrlQueue::DequeueBufferLocked(_Out_ PUINT len)
{
    DbgPrint(TRACE_LEVEL_VERBOSE, ("---> %s\n", __FUNCTION__));
    PGPU_VBUFFER buf = NULL;
    buf = (PGPU_VBUFFER)GetBuf(len);
    DbgPrint(TRACE_LEVEL_VERBOSE, ("<--- %s buf %p len = %d\n", __FUNCTION__, buf, len));
    return buf;
}

PGPU_VBUFFER CtrlQueue::DequeueBuffer(_Out_ UINT* len)
{
    DbgPrint(TRACE_LEVEL_VERBOSE, ("---> %s\n", __FUNCTION__));

    PGPU_VBUFFER buf = NULL;
    KIRQL SavedIrql;
    Lock(&SavedIrql);
    buf = DequeueBufferLocked(len);
    Unlock(SavedIrql);
    if (buf == NULL)
    {
        *len = 0;
    }
    DbgPrint(TRACE_LEVEL_VERBOSE, ("<--- %s\n", __FUNCTION__));

    return buf;
}


void VioGpuQueue::ReleaseBuffer(PGPU_VBUFFER buf)
{
    PAGED_CODE();

    DbgPrint(TRACE_LEVEL_VERBOSE, ("---> %s\n", __FUNCTION__));

    m_pBuf->FreeBuf(buf);

    DbgPrint(TRACE_LEVEL_VERBOSE, ("<--- %s\n", __FUNCTION__));
}

void CtrlQueue::CreateResource(UINT res_id, UINT format, UINT width, UINT height)
{
    PAGED_CODE();

    DbgPrint(TRACE_LEVEL_VERBOSE, ("---> %s\n", __FUNCTION__));

    PGPU_RES_CREATE_2D cmd;
    PGPU_VBUFFER vbuf;
    cmd = (PGPU_RES_CREATE_2D)AllocCmd(&vbuf, sizeof(*cmd));
    RtlZeroMemory(cmd, sizeof(*cmd));

    cmd->hdr.type = VIRTIO_GPU_CMD_RESOURCE_CREATE_2D;
    cmd->resource_id = res_id;
    cmd->format = format;
    cmd->width = width;
    cmd->height = height;

//FIXME if 
    QueueBuffer(vbuf);

    DbgPrint(TRACE_LEVEL_VERBOSE, ("<--- %s\n", __FUNCTION__));
}

void CtrlQueue::UnrefResource(UINT res_id)
{
    PAGED_CODE();

    DbgPrint(TRACE_LEVEL_VERBOSE, ("---> %s\n", __FUNCTION__));

    PGPU_RES_UNREF cmd;
    PGPU_VBUFFER vbuf;
    cmd = (PGPU_RES_UNREF)AllocCmd(&vbuf, sizeof(*cmd));
    RtlZeroMemory(cmd, sizeof(*cmd));

    cmd->hdr.type = VIRTIO_GPU_CMD_RESOURCE_UNREF;
    cmd->resource_id = res_id;

    QueueBuffer(vbuf);

    DbgPrint(TRACE_LEVEL_VERBOSE, ("<--- %s\n", __FUNCTION__));
}

void CtrlQueue::InvalBacking(UINT res_id)
{
    PAGED_CODE();

    DbgPrint(TRACE_LEVEL_VERBOSE, ("---> %s\n", __FUNCTION__));

    PGPU_RES_DETACH_BACKING cmd;
    PGPU_VBUFFER vbuf;
    cmd = (PGPU_RES_DETACH_BACKING)AllocCmd(&vbuf, sizeof(*cmd));
    RtlZeroMemory(cmd, sizeof(*cmd));

    cmd->hdr.type = VIRTIO_GPU_CMD_RESOURCE_DETACH_BACKING;
    cmd->resource_id = res_id;

    QueueBuffer(vbuf);

    DbgPrint(TRACE_LEVEL_VERBOSE, ("<--- %s\n", __FUNCTION__));
}

void CtrlQueue::SetScanout(UINT scan_id, UINT res_id, UINT width, UINT height, UINT x, UINT y)
{
    PAGED_CODE();

    DbgPrint(TRACE_LEVEL_VERBOSE, ("---> %s\n", __FUNCTION__));

    PGPU_SET_SCANOUT cmd;
    PGPU_VBUFFER vbuf;
    cmd = (PGPU_SET_SCANOUT)AllocCmd(&vbuf, sizeof(*cmd));
    RtlZeroMemory(cmd, sizeof(*cmd));

    cmd->hdr.type = VIRTIO_GPU_CMD_SET_SCANOUT;
    cmd->resource_id = res_id;
    cmd->scanout_id = scan_id;
    cmd->r.width = width;
    cmd->r.height = height;
    cmd->r.x = x;
    cmd->r.y = y;

//FIXME if 
    QueueBuffer(vbuf);

    DbgPrint(TRACE_LEVEL_VERBOSE, ("<--- %s\n", __FUNCTION__));
}

void CtrlQueue::ResFlush(UINT res_id, UINT width, UINT height, UINT x, UINT y)
{
    PAGED_CODE();

    DbgPrint(TRACE_LEVEL_VERBOSE, ("---> %s\n", __FUNCTION__));
    PGPU_RES_FLUSH cmd;
    PGPU_VBUFFER vbuf;
    cmd = (PGPU_RES_FLUSH)AllocCmd(&vbuf, sizeof(*cmd));
    RtlZeroMemory(cmd, sizeof(*cmd));

    cmd->hdr.type = VIRTIO_GPU_CMD_RESOURCE_FLUSH;
    cmd->resource_id = res_id;
    cmd->r.width = width;
    cmd->r.height = height;
    cmd->r.x = x;
    cmd->r.y = y;

    QueueBuffer(vbuf);

    DbgPrint(TRACE_LEVEL_VERBOSE, ("<--- %s\n", __FUNCTION__));
}

void CtrlQueue::TransferToHost2D(UINT res_id, ULONG offset, UINT width, UINT height, UINT x, UINT y)
{
    PAGED_CODE();

    DbgPrint(TRACE_LEVEL_VERBOSE, ("---> %s\n", __FUNCTION__));
    PGPU_RES_TRANSF_TO_HOST_2D cmd;
    PGPU_VBUFFER vbuf;
    cmd = (PGPU_RES_TRANSF_TO_HOST_2D)AllocCmd(&vbuf, sizeof(*cmd));
    RtlZeroMemory(cmd, sizeof(*cmd));

    cmd->hdr.type = VIRTIO_GPU_CMD_TRANSFER_TO_HOST_2D;
    cmd->resource_id = res_id;
    cmd->offset = offset;
    cmd->r.width = width;
    cmd->r.height = height;
    cmd->r.x = x;
    cmd->r.y = y;

//FIXME add fence 
    QueueBuffer(vbuf);

    DbgPrint(TRACE_LEVEL_VERBOSE, ("<--- %s\n", __FUNCTION__));
}

void CtrlQueue::AttachBacking(UINT res_id, PGPU_MEM_ENTRY ents, UINT nents)
{
    PAGED_CODE();

    DbgPrint(TRACE_LEVEL_VERBOSE, ("---> %s\n", __FUNCTION__));

    PGPU_RES_ATTACH_BACKING cmd;
    PGPU_VBUFFER vbuf;
    cmd = (PGPU_RES_ATTACH_BACKING)AllocCmd(&vbuf, sizeof(*cmd));
    RtlZeroMemory(cmd, sizeof(*cmd));

    cmd->hdr.type = VIRTIO_GPU_CMD_RESOURCE_ATTACH_BACKING;
    cmd->resource_id = res_id;
    cmd->nr_entries = nents;

    vbuf->data_buf = ents;
    vbuf->data_size = sizeof(*ents) * nents;

//FIXME add fence 
    QueueBuffer(vbuf);

    DbgPrint(TRACE_LEVEL_VERBOSE, ("<--- %s\n", __FUNCTION__));
}


VioGpuBuf::VioGpuBuf()
{
    PAGED_CODE();

    DbgPrint(TRACE_LEVEL_VERBOSE, ("---> %s\n", __FUNCTION__));

    m_uCount = 0;

    DbgPrint(TRACE_LEVEL_VERBOSE, ("<--- %s\n", __FUNCTION__));
}

VioGpuBuf::~VioGpuBuf()
{
    PAGED_CODE();

    DbgPrint(TRACE_LEVEL_VERBOSE, ("---> %s\n", __FUNCTION__));

    Close();

    DbgPrint(TRACE_LEVEL_VERBOSE, ("<--- %s\n", __FUNCTION__));
}

BOOLEAN VioGpuBuf::Init(_In_ UINT cnt)
{
    PAGED_CODE();

    DbgPrint(TRACE_LEVEL_VERBOSE, ("---> %s\n", __FUNCTION__));

    InitializeListHead(&m_FreeBufs);
    KeInitializeSpinLock(&m_SpinLock);

    for (UINT i = 0; i < cnt; ++i) {
        PGPU_VBUFFER pvbuf = reinterpret_cast<PGPU_VBUFFER>
                                (new (NonPagedPoolNx) BYTE[VBUFFER_SIZE]);

        if (pvbuf)
        {
            ExInterlockedInsertTailList(&m_FreeBufs, &pvbuf->list_entry, &m_SpinLock);
            ++m_uCount;
        }
    }
    ASSERT(m_uCount == cnt);

    DbgPrint(TRACE_LEVEL_VERBOSE, ("<--- %s\n", __FUNCTION__));

    return (m_uCount > 0);
}
    
void VioGpuBuf::Close(void)
{
    PAGED_CODE();

    DbgPrint(TRACE_LEVEL_VERBOSE, ("---> %s\n", __FUNCTION__));

    while(!IsListEmpty(&m_FreeBufs))
    {
        LIST_ENTRY* pListItem = ExInterlockedRemoveHeadList(&m_FreeBufs,
                                                 &m_SpinLock);
        if (pListItem)
        {
            PGPU_VBUFFER pvbuf = CONTAINING_RECORD(pListItem, GPU_VBUFFER, list_entry);
            ASSERT(pvbuf);
            ASSERT(pvbuf->resp_size <= MAX_INLINE_RESP_SIZE);

            delete [] reinterpret_cast<PBYTE>(pvbuf);
            --m_uCount;
        }
    }

    ASSERT(m_uCount == 0);

    DbgPrint(TRACE_LEVEL_VERBOSE, ("<--- %s\n", __FUNCTION__));
}

PGPU_VBUFFER VioGpuBuf::GetBuf(
        _In_ int size,
        _In_ int resp_size,
        _In_ void *resp_buf)
{
    PAGED_CODE();

    DbgPrint(TRACE_LEVEL_VERBOSE, ("---> %s\n", __FUNCTION__));

    PGPU_VBUFFER pbuf = NULL;
    PLIST_ENTRY pListItem = NULL;
    ASSERT(IsListEmpty(&m_FreeBufs));
    
    pListItem = ExInterlockedRemoveHeadList(&m_FreeBufs, &m_SpinLock);
    if (pListItem)
    {
        pbuf = CONTAINING_RECORD(pListItem, GPU_VBUFFER, list_entry);
        ASSERT(pvbuf);
        memset(pbuf, 0, VBUFFER_SIZE);
        ASSERT(size > MAX_INLINE_CMD_SIZE);

        pbuf->buf = (char *)((ULONG_PTR)pbuf + sizeof(*pbuf));
        pbuf->size = size;

        pbuf->resp_size = resp_size;
        if (resp_size <= MAX_INLINE_RESP_SIZE)
        {
            pbuf->resp_buf = (char *)((ULONG_PTR)pbuf->buf + size);
        }
        else
        {
            pbuf->resp_buf = (char *)resp_buf;
        }
        ASSERT(vbuf->resp_buf);
    }

    DbgPrint(TRACE_LEVEL_VERBOSE, ("<--- %s buf = %p\n", __FUNCTION__, pbuf));

    return pbuf;
}

void VioGpuBuf::FreeBuf(
        _In_ PGPU_VBUFFER pbuf)
{
    PAGED_CODE();

    DbgPrint(TRACE_LEVEL_VERBOSE, ("---> %s buf = %p\n", __FUNCTION__, pbuf));

    if (pbuf->resp_buf && pbuf->resp_size > MAX_INLINE_RESP_SIZE)
    {
        delete [] reinterpret_cast<PBYTE>(pbuf->resp_buf);
        pbuf->resp_buf = NULL;
        pbuf->resp_buf = 0;
    }

    if (pbuf->data_buf && pbuf->data_size)
    {
        delete [] reinterpret_cast<PBYTE>(pbuf->data_buf);
        pbuf->data_buf = NULL;
        pbuf->data_size = 0;
    }
    ExInterlockedInsertTailList(&m_FreeBufs, &pbuf->list_entry, &m_SpinLock);

    DbgPrint(TRACE_LEVEL_VERBOSE, ("<--- %s\n", __FUNCTION__));
}

VioGpuObj::VioGpuObj(void)
{
    PAGED_CODE();

    DbgPrint(TRACE_LEVEL_VERBOSE, ("---> %s\n", __FUNCTION__));

    m_uiHwRes = 0;
    m_pSGList = NULL;
    m_pVAddr = NULL;
    m_pMdl = NULL;
    m_bDumb = TRUE;

    DbgPrint(TRACE_LEVEL_VERBOSE, ("<--- %s\n", __FUNCTION__));
}

VioGpuObj::~VioGpuObj(void)
{
    PAGED_CODE();

    DbgPrint(TRACE_LEVEL_VERBOSE, ("---> %s\n", __FUNCTION__));

    Close();

    DbgPrint(TRACE_LEVEL_VERBOSE, ("<--- %s\n", __FUNCTION__));
}

BOOLEAN VioGpuObj::Init(_In_ UINT size)
{
    PAGED_CODE();

    DbgPrint(TRACE_LEVEL_VERBOSE, ("---> %s\n", __FUNCTION__));

    ASSERT(size);
    UINT pages = BYTES_TO_PAGES(size);
    UINT sglsize = sizeof(SCATTER_GATHER_LIST) + (sizeof(SCATTER_GATHER_ELEMENT) * pages);
    size = pages * PAGE_SIZE;
    m_pVAddr = new (NonPagedPoolNx) BYTE[size];
    PVOID buf = PAGE_ALIGN(m_pVAddr);
    if(!m_pVAddr)
    {
        DbgPrint(TRACE_LEVEL_FATAL, ("%s insufficient resources to allocate %x bytes\n", __FUNCTION__, size));
        return FALSE;
    }
    m_pMdl = IoAllocateMdl(m_pVAddr, size,  FALSE, FALSE, NULL);
    if(!m_pMdl)
    {
        DbgPrint(TRACE_LEVEL_FATAL, ("%s insufficient resources to allocate MDLs\n", __FUNCTION__));
        return FALSE;
    }
    __try
    {
        // Probe and lock the pages of this buffer in physical memory.
        // We need only IoReadAccess.
        MmProbeAndLockPages(m_pMdl, KernelMode, IoWriteAccess);
    }
    #pragma prefast(suppress: __WARNING_EXCEPTIONEXECUTEHANDLER, "try/except is only able to protect against user-mode errors and these are the only errors we try to catch here");
    __except(EXCEPTION_EXECUTE_HANDLER)
    {
        DbgPrint(TRACE_LEVEL_FATAL, ("%s Failed to lock pages with error %x\n", __FUNCTION__, GetExceptionCode()));
        IoFreeMdl(m_pMdl);
        return FALSE;
    }
    m_pSGList = reinterpret_cast<PSCATTER_GATHER_LIST>
        (new (NonPagedPoolNx) BYTE[sglsize]);
    m_pSGList->NumberOfElements = 0;
    m_pSGList->Reserved = 0;
//       m_pSAddr = reinterpret_cast<BYTE*>
//    (MmGetSystemAddressForMdlSafe(m_pMdl, NormalPagePriority | MdlMappingNoExecute));

    RtlZeroMemory(m_pSGList, sglsize);

    for (UINT i = 0; i < pages; ++i)
    {
        PHYSICAL_ADDRESS pa = {0};
        ASSERT(MmIsAddressValid(buf));
        pa = MmGetPhysicalAddress(buf);
        if (pa.QuadPart == 0LL)
        {
            DbgPrint(TRACE_LEVEL_FATAL, ("%s Invalid PA buf = %p element %d\n", __FUNCTION__, buf, i));
            break;
        }
        m_pSGList->Elements[i].Address = pa;
        m_pSGList->Elements[i].Length = PAGE_SIZE;
        buf = (PVOID)((LONG_PTR)(buf) + PAGE_SIZE);
        m_pSGList->NumberOfElements++;
    }

    DbgPrint(TRACE_LEVEL_VERBOSE, ("<--- %s\n", __FUNCTION__));

    return TRUE;
}

PHYSICAL_ADDRESS VioGpuObj::GetPhysicalAddress(void)
{
    PAGED_CODE();

    DbgPrint(TRACE_LEVEL_VERBOSE, ("---> %s\n", __FUNCTION__));

    PHYSICAL_ADDRESS pa = { 0 };
    if (m_pVAddr && MmIsAddressValid(m_pVAddr))
    {
        pa = MmGetPhysicalAddress(m_pVAddr);
    }

    DbgPrint(TRACE_LEVEL_VERBOSE, ("<--- %s\n", __FUNCTION__));

    return pa;
}

void VioGpuObj::Close(void)
{
    PAGED_CODE();

    DbgPrint(TRACE_LEVEL_VERBOSE, ("---> %s\n", __FUNCTION__));

    if (m_pMdl)
    {
        MmUnlockPages(m_pMdl);
        IoFreeMdl(m_pMdl);
        m_pMdl = NULL;
    }

    delete [] m_pVAddr;
    m_pVAddr = NULL;

    delete [] reinterpret_cast<PBYTE>(m_pSGList);
    m_pSGList = NULL;

    DbgPrint(TRACE_LEVEL_VERBOSE, ("<--- %s\n", __FUNCTION__));
}

PVOID CrsrQueue::AllocCursor(PGPU_VBUFFER* buf)
{
    PAGED_CODE();

    DbgPrint(TRACE_LEVEL_VERBOSE, ("---> %s\n", __FUNCTION__));

    PGPU_VBUFFER vbuf;
    vbuf = m_pBuf->GetBuf(sizeof(GPU_UPDATE_CURSOR), 0, NULL);
    ASSERT(vbuf);
    *buf = vbuf;

    DbgPrint(TRACE_LEVEL_VERBOSE, ("<--- %s  vbuf = %p\n", __FUNCTION__, vbuf));

    return vbuf ? vbuf->buf : NULL;
}

UINT CrsrQueue::QueueCursor(PGPU_VBUFFER buf)
{
    PAGED_CODE();

    DbgPrint(TRACE_LEVEL_VERBOSE, ("---> %s\n", __FUNCTION__));

    UINT res = 0;
    KIRQL SavedIrql;
    Lock(&SavedIrql);

    VirtIOBufferDescriptor  sg[1];
    int outcnt = 0;
    UINT ret = 0;

    ASSERT(buf->size <= PAGE_SIZE);
    if (BuildSGElement(&sg[outcnt], (PVOID)buf->buf, buf->size))
    {
        outcnt++;
    }

    ASSERT(outcnt);
    ret = AddBuf(&sg[0], outcnt, 0, buf, NULL, 0);
    Kick();
    if (!ret)
    {
        ret = NumFree();
    }

    Unlock(SavedIrql);
    DbgPrint(TRACE_LEVEL_VERBOSE, ("<--- %s vbuf = %p outcnt = %d, ret = %d\n", __FUNCTION__, buf, outcnt, ret));
    return res;
}

PGPU_VBUFFER CrsrQueue::DequeueCursor(_Out_ UINT* len)
{
    DbgPrint(TRACE_LEVEL_VERBOSE, ("---> %s\n", __FUNCTION__));

    PGPU_VBUFFER buf = NULL;
    KIRQL SavedIrql;
    Lock(&SavedIrql);
    buf = (PGPU_VBUFFER)GetBuf(len);
    Unlock(SavedIrql);
    if (buf == NULL)
    {
        *len = 0;
    }
    DbgPrint(TRACE_LEVEL_VERBOSE, ("<--- %s buf %p len = %u\n", __FUNCTION__, buf, len));
    return buf;
}
