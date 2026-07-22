#ifndef _MUSICMANAGER_G_H_
#define _MUSICMANAGER_G_H_
#include "Manipulator_Z.h"

class MusicManager_G : public Manipulator_Z {
public:
    virtual ~MusicManager_G();
    virtual void Init();
    virtual void Reset();
    virtual void Update(Float i_DeltaTime);

    static BaseObject_Z* NewObject() { return NewL_Z(109) MusicManager_G; }

private:
    U8 m_Unk_0x20[0x134];
};
#endif // _MUSICMANAGER_G_H_
