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
    // TODO: fields - see decomp_dump/types.h `struct CameraAgent_G` for Ghidra's inferred layout
END_AGENT_CLASS

// clang-format on
#endif // _CAMERAAGENT_G_H_
