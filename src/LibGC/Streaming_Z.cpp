#include "GCMain_Z.h"
#include "Streaming_Z.h"
#include "StreamManager_Z.h"
#include "Timer_Z.h"

Float GCStream_ZAbsTime;

void GCStream_Z::Open(const Char* i_FilePath) {
    m_LoadStage = 1;
    gData.StreamMgr->Open(i_FilePath, this, NULL, 0, -1);
}

void GCStream_Z::Opened(S32 i_ErrorCode, S32 i_FileSize, S32 i_UserDefined) {
    if (i_ErrorCode) {
        m_LoadStage = 3;
        m_ErrorCode = 1;
    }
    else {
        m_Size = i_FileSize;
        m_LoadStage = 0;
        GCStream_ZAbsTime = GetAbsoluteTime();
    }
}

void GCStream_Z::Close() {
    gData.StreamMgr->Close(this, TRUE);
    GetAbsoluteTime();
}

void GCStream_Z::Process(void* o_Buffer, S32 i_BlockIdx, S32 i_BlockCount) {
    m_LoadStage = 1;
    gData.StreamMgr->Read(o_Buffer, i_BlockIdx << 11, i_BlockCount << 11, this, -1);
}

void GCStream_Z::Readed(S32 i_ErrorCode, S32 i_NbBytesRead, S32 i_UserDefined) {
    if (i_ErrorCode != 0) {
        m_LoadStage = 3;
        m_ErrorCode = 1;
        return;
    }

    m_LoadStage = 0;
    m_ErrorCode = 0;
}
