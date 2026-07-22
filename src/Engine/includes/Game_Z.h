#ifndef _GAME_Z_H_
#define _GAME_Z_H_
#include "BaseObject_Z.h"
#include "World_ZHdl.h"
#include "Node_ZHdl.h"
#include "CameraAgent_ZHdl.h"
#include "LodMove_ZHdl.h"
#include "LodAgent_ZHdl.h"
#include "ObjectsGame_ZHdl.h"
#include "RtcAgent_ZHdl.h"
#include "Math_Z.h"
#include "SubWorld_ZHdl.h"

Extern_Z void RegisterGameCommand();
Bool Cmd_LoadSubLevel();
Bool Cmd_RemoveSubLevel();
Bool Cmd_LoadSubData();
Bool Cmd_RemoveSubData();

#define GAME_MESSAGE_TARGET_PLAYER_LOD_AGENTS (1 << 0)
#define GAME_MESSAGE_TARGET_PLAYER_CAMERA_AGENTS (1 << 1)
#define GAME_MESSAGE_TARGET_RTC_AGENTS (1 << 2)
#define GAME_MESSAGE_TARGET_GAME_AGENTS (1 << 3)

class Game_Z : public BaseObject_Z {
    friend class GameManager_Z;

public:
    struct SubInterp_Z {
        U8 m_UnkBytes_0x0[20];
    };

    typedef DynArray_Z<SubInterp_Z, 8, FALSE, TRUE> SubInterp_ZDA;

    Game_Z();

    virtual void Init();
    virtual ~Game_Z();
    virtual Bool MarkHandles();

    static BaseObject_Z* NewObject() { return NewL_Z(149) Game_Z; }

    void DeclareObjectGame(const ObjectGame_ZHdl& i_ObjectGameHdl);
    void InitAgent(Node_Z* i_StartNode, Bool i_RecursiveBelow, Bool i_RecursiveNextTo);
    U32 GetFirstVp() const;
    U32 GetNbVp() const;
    void SetGameWorld(const World_ZHdl& i_WorldHdl, const Char* i_GameName);
    void Update(Float i_DeltaTime);
    void Stream(const Vec3f& i_Pos, Agent_ZHdl i_NotifyAgent, abc_message i_Msg = msg_no_message);
    void SetGamePlayerNb(S32 i_Nb, Bool i_IsMono, const Name_Z& i_CameraAgentClass);
    S32 GetSubId(S32 i_SubDataId, S32 i_Unk);
    void AddSubLevel(const SubWorld_ZHdl& i_SubWorldHdl, S32 i_SubLevelId);
    void SendMessage(U32 i_Target, abc_message i_Message, Float i_Param);

    const World_ZHdl& GetWorld() const {
        return m_WorldHdl;
    }

private:
    World_ZHdl m_WorldHdl;
    Node_ZHdl m_SubRootNodeHdl;
    String_Z<ARRAY_CHAR_MAX> m_GameName;
    CameraAgent_ZHdlDA m_PlayerCamAgentHdls;
    LodMove_ZHdlDA m_PlayerLodMoveHdls;
    LodAgent_ZHdlDA m_PlayerLodAgentHdls;
    Node_ZHdlDA m_PlayerCamNodeHdls;
    ObjectsGame_ZHdl m_ObjectsGameMgrHdl;
    ObjectGame_ZHdlDA m_ObjectGameHdls;
    Agent_ZHdlDA m_GameAgentHdls;
    RtcAgent_ZHdlDA m_RtcAgentHdls;
    S32 m_NbPlayer;
    Bool m_InitAgentRunning;
    S32 m_NbAgent;
    S32 m_InitAgentDepth;
    Agent_ZHdl m_StreamNotifyAgent;
    S32DA m_StreamUnkDA_0x164;
    abc_message m_StreamNotifyMsg;
    Vec3f m_StreamWorldPos;
    BitArray_Z m_StreamUnkBA_0x17c;
    S32DA m_StreamUnkDA_0x188;
    SubInterp_ZDA m_StreamSubInterps;
};

#endif // _GAME_Z_H_
