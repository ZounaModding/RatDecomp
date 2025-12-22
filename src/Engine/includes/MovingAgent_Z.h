#ifndef _MOVINGAGENT_Z_H_
#define _MOVINGAGENT_Z_H_
#include "AnimatedAgent_Z.h"
#include "ObjectMove_ZHdl.h"
// clang-format off

BEGIN_AGENT_CLASS(MovingAgent_Z, AnimatedAgent_Z, 8)
public:
    MovingAgent_Z();

    virtual ~MovingAgent_Z() {}
    virtual void Init();
    virtual void Suspend();
    virtual void Restore();
    virtual void UseNode(Node_ZHdl& a1);
    virtual void RegisterWithGame(Game_ZHdl& a1);
    virtual void UnRegisterFromGame(Game_ZHdl& a1);

    void SetObjectMove(const BaseObject_ZHdl& i_ObjectMoveHdl);

    inline ObjectMove_ZHdl GetObjectMove() {
        return m_ObjectMoveHdl;
    }

protected:
    ObjectMove_ZHdl m_ObjectMoveHdl;
END_AGENT_CLASS

// clang-format on
#endif // _MOVINGAGENT_Z_H_
