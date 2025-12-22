#ifndef _RTCAGENT_Z_H_
#define _RTCAGENT_Z_H_
#include "ABC_AgentMacros.h"
#include "AnimatedMsgAgent_Z.h"
#include "RtcPlayer_ZHdl.h"
#include "Rtc_ZHdl.h"
// clang-format off

BEGIN_AGENT_CLASS(RtcAgent_Z, AnimatedMsgAgent_Z, 13)
public:
    RtcAgent_Z();
    virtual void Init();
    virtual ~RtcAgent_Z() { }
    virtual Bool MarkHandles();
    virtual void Suspend();
    virtual void Restore();
    virtual void Reset();
    virtual void RegisterWithGame(Game_ZHdl& a1);
    virtual void UnRegisterFromGame(Game_ZHdl& a1);

    DECL_BHV(RtcEnd)

    virtual void Start(S32 a1);
    virtual void End();
    virtual void Shut();
    virtual void GlobalTrackMessage(RegMessage_Z& a1, bool a2);
    virtual void NodeMessage(RegMessage_Z& a1, bool a2);

private:
    int m_ViewportId;
    Rtc_ZHdl m_RtcHdl;
    RtcPlayer_ZHdl m_RtcPlayerHdl;
END_AGENT_CLASS

// clang-format on
#endif // _RTCAGENT_Z_H_
