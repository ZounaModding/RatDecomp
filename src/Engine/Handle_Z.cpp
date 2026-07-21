#include "Handle_Z.h"
#include "Main_Z.h"
#include "Program_Z.h"
#include "XRamManager_Z.h"
#include "ClassManager_Z.h"
#include "World_Z.h"
#include "ABC_ScriptManager.h"
#include "AnimationManager_Z.h"
#include "SoundManager_Z.h"
#include "SystemDatas_Z.h"
#include "MaterialManager_Z.h"
#include "Movie_Z.h"
#include "GameManager_Z.h"
#include "ObjectBankManager_Z.h"
#include "ManipulatorManager_Z.h"
#include "ParticlesManager_Z.h"
#include "VolatileMem_Z.h"
#include "InputEngine_Z.h"
#include "Console_Z.h"
#include "Renderer_Z.h"
#include "Timer_Z.h"
#include "LowMemory_Z.h"

HandleStream_Z HandleManager_Z::HandleStream;
StrFileHeader_Z HandleStream_Z::gStrFileHeader;

HandleManager_Z::HandleManager_Z() {
    m_NbFree = 0;
    m_HandleRecDASize = 0;
    ExpandSize();
    m_UnkS32_0x44 = 0;
    m_UnkBool_SetsDeltaTimeTo30fps_0x34 = FALSE;
    m_DoAsynchCheckHandles = FALSE;
    m_LastDeleteFrameNb = -1;
    m_CheckHandlesQueued = FALSE;
    m_ForbidCheckHandles = FALSE;
}

const BaseObject_ZHdl& HandleManager_Z::CreateNewHandle(BaseObject_Z* i_ObjPtr, const Name_Z& i_Name, S16 i_ClassID, U8 i_Flag) {
    if (!m_NbFree) {
        ExpandSize();
    }

    int l_FreeRecID = m_FreeRecDA[--m_NbFree];

    HandleRec_Z& l_Rec = m_HandleRecDA[l_FreeRecID];
    l_Rec.m_ObjPtr = i_ObjPtr;
    l_Rec.m_Name = i_Name;
    l_Rec.m_xRamBlock = -1;
    l_Rec.m_ClassID = i_ClassID;
    l_Rec.m_Flag = i_Flag;
    l_Rec.m_Marked = HANDLE_MARKED_UNK;

    i_ObjPtr->SetHandle(l_FreeRecID, l_Rec.m_Key);
    i_ObjPtr->SetName(i_Name);

    i_ObjPtr->Init();
    AddResourceRef(l_Rec, l_FreeRecID);

    return i_ObjPtr->GetHandle();
}

void HandleManager_Z::DeleteHandle(const BaseObject_ZHdl& i_Hdl) {
    U32 l_ID = i_Hdl.GetID();
    HandleRec_Z& l_Rec = m_HandleRecDA[l_ID];

    if (l_Rec.m_xRamBlock != -1) {
        gData.XRamMgr->FreeBlock(l_Rec.m_xRamBlock);
    }
    l_Rec.m_xRamBlock = -1;

    RemoveResourceRef(l_Rec);

    l_Rec.m_Marked = HANDLE_MARKED_FALSE;
    l_Rec.m_ClassID = -1;

    Delete_Z l_Rec.m_ObjPtr;

    l_Rec.m_Name = m_NullName;
    l_Rec.m_Key++;
    if (l_Rec.m_Key == 0) {
        l_Rec.m_Key++;
    }

    l_Rec.m_LastKeyBeforeAsyncDelete = FALSE;
    l_Rec.m_Flag &= HandleRec_Z::KEEP;
    l_Rec.m_ObjPtr = NULL;

    m_FreeRecDA[m_NbFree++] = l_ID;
}

void HandleManager_Z::RemoveResourceRef(const HandleRec_Z& i_HandleRec) {
    if (i_HandleRec.m_Flag & HandleRec_Z::RSC) {
        S32Hash_Z l_HashElt(i_HandleRec.m_Name.GetID());
        Bool l_Result = m_HandleIdHT.Suppress(l_HashElt);
        ASSERTLE_Z(l_Result, "", 121, "bResult");
    }
}

void HandleManager_Z::AddResourceRef(const HandleRec_Z& i_HandleRec, S32 i_Index) {
    if (i_HandleRec.m_Flag & HandleRec_Z::RSC) {
        S32Hash_Z l_HashElt(i_HandleRec.m_Name.GetID(), i_Index);
        Bool l_Result = m_HandleIdHT.Insert(l_HashElt);
        ASSERTLE_Z(gData.m_UpdatingResource || l_Result, "", 131, "gData.UpdatingResource || bResult");
    }
}

S32 HandleManager_Z::IsResourceRef(S32 i_Hdl) {
    S32Hash_Z l_HashElt(i_Hdl);
    const S32Hash_Z* l_Result = m_HandleIdHT.Search(l_HashElt);
    if (l_Result) {
        return l_Result->m_Ref;
    }
    return -1;
}

S32 HandleManager_Z::ChangeHandleName(const BaseObject_ZHdl& i_Hdl, const Name_Z& i_Name) {
    S32 l_ID = i_Hdl.GetID();
    if (CheckKey(l_ID, i_Hdl.GetKey())) {
        HandleRec_Z& l_HandleRec = m_HandleRecDA[l_ID];
        ASSERTLE_Z(!(l_HandleRec.m_Flag & HandleRec_Z::RSC), "", 157, "!(HdlRec.Flag&HandleRec_Z::RSC)");
        l_HandleRec.m_Name = i_Name;
        return TRUE;
    }
    return FALSE;
}

S32 HandleManager_Z::MarkHandles(S32 i_Manager) {
    S32 l_NextManager = i_Manager + 1;

    if ((i_Manager < 0 || i_Manager == 0) && gData.AnimMgr) {
        gData.AnimMgr->MarkHandles();
    }
    if ((i_Manager < 0 || i_Manager == 1) && gData.SoundMgr) {
        gData.SoundMgr->MarkHandles();
    }
    if ((i_Manager < 0 || i_Manager == 2) && gData.WorldMgr) {
        gData.WorldMgr->MarkHandles();
    }
    if ((i_Manager < 0 || i_Manager == 3) && gData.ScriptMgr) {
        gData.ScriptMgr->MarkHandles();
    }
    if ((i_Manager < 0 || i_Manager == 4) && gData.GameMgr) {
        gData.GameMgr->MarkHandles();
    }
    if ((i_Manager < 0 || i_Manager == 5) && gData.SystemDatas) {
        gData.SystemDatas->MarkHandles();
    }
    if ((i_Manager < 0 || i_Manager == 6) && gData.ObjectBankMgr) {
        gData.ObjectBankMgr->MarkHandles();
    }
    if ((i_Manager < 0 || i_Manager == 7) && gData.MaterialMgr) {
        gData.MaterialMgr->MarkHandles();
    }
    if ((i_Manager < 0 || i_Manager == 8) && gData.MainRdr) {
        gData.MainRdr->MarkHandles();
    }
    if ((i_Manager < 0 || i_Manager == 9) && gData.Cons) {
        gData.Cons->MarkHandles();
    }
    if ((i_Manager < 0 || i_Manager == 10) && gData.ManipMgr) {
        gData.ManipMgr->MarkHandles();
    }
    if ((i_Manager < 0 || i_Manager == 11) && gData.MovieMgr) {
        gData.MovieMgr->MarkHandles();
    }
    if (i_Manager == 12) {
        l_NextManager = -1;
    }
    return l_NextManager;
}

void HandleManager_Z::Minimize() {
    m_HandleIdHT.Minimize();

    if (gData.AnimMgr) {
        gData.AnimMgr->Minimize();
    }
    if (gData.SoundMgr) {
        gData.SoundMgr->Minimize();
    }
    if (gData.ScriptInputMgr) {
        gData.ScriptInputMgr->Minimize();
    }
    if (gData.SystemDatas) {
        gData.SystemDatas->Minimize();
    }
    if (gData.ObjectBankMgr) {
        gData.ObjectBankMgr->Minimize();
    }
    if (gData.MainRdr) {
        gData.MainRdr->Minimize();
    }
    if (gData.ManipMgr) {
        gData.ManipMgr->Minimize();
    }
    if (gData.ScriptMgr) {
        gData.ScriptMgr->Minimize();
    }
    if (gData.VolatileMgr) {
        gData.VolatileMgr->Minimize();
    }
    if (gData.ParticlesMgr) {
        gData.ParticlesMgr->Minimize();
    }
    if (gData.InputMgr) {
        gData.InputMgr->Minimize();
    }
    gData.Minimize();
    PrintFreeMem();
}

void HandleManager_Z::Draw(DrawInfo_Z& i_DrawInfo) {
    HandleStream.Draw(i_DrawInfo);
}

void HandleManager_Z::ClearMark() {
    HandleRec_Z* l_Rec = m_HandleRecDA.GetArrayPtr();
    S32 i = m_HandleRecDA.GetSize();
    while (i--) {
        if (l_Rec->m_ObjPtr) {
            l_Rec->m_Marked = HANDLE_MARKED_FALSE;
        }
        l_Rec++;
    }
}

S32 HandleManager_Z::CheckHandles() {
    if (m_DoAsynchCheckHandles) {
        return 0;
    }

    if (m_ForbidCheckHandles) {
        m_CheckHandlesQueued = TRUE;
        return 0;
    }
    m_CheckHandlesQueued = FALSE;

    S32 l_NbTotalDeleted = 0;
    S32 l_NbDeleted = 1;
    while (l_NbDeleted) {
        l_NbDeleted = 0;

        ClearMark();
        MarkHandles(-1);

        HandleRec_Z* l_Rec = m_HandleRecDA.GetArrayPtr();
        S32 i = m_HandleRecDA.GetSize();
        while (i--) {
            if (l_Rec->m_ObjPtr && !l_Rec->m_Marked) {
                if (!l_NbTotalDeleted && !l_NbDeleted && gData.MainRdr) {
                    gData.MainRdr->CleanAllDatas();
                }
                DeleteHandle(l_Rec->m_ObjPtr->GetHandle());
                l_NbDeleted++;
            }
            l_Rec++;
        }

        l_NbTotalDeleted += l_NbDeleted;
    }

    if (l_NbTotalDeleted) {
        m_LastDeleteFrameNb = gData.m_FrameCount;
        PrintFreeMem();
    }

    Minimize();
    m_UnkBool_SetsDeltaTimeTo30fps_0x34 = TRUE;
    return l_NbTotalDeleted;
}

void HandleManager_Z::ForbidCheckHandles(Bool i_ForbidCheckHandles) {
    if (i_ForbidCheckHandles) {
        if (m_ForbidCheckHandles) {
            return;
        }
        m_CheckHandlesQueued = FALSE;
    }
    m_ForbidCheckHandles = i_ForbidCheckHandles;
    if (m_CheckHandlesQueued) {
        m_ForbidCheckHandles = FALSE;
        CheckHandles();
    }
}

void HandleManager_Z::Update(Float i_DeltaTime) {
    if (!m_DoAsynchCheckHandles) {
        HandleStream.Update(i_DeltaTime);
        return;
    }

    if (m_NextHandleToDelete < 0) {
        GetAbsoluteTime();

        S32 l_Manager = m_NextManagerToMarkHandles;
        if (l_Manager != 0) {
            if (l_Manager >= 0 || l_Manager < -1) {
                m_NextManagerToMarkHandles = MarkHandles(l_Manager - 1);
            }
            else {
                gData.WorldMgr->Assume();

                m_NextHandleToDelete = m_HandleRecDA.GetSize();
                HandleRec_Z* l_Rec = m_HandleRecDA.GetArrayPtr() + m_NextHandleToDelete - 1;
                while (m_NextHandleToDelete-- != 0) {
                    if (l_Rec->m_ObjPtr && !l_Rec->m_Marked) {
                        break;
                    }
                    l_Rec--;
                }

                if (m_NextHandleToDelete < 0) {
                    m_DoAsynchCheckHandles = FALSE;
                    Minimize();
                    gData.Cons->DisableFlag(FL_CONSOLE_PAUSED);
                    return;
                }
                m_NextManagerToMarkHandles = -1;
                m_FramesSpentDeleting = 30.0f / i_DeltaTime;
            }
        }

        if (m_NextManagerToMarkHandles >= 0) {
            m_NextManagerToMarkHandles++;
            GetAbsoluteTime();
        }
    }
    else {
        Float l_FramesSpent = 30.0f / i_DeltaTime;
        Float l_Time = GetAbsoluteTime();
        S32 l_NbDeleted = 0;
        HandleRec_Z* l_Rec = m_HandleRecDA.GetArrayPtr() + m_NextHandleToDelete;
        while (i_DeltaTime > 0.0f && m_NextHandleToDelete >= 0) {
            if (l_Rec->m_ObjPtr && !l_Rec->m_Marked) {
                DeleteHandle(l_Rec->m_ObjPtr->GetHandle());
                l_NbDeleted++;
                if (l_NbDeleted > 5) {
                    l_NbDeleted = 0;
                    Float l_Now = GetAbsoluteTime();
                    i_DeltaTime -= l_Now - l_Time;
                    l_Time = l_Now;
                }
            }
            l_Rec--;
            m_NextHandleToDelete--;
        }
        m_FramesSpentDeleting = l_FramesSpent;
    }
}

Bool HandleManager_Z::AsynchCheckHandles() {
    if (gData.Cons->IsFlagEnable(FL_CONSOLE_PAUSED)) {
        return FALSE;
    }

    m_DoAsynchCheckHandles = TRUE;
    m_NextHandleToDelete = -1;
    m_NextManagerToMarkHandles = 0;
    m_FramesSpentDeleting = 0.0f;

    ClearMark();
    gData.Cons->EnableFlag(FL_CONSOLE_PAUSED);
    return TRUE;
}

Bool HandleManager_Z::IsAsynchDelHandle(const BaseObject_ZHdl& i_Hdl) const {
    if (!m_DoAsynchCheckHandles) {
        return FALSE;
    }

    S32 l_ID = i_Hdl.GetID();
    char l_Key = i_Hdl.GetKey();
    const HandleRec_Z& l_Rec = m_HandleRecDA[l_ID];
    if (l_Rec.m_Key == 0 && l_Rec.m_LastKeyBeforeAsyncDelete && l_Rec.m_LastKeyBeforeAsyncDelete == l_Key) {
        U32 l_EngineFlag = gData.m_EngineFlag;
        gData.DisableEngineFlag(FL_DISABLE_ASSERT);
        gData.m_EngineFlag = l_EngineFlag;
        return TRUE;
    }
    return FALSE;
}

Bool HandleManager_Z::MarkHandle(const BaseObject_ZHdl& i_Hdl) {
    HandleRec_Z& l_Rec = m_HandleRecDA[i_Hdl.GetID()];
    if (l_Rec.m_Marked == HANDLE_MARKED_TRUE) {
        return FALSE;
    }
    l_Rec.m_Marked = HANDLE_MARKED_TRUE;

    if (m_UnkS32_0x44) {
        m_UnkS32_0x44++;
        if (m_UnkS32_0x44 != 2) {
            return FALSE;
        }
    }
    return TRUE;
}

void HandleManager_Z::MarkHandles(const BaseObject_ZHdl& i_Hdl) {
    S32 l_ID = i_Hdl.GetID();
    if (CheckKey(l_ID, i_Hdl.GetKey()) && m_HandleRecDA[l_ID].m_Marked != HANDLE_MARKED_TRUE) {
        m_HandleRecDA[l_ID].m_ObjPtr->MarkHandles();
    }
}

void HandleManager_Z::MarkU32Handle(U32 i_Hdl) {
    BaseObject_ZHdl& l_Hdl = *(BaseObject_ZHdl*)&i_Hdl;
    S32 l_ID = l_Hdl.GetID();
    Bool l_KeyOk = l_ID < m_HandleRecDA.GetSize() && l_Hdl.GetKey() == m_HandleRecDA[l_ID].m_Key;
    if (l_KeyOk) {
        m_HandleRecDA[l_ID].m_ObjPtr->MarkHandles();
    }
}

U8 HandleManager_Z::IsMarked(const BaseObject_ZHdl& i_Hdl) {
    return m_HandleRecDA[i_Hdl.GetID()].m_Marked;
}

void HandleManager_Z::ExpandSize(S32 i_NewSize) {
    S32 l_NewSize = m_HandleRecDASize + i_NewSize;

    m_HandleRecDA.SetSize(l_NewSize);
    m_FreeRecDA.SetSize(l_NewSize);

    m_NbFree = i_NewSize;
    m_HandleRecDASize = l_NewSize;

    S32 i = 0;
    while (i_NewSize-- > 0) {
        m_FreeRecDA[i] = --l_NewSize;
        i++;
    }
}

S32 HandleManager_Z::HandleToU32(const BaseObject_ZHdl& i_Hdl) {
    return i_Hdl.m_RealID.GblID;
}

BaseObject_ZHdl HandleManager_Z::U32ToHandle(S32 i_Hdl) {
    BaseObject_ZHdl l_Hdl;
    l_Hdl.m_RealID.GblID = i_Hdl;
    return l_Hdl;
}

Bool HandleManager_Z::RemovedXRamResource(S32 i_Handle, U8 i_Flag) {
    BaseObject_ZHdl l_GblID = U32ToHandle(i_Handle);
    S32 l_ID = l_GblID.GetID();
    Bool l_KeyOk = FALSE;
    if (l_ID < m_HandleRecDA.GetSize() && l_GblID.GetKey() == m_HandleRecDA[l_ID].m_Key) {
        l_KeyOk = TRUE;
    }
    ASSERTLE_Z(l_KeyOk, "", 661, "CheckKey(aGblID.Ref.ID,aGblID.Ref.Key)");

    HandleRec_Z& l_HandleRec = m_HandleRecDA[l_ID];
    U8 l_Flag = l_HandleRec.m_Flag;
    if (l_Flag & i_Flag) {
        l_HandleRec.m_ObjPtr->Clean();
        l_HandleRec.m_Flag &= ~i_Flag;
        return TRUE;
    }
    return FALSE;
}

BaseObject_Z* HandleManager_Z::GetPtrXRam(const HandleRec_Z& i_HandleRec) const {
    HandleRec_Z& l_HandleRec = (HandleRec_Z&)i_HandleRec;
    U8 l_Flag = l_HandleRec.m_Flag;
    if (l_Flag & HandleRec_Z::RSC_XRAM) {
        if (l_Flag & HandleRec_Z::RSC_XRAM_LOADED) {
            gData.XRamMgr->LockBlockAt(l_HandleRec.m_xRamBlock, gData.m_FrameCount);
        }
        else {
            void* l_Data = gData.XRamMgr->Get(l_HandleRec.m_xRamBlock, gData.m_FrameCount);
            U8* l_Buffer = (U8*)l_Data;
            if (!l_Buffer) {
                return NULL;
            }

            l_HandleRec.m_Flag |= HandleRec_Z::RSC_XRAM_LOADED;
            l_HandleRec.m_ObjPtr->Load(&l_Data);
            l_HandleRec.m_ObjPtr->EndLoadLinks();
            l_HandleRec.m_ObjPtr->AfterEndLoad();
            gData.XRamMgr->EndGet(l_Buffer);
        }
    }
    return l_HandleRec.m_ObjPtr;
}

void HandleStream_Z::Draw(DrawInfo_Z& i_DrawInfo) {
    if (!m_StreamStage) {
        return;
    }

    Float l_Width;
    Float l_WorkingHeight;
    Float l_WorkingOffset;
    Float l_ColorStep;
    Float l_Near = Renderer_Z::GetDefaultNear();
    Vec2f l_CurRscTopLeft;
    Vec2f l_StartTopLeft;
    Vec2f l_CurRscBottomRight;
    Vec2f l_EndBottomRight;
    Color l_Blue;
    Color l_Magenta;
    Color l_RscColors[2];

    l_StartTopLeft.x = 10.0f;
    l_StartTopLeft.y = 100.0f;
    l_EndBottomRight.x = i_DrawInfo.m_VpSizeX - 10.0f;
    l_EndBottomRight.y = 130.0f;
    l_Width = l_EndBottomRight.x - 10.0f;

    l_CurRscTopLeft.x = 10.0f;
    l_CurRscTopLeft.y = 100.0f;
    l_CurRscBottomRight.x = l_EndBottomRight.x;
    l_CurRscBottomRight.y = 130.0f;

    l_Blue.r = 0.0f;
    l_Blue.g = 0.0f;
    l_Blue.b = 0.5f;
    l_Blue.a = 0.6f;
    l_RscColors[0].r = 1.0f;
    l_RscColors[0].g = 0.0f;
    l_RscColors[0].b = 0.0f;
    l_RscColors[0].a = 1.0f;
    l_RscColors[1].r = 1.0f;
    l_RscColors[1].g = 1.0f;
    l_RscColors[1].b = 0.0f;
    l_RscColors[1].a = 1.0f;

    gData.MainRdr->DrawQuad(l_StartTopLeft, l_EndBottomRight, l_Blue, l_Near);

    Float l_TotalDataBytes = (Float)(gStrFileHeader.m_DataBlockEnd - gStrFileHeader.m_DataBlockStart);
    for (S32 i = 0; i < m_TableOfContentsDA.GetSize(); i++) {
        S32 l_BlockEnd = m_TableOfContentsDA[i].m_BlockEndIdx;
        S32 l_BlockStart = m_TableOfContentsDA[i].m_BlockStartIdx;
        l_CurRscBottomRight.x = l_Width * (Float)l_BlockEnd / l_TotalDataBytes + l_StartTopLeft.x;
        l_CurRscTopLeft.x = l_Width * (Float)l_BlockStart / l_TotalDataBytes + l_StartTopLeft.x;
        gData.MainRdr->DrawQuad(l_CurRscTopLeft, l_CurRscBottomRight, l_RscColors[m_TableOfContentsDA[i].m_BlockStartIdx & 1], l_Near);
    }

    l_Magenta.r = 1.0f;
    l_Magenta.g = 1.0f;
    l_Magenta.b = 0.0f;
    l_Magenta.a = 1.0f;

    l_WorkingHeight = 5.0f;
    l_WorkingOffset = 2.0f;
    l_ColorStep = 0.1f;
    S32 l_Operation = m_WorkingBufferLastOperationCount - 1;
    S32 l_OperationOffset = l_Operation * sizeof(StrWorkingBuffer_Z);
    while (l_Operation >= 0) {
        StrWorkingBuffer_Z* l_WorkingBuffer = (StrWorkingBuffer_Z*)((U8*)m_WorkingBufferLastOperations + l_OperationOffset);
        l_CurRscTopLeft.y = l_StartTopLeft.y - l_WorkingHeight;
        l_CurRscBottomRight.y = l_StartTopLeft.y;
        S32 l_BlockEnd = l_WorkingBuffer->m_BlockEndIdx;
        S32 l_BlockStart = l_WorkingBuffer->m_BlockStartIdx;
        l_CurRscTopLeft.x = l_Width * (Float)l_BlockStart / l_TotalDataBytes + l_StartTopLeft.x;
        l_CurRscBottomRight.x = l_Width * (Float)l_BlockEnd / l_TotalDataBytes + l_StartTopLeft.x;
        gData.MainRdr->DrawQuad(l_CurRscTopLeft, l_CurRscBottomRight, l_Magenta, l_Near);

        l_CurRscTopLeft.y = l_EndBottomRight.y;
        l_CurRscBottomRight.y = l_EndBottomRight.y + l_WorkingOffset;
        gData.MainRdr->DrawQuad(l_CurRscTopLeft, l_CurRscBottomRight, l_Magenta, l_Near);

        l_Magenta.r -= l_ColorStep;
        l_Magenta.g -= l_ColorStep;
        l_OperationOffset -= sizeof(StrWorkingBuffer_Z);
        l_Operation--;
    }
}

Float Renderer_Z::GetDefaultNear() {
    return Renderer_ZDefaultNear;
}

void HandleStream_Z::Open(const Char* i_FileName) {
    m_StrFileName = i_FileName;
    m_Resources.Empty();
    m_StreamStage = HDL_STR_LOAD_STAGE_INIT;
    m_Str.Reset();
    gData.Cons->EnableFlag(FL_CONSOLE_PAUSED);
}

void HandleStream_Z::Update(Float i_DeltaTime) {
    if (!m_StreamStage) {
        return;
    }

    m_Str.Update();
    if (m_Str.GetLoadStage() == STR_LOAD_STAGE_PROCESSING || m_Str.GetLoadStage() == STR_LOAD_STAGE_UNK_2) {
        return;
    }

    switch (m_StreamStage) {
        case HDL_STR_LOAD_STAGE_INIT:
            m_Str.Open(m_StrFileName);
            m_StreamStage++;
            return;

        case HDL_STR_LOAD_STAGE_READ_HEADER:
            m_Str.Process(&gStrFileHeader, 0, 1);
            m_StreamStage++;
            return;

        case HDL_STR_LOAD_STAGE_PARSE_HEADER:
            if (GetHeader()) {
                m_StreamStage++;
            }
            else {
                m_StreamStage--;
            }
            return;

        case HDL_STR_LOAD_STAGE_READ_TOC: {
            m_TableOfContentsDA.SetSize(gStrFileHeader.m_TableOfContentsCount);
            m_DependencyDA.SetSize(gStrFileHeader.m_DependencyCount);

            S32* l_DependencyArray = m_DependencyDA.GetArrayPtr();
            void* l_Dependencies = Sys_Z::MemCpyFrom(
                m_TableOfContentsDA.GetArrayPtr(), m_WorkingBuffers[0].m_Data, m_TableOfContentsDA.GetSize() * sizeof(StrFileToc_Z)
            );
            Sys_Z::MemCpyFrom(l_DependencyArray, l_Dependencies, m_DependencyDA.GetSize() * sizeof(S32));

            for (S32 i = 0; i < m_TableOfContentsDA.GetSize(); i++) {
                Queue(i, 0.0f, FALSE);
            }
            m_StreamStage++;
            m_CurBuffer = 0;
            return;
        }

        case HDL_STR_LOAD_STAGE_READ_BLOCKS:
        case HDL_STR_LOAD_STAGE_READ_REQUESTED_BLOCKS:
            if (Read()) {
                m_StreamStage++;
            }
            return;

        case HDL_STR_LOAD_STAGE_LOAD_RESOURCES:
        case HDL_STR_LOAD_STAGE_LOAD_REQUESTED_RESOURCES: {
            S32 l_BlockStart = m_WorkingBuffers[m_CurBuffer].m_BlockStartIdx;
            S32 l_BlockEnd = m_WorkingBuffers[m_CurBuffer].m_BlockEndIdx;
            RscQueue_Z* l_QueuedRsc = m_WorkingBuffers[m_CurBuffer].m_CurRscOrder->m_QueuedRsc;
            RscQueue_Z* l_FirstLoaded = NULL;

            while (l_QueuedRsc) {
                if (l_QueuedRsc->m_BlockEndIdx > l_BlockEnd) {
                    break;
                }
                if (l_QueuedRsc->m_BlockStartIdx >= l_BlockStart) {
                    if (!l_FirstLoaded) {
                        l_FirstLoaded = l_QueuedRsc;
                    }

                    BigFileRscHeader_Z* l_Header = (BigFileRscHeader_Z*)(m_WorkingBuffers[m_CurBuffer].m_Data + (l_QueuedRsc->m_BlockStartIdx - l_BlockStart) * 0x800);
                    BigFileRsc_Z l_Resource;
                    l_Resource.m_Header = l_Header;
                    l_Resource.m_Flag = m_StreamStage != HDL_STR_LOAD_STAGE_LOAD_REQUESTED_RESOURCES;

                    if (l_Resource.m_Flag) {
                        if (gData.ClassMgr->LoadResource(l_Resource)) {
                            m_Resources.Add(l_Resource.m_Rsc);
                        }
                    }
                    else {
                        gData.ClassMgr->LoadResourceData(l_Resource);
                        l_Resource.m_Rsc->EndLoadLinks();
                        l_Resource.m_Rsc->AfterEndLoad();
                    }
                }
                l_QueuedRsc = l_QueuedRsc->m_Next;
            }

            if (l_FirstLoaded) {
                if (l_FirstLoaded->m_Prev) {
                    l_FirstLoaded->m_Prev->m_Next = l_QueuedRsc;
                    if (l_QueuedRsc) {
                        if (l_QueuedRsc->m_Prev) {
                            l_QueuedRsc->m_Prev->m_Next = NULL;
                        }
                        l_QueuedRsc->m_Prev = l_FirstLoaded->m_Prev;
                    }
                }
                else {
                    if (l_QueuedRsc) {
                        if (l_QueuedRsc->m_Prev) {
                            l_QueuedRsc->m_Prev->m_Next = NULL;
                        }
                        l_QueuedRsc->m_Prev = NULL;
                    }
                    m_WorkingBuffers[m_CurBuffer].m_CurRscOrder->m_QueuedRsc = l_QueuedRsc;
                    if (!m_WorkingBuffers[m_CurBuffer].m_CurRscOrder->m_QueuedRsc) {
                        Remove(m_WorkingBuffers[m_CurBuffer].m_CurRscOrder, m_WorkingBuffers[m_CurBuffer].m_CurRscOrder->m_Next);
                    }
                }

                if (l_FirstLoaded) {
                    RscQueue_Z* l_Next;
                    do {
                        l_Next = l_FirstLoaded->m_Next;
                        m_RscQueueBank.Release(l_FirstLoaded);
                        l_FirstLoaded = l_Next;
                    } while (l_Next);
                }
            }

            m_CurBuffer = 1 - m_CurBuffer;
            if (Read()) {
                return;
            }
            if (m_StreamStage != HDL_STR_LOAD_STAGE_LOAD_RESOURCES) {
                return;
            }
            m_PendingResources = m_Resources.GetSize();
            m_StreamStage++;
            return;
        }

        default: {
            Float l_Before = GetAbsoluteTime();
            while (i_DeltaTime > 0.0f && m_PendingResources--) {
                switch (m_StreamStage) {
                    case HDL_STR_LOAD_STAGE_ENDLOAD:
                        m_Resources[m_PendingResources]->EndLoad();
                        break;
                    case HDL_STR_LOAD_STAGE_AFTERENDLOAD:
                        m_Resources[m_PendingResources]->AfterEndLoad();
                        break;
                    case HDL_STR_LOAD_STAGE_LOADDONE:
                        m_Resources[m_PendingResources]->LoadDone();
                        break;
                }

                Float l_After = GetAbsoluteTime();
                i_DeltaTime -= l_After - l_Before;
                l_Before = l_After;
            }

            if (m_PendingResources < 0) {
                m_StreamStage++;
                m_PendingResources = m_Resources.GetSize();
            }
            if (m_StreamStage != HDL_STR_LOAD_STAGE_FINISHED) {
                return;
            }
            gData.Cons->DisableFlag(FL_CONSOLE_PAUSED);
            m_StreamStage = HDL_STR_LOAD_STAGE_READ_REQUESTED_BLOCKS;
            return;
        }
    }
}

void HandleStream_Z::Queue(S32 i_TocIdx, Float i_Priority, Bool i_QueueDependencies) {
    RscOrder_Z* l_Order = QueueZ(i_Priority);
    QueueRsc(&l_Order->m_QueuedRsc, i_TocIdx);

    if (i_QueueDependencies) {
        S32 l_DependencyIdx = m_TableOfContentsDA.GetArrayPtr()[i_TocIdx].m_DependenciesIdx;
        if (l_DependencyIdx < 0) {
            return;
        }
        S32* l_Dependency = m_DependencyDA.GetArrayPtr() + l_DependencyIdx;
        S32 l_DependencyCount = *l_Dependency++;
        while (l_DependencyCount--) {
            i_TocIdx = *l_Dependency++;
            ClassManager_Z* l_ClassManager = gData.ClassMgr;
            S32 l_NameId = m_TableOfContentsDA[i_TocIdx].m_Name.GetID();
            S32 l_HandleId = l_ClassManager->IsResourceRef(l_NameId);
            HandleRec_Z& l_HandleRec = l_ClassManager->m_HandleRecDA[l_HandleId];

            U8 l_Flag = l_HandleRec.m_Flag;
            if ((l_Flag & HandleRec_Z::RSC_STR) && !(l_Flag & HandleRec_Z::RSC_STR_DONE) && !(l_Flag & HandleRec_Z::RSC_STR_LOADING)) {
                l_HandleRec.m_Flag = l_Flag | HandleRec_Z::RSC_STR_LOADING;
                QueueRsc(&l_Order->m_QueuedRsc, i_TocIdx);
            }
        }
    }
}

void HandleStream_Z::Remove(RscOrder_Z* i_First, RscOrder_Z* i_End) {
    if (i_First) {
        if (i_First->m_Prev) {
            i_First->m_Prev->m_Next = i_End;
            if (i_End) {
                if (i_End->m_Prev) {
                    i_End->m_Prev->m_Next = NULL;
                }
                i_End->m_Prev = i_First->m_Prev;
            }
        }
        else {
            if (i_End) {
                if (i_End->m_Prev) {
                    i_End->m_Prev->m_Next = NULL;
                }
                i_End->m_Prev = NULL;
            }
            m_HeadRscOrder = i_End;
        }
    }

    RscOrder_Z* l_Order = i_First;
    if (!l_Order) {
        return;
    }

    RscOrder_Z* l_Next;
    do {
        l_Next = l_Order->m_Next;
        m_RscOrderBank.Release(l_Order);
        l_Order = l_Next;
    } while (l_Next);
}

void HandleStream_Z::QueueRsc(RscQueue_Z** io_Queue, S32 i_TocRscIndex) {
    StrFileToc_Z* l_TocRsc = &m_TableOfContentsDA[i_TocRscIndex];
    RscQueue_Z* l_Current = *io_Queue;
    RscQueue_Z* l_Previous = NULL;
    while (l_Current) {
        if (l_Current->m_BlockStartIdx > l_TocRsc->m_BlockStartIdx) {
            break;
        }
        l_Previous = l_Current;
        l_Current = l_Current->m_Next;
    }

    RscQueue_Z* l_NewRsc = m_RscQueueBank.Get();
    l_NewRsc->m_TocRscIndex = i_TocRscIndex;
    l_NewRsc->m_BlockStartIdx = l_TocRsc->m_BlockStartIdx;
    l_NewRsc->m_BlockEndIdx = l_TocRsc->m_BlockEndIdx;

    if (!*io_Queue) {
        l_NewRsc->m_Prev = NULL;
        *io_Queue = l_NewRsc;
    }
    else if (l_Previous) {
        l_NewRsc->m_Next = l_Previous->m_Next;
        l_NewRsc->m_Prev = l_Previous;
        if (l_Previous->m_Next) {
            l_Previous->m_Next->m_Prev = l_NewRsc;
        }
        l_Previous->m_Next = l_NewRsc;
    }
    else {
        l_NewRsc->m_Next = *io_Queue;
        l_NewRsc->m_Prev = NULL;
        (*io_Queue)->m_Prev = l_NewRsc;
        *io_Queue = l_NewRsc;
    }
}

RscOrder_Z* HandleStream_Z::QueueZ(Float i_Priority) {
    RscOrder_Z* l_Current = m_HeadRscOrder;
    RscOrder_Z* l_Previous = NULL;
    while (l_Current) {
        if (l_Current->m_Priority > i_Priority) {
            break;
        }
        l_Previous = l_Current;
        l_Current = l_Current->m_Next;
    }

    RscOrder_Z* l_NewOrder = m_RscOrderBank.Get();
    l_NewOrder->m_Priority = i_Priority;
    l_NewOrder->m_QueuedRsc = NULL;

    if (!m_HeadRscOrder) {
        l_NewOrder->m_Prev = NULL;
        m_HeadRscOrder = l_NewOrder;
    }
    else if (l_Previous) {
        l_NewOrder->m_Next = l_Previous->m_Next;
        l_NewOrder->m_Prev = l_Previous;
        if (l_Previous->m_Next) {
            l_Previous->m_Next->m_Prev = l_NewOrder;
        }
        l_Previous->m_Next = l_NewOrder;
    }
    else {
        l_NewOrder->m_Next = m_HeadRscOrder;
        l_NewOrder->m_Prev = NULL;
        m_HeadRscOrder->m_Prev = l_NewOrder;
        m_HeadRscOrder = l_NewOrder;
    }
    return l_NewOrder;
}

static inline Bool s_IsWorkingBufferHistoryFull(S32 i_Count) {
    return !(10 - i_Count);
}

Bool HandleStream_Z::Read() {
    if (!m_HeadRscOrder || !m_HeadRscOrder->m_QueuedRsc) {
        return FALSE;
    }

    m_CurBuffer &= 1;
    m_WorkingBuffers[m_CurBuffer].m_CurRscOrder = m_HeadRscOrder;
    RscQueue_Z* l_QueuedRsc = m_HeadRscOrder->m_QueuedRsc;
    m_WorkingBuffers[m_CurBuffer].m_BlockStartIdx = l_QueuedRsc->m_BlockStartIdx;

    U32 l_CurBuffer = m_CurBuffer;
    U32 l_WorkingBufferSize = gStrFileHeader.m_WorkingBufferSize[l_CurBuffer];
    S32 l_MaxEnd = (S32)l_WorkingBufferSize / 0x800 + m_WorkingBuffers[l_CurBuffer].m_BlockStartIdx;
    while (l_QueuedRsc && l_QueuedRsc->m_BlockEndIdx <= l_MaxEnd) {
        m_WorkingBuffers[l_CurBuffer].m_BlockEndIdx = l_QueuedRsc->m_BlockEndIdx;
        break;
    }

    m_Str.Process(m_WorkingBuffers[m_CurBuffer].m_Data, m_WorkingBuffers[m_CurBuffer].m_BlockStartIdx + gStrFileHeader.m_DataBlockStart, m_WorkingBuffers[m_CurBuffer].m_BlockEndIdx - m_WorkingBuffers[m_CurBuffer].m_BlockStartIdx);

    Bool l_FullHistory = s_IsWorkingBufferHistoryFull(m_WorkingBufferLastOperationCount);
    if (l_FullHistory) {
        memcpy(m_WorkingBufferLastOperations, m_WorkingBufferLastOperations + 1, (m_WorkingBufferLastOperationCount - 1) * sizeof(StrWorkingBuffer_Z));
        m_WorkingBufferLastOperationCount--;
    }

    StrWorkingBuffer_Z* l_WorkingBuffer = &m_WorkingBuffers[m_CurBuffer];
    S32 l_Operation = m_WorkingBufferLastOperationCount;
    if (l_Operation < 10) {
        m_WorkingBufferLastOperations[l_Operation] = *l_WorkingBuffer;
        m_WorkingBufferLastOperationCount++;
    }
    return TRUE;
}

Bool HandleStream_Z::GetHeader() {
    if (strcmp(gStrFileHeader.m_HeaderText, gData.ClassMgr->GetBigFileHeaderText())) {
        if (gStrFileHeader.m_HeaderText[0]) {
            ASSERTL_Z(FALSE, "Wrong version of BigFile.. Rebuild them", 1256);
        }
        ASSERTL_Z(FALSE, "BigFile not found..", 1259);
        return FALSE;
    }

    MemManager.m_FreeMemCached = s_GetFreeMem();
    s_GetLargestFree();
    m_WorkingBuffers[0].m_Data = (U8*)AllocEndAlignL_Z(gStrFileHeader.m_WorkingBufferSize[0], 0x4fd, 0x80);
    MemManager.m_FreeMemCached = s_GetFreeMem();
    s_GetLargestFree();
    m_WorkingBuffers[1].m_Data = (U8*)AllocEndAlignL_Z(gStrFileHeader.m_WorkingBufferSize[1], 0x501, 0x80);
    m_Str.Process(m_WorkingBuffers[0].m_Data, 1, gStrFileHeader.m_TocAndDependenciesBlockCount);
    return TRUE;
}
