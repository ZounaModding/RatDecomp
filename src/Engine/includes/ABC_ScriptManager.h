#ifndef _ABC_SCRIPT_MANAGER_H_
#define _ABC_SCRIPT_MANAGER_H_
#include "Renderer_Z.h"
#include "Node_ZHdl.h"
#include "Game_ZHdl.h"
#include "World_ZHdl.h"
#include "StreamAgent_ZHdl.h"
#include "ABC_Agent.h"
#include "BnkLinkArray_Z.h"
class Message_Z;
class RegMessage_Z;

typedef BnkLinkArray_Z<ABC_Message> MessageBnkLinkArray;
typedef BnkLinkArray_Z<ABC_Behavior> BehaviorBnkLinkArray;
typedef BnkLinkArray_Z<ABC_BehaviorHolder> BehaviorHolderBnkLinkArray;
typedef BnkLinkArray_Z<ABC_CategoryHolder> CategoryHolderBnkLinkArray;
typedef BnkLinkArray_Z<ABC_CategoryData> CategoryDataBnkLinkArray;

struct Param_Z {
};

class ABC_ScriptManager {

private:
    Bool m_IsPaused;
    Float m_ScriptTime;
    Float m_CycleDuration;
    U32 m_CycleCount;
    Float m_DisplayMaxUsageTime; // MonopolyX360SUB.xdb
    StreamAgent_ZHdl m_StreamAgentHdl;
    ABC_Message* m_TimedMessages;
    HashName_ZTable_Z m_MsgEnum;

public:
    virtual ~ABC_ScriptManager();
    virtual void Init();
    virtual void Shut();
    virtual void MarkHandles();
    virtual void Draw(DrawInfo_Z& i_DrawInfo);
    virtual void ReadEnumFromFiles();
    virtual void NoteTrackInterpMessage(StaticArray_Z<Param_Z, 16, 1, 1>&, Message_Z&);
    virtual void NoteTrackGlobalFlag(Message_Z& a1);
    virtual void MateriaInterpColFlag(const Char* a1);
    virtual void MateriaInterpRdrFlag(const Char* a1);
    virtual void MateriaInterpObjFlag(const Char* a1);
    virtual void MateriaRemoveColFlag(const Char* a1);
    virtual void MateriaRemoveRdrFlag(const Char* a1);
    virtual void MateriaRemoveObjFlag(const Char* a1);
    virtual void InterGraphFlag(const Char* a1);
    virtual void InterGraphFlag(U64 a1);
    virtual void GameSet(const Game_ZHdl& i_GameHdl);
    virtual void GameAgentSet(const Game_ZHdl& i_GameHdl);
    virtual void RemoveGame(const Game_ZHdl& i_GameHdl);
    virtual void GameRemoved();
    virtual void GameReseted(const Game_ZHdl& i_GameHdl);
    virtual void ActivateGame(const Game_ZHdl& i_GameHdl);
    virtual void ViewportDone(const Game_ZHdl& i_GameHdl);
    virtual void WorldAdded(const World_ZHdl& a1);
    virtual void UpdateIndependentResources(const World_ZHdl& a1);
    virtual void InterpKeyframeMsg(const RegMessage_Z& a1) {};
    virtual void StreamRestored(const Node_ZHdl& a1);
    virtual void StreamRemoving(const Game_ZHdl& i_GameHdl, const Node_ZHdl& a2);
    virtual void StreamDone(const Game_ZHdl& i_GameHdl, const Node_ZHdl& a2);
    virtual void StreamDone(const Game_ZHdl& i_GameHdl, S32 a2);
    virtual void Update(Float a1);
    virtual void Minimize();

    void RemoveAgent(ABC_AgentHdl& i_AgentHdl);
    void FlushTimedMessagesTo(ABC_Agent* i_Agent, abc_message i_Msg = msg_no_message);
    void SetAgentState(ABC_Agent* i_Agent, abc_agentState i_State);
    ABC_CategoryData* GetNewCategory();

    Float GetTime() { return m_ScriptTime; }

private:
    // $VIOLET: TODO: add more defs
    U32 m_AgentCountMaybe;
    ABC_AgentList_Z m_AgentsNoPause[agent_last];
    ABC_AgentList_Z m_Agents[agent_last];
    MessageBnkLinkArray m_MessageReserve;
    BehaviorBnkLinkArray m_BehaviorReserve;
    BehaviorHolderBnkLinkArray m_BehaviorHolderReserve;
    CategoryHolderBnkLinkArray m_CategoryHolderReserve;
    CategoryDataBnkLinkArray m_CategoryDataReserve;
};

#endif
