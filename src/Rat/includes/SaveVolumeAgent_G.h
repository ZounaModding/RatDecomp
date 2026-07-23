#ifndef _SAVEVOLUMEAGENT_G_H_
#define _SAVEVOLUMEAGENT_G_H_
#include "VolumeDelayedStart_G.h"
// clang-format off

BEGIN_AGENT_CLASS(SaveVolumeAgent_G, VolumeDelayedStart_G, 12)
public:
    SaveVolumeAgent_G();

    virtual ~SaveVolumeAgent_G() {}
    virtual void Init();
private:
END_AGENT_CLASS

// clang-format on
#endif // _SAVEVOLUMEAGENT_G_H_
