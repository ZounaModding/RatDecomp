#ifndef _BREAKABLE_Z_H_
#define _BREAKABLE_Z_H_
#include "ObjectsBreak_ZHdl.h"
#include "Throwable_Z.h"
// clang-format off

BEGIN_AGENT_CLASS(Breakable_Z, Throwable_Z, 9)
public:
    Breakable_Z();

    virtual ~Breakable_Z() {}
    virtual void Init();
    virtual Bool MarkHandles();
    virtual void Suspend();
    virtual void Restore();
    virtual void Reset();
    virtual void UseNode(Node_ZHdl& a1);
    virtual void RegisterWithGame(Game_ZHdl& a1);
    virtual void UnRegisterFromGame(Game_ZHdl& a1);

private:
    ObjectsBreak_ZHdl m_ObjectsBreakHdl;
END_AGENT_CLASS

// clang-format on
#endif // _BREAKABLE_Z_H_
