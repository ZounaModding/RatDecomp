#ifndef _RTCSCRIPTAGENT_G_H_
#define _RTCSCRIPTAGENT_G_H_
#include "RtcAgent_Z.h"
// clang-format off

BEGIN_AGENT_CLASS(RtcScriptAgent_G, RtcAgent_Z, 22)
public:
    RtcScriptAgent_G();

    virtual ~RtcScriptAgent_G() {}
    virtual void Init();

    DECL_BHV(CheckNextCommand);
    DECL_BHV(CheckSkipRtc);
    DECL_BHV(CheckSkipText);
    DECL_BHV(CheckEndOfGoto);
    DECL_BHV(RtcParticlesEfx);
    DECL_BHV(RtcWaitPreLoad);

private:
    // TODO: fields - see decomp_dump/types.h `struct RtcScriptAgent_G` for Ghidra's inferred layout
END_AGENT_CLASS

// clang-format on
#endif // _RTCSCRIPTAGENT_G_H_
