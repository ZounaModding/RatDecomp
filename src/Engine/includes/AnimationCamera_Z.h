#ifndef _ANIMATIONCAMERA_Z_H_
#define _ANIMATIONCAMERA_Z_H_
#include "Keyframer_Z.h"
#include "Name_Z.h"
#include "Types_Z.h"

class Camera_Z;

class AnimationCamera_Z {
private:
    Name_Z m_ObjectName; // Camera Node name
    S16 m_ObjectId;      // Camera Node index
    KeyframerFloatComp_Z m_RollKfr;
    KeyframerFloatComp_Z m_FovKfr;
    KeyframerFloat_Z m_FarClipKfr;
    KeyframerFloatComp_Z m_NearClipKfr;

public:
    void UpdateFrame(Float i_Time, Camera_Z* i_Camera);
    void Load(void** i_Data);
};

#endif // _ANIMATIONCAMERA_Z_H_
