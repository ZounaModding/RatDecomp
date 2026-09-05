#ifndef _PLAYPARTICLES_Z_H_
#define _PLAYPARTICLES_Z_H_
#include "Manipulator_Z.h"
#include "Node_ZHdl.h"
#include "Math_Z.h"

class ParticlesData_Z;

class PlayParticles_Z : public Manipulator_Z {
public:
    virtual void Init();
    virtual ~PlayParticles_Z();
    virtual Bool MarkHandles();
    virtual void Reset();
    virtual void Replay();

    virtual void SetNode(const Node_ZHdl& a1) { }

    virtual void SetNodeEmiter(const Node_ZHdl& a1);

    virtual void GetEmitRange(S32 a1) { }

    virtual void GetEmitTime(S32 a1) { }

    virtual void SetEmitColor(const Vec3f& a1, S32 a2) { }

    virtual void Pause() { }

    virtual void Resume();

    inline Node_Z* GetNode() const {
        return m_NodeHdl;
    }

protected:
    Node_ZHdl m_NodeHdl;
    Node_ZHdl m_EmiterNodeHdl;
    Bool m_IsPlaying;
    ParticlesData_Z* m_Data;
};

#endif // _PLAYPARTICLES_Z_H_
