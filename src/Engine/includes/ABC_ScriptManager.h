#ifndef _ABC_SCRIPT_MANAGER_H_
#define _ABC_SCRIPT_MANAGER_H_
#include "Renderer_Z.h"
#include "Node_ZHdl.h"
#include "Game_ZHdl.h"
#include "World_ZHdl.h"
#include "StreamAgent_ZHdl.h"
#include "ABC_Agent.h"
#include "BnkLinkArray_Z.h"
#include "StaticArray_Z.h"

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
    ABC_ScriptManager();
    virtual ~ABC_ScriptManager();
    virtual void Init();
    virtual void Shut();
    virtual void MarkHandles();
    virtual void Draw(DrawInfo_Z& i_DrawInfo);
    virtual void ReadEnumFromFiles();

    virtual void NoteTrackInterpMessage(StaticArray_Z<Param_Z, 16, 1, 1>&, Message_Z&) { }

    virtual void NoteTrackGlobalFlag(Message_Z& a1);

    virtual U32 MateriaInterpColFlag(const Char* a1) { return 0; }

    virtual U32 MateriaInterpRdrFlag(const Char* a1);

    virtual U32 MateriaInterpObjFlag(const Char* a1) { return 0; }

    virtual U32 MateriaRemoveColFlag(const Char* a1) { return 0; }

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

    virtual void UpdateIndependentResources(const World_ZHdl& a1) { }

    virtual void InterpKeyframeMsg(const RegMessage_Z& a1) { };
    virtual void StreamRestored(const Node_ZHdl& a1);

    virtual void StreamRemoving(const Game_ZHdl& i_GameHdl, const Node_ZHdl& a2) { }

    virtual void StreamDone(const Game_ZHdl& i_GameHdl, const Node_ZHdl& a2) { }

    virtual void StreamDone(const Game_ZHdl& i_GameHdl, S32 a2);
    virtual void Update(Float i_DeltaTime);
    virtual Bool Minimize();

    void RemoveAgent(ABC_AgentHdl& i_AgentHdl);
    void FlushTimedMessagesTo(ABC_Agent* i_Agent, abc_message i_Msg = msg_no_message);
    void SetAgentState(ABC_Agent* i_Agent, abc_agentState i_State);
    ABC_CategoryData* GetNewCategory();
    void ReleaseCategories(ABC_CategoryData* i_CategoryData);
    ABC_Behavior* GetNewBehavior();
    void ReleaseBehaviors(ABC_Behavior* i_Behavior);
    ABC_BehaviorHolder* GetNewBehaviorHolder();
    ABC_CategoryHolder* GetNewCategoryHolder();
    void ReleaseCategoryHolders(ABC_CategoryHolder* i_CategoryHolder);
    ABC_Message* GetNewMessage();
    void ReleaseMessages(ABC_Message* i_Msg);
    void ReleaseMessage(ABC_Message* i_Msg);
    void FlushAllTimedMessages();
    void ReadEnumFromFile(Char const* i_Path, Char const* i_EnumKey, HashTableBase_Z<Name_ZHash_Z>& i_EnumTab);
    ABC_AgentHdl NewAgent(const Char* i_AgentClassName);
    ABC_AgentHdl NewAgent(const Name_Z& i_AgentClassName, const Char* i_AgentName = NULL);
    void CheckTimedMessages(Float i_DeltaTime);
    void MinimizeReserve();
    void Send(abc_message i_Msg, ABC_Agent* i_Agent, Float i_Param = UNDEFINED_FVALUE);
    void SendTimed(abc_message i_Msg, ABC_Agent* i_Agent, Float i_Delay, Float i_Param = UNDEFINED_FVALUE);
    StreamAgent_ZHdl& SetStreamAgent(const Name_Z& i_AgentClassName);

    StreamAgent_ZHdl* GetStreamAgent() { return &m_StreamAgentHdl; }

    Float GetTime() { return m_ScriptTime; }

    inline void EndPause() {
        m_IsPaused = FALSE;
    }

private:
    S32 m_AgentId;
    ABC_AgentList_Z m_PausedAgentState[agent_last];
    ABC_AgentList_Z m_AgentState[agent_last];
    MessageBnkLinkArray m_LinkedMessageReserve;
    BehaviorBnkLinkArray m_LinkedBehaviorReserve;
    BehaviorHolderBnkLinkArray m_LinkedBehaviorHolderReserve;
    CategoryHolderBnkLinkArray m_LinkedCategoryHolderReserve;
    CategoryDataBnkLinkArray m_LinkedCategoryDataReserve;
};

// External functions

void InitEngineScriptClasses();
Extern_Z void InitGameScriptClasses();

#endif
