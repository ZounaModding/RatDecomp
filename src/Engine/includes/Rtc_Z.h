#ifndef _RTC_Z_H_
#define _RTC_Z_H_
#include "ResourceObject_Z.h"
#include "AnimMessage_Z.h"
#include "AnimationCamera_Z.h"
#include "AnimationOmni_Z.h"
#include "Animation_ZHdl.h"
#include "Object_ZHdl.h"
#include "RtcAnimationNode_Z.h"
#include "RtcPlayer_Z.h"
#include "Sound_ZHdl.h"
#include "Camera_ZHdl.h"

typedef DynArray_Z<RtcAnimationNode_Z, 16> RtcAnimationNode_ZDA;
typedef DynArray_Z<AnimationCamera_Z, 32> AnimationCamera_ZDA;
typedef DynArray_Z<AnimationOmni_Z, 32> AnimationOmni_ZDA;
typedef DynArray_Z<RtcNode, 16> RtcNodeDA;
typedef DynArray_Z<RtcPerso, 16> RtcPersoDA;
typedef DynArray_Z<Animation_ZHdl, 32> Animation_ZHdlDA;
typedef DynArray_Z<Sound_ZHdl, 32> Sound_ZHdlDA;

class Rtc_Z : public ResourceObject_Z {
public:
    Rtc_Z();
    virtual ~Rtc_Z();
    virtual void Load(void** i_Data);
    virtual void EndLoad();
    virtual void AfterEndLoad();
    virtual Bool MarkHandles();

    static BaseObject_Z* NewObject() { return NewL_Z(58) Rtc_Z; }

    S32 GetCameraByName(const Name_Z& i_Name);

private:
    RtcAnimationNode_ZDA m_RtcAnimationNodes;
    AnimationCamera_ZDA m_AnimationCameras;
    AnimationOmni_ZDA m_AnimationOmnis;
    RtcNodeDA m_RtcNodes;
    RtcPersoDA m_RtcPersos;
    Node_ZHdlDA m_Targets;
    KeyframerMessage_Z m_MsgKfr;
    Float m_Duration;
    Animation_ZHdlDA m_AnimHdls;
    Sound_ZHdlDA m_SoundHdls;
};

#endif // _RTC_Z_H_
