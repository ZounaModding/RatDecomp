#ifndef _SUPERSPRAY_Z_H_
#define _SUPERSPRAY_Z_H_
#include "PlayParticles_Z.h"
#include "Math_Z.h"

class Node_ZHdl;
class ParticlesEle_Z;
class ParticlesDataEle_Z;

struct ParticlesEleState_Z {
    ParticlesEle_Z* m_ParticlesEle;
    ParticlesDataEle_Z* m_ParticlesDataEle;
    Float m_Num;
    Float m_TimeElapsed;
    Float m_PeriodTime;
    Bool m_StopEmit;
    S32 m_UsedNb;
    Bool m_UseDefaultColor;
    Vec3f m_Color;
};

class SuperSpray_Z : public PlayParticles_Z {
public:
    SuperSpray_Z() { }

    virtual ~SuperSpray_Z() { }

    virtual void Reset();
    virtual void Update(Float a1);
    virtual void Replay();
    virtual void SetNode(const Node_ZHdl& a1);
    virtual void GetEmitRange(S32 a1);
    virtual void GetEmitTime(S32 a1);
    virtual void SetEmitColor(const Vec3f& a1, S32 a2);
    virtual void Pause();
    virtual void Resume();

    static BaseObject_Z* NewObject() { return NewL_Z(32) SuperSpray_Z; }

private:
    DynArray_Z<ParticlesEleState_Z, 1> m_ParticleEmitters;
};

#endif // _SUPERSPRAY_Z_H_
