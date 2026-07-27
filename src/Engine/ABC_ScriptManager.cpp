#include "ABC_ScriptManager.h"
#include "LodDynAgent_Z.h"
#include "Program_Z.h"
#include "CameraAgent_Z.h"
#include "SkinAgent_Z.h"
#include "StreamAgent_Z.h"
#include "LodAgent_Z.h"
#include "Breakable_Z.h"
#include "RtcAgent_Z.h"
#include "SubLevelVolumeAgent_Z.h"
#include "ParticlesAgent_Z.h"
#include "AnimationManager_Z.h"
#include "SoundManager_Z.h"
#include "File_Z.h"
#include "Types_Z.h"

static Name_Z InHeritName(Name_Z::GetID("ABC_Agent", 0));

void InitEngineScriptClasses() {
    INIT_AGENT_CLASS(CameraAgent_Z);
    INIT_AGENT_CLASS(Agent_Z);
    INIT_AGENT_CLASS(StreamAgent_Z);
    INIT_AGENT_CLASS(AnimatedMsgAgent_Z);
    INIT_AGENT_CLASS(AnimatedAgent_Z);
    INIT_AGENT_CLASS(LodDynAgent_Z);
    INIT_AGENT_CLASS(SkinAgent_Z);
    INIT_AGENT_CLASS(MovingAgent_Z);
    INIT_AGENT_CLASS(Throwable_Z);
    INIT_AGENT_CLASS(LodAgent_Z);
    INIT_AGENT_CLASS(Breakable_Z);
    INIT_AGENT_CLASS(RtcAgent_Z);
    INIT_AGENT_CLASS(VolumeAgent_Z);
    INIT_AGENT_CLASS(SubLevelVolumeAgent_Z);
    INIT_AGENT_CLASS(ParticlesAgent_Z);
}

ABC_ScriptManager::ABC_ScriptManager()
    : m_MsgEnum(64) {
    m_DisplayMaxUsageTime = 0.0f;
    m_IsPaused = FALSE;
    m_TimedMessages = NULL;
    m_ScriptTime = 0.0f;
    m_CycleDuration = 0.0f;
    m_CycleCount = 0;
}

void ABC_ScriptManager::Init() {
    InitEngineScriptClasses();
}

void ABC_ScriptManager::Shut() {
    SHUT_AGENT_CLASS(CameraAgent_Z);
    SHUT_AGENT_CLASS(Agent_Z);
    SHUT_AGENT_CLASS(StreamAgent_Z);
    SHUT_AGENT_CLASS(AnimatedMsgAgent_Z);
    SHUT_AGENT_CLASS(AnimatedAgent_Z);
    SHUT_AGENT_CLASS(LodDynAgent_Z);
    SHUT_AGENT_CLASS(SkinAgent_Z);
    SHUT_AGENT_CLASS(MovingAgent_Z);
    SHUT_AGENT_CLASS(Throwable_Z);
    SHUT_AGENT_CLASS(LodAgent_Z);
    SHUT_AGENT_CLASS(Breakable_Z);
    SHUT_AGENT_CLASS(RtcAgent_Z);
    SHUT_AGENT_CLASS(VolumeAgent_Z);
    SHUT_AGENT_CLASS(SubLevelVolumeAgent_Z);
    SHUT_AGENT_CLASS(ParticlesAgent_Z);
}

void ABC_ScriptManager::MarkHandles() {
    if (m_StreamAgentHdl.IsValid()) {
        m_StreamAgentHdl->MarkHandles();
    }
}

ABC_ScriptManager::~ABC_ScriptManager() {
    FlushAllTimedMessages();
}

void ABC_ScriptManager::ReadEnumFromFiles() {
    ReadEnumFromFile("..\\Engine\\Includes\\ABC_Message_Engine.h", "msg_", m_MsgEnum);
    ReadEnumFromFile("..\\Engine\\Includes\\Anim_Symbol_Engine.h", "anim_", gData.AnimMgr->GetEnum());
    ReadEnumFromFile("..\\Engine\\Includes\\Sound_Symbol_Engine.h", "snd_", gData.SoundMgr->GetEnum());
}

ABC_AgentHdl ABC_ScriptManager::NewAgent(const Char* i_AgentClassName) {
    return NewAgent(Name_Z(i_AgentClassName));
}

ABC_AgentHdl ABC_ScriptManager::NewAgent(const Name_Z& i_AgentClassName, const Char* i_AgentName) {
    if (InHeritName != i_AgentClassName && !gData.ClassMgr->IsObjectInherit(i_AgentClassName, InHeritName)) {
        return HANDLE_NULL;
    }

    ABC_AgentHdl l_AgentHdl;
    if (!i_AgentName) {
        l_AgentHdl = gData.ClassMgr->NewObject(i_AgentClassName);
    }
    else {
        l_AgentHdl = gData.ClassMgr->NewObject(i_AgentClassName, Name_Z(i_AgentName));
    }

    if (!l_AgentHdl.IsValid()) {
        return HANDLE_NULL;
    }

    l_AgentHdl->SetName(i_AgentClassName);
    if (l_AgentHdl->HasFlag(FL_AGENT_PAUSED)) {
        m_PausedAgentState[l_AgentHdl->GetState()].Add(l_AgentHdl);
    }
    else {
        m_AgentState[l_AgentHdl->GetState()].Add(l_AgentHdl);
    }
    return l_AgentHdl;
}

void ABC_ScriptManager::RemoveAgent(ABC_AgentHdl& i_AgentHdl) {
    ABC_Agent* l_Agent = i_AgentHdl;
    if (!l_Agent) {
        return;
    }

    FlushTimedMessagesTo(i_AgentHdl);

    if (l_Agent->HasFlag(FL_AGENT_PAUSED)) {
        m_PausedAgentState[l_Agent->GetState()].Remove(l_Agent);
    }
    else {
        m_AgentState[l_Agent->GetState()].Remove(l_Agent);
    }
    m_AgentId--;
}

void ABC_ScriptManager::SetAgentState(ABC_Agent* i_Agent, abc_agentState i_State) {
    if (i_Agent->HasFlag(FL_AGENT_PAUSED)) {
        m_PausedAgentState[i_Agent->GetState()].Remove(i_Agent);
        m_PausedAgentState[i_State].Add(i_Agent);
    }
    else {
        m_AgentState[i_Agent->GetState()].Remove(i_Agent);
        m_AgentState[i_State].Add(i_Agent);
    }
}

void ABC_ScriptManager::Update(Float i_DeltaTime) {
    m_CycleCount++;
    CheckTimedMessages(i_DeltaTime);
    gData.ClassMgr->ForbidCheckHandles(TRUE);

    ABC_Agent* l_Agent = m_PausedAgentState[agent_toBeActivated].m_First;
    ABC_Agent* l_NextAgent;
    while (l_Agent) {
        l_NextAgent = l_Agent->m_Next;
        l_Agent->SetState(agent_active);
        l_Agent = l_NextAgent;
    }
    l_Agent = m_PausedAgentState[agent_active].m_First;
    while (l_Agent) {
        l_NextAgent = l_Agent->m_Next;
        l_Agent->Execution();
        l_Agent = l_NextAgent;
    }

    if (!m_IsPaused) {
        if (i_DeltaTime) {
            gData.AnimMgr->SwapMsgArray();
        }
        m_ScriptTime += i_DeltaTime;
        m_CycleDuration = i_DeltaTime;

        l_Agent = m_AgentState[agent_toBeActivated].m_First;
        while (l_Agent) {
            l_NextAgent = l_Agent->m_Next;
            l_Agent->SetState(agent_active);
            l_Agent = l_NextAgent;
        }
        l_Agent = m_AgentState[agent_active].m_First;
        while (l_Agent) {
            l_NextAgent = l_Agent->m_Next;
            l_Agent->Execution();
            l_Agent = l_NextAgent;
        }
    }

    MinimizeReserve();
    gData.ClassMgr->ForbidCheckHandles(FALSE);
}

void ABC_ScriptManager::Draw(DrawInfo_Z& i_DrawInfo) { }

Bool ABC_ScriptManager::Minimize() {
    MinimizeReserve();
    return TRUE;
}

void ABC_ScriptManager::Send(abc_message i_Msg, ABC_Agent* i_Agent, Float i_Param) {
    i_Agent->Receive(i_Msg, BaseObject_ZHdl(0), i_Param);
}

void ABC_ScriptManager::SendTimed(abc_message i_Msg, ABC_Agent* i_Agent, Float i_Delay, Float i_Param) {
    ABC_Message* l_Msg = GetNewMessage();
    l_Msg->m_Id = i_Msg;
    l_Msg->m_Sender = i_Agent->GetHandle();
    l_Msg->m_Param = i_Param;
    l_Msg->m_Time = i_Delay;
    l_Msg->m_Next = m_TimedMessages;
    m_TimedMessages = l_Msg;
}

void ABC_ScriptManager::FlushTimedMessagesTo(ABC_Agent* i_Agent, abc_message i_Msg) {
    ABC_Message* l_Msg = m_TimedMessages;
    ABC_Message* l_PrevMsg = NULL;
    while (l_Msg) {
        if (i_Agent == l_Msg->m_Sender && (i_Msg == msg_no_message || i_Msg == l_Msg->m_Id)) {
            if (l_PrevMsg) {
                l_PrevMsg->m_Next = l_Msg->m_Next;
            }
            else {
                m_TimedMessages = l_Msg->m_Next;
            }
            ReleaseMessage(l_Msg);
            if (l_PrevMsg) {
                l_Msg = l_PrevMsg->m_Next;
            }
            else {
                l_Msg = m_TimedMessages;
            }
        }
        else {
            l_PrevMsg = l_Msg;
            l_Msg = l_Msg->m_Next;
        }
    }
}

void ABC_ScriptManager::FlushAllTimedMessages() {
    ABC_Message* l_Msg;
    while (l_Msg = m_TimedMessages, l_Msg) {
        m_TimedMessages = l_Msg->m_Next;
        ReleaseMessage(l_Msg);
    }
}

void ABC_ScriptManager::CheckTimedMessages(Float i_DeltaTime) {
    ABC_Message* l_Msg = m_TimedMessages;
    ABC_Message* l_PrevMsg = NULL;
    ABC_Message* l_NextMsg;
    ABC_Agent* l_Agent;

    while (l_Msg) {
        l_Agent = (ABC_Agent*)GETPTR(l_Msg->m_Sender);
        if (m_IsPaused && !l_Agent->HasFlag(FL_AGENT_PAUSED)) {
            l_PrevMsg = l_Msg;
            l_Msg = l_Msg->m_Next;
            continue;
        }

        l_Msg->m_Time -= i_DeltaTime;
        l_NextMsg = l_Msg->m_Next;

        if (l_Msg->m_Time <= 0.0f) {
            l_Msg->m_Next = NULL;
            l_Msg->m_Sender = BaseObject_ZHdl(0);

            l_Agent->Receive(l_Msg);

            if (!l_PrevMsg) {
                m_TimedMessages = l_NextMsg;
            }
            else {
                l_PrevMsg->m_Next = l_NextMsg;
            }
        }
        else {
            l_PrevMsg = l_Msg;
        }

        l_Msg = l_NextMsg;
    }
}

// TODO: FIX
StreamAgent_ZHdl& ABC_ScriptManager::SetStreamAgent(const Name_Z& i_AgentClassName) {
    m_StreamAgentHdl = gData.ScriptMgr->NewAgent(i_AgentClassName);
    return m_StreamAgentHdl;
}

void ABC_ScriptManager::RemoveGame(const Game_ZHdl& i_GameHdl) { }

void ABC_ScriptManager::GameRemoved() { }

// TODO: Match properly
void ABC_ScriptManager::ReadEnumFromFile(Char const* i_Path, Char const* i_EnumKey, HashTableBase_Z<Name_ZHash_Z>& i_EnumTab) {
    File_Z l_EnumFile;
    if (!gData.ClassMgr->GetFile(i_Path, l_EnumFile)) {
        return;
    }

    String_Z<ARRAY_CHAR_MAX> l_Str;
    do {
        if (!l_EnumFile.ReadString(l_Str)) break;
    } while (!strstr(l_Str, "{"));

    while (!l_EnumFile.IsEOF()) {
        if (!l_EnumFile.ReadString(l_Str)) {
            break;
        }
        if (strstr(l_Str, "/*") != NULL) {
            const Char* l_Pos = NULL;

            while (!l_EnumFile.IsEOF()) {
                if (!l_EnumFile.ReadString(l_Str)) {
                    break;
                }

                l_Pos = strstr(l_Str, "*/");
                if (l_Pos != NULL) {
                    U32 l_Len = strlen(l_Pos) - 2;

                    strncpy(l_Str, l_Pos + 2, l_Len);
                    l_Str[l_Len] = '\0';

                    break;
                }
            }
            U32 l_Len = strlen(l_Str);
            if (!l_Len) continue;
        }
        else if (strstr(l_Str, "//") != NULL) {
            Char l_C;
            do {
                l_C = l_EnumFile.ReadChar();
            } while (l_C != '\n');
            continue;
        }

        if (strstr(l_Str, "}")) {
            break;
        }

        if (strstr(l_Str, "=") != NULL) {
            continue;
        }

        U32 l_Len = strlen(l_Str) - 1;
        Char* l_Ch = &l_Str[l_Len];

        while (*l_Ch == ' ' || *l_Ch == '\t' || *l_Ch == ',') {
            l_Ch--;
            l_Len--;
        }

        // $SABE: God forgive me
        Name_Z l_Name;
        Char* l_Start = l_Str;
        Char* l_Base = l_Str;
        l_Base++;
        Char* l_End = &l_Base[l_Len];
        *l_End = '\0';
        if (l_Start != NULL) {
            l_Name = Name_Z::GetID(l_Start, 0);
        }
        // $SABE: I deserve eternal suffering
        Name_ZHash_Z l_Entry(l_Name, i_EnumTab.GetNbElem());
        i_EnumTab.Insert(Name_ZHash_Z(l_Name, i_EnumTab.GetNbElem()));
    }
}

static const Char s_Unused_3768[] = "Anonymous Alloc";
static const Char s_Unused_3769[] = "DynArray_Z.h";
