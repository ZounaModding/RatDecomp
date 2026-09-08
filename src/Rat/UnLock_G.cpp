#include "UnLock_G.h"

void UnLockEvents_G::CheckUnlock(Bool i_Force) {
}

void UnLockEvents_G::AddUnlockEvent(Console_Z* i_Console) {
}

Bool UnlockInfo_G::GetDifficulty(Char* i_Name) {
    m_Value = 0;
    if (!stricmp(i_Name, "MODE_EASY")) {
        m_Value = DifficultyEasy;
    }
    else if (!stricmp(i_Name, "MODE_NORMAL")) {
        m_Value = DifficultyNormal;
    }
    else if (!stricmp(i_Name, "MODE_HARD")) {
        m_Value = DifficultyHard;
    }
    else if (!stricmp(i_Name, "MODE_NIGHTMARE")) {
        m_Value = DifficultyNightmare;
    }
    else {
        return FALSE;
    }

    return TRUE;
}

Bool UnlockInfo_G::GetAbility(Char* i_Name) {
    m_Value = 0;
    if (!stricmp(i_Name, "WALLJUMP")) {
        m_Value = ABILITY_WALLJUMP;
    }
    else if (!stricmp(i_Name, "POWERJUMP")) {
        m_Value = ABILITY_POWERJUMP;
    }
    else if (!stricmp(i_Name, "LONGJUMP")) {
        m_Value = ABILITY_LONGJUMP;
    }
    else if (!stricmp(i_Name, "SMASHJUMP")) {
        m_Value = ABILITY_SMASHJUMP;
    }
    else {
        return FALSE;
    }

    return TRUE;
}
