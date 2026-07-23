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
END_AGENT_CLASS

// clang-format on
#endif // _EVENEMENTVOLAGENT_G_H_
