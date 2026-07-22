#ifndef _GAMEMGR_G_H_
#define _GAMEMGR_G_H_
#include "DynArray_Z.h"
#include "Math_Z.h"
#include "Purchase.h"
#include "Types_Z.h"

class IsToBePlayed;

#define RAT_INPUT_ACTION_COUNT 49

struct CInputDef_G {
    void InitInputs();

    S32 m_Actions[RAT_INPUT_ACTION_COUNT];
    S32 m_Unk_0xc4;
    S32 m_Unk_0xc8;
    S32 m_ActionContextIdx;
};

struct ConfigStruct {
    S32 m_VideoMode;
    U32 m_MusicVolume;
    U32 m_SfxVolume;
    U32 m_DialogVolume;
    U32 m_CamSens;
    S32 m_ScreenPosX;
    S32 m_ScreenPosY;
    Bool m_Subtitles;
    Bool m_InvertX;
    Bool m_InvertY;
    U8 m_Pad_0x1f;
    S32 m_Difficulty;
    Bool m_UnkBool_0x24;
    Bool m_UnkBool_0x25;
    Bool m_UnkBool_0x26;
    Bool m_UnkBool_0x27;
    Bool m_IsNightmare;
    Bool m_UnkBool_0x29;
    Bool m_UnkBool_0x2a;
    Bool m_UnkBool_0x2b;
    Bool m_UnkBool_0x2c;
    U8 m_Pad_0x2d[3];
};

struct SaveStruct {
    Float m_HoursPlayed;
    Float m_SecondsPlayed;
    Float m_LogicAdvancement;
    S32 m_LevelDataId;
    Bool m_Active;
    Bool m_UnkBool_0x11;
    Bool m_UnkBool_0x12;
    Bool m_UnkBool_0x13;
    S32 m_UnkS32_0x14;
    S32 m_UnkS32_0x18;
    S32 m_UnkS32_0x1c;
};

struct LastSavedBuffer {
    void* m_Buffer;
    S32 m_ShopPos;
    S32 m_ScoresPos;
    S32 m_ConfigPos;
    S32 m_Size;
};

struct RemapTextInfo {
    U8 m_Data[0x20];
};

typedef DynArray_Z<RemapTextInfo, 1> RemapTextInfoDA;

class CTFGameMgr_G {
public:
    CTFGameMgr_G();
    ~CTFGameMgr_G();

    void InitConfiguration();
    void AddTime(Float i_DeltaTime);

    CInputDef_G* GetInputDef() { return &m_InputDef; }

private:
    CInputDef_G m_InputDef;
    ConfigStruct m_ConfigStruct;
    Shop m_Shop;
    Vec3f m_PlayerPos;
    U32 m_UnkU32_0x6484[2];
    S32 m_UnkS32_0x648c;
    Quat m_PlayerRot;
    IsToBePlayed* m_ToBePlayed;
    Bool m_AutoSaveEnabled;
    S32 m_FileIdToLoad;
    Bool m_DontSave;
    S32 m_UnkS32_One_0x64b0;
    SaveStruct m_SaveStruct;
    LastSavedBuffer m_LastSavedBuffer;
    Bool m_Initialized;
    Float m_FadeVolume;
    Float m_UnkFloat_Zero_0x64f0;
    Float m_UnkFloat_PointOne_0x64f4;
    RemapTextInfoDA m_RemapTextInfos;
};

#endif // _GAMEMGR_G_H_
