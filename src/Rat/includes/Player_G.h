#ifndef _PLAYER_G_H_
#define _PLAYER_G_H_
#include "Friends_G.h"
#include "DynPtrArray_Z.h"
// clang-format off

BEGIN_AGENT_CLASS(Player_G, Friends_G, 33)
public:
    Player_G();

    virtual ~Player_G() {}
    virtual void Init();
    void AddToStaticList();
    void RemoveFromStaticList();

    DECL_BHV(CheckWarpColor);
    DECL_BHV(CheckSoundClothe);
    DECL_BHV(CheckAnimEvent);
    DECL_BHV(CheckHit);

private:
    static DynPtrArray_Z<Player_G*> instances;
END_AGENT_CLASS

// clang-format on
#endif // _PLAYER_G_H_
