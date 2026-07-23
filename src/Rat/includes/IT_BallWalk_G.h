#ifndef _IT_BALLWALK_G_H_
#define _IT_BALLWALK_G_H_
#include "MovingAgent_Z.h"
#include "DynPtrArray_Z.h"
// clang-format off

BEGIN_AGENT_CLASS(IT_BallWalk_G, MovingAgent_Z, 8)
public:
    IT_BallWalk_G();

    virtual ~IT_BallWalk_G();
    virtual void Init();
    void AddToStaticList();
    void RemoveFromStaticList();

    DECL_BHV(CheckRespawn);

private:
    static DynPtrArray_Z<IT_BallWalk_G*> instances;
END_AGENT_CLASS

// clang-format on
#endif // _IT_BALLWALK_G_H_
