#ifndef _LEVELADVANCEMENT_G_H_
#define _LEVELADVANCEMENT_G_H_
#include "Types_Z.h"
#include <string.h>

// $SABE: These might go somewhere else
#define MISSION_DIFFICULTY_COUNT 3
#define MISSION_NAME_LEN 16

#define MISSION_PLATFORM_GC (S32)(1 << 3)
#define MISSION_PLATFORM_ALL 0x1f

struct Score {
    Score() { m_Value = -1; }

    void Display();

    S32 m_Value;
};

struct Time {
    Time() { m_Value = -1.f; }

    void Display();

    Float m_Value;
};

struct Name {
    Name() { memset(m_Name, 0, MISSION_NAME_LEN); }

    void Display();

    Char m_Name[MISSION_NAME_LEN];
};

class MissionDef_G {
public:
    MissionDef_G();

    void Reset();
    void IncreaseNbTimesPlayed();
    Bool IsMissionOnCurrentPlateform();
    void SetRunning(Bool i_Running, Bool i_Force);
    void SetWon(Bool i_Won, Bool i_Force);
    void ForceOpened(Bool i_Opened);
    void SetOpened(Bool i_Opened);
    Bool IsRunning(Bool i_Force);
    Bool IsWon(Bool i_Force);
    Bool IsOpened(Bool i_Force);
    S32 IsMissionLaunchedFrom();

private:
    Bool m_Opened;
    Bool m_Won;
    Bool m_Running;
    Bool m_OpenedFrom;
    Bool m_WonFrom;
    Bool m_RunningFrom;
    Bool m_Unk_0x06;
    U8 m_Pad_0x07;
    S32 m_NbTimesPlayed;
    S32 m_Unk_0x0c;
    S32 m_Unk_0x10;
    S32 m_Unk_0x14;
    S32 m_Unk_0x18;
    S32 m_Unk_0x1c;
    S32 m_Unk_0x20;
    S32 m_Unk_0x24;
    U32 m_PlatformFlags;
    S32 m_Unk_0x2c;
    Score m_BestScore;
    Score m_BestScores[MISSION_DIFFICULTY_COUNT];
    Time m_BestTime;
    Time m_BestTimes[MISSION_DIFFICULTY_COUNT];
    Name m_BestNames[MISSION_DIFFICULTY_COUNT];
};

#endif // _LEVELADVANCEMENT_G_H_
