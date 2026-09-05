#ifndef _CAMERAAGENT_Z_H_
#define _CAMERAAGENT_Z_H_
#include "Agent_Z.h"
#include "ParticlesAgent_ZHdl.h"
#include "CameraEngine_ZHdl.h"
// clang-format off

BEGIN_AGENT_CLASS(CameraAgent_Z, Agent_Z, 11)
public:
    CameraAgent_Z();
    virtual ~CameraAgent_Z() { }
    virtual void Init();
    virtual Bool MarkHandles();
    virtual void Suspend();
    virtual void Restore();
    virtual void Reset();
    virtual void RegisterWithGame(Game_ZHdl& a1);
    virtual void UnRegisterFromGame(Game_ZHdl& a1);
    virtual S32 GetNbCameraManipulator() { return 0; }

    virtual CameraEngine_ZHdl& GetCameraManipulator(S32 a1) { return m_DefaultCameraEngineHdl; }

    virtual void SetViewport(S32 m_ViewportId);
    virtual void InterpMessage(int a1, Float a2) { } 

    void FxSuspend(Bool i_Hide);

    S32 GetViewport() {
        return m_ViewportId;
    }

    ParticlesAgent_ZHdlDA& GetParticlesAgents() {
        return m_ParticlesAgents;
    }

private:
    CameraEngine_ZHdl m_DefaultCameraEngineHdl;
    S32 m_ViewportId;
    ParticlesAgent_ZHdlDA m_ParticlesAgents;

    DECL_BHV(CameraAgentSuspend);
END_AGENT_CLASS

// clang-format on
#endif // _CAMERAAGENT_Z_H_
