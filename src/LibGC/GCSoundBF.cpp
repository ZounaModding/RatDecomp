#include "GCSoundBF.h"
#include "KSys_Z.h"
#include "GCMain_Z.h"
#include "StreamManager_Z.h"

SBFHeader GCSoundBF_Z::gSbfHeader;

Bool GCSoundBF_Z::Open(String_Z<256>& i_FileName) {
    PathKDBToSys(i_FileName, m_FileName);
    gData.StreamMgr->Open(m_FileName, this, NULL, 0, -1);
    return TRUE;
}

void GCSoundBF_Z::Close() {
    gSbfHeader.m_EntryCount = 0;
    if (gData.StreamMgr) {
        gData.StreamMgr->Close(this, TRUE);
    }
}

S32 GCSoundBF_Z::Get(const Char* i_Name, SBFEntry& o_Entry) {
    U32 l_NameId = Name_Z::GetID(i_Name);
    for (S32 i = 0; i < gSbfHeader.m_EntryCount; i++) {
        if (l_NameId == gSbfHeader.m_Entries[i].m_NameId) {
            o_Entry = gSbfHeader.m_Entries[i];
            return i;
        }
    }
    return -1;
}

void GCSoundBF_Z::Readed(S32 i_ErrorCode, S32 i_BytesRead, S32 i_UserDefined) {
    if (gSbfHeader.m_Magic != 0x103EB) {
        Close();
    }
}

void GCSoundBF_Z::Opened(S32 i_ErrorCode, S32 i_FileSize, S32 i_UserDefined) {
    if (!i_ErrorCode) {
        gData.StreamMgr->Read(&gSbfHeader, 0, sizeof(gSbfHeader), this, -1);
    }
}
