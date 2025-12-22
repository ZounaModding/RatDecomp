#ifndef _CAMERAAGENT_Z_H_
#define _CAMERAAGENT_Z_H_
#include "Agent_Z.h"
#include "ParticlesAgent_ZHdl.h"
#include "CameraEngine_ZHdl.h"
// clang-format off

BEGIN_AGENT_CLASS(CameraAgent_Z, Agent_Z, 11)
public:
    CameraAgent_Z();
    virtual ~CameraAgent_Z() { }                                               /* 0x0C */
    virtual void Init();                                                       /* 0x08 */
    virtual Bool MarkHandles();                                                /* 0x10 */
    virtual void Suspend();                                                    /* 0x24 */
    virtual void Restore();                                                    /* 0x28 */
    virtual void Reset();                                                      /* 0x2C */
    virtual void RegisterWithGame(Game_ZHdl& a1);                              /* 0x30 */
    virtual void UnRegisterFromGame(Game_ZHdl& a1);                            /* 0x34 */
    virtual void GetNbCameraManipulator();                                     /* 0x38 */
    virtual void GetCameraManipulator(S32 a1);                                 /* 0x3C */
    virtual void SetViewport(S32 m_ViewportId);                                /* 0x40 */
    virtual void InterpMessage(int a1, Float a2);                              /* 0x44 */

    S32 GetViewport() {
        return m_ViewportId;
    }

private:
    CameraEngine_ZHdl m_DefaultCameraEngineHdl;
    S32 m_ViewportId;
    ParticlesAgent_ZHdlDA m_ParticlesAgents;

    DECL_BHV(CameraAgentSuspend)
END_AGENT_CLASS

// clang-format on
#endif // _CAMERAAGENT_Z_H_
