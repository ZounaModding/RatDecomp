#ifndef _THROWABLE_Z_H_
#define _THROWABLE_Z_H_
#include "MovingAgent_Z.h"
#include "ObjectThrow_ZHdl.h"
// clang-format off

class ObjectMoveData_Z;

BEGIN_AGENT_CLASS(Throwable_Z, MovingAgent_Z, 9)
public:
    Throwable_Z();

    virtual ~Throwable_Z() {}
    virtual void Init();
    virtual void Reset();
    virtual void GetCollisionMessageContactActive(int& a1);
    virtual void GetCollisionMessageContactPassive(int& a1);
    virtual void SetCollisionMessageContactActive(int a1);
    virtual void SetCollisionMessageContactPassive(int a1);

    DECL_BHV(CheckThrown);

    virtual void ResetCollisionMessages();
    virtual void UnLinked();
    virtual void ThrowBegin();
    virtual void ThrowEnd();
    virtual void GetThrowParameters(ObjectMoveData_Z* a1);

    ObjectThrow_ZHdl GetObjectThrow() {
        return (ObjectThrow_ZHdl)m_ObjectMoveHdl;
    }

private:
    abc_message m_CollisionMessageContactActive;
    abc_message m_CollisionMessageContactPassive;
END_AGENT_CLASS

// clang-format on
#endif // _THROWABLE_Z_H_
