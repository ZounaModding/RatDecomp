#include "StreamFile_Z.h"
#include "Handle_Z.h"
#include "Renderer_Z.h"
#include "Timer_Z.h"
#include "Console_Z.h"

U8 StreamFile_Z::gBigFileHeader[sizeof(BigFileHeader_Z)];
Float StartLoadingTime;

StreamFile_Z::~StreamFile_Z() {
    ResetBuffers();
}

void StreamFile_Z::Init() {
    Manipulator_Z::Init();
    SetGroup(ag_notpaused_last);
    m_LoadStage = STR_FILE_LOAD_STAGE_INIT;
}

void StreamFile_Z::Start(const Char* i_FilePath, const StreamAgent_ZHdl& i_AgentHdl) {
    m_AgentHdl = i_AgentHdl;
    StartLoadingTime = GetAbsoluteTime();
    m_BfPath.StrCpy(i_FilePath);
    gData.ClassMgr->SetBigFileOpened(TRUE);
    gData.Cons->EnableFlag(FL_CONSOLE_PAUSED);
    m_RestoreVsync = gData.MainRdr->IsEffectFlag(FL_EFFECT_VSYNC);
    gData.MainRdr->DisableEffectFlag(FL_EFFECT_VSYNC);
    g_BigFileHeader.m_VersionString[0] = '\0';
    m_LoadStage = 0;
    m_LoadStageBeforeResume = 0;
    m_CurBlockIdx = 0;
    Activate();
    m_Str.m_LoadStage = STR_FILE_LOAD_STAGE_INIT;
    m_Str.m_ErrorCode = STR_FILE_ERROR_NONE;
}

void StreamFile_Z::Update(Float i_DeltaTime) {
    if (!m_IsActive) {
        return;
    }

    m_Str.Update();

    if (m_Str.GetLoadStage() == STR_LOAD_STAGE_PROCESSING || m_Str.GetLoadStage() == STR_LOAD_STAGE_UNK_2) {
        return;
    }
    if (m_Str.GetLoadStage() == STR_LOAD_STAGE_ERROR) {
        return;
    }

    if (m_LoadStageBeforeResume != STR_FILE_LOAD_STAGE_INIT && m_LoadStage == m_LoadStageBeforeResume) {
        m_LoadStageBeforeResume = STR_FILE_LOAD_STAGE_INIT;
        Send(msg_stream_resumed, m_AgentHdl);
    }

    switch (m_LoadStage) {
        case STR_FILE_LOAD_STAGE_ABORT: {
            Aborted();
            break;
        }
        case STR_FILE_LOAD_STAGE_INIT: {
            m_Str.Open(m_BfPath);
            m_LoadStage++;
            break;
        }
        case STR_FILE_LOAD_STAGE_READ_HEADER: {
            m_Str.Process(&g_BigFileHeader, 0, 1);
            m_LoadStage++;
            break;
        }
        case STR_FILE_LOAD_STAGE_PARSE_HEADER: {
            if (GetHeader()) {
                m_LoadStage++;
            }
            else {
                m_LoadStage--;
            }
            break;
        }
        case STR_FILE_LOAD_STAGE_READ_BLOCK: {
            Read(m_CurBlockIdx);
            m_LoadStage++;
            break;
        }
        case STR_FILE_LOAD_STAGE_LOAD_RESOURCES: {
            m_BlockOffsetToRead += (g_BigFileHeader.m_Blocks[m_CurBlockIdx].m_PaddedSize / 2048);
            BlockDescription_Z* l_BlockDesc = &g_BigFileHeader.m_Blocks[m_CurBlockIdx];
            m_CurResource.m_Header = (BigFileRscHeader_Z*)m_Buf[0].m_AlignedPtr;
            m_CurResource.m_Data = (U8*)m_Buf[0].m_RealPtr;

            while (l_BlockDesc->m_ObjectCount--) {
                BigFileRscHeader_Z* l_PrevRscHeader = m_CurResource.m_Header;
                if (gData.ClassMgr->LoadResource(m_CurResource)) {
                    m_Resources.Add(m_CurResource.m_ObjPtr);
                }
                BigFileRscHeader_Z l_RscHeader;
                Sys_Z::MemCpyFrom(&l_RscHeader, m_CurResource.m_Header, sizeof(BigFileRscHeader_Z));
                BigFileRscHeader_Z* l_NextRscHeader = (BigFileRscHeader_Z*)((U32)l_PrevRscHeader + l_RscHeader.m_Size);
                m_CurResource.m_Header = &l_NextRscHeader[1];
            }

            Read(m_CurBlockIdx + 1);
            m_CurBlockIdx++;
            if (m_CurBlockIdx == g_BigFileHeader.m_BlockCount) {
                m_CurResourceIdx = m_Resources.GetSize();
                m_LoadStage++;
            }
            break;
        }
        default: {
            Float l_TimeBeforeProcessingOne = GetAbsoluteTime();
            while (i_DeltaTime > 0.0f && m_CurResourceIdx--) {
                switch (m_LoadStage) {
                    case STR_FILE_LOAD_STAGE_ENDLOAD: {
                        m_Resources[m_CurResourceIdx]->EndLoad();
                        break;
                    }
                    case STR_FILE_LOAD_STAGE_AFTERENDLOAD: {
                        m_Resources[m_CurResourceIdx]->AfterEndLoad();
                        break;
                    }
                    case STR_FILE_LOAD_STAGE_LOADDONE: {
                        m_Resources[m_CurResourceIdx]->LoadDone();
                        break;
                    }
                }
                Float l_TimeAfterProcessingOne = GetAbsoluteTime();
                i_DeltaTime -= (l_TimeAfterProcessingOne - l_TimeBeforeProcessingOne);
                l_TimeBeforeProcessingOne = l_TimeAfterProcessingOne;
            }
            if (m_CurResourceIdx < 0) {
                m_LoadStage++;
                m_CurResourceIdx = m_Resources.GetSize();
            }
            if (m_LoadStage == STR_FILE_LOAD_STAGE_FINISHED) {
                End();
            }
        }
    }
}

void StreamFile_Z::Read(S32 i_BlockIdx) {
    if (i_BlockIdx == g_BigFileHeader.m_BlockCount) {
        return;
    }

    BlockDescription_Z* l_BlockDesc = &g_BigFileHeader.m_Blocks[i_BlockIdx];

    if (!(g_BigFileHeader.m_Flag & BF_HEADER_FLAG_COMPRESSED)) {
        Free_Z(m_Buf[0].m_RealPtr);
        m_Buf[0].m_RealPtr = AllocEndAlignL_Z(l_BlockDesc->m_PaddedSize, 214, 128);
        m_Buf[0].m_AlignedPtr = m_Buf[0].m_RealPtr;
    }
    else {
        m_Buf[0].m_AlignedPtr = (void*)((U32)m_Buf[0].m_RealPtr + l_BlockDesc->m_WorkingBufferOffset);
    }

    m_Str.Process(m_Buf[0].m_AlignedPtr, m_BlockOffsetToRead, l_BlockDesc->m_PaddedSize / 2048);
}

void StreamFile_Z::Resume() {
    Activate();
    m_LoadStageBeforeResume = m_LoadStage;
    m_LoadStage--;
    m_Str.m_LoadStage = STR_FILE_LOAD_STAGE_INIT;
    m_Str.m_ErrorCode = STR_FILE_ERROR_NONE;
}

Bool StreamFile_Z::GetHeader() {
    Bool l_Result;
    if (strcmp(g_BigFileHeader.m_VersionString, gData.ClassMgr->GetBigFileHeaderText())) {
        if (g_BigFileHeader.m_VersionString[0]) {
            ASSERTL_Z(FALSE, "Wrong version of BigFile.. Rebuild them", 242);
        }
        ASSERTL_Z(FALSE, "BigFile not found..", 245);
        l_Result = FALSE;
    }
    else {
        if (g_BigFileHeader.m_BlockCount == 0) {
            End();
            l_Result = FALSE;
        }
        else {
            MemManager.m_FreeMemCached = s_GetFreeMem();
            s_GetLargestFree();
            m_Buf[0].m_RealPtr = AllocEndAlignL_Z(g_BigFileHeader.m_WorkingBufferCapacityEven, 276, 128);
            l_Result = TRUE;
            m_CurBlockIdx = 0;
            m_BlockOffsetToRead = 1;
        }
    }
    return l_Result;
}

void StreamFile_Z::End() {
    m_Str.Close();
    m_Resources.Empty();
    m_Resources.Minimize();
    if (m_Buf[0].m_RealPtr) {
        Free_Z(m_Buf[0].m_RealPtr);
    }
    m_Buf[0].m_RealPtr = NULL;
    if (m_Buf[1].m_RealPtr) {
        Free_Z(m_Buf[1].m_RealPtr);
    }
    m_Buf[1].m_RealPtr = NULL;
    if (m_RestoreVsync) {
        gData.MainRdr->EnableEffectFlag(FL_EFFECT_VSYNC);
    }
    Send(msg_stream_end, m_AgentHdl);
    Deactivate();
    gData.ClassMgr->SetBigFileOpened(FALSE);
    gData.Cons->DisableFlag(FL_CONSOLE_PAUSED);
}

void StreamFile_Z::Aborted() {
}
