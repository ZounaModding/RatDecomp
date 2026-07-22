#ifndef _MISSIONMANAGER_G_H_
#define _MISSIONMANAGER_G_H_
#include "Manipulator_Z.h"

class MissionManager_G : public Manipulator_Z {
public:
    virtual ~MissionManager_G();
    virtual void Init();
    virtual void Reset();
    virtual void Update(Float i_DeltaTime);

    static BaseObject_Z* NewObject() { return NewL_Z(144) MissionManager_G; }

private:
    U8 m_Unk_0x20[0x3a0];
};
#endif // _MISSIONMANAGER_G_H_
