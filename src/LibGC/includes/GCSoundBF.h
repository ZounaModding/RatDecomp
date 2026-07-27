#ifndef _GCSOUNDBF_H_
#define _GCSOUNDBF_H_
#include "BaseStream_Z.h"
#include <String_Z.h>

class SBFEntry {
public:
    U32 m_NameId;
    U16 m_Unk_0x4;
    U16 m_Unk_0x6;
    U32 m_Unk_0x8;
    U32 m_Unk_0xc;
};

class SBFHeader {
public:
    SBFHeader()
        : m_EntryCount(0) { }

    U32 m_Magic;
    S32 m_EntryCount;
    U8 m_Unk_0x8[8];
    SBFEntry m_Entries[3327];
};

class GCSoundBF_Z : public BaseStream_Z {
public:
    ~GCSoundBF_Z() {
        Close();
    }

    Bool Open(String_Z<256>& i_FileName);
    void Close();
    S32 Get(const Char* i_Name, SBFEntry& o_Entry);
    virtual void Readed(S32 i_ErrorCode, S32 i_BytesRead, S32 i_UserDefined);
    virtual void Opened(S32 i_ErrorCode, S32 i_FileSize, S32 i_UserDefined);

    static SBFHeader gSbfHeader;

private:
    Char m_FileName[256];
};
#endif
