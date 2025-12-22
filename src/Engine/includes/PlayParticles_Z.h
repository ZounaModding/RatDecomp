#ifndef _PLAYPARTICLES_Z_H_
#define _PLAYPARTICLES_Z_H_
#include "Manipulator_Z.h"
#include "Node_ZHdl.h"

class ParticlesData_Z;

class PlayParticles_Z : public Manipulator_Z {
public:
    virtual void Init();                                /* 0x08 */
    virtual ~PlayParticles_Z();                         /* 0x0C */
    virtual Bool MarkHandles();                         /* 0x10 */
    virtual void Reset();                               /* 0x14 */
    virtual void Replay();                              /* 0x18 */
    virtual void SetNode(const Node_ZHdl& a1);          /* 0x1C */
    virtual void SetNodeEmiter(const Node_ZHdl& a1);    /* 0x20 */
    virtual void GetEmitRange(S32 a1);                  /* 0x24 */
    virtual void GetEmitTime(S32 a1);                   /* 0x28 */
    virtual void SetEmitColor(const Vec3f& a1, S32 a2); /* 0x2C */
    virtual void Pause();                               /* 0x30 */
    virtual void Resume();                              /* 0x34 */
private:
    Node_ZHdl m_NodeHdl;
    Node_ZHdl m_EmiterNodeHdl;
    ParticlesData_Z* m_Data;
};

#endif // _PLAYPARTICLES_Z_H_
