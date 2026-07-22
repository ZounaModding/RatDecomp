#ifndef _EVENEMENTVOLAGENT_G_H_
#define _EVENEMENTVOLAGENT_G_H_
#include "VolumeDelayedStart_G.h"
// clang-format off

BEGIN_AGENT_CLASS(EvenementVolAgent_G, VolumeDelayedStart_G, 14)
public:
    EvenementVolAgent_G();

    virtual ~EvenementVolAgent_G() {}
    virtual void Init();

    DECL_BHV(BhvCheckActionDansVolume);
    DECL_BHV(BhvWaitExplosion);

private:
    // TODO: fields - see decomp_dump/types.h `struct EvenementVolAgent_G` for Ghidra's inferred layout
END_AGENT_CLASS

// clang-format on
#endif // _EVENEMENTVOLAGENT_G_H_
