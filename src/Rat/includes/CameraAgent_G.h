#ifndef _CAMERAAGENT_G_H_
#define _CAMERAAGENT_G_H_
#include "CameraAgent_Z.h"
// clang-format off

BEGIN_AGENT_CLASS(CameraAgent_G, CameraAgent_Z, 7)
public:
    CameraAgent_G();

    virtual ~CameraAgent_G() {}
    virtual void Init();

    DECL_BHV(CameraAgentQuartier);
private:
END_AGENT_CLASS

// clang-format on
#endif // _CAMERAAGENT_G_H_
