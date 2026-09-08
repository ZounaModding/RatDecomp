#ifndef _LOGICLEVEL_H_
#define _LOGICLEVEL_H_
#include "BaseObject_Z.h"
#include "DynArray_Z.h"
#include "LevelData_GHdl.h"
#include "Name_Z.h"

// $SABE: Fake name
struct LogicMissionEntry {
    U8 m_Unk_0x00[12];
};

class LogicLevel_G : public BaseObject_Z {
    Name_Z m_Name;
    U8 m_Unk_0x10[8];
    S32 m_Opened;
    DynArray_Z<LevelData_GHdl> m_Levels;
    DynArray_Z<LogicMissionEntry> m_Missions;

public:
    static BaseObject_Z* NewObject() { return NewL_Z(44) LogicLevel_G; }

    Bool IsOpened();
    S32 GetNbMissions();
};
#endif // _LOGICLEVEL_H_
