#ifndef _PLAYER_G_H_
#define _PLAYER_G_H_
#include "BaseInGameDatas_GHdl.h"
#include "DynPtrArray_Z.h"
#include "Friends_G.h"
#include "Math_Z.h"
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

    S32 GetViewportId() { return m_ViewportId; }

    void SetPlayerId(S32 i_PlayerId) { m_PlayerId = i_PlayerId; }

    void SetTeamId(S32 i_TeamId) { }

    void SetViewportId(S32 i_ViewportId) { m_ViewportId = i_ViewportId; }

private:
    static DynPtrArray_Z<Player_G*> instances;
    BaseInGameDatas_GHdl m_InGameDatasHdl;
    Vec3f m_SavedPos;
    Quat m_SavedRot;
    S32 m_PlayerId;
    S32 m_Unk_0x114;
    S32 m_ViewportId;
    Float m_Unk_0x11c;
    Bool m_Unk_0x120;
    U8 m_Pad_0x121[3];
    U8 m_Unk_0x124[12];
    Vec3f m_Unk_0x130;
    S32 m_Unk_0x13c;
    Bool m_Unk_0x140;
    Bool m_Unk_0x141;
    Bool m_Unk_0x142;
    Bool m_Unk_0x143;
    U8 m_Unk_0x144[12];
END_AGENT_CLASS

// clang-format on
#endif // _PLAYER_G_H_
