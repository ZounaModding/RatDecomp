#include "ARAM_Z.h"
#include "Assert_Z.h"
#include "GCMain_Z.h"
#include <ar.h>
#include <os/OSCache.h>

HoleArray_Z<U8, 256> AramManager_Z::ReqTab;

void ARQCbRead(U32 i_RequestAddress) {
    ARQRequest* l_Req = ((ARQRequest*)i_RequestAddress);
    AramManager_Z::ReqTab.Remove(l_Req->owner);
}

static void ARQCbWrite(U32 i_RequestAddress) {
    ARQRequest* l_Req = ((ARQRequest*)i_RequestAddress);
    AramManager_Z::ReqTab.Remove(l_Req->owner);
}

Bool AramManager_Z::Init() {
    ARInit(NULL, 0);
    ARQInit();

    m_ARamAllocator.Init(ARGetBaseAddress(), 15, FALSE);
    S16 l_BlockIdx = m_ARamAllocator.AllocBlock(ARAM_MGR_BLOCK_COUNT, -1, -1);
    U32 l_BlockAddress = m_ARamAllocator.GetBlockPtr(l_BlockIdx);
    U8* l_ZeroBuffer = (U8*)AllocAlignL_Z(ARAM_MGR_BLOCK_COUNT, 30, ARAM_MGR_ALIGNMENT);

    memset(l_ZeroBuffer, 0, ARAM_MGR_BLOCK_COUNT);
    ARStartDMA(ARAM_DIR_MRAM_TO_ARAM, (U32)l_ZeroBuffer, l_BlockAddress, ARAM_MGR_BLOCK_COUNT);
    Flush();
    Free_Z(l_ZeroBuffer);
    return TRUE;
}

void AramManager_Z::Shut() {
    for (S32 i = 0; i < ReqTab.GetSize(); i++) {
        ReqTab.Remove(i);
    }
    ReqTab.Minimize();
}

void AramManager_Z::Flush() {
    ARQFlushQueue();
}

void AramManager_Z::Minimize() {
    S16 l_BlockIdx = m_ARamAllocator.GetNextBlock(-1);
    while (l_BlockIdx >= 0) {
        S32 l_BlockIdxLong = l_BlockIdx;
        S32 l_Handle = m_ARamAllocator.GetBlockOwnerKey(l_BlockIdxLong);
        S32 l_LastUsedFrame = m_ARamAllocator.GetBlockDate(l_BlockIdxLong);
        if (l_Handle != -1 && l_LastUsedFrame != -1 && (S32)(gData.m_FrameCount - l_LastUsedFrame) > 1 && gData.ClassMgr->RemovedXRamResource(l_Handle, 4)) {
            m_ARamAllocator.SetBlockDate(l_BlockIdxLong, -1);
        }
        l_BlockIdx = m_ARamAllocator.GetNextBlock(l_BlockIdx);
    }
}

void AramManager_Z::Read(U8* i_Destination, S32 i_Source, U32 i_Size, bool i_Async) {
    ASSERTLEF_Z(IsAligned_Z(i_Destination, ARAM_MGR_ALIGNMENT), "AramManager_Z::Read Dest not aligned", 88, "ARAM_Z.cpp", "IS_ALIGN_Z(To,32)");
    ASSERTLEF_Z(IsAligned_Z(i_Source, ARAM_MGR_ALIGNMENT), "AramManager_Z::Read Src not aligned", 89, "ARAM_Z.cpp", "IS_ALIGN_Z(FromRamAddr,32)");
    ASSERTLEF_Z(IsAligned_Z(i_Size, ARAM_MGR_ALIGNMENT), "AramManager_Z::Read Size not aligned", 90, "ARAM_Z.cpp", "IS_ALIGN_Z(Size,32)");

    DCInvalidateRange(i_Destination, i_Size);

    ARQRequest l_Request;
    S32 l_RequestId = ReqTab.Add((U8)1);
    ARQPostRequest(&l_Request, l_RequestId, ARQ_TYPE_ARAM_TO_MRAM, ARQ_PRIORITY_HIGH, i_Source, (U32)i_Destination, i_Size, ARQCbRead);

    if (!i_Async) {
        while (ReqTab.IsElement(l_RequestId)) {
        }
    }

    DCStoreRange(i_Destination, i_Size);
}

void AramManager_Z::Write(U8* i_Source, S32 i_Destination, U32 i_Size, bool i_Async) {
    ASSERTLEF_Z(IsAligned_Z(i_Destination, ARAM_MGR_ALIGNMENT), "AramManager_Z::Write Dest not aligned", 104, "ARAM_Z.cpp", "IS_ALIGN_Z(ToRamAddr,32)");

    U8* l_AlignedBuffer = NULL;
    U32 l_RemainingSize = AlignUp_Z(i_Size, ARAM_MGR_ALIGNMENT);
    if (!IsAligned_Z(i_Source, ARAM_MGR_ALIGNMENT)) {
        l_AlignedBuffer = (U8*)AllocAlignL_Z(32768, 112, ARAM_MGR_ALIGNMENT);
        while (l_RemainingSize != 0) {
            U32 l_TransferSize = Min<U32>(32768, l_RemainingSize);
            Sys_Z::MemCpyFrom(l_AlignedBuffer, i_Source, l_TransferSize);
            DCStoreRange(l_AlignedBuffer, l_TransferSize);

            ARQRequest l_Request;
            S32 l_RequestId = ReqTab.Add((U8)1);
            ARQPostRequest(&l_Request, 0, ARQ_TYPE_MRAM_TO_ARAM, ARQ_PRIORITY_HIGH, (U32)l_AlignedBuffer, i_Destination, l_TransferSize, ARQCbWrite);
            while (ReqTab.IsElement(l_RequestId)) {
            }

            l_RemainingSize -= l_TransferSize;
            i_Destination += l_TransferSize;
            i_Source += l_TransferSize;
        }
    }
    else {
        DCStoreRange(i_Source, l_RemainingSize);

        ARQRequest l_Request;
        S32 l_RequestId = ReqTab.Add((U8)1);
        ARQPostRequest(&l_Request, 0, ARQ_TYPE_MRAM_TO_ARAM, ARQ_PRIORITY_HIGH, (U32)i_Source, i_Destination, l_RemainingSize, ARQCbWrite);

        if (!i_Async) {
            while (ReqTab.IsElement(l_RequestId)) {
            }
        }
    }

    if (l_AlignedBuffer) {
        Free_Z(l_AlignedBuffer);
    }
}

U8* AramManager_Z::Get(S16 i_BlockIdx, S32 i_User) {
    U32 l_Size = GetBlockSize(i_BlockIdx);
    U8* l_Data = (U8*)AllocAlignCL_Z(l_Size, "AramManager_Z::Get", 146, ARAM_MGR_ALIGNMENT);
    if (!l_Data) {
        return NULL;
    }
    Read(l_Data, GetBlockPtr(i_BlockIdx), l_Size, FALSE);
    LockBlockAt(i_BlockIdx, i_User);
    return l_Data;
}

S16 AramManager_Z::AllocBlock(S32 i_Size, S32 i_UserParam1, S32 i_UserParam2) {
    return m_ARamAllocator.AllocBlock(i_Size, i_UserParam1, i_UserParam2);
}

void AramManager_Z::FreeBlock(S16 i_BlockIdx) {
    m_ARamAllocator.FreeBlock(i_BlockIdx);
}

void AramManager_Z::Update(Float i_DeltaTime) {
    S16 l_BlockIdx = m_ARamAllocator.GetNextBlock(-1);
    while (l_BlockIdx >= 0) {
        S32 l_BlockIdxLong = l_BlockIdx;
        S32 l_Handle = m_ARamAllocator.GetBlockOwnerKey(l_BlockIdxLong);
        S32 l_LastUsedFrame = m_ARamAllocator.GetBlockDate(l_BlockIdxLong);
        if (l_Handle != -1 && l_LastUsedFrame != -1 && (S32)(gData.m_FrameCount - l_LastUsedFrame) > 10 && gData.ClassMgr->RemovedXRamResource(l_Handle, 4)) {
            m_ARamAllocator.SetBlockDate(l_BlockIdxLong, -1);
        }
        l_BlockIdx = m_ARamAllocator.GetNextBlock(l_BlockIdx);
    }
}

void XRamManager_Z::EndGet(U8* i_Data) {
    Free_Z(i_Data);
}

template U32 Min<U32>(U32 i_V1, U32 i_V2);

void* AramManager_Alloc(S32 i_Size) {
    return NULL;
}

void AramManager_Free(void* i_Ptr) {
}
