#ifndef _LEVELDATA_G_H_
#define _LEVELDATA_G_H_
#include "BaseObject_Z.h"
#include "DynArray_Z.h"
#include "LevelAdvancement_GHdl.h"

// $SABE: Might go somewhere else
struct MaterialLib {
    Name_Z m_Name;
    S32 m_TextId;
    U8 m_Data[76];
};

typedef DynArray_Z<MaterialLib, 1> MaterialLibDA;

// $SABE: Fake name
struct LevelRtc_G {
    U8 m_Unk_0x00[5];
    Char m_Name[11];
};

typedef DynArray_Z<LevelRtc_G, 1> LevelRtc_GDA;

class LevelData_G : public BaseObject_Z {
public:
    MaterialLib* GetMaterialLib(const Name_Z& i_Name);
    MaterialLib* GetMaterialLibLoaded(const Name_Z& i_Name);
    void FreeMaterialLib(MaterialLib* i_MaterialLib, Bool i_Force);
    void ResetAdvancement();
    void SetRTCs(Char* i_Rtc1, Char* i_Rtc2);

private:
    S32 m_Unk_0x0c;
    S32 m_Unk_0x10;
    DynArray_Z<S32> m_MusicIds;
    Char m_Unk_0x1c[10];
    Char m_LevelName[32];
    Char m_Rtc1[32];
    Char m_Unk_0x66[32];
    Char m_Rtc2[32];
    U8 m_Pad_0xa6[2];
    MaterialLibDA m_MaterialLibs;
    S32 m_Unk_0xb0;
    Bool m_MissionBriefingLoaded;
    Bool m_Unk_0xb5;
    U8 m_Pad_0xb6[2];
    S32 m_Unk_0xb8;
    S32 m_Unk_0xbc;
    S32 m_Unk_0xc0;
    LevelAdvancement_GHdl m_LevelAdvancementHdl;
    LevelRtc_GDA m_Rtcs;
};
#endif // _LEVELDATA_G_H_
