#ifndef _ANIMATIONOMNI_Z_H_
#define _ANIMATIONOMNI_Z_H_
#include "Keyframer_Z.h"
#include "Name_Z.h"
#include "Types_Z.h"

class Omni_Z;

class AnimationOmni_Z {
private:
    Name_Z m_ObjectName; // Omni Node name
    S16 m_ObjectId;      // Omni Node index
    KeyframerVec3fComp_Z m_ColorKfr;
    KeyframerFloatComp_Z m_StartKfr;
    KeyframerFloatComp_Z m_EndKfr;

public:
    void UpdateFrame(Float i_Time, Omni_Z* i_Omni);
    void Load(void** i_Data);
};

#endif // _ANIMATIONOMNI_Z_H_
