#ifndef _SUBLEVELVOLUMEAGENT_Z_H_
#define _SUBLEVELVOLUMEAGENT_Z_H_
#include "Game_ZHdl.h"
#include "VolumeAgent_Z.h"

// clang-format off

BEGIN_AGENT_CLASS(SubLevelVolumeAgent_Z, VolumeAgent_Z, 9)
public:
    virtual ~SubLevelVolumeAgent_Z() {}
    virtual void RegisterWithGame(Game_ZHdl& a1);
    virtual void Entering(const Agent_ZHdl& a1);
    virtual void Inside(const Agent_ZHdl& a1);
    virtual void Exiting(const Agent_ZHdl& a1);

private:
    Game_ZHdl m_GameHdlCopy;
END_AGENT_CLASS

// clang-format on

#endif // _SUBLEVELVOLUMEAGENT_Z_H_
