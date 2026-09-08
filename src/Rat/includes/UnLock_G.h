#ifndef _UNLOCK_G_H_
#define _UNLOCK_G_H_
#include "DynArray_Z.h"
#include "Types_Z.h"

// $SABE: These and the difficulty below might go somewhere else
#define ABILITY_NONE (S32)(0 << 0)
#define ABILITY_WALLJUMP (S32)(1 << 0)
#define ABILITY_POWERJUMP (S32)(1 << 1)
#define ABILITY_SMASHJUMP (S32)(1 << 2)
#define ABILITY_LONGJUMP (S32)(1 << 3)

enum Difficulty {
    DifficultyEasy = 1,
    DifficultyNormal = 2,
    DifficultyHard = 3,
    DifficultyNightmare = 4,
};

class Console_Z;
class MissionDef_G;

#define UNLOCK_DESC_LEN 64

enum UNLOCK_TYPE {
    unlock_mission = 0,
    unlock_level = 1,
    unlock_ability = 2,
    unlock_difficulty = 3,
    unlock_always = 4,
    unlock_all_missions = 5,
    unlock_none = 6
};

class UnlockInfo_G {
public:
    UnlockInfo_G() {
        m_Type = unlock_none;
        m_Desc[0] = 0;
        m_MissionType = 0;
        m_Value = 0;
        m_MissionIndex = 0;
    }

    Bool ConditionOkToUnlock(Bool i_Apply);
    Bool AlreadyUnlocked();
    void Unlock();
    Bool GetValues(Char* i_Type, Char* i_Param1, Char* i_Param2);
    Bool GetDifficulty(Char* i_Name);
    Bool GetAbility(Char* i_Name);
    Bool GetLevel(Char* i_Name);
    Bool GetMission(Char* i_Level, Char* i_Mission);
    MissionDef_G* GetMission();

private:
    Char m_Desc[UNLOCK_DESC_LEN];
    S32 m_Type;
    S32 m_Value;
    S32 m_MissionType;
    S32 m_MissionIndex;
};

class UnlockElem_G {
public:
    Bool m_JustUnlocked;
    U8 m_Pad_0x01[3];
    S32 m_TextId;
    Char m_Name[24];
    UnlockInfo_G m_ToUnlock;
    UnlockInfo_G m_Condition;
};

typedef DynArray_Z<UnlockElem_G, 4> UnlockElem_GDA;

class UnLockEvents_G {
    UnlockElem_GDA m_UnlockElemDA;

public:
    void AddUnlockEvent(Console_Z* i_Console);
    void CheckUnlock(Bool i_Force);
};

#endif // _UNLOCK_G_H_
