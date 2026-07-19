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

HandleStream_Z HandleManager_Z::HandleStream;

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

void HandleStream_Z::Draw(DrawInfo_Z& i_DrawInfo) {
}

void HandleStream_Z::Update(Float i_DeltaTime) {
}
