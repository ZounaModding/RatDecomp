#ifndef _LODAGENT_Z_H_
#define _LODAGENT_Z_H_
#include "Throwable_Z.h"
// clang-format off

BEGIN_AGENT_CLASS(LodAgent_Z, Throwable_Z, 7)
public:
    LodAgent_Z();

    virtual ~LodAgent_Z() {}
    virtual void Init();
    virtual void SetPlayerId(S32 a1);
    virtual void SetTeamId(S32 a1);
    virtual void SetViewportId(S32 a1);
    virtual void GetViewportId();
    virtual void SetInputId(S32 a1);
    virtual void GetInputId();
private:
END_AGENT_CLASS

// clang-format on
#endif // _LODAGENT_Z_H_
