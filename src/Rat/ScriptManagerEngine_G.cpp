#include "ScriptManager_G.h"
#include "ActionHelper_G.h"
#include "EnemyGenerator_G.h"
#include "FootPrints_G.h"
#include "GameManager_Z.h"
#include "IT_Break.h"
#include "IT_Carrying.h"
#include "IT_Condition.h"
#include "IT_Switch.h"
#include "LiquidFlow_G.h"
#include "MathTools_Z.h"
#include "ObjectAgent_G.h"
#include "Omni_Z.h"
#include "PointJump_G.h"
#include "Program_Z.h"
#include "Ropes_G.h"
#include "Smell_G.h"
#include "ScriptExternalCommands_G.h"
#include "TextGameDraw_G.h"
#include "WanderingPath_G.h"
#include "Node_Z.h"
#include "World_Z.h"

void ScriptManager_G::ReadEnumFromFiles() {
}

void ScriptManager_G::NoteTrackInterpMessage(StaticArray_Z<Param_Z, 16, 1, 1>& i_Params, Message_Z& i_Message) {
}

U32 ScriptManager_G::MateriaRemoveColFlag(const Char* i_Flag) {
    return 0;
}

U32 ScriptManager_G::MateriaInterpColFlag(const Char* i_Flag) {
    return 0;
}

U32 ScriptManager_G::MateriaInterpObjFlag(const Char* i_Flag) {
    return 0;
}

void ScriptManager_G::RemoveGame(const Game_ZHdl& i_GameHdl) {
    DeleteOmniForFX();
    RemoveLogicAgent(i_GameHdl);
    m_TextGameDrawMgrHdl->Minimize();
    m_ActionHelperMgrHdl->Minimize();
    m_FootPrintsMgrHdl->Minimize();
    RemoveAIDummies(i_GameHdl);
    m_RopesMgrHdl->Minimize();
    m_SmellMgrHdl->Minimize();
    m_LiquidFlowMgrHdl->Minimize();
    m_ConditionMgrHdl->Minimize();
    m_WanderingPathMgrHdl->Minimize();
    m_PointJumpMgrHdl->Minimize();
}

void ScriptManager_G::ActivateGame(const Game_ZHdl& i_GameHdl) {
}

void ScriptManager_G::SetLevelObjectsFromSave() {
}

void ScriptManager_G::GameSet(const Game_ZHdl& i_GameHdl) {
    CreateOmniForFX(i_GameHdl);
    m_PhysicWorldHdl = gData.ClassMgr->NewObject(Name_Z(Name_Z::GetID("PhysicWorld_G")), Name_Z(Name_Z::GetID("PhysicWorld_G")));

    m_TextGameDrawMgrHdl->Reset();
    i_GameHdl->GetWorld()->AddManipulatorSceneDraw(m_TextGameDrawMgrHdl);
    m_ActionHelperMgrHdl->Reset();
    i_GameHdl->GetWorld()->AddManipulatorSceneDraw(m_ActionHelperMgrHdl);
    m_FootPrintsMgrHdl->Reset();
    i_GameHdl->GetWorld()->AddManipulatorSceneDraw(m_FootPrintsMgrHdl);
    m_RopesMgrHdl->Reset();
    i_GameHdl->GetWorld()->AddManipulatorSceneDraw(m_RopesMgrHdl);
    i_GameHdl->GetWorld()->AddManipulatorSceneDraw(m_InGameFXMgrHdl);
    m_SmellMgrHdl->Reset();
    i_GameHdl->GetWorld()->AddManipulatorSceneDraw(m_SmellMgrHdl);
    m_PointJumpMgrHdl->Reset();
    m_ConditionMgrHdl->Reset();
    m_LiquidFlowMgrHdl->Reset();
    i_GameHdl->GetWorld()->AddManipulatorSceneDraw(m_LiquidFlowMgrHdl);
}

void ScriptManager_G::GameReseted(const Game_ZHdl& i_GameHdl) {
    IT_Break::ResetAll();
    IT_Carrying::ResetAll();
    IT_Switch::ResetAll();
}

void ScriptManager_G::UpdateIndependentResources(const World_ZHdl& i_WorldHdl) {
    S32 l_GameId = gData.GameMgr->GetGameIdByWorld(i_WorldHdl);
    if (l_GameId >= 0) {
        gData.GameMgr->GetGame(l_GameId)->SendMessage(
            GAME_MESSAGE_TARGET_PLAYER_CAMERA_AGENTS, msg_anim_framelink, -1.0f
        );
    }
}

// TODO: Finish matching
void ScriptManager_G::GameAgentSet(const Game_ZHdl& i_GameHdl) {
    Game_Z* l_Game = i_GameHdl;
    l_Game->DeclareObjectGame(m_PhysicWorldHdl);

    ObjectGame_ZHdl l_ObjectGameHdl;
    l_ObjectGameHdl = gData.ClassMgr->NewObject("Game_SoundMgr");
    l_Game->DeclareObjectGame(l_ObjectGameHdl);
    m_GameSoundMgrHdl = (Game_SoundMgrHdl&)l_ObjectGameHdl;

    l_ObjectGameHdl = gData.ClassMgr->NewObject("ProGroundMgr_G");
    l_Game->DeclareObjectGame(l_ObjectGameHdl);
    m_ProGroundMgrHdl = (ProGroundMgr_GHdl&)l_ObjectGameHdl;

    l_ObjectGameHdl = gData.ClassMgr->NewObject("GameParticleMgr_G");
    l_Game->DeclareObjectGame(l_ObjectGameHdl);
    m_GameParticleMgrHdl = (GameParticleMgr_GHdl&)l_ObjectGameHdl;

    l_ObjectGameHdl = gData.ClassMgr->NewObject("ItemMgr_G");
    l_Game->DeclareObjectGame(l_ObjectGameHdl);
    m_ItemMgrHdl = (ItemMgr_GHdl&)l_ObjectGameHdl;

    l_ObjectGameHdl = gData.ClassMgr->NewObject("CreaturesMgr_G");
    l_Game->DeclareObjectGame(l_ObjectGameHdl);
    m_CreaturesMgrHdl = (CreaturesMgr_GHdl&)l_ObjectGameHdl;

    l_ObjectGameHdl = gData.ClassMgr->NewObject("EnemyGenerator_G");
    l_Game->DeclareObjectGame(l_ObjectGameHdl);
    m_EnemyGeneratorMgrHdl = (EnemyGenerator_GHdl&)l_ObjectGameHdl;
}

void ScriptManager_G::StreamDone(const Game_ZHdl& i_GameHdl, const Node_ZHdl& i_NodeHdl) {
    GetAIDummies(i_GameHdl);
    m_TextGameDrawMgrHdl->StreamDone(i_GameHdl, i_NodeHdl);
    m_ActionHelperMgrHdl->StreamDone(i_GameHdl, i_NodeHdl);
    m_EnemyGeneratorMgrHdl->ParseHierarchy(i_NodeHdl->GetParent(), NULL);
    ParseHierarchy(i_NodeHdl, FALSE);
}

void ScriptManager_G::StreamRemoving(const Game_ZHdl& i_GameHdl, const Node_ZHdl& i_NodeHdl) {
    m_TextGameDrawMgrHdl->StreamRemoving(i_GameHdl, i_NodeHdl);
    m_ActionHelperMgrHdl->StreamRemoving(i_GameHdl, i_NodeHdl);
    CheckNodeOnAIDummies(i_NodeHdl);
    m_EnemyGeneratorMgrHdl->StreamRemoving(i_NodeHdl);
    ParseHierarchy(i_NodeHdl, TRUE);
}

void ScriptManager_G::StreamDone(const Game_ZHdl& i_GameHdl, S32 i_StreamId) {
}

void ScriptManager_G::RemoveAIDummies(const Game_ZHdl& i_GameHdl) {
    m_FXOmnis.Minimize();
    m_AIDummies_0x6650.Minimize();
    m_AIDummies_0x6658.Minimize();
}

void ScriptManager_G::GetAIDummies(const Game_ZHdl& i_GameHdl) {
    RemoveAIDummies(i_GameHdl);
    ParseAIHierarchy(i_GameHdl->GetWorld()->GetRoot());
}

void ScriptManager_G::CheckNodeOnAIDummies(const Node_ZHdl& i_NodeHdl) {
}

void ScriptManager_G::ParseAIHierarchy(Node_Z* i_Node) {
}

void ScriptManager_G::InterpKeyframeMsg(const RegMessage_Z& i_Message) {
}

void ScriptManager_G::UpdateOmnis(Float i_DeltaTime) {
    for (S32 i = 0; i < m_FXOmnis.GetSize(); ++i) {
        if (m_FXOmnis[i].m_IsActive) {
            Float l_Blink = 0.0f;
            Float l_Duration = m_FXOmnis[i].m_Duration;
            if (l_Duration != l_Blink) {
                if (m_FXOmnis[i].m_Time > m_FXOmnis[i].m_Duration) {
                    m_FXOmnis[i].m_OmniHdl->SetActive(FALSE);
                    m_FXOmnis[i].m_IsActive = FALSE;
                }
                else {
                    Vec3f l_Color = m_FXOmnis[i].m_Color;

                    switch (m_FXOmnis[i].m_Type) {
                        case FX_OMNI_INTERP_LINEAR:
                            l_Color *= 1.0f - m_FXOmnis[i].m_Time / m_FXOmnis[i].m_Duration;
                            break;
                        case FX_OMNI_INTERP_SINE:
                            l_Color = m_FXOmnis[i].m_Color * O_Sin(Pi * (0.5f * (1.0f - m_FXOmnis[i].m_Time / m_FXOmnis[i].m_Duration)));
                            break;
                        case FX_OMNI_INTERP_DOUBLE_SINE: {
                            Float l_Factor = O_Sin(Pi * (0.5f * (1.0f - m_FXOmnis[i].m_Time / m_FXOmnis[i].m_Duration)));
                            l_Color = m_FXOmnis[i].m_Color * O_Sin(Pi * (0.5f * l_Factor));
                            break;
                        }
                        case FX_OMNI_INTERP_DOUBLE_SINE_BLINK: {
                            Float l_Ratio = m_FXOmnis[i].m_Time / m_FXOmnis[i].m_Duration;
                            if (l_Ratio < FX_OMNI_BLINK_FIRST_END) {
                                l_Blink = 1.0f;
                            }
                            else if (!(l_Ratio < FX_OMNI_BLINK_SECOND_START)) {
                                if (l_Ratio < FX_OMNI_BLINK_SECOND_END) {
                                    l_Blink = 1.0f;
                                }
                                else if (!(l_Ratio < FX_OMNI_BLINK_FINAL_START)) {
                                    l_Blink = 1.0f;
                                }
                            }
                            Float l_Factor = O_Sin(Pi * (0.5f * (1.0f - m_FXOmnis[i].m_Time / m_FXOmnis[i].m_Duration)));
                            l_Factor = O_Sin(Pi * (0.5f * l_Factor));
                            l_Color = m_FXOmnis[i].m_Color * l_Factor * l_Blink;
                            break;
                        }
                        case FX_OMNI_INTERP_COUNT:
                            break;
                    }

                    m_FXOmnis[i].m_OmniHdl->SetColor(l_Color);
                }

                m_FXOmnis[i].m_NodeHdl->Changed();
                m_FXOmnis[i].m_NodeHdl->Update();
            }

            m_FXOmnis[i].m_Time += i_DeltaTime;
        }
    }
}

void ScriptManager_G::CreateOmniForFX(const Game_ZHdl& i_GameHdl) {
    if (!m_OmniNodeHdlForFX.IsValid()) {
        m_OmniHdlForFX = gData.ClassMgr->NewObject("Omni_Z");
        m_OmniNodeHdlForFX = gData.ClassMgr->NewObject("Node_Z");

        Omni_Z* l_Omni = m_OmniHdlForFX;
        Node_Z* l_Node = m_OmniNodeHdlForFX;
        l_Omni->SetColor(Vec3f(1.0f, 1.0f, 1.0f));
        l_Omni->SetActive(FALSE);
        l_Omni->SetStart(4.0f);
        l_Omni->SetEnd(5.0f);
        l_Omni->GetBSphere().Center = VEC3F_NULL;

        i_GameHdl->GetWorld()->GetRoot()->AddSon(m_OmniNodeHdlForFX, FALSE, TRUE);
        l_Node->SetObject(m_OmniHdlForFX);
        l_Node->Changed();
        l_Node->Update();
    }
}

void ScriptManager_G::DeleteOmniForFX() {
}

void ScriptManager_G::UpdatePlatForms(Float i_DeltaTime) {
    Float l_Duration = PLTF02_GROUP::animDefaultDuration;
    if (IsPaused()) {
        return;
    }
    if (l_Duration > 0.0f) {
        m_SynchedPlatformsTime += i_DeltaTime;
        if (m_SynchedPlatformsTime > l_Duration) {
            m_SynchedPlatformsTime -= l_Duration;
        }
    }
}

void ScriptManager_G::ParseHierarchy(const Node_ZHdl& i_NodeHdl, Bool i_Add) {
    Node_Z* l_Nodes[1024];
    Char l_Command[64];
    Char l_Param1[64];
    Char l_Param2[64];
    Char l_Param3[64];
    Char l_Param4[64];
    U32 l_Length;

    S32 l_NodeCount = 0;
    l_Nodes[l_NodeCount++] = i_NodeHdl->GetHeadSon();
    do {
        Node_Z* l_Node = l_Nodes[--l_NodeCount];
        while (l_Node) {
            if (l_Node->GetHeadSon()) {
                l_Nodes[l_NodeCount++] = l_Node->GetHeadSon();
            }

            if (l_Node->GetObject()) {
                UserDefine_Z* l_UserDefine = l_Node->GetUserDefine();
                if (l_UserDefine) {
                    Char* l_Line = l_UserDefine->GetFirstCommand(l_Length);
                    Node_ZHdl l_NodeHdl = l_Node->GetHandle();
                    while (l_Line) {
                        EXT_InterpLine(l_Line, l_Command, l_Param1, l_Param2, l_Param3, l_Param4);
                        l_Line = l_UserDefine->GetNextCommand(l_Length);
                        if (!i_Add && stricmp(l_Command, "VISION") == 0) {
                            l_Node->EnableFlag(FL_NODE_SPECIAL_VISION);
                        }
                    }
                }
            }

            l_Node = l_Node->GetNext();
        }
    } while (l_NodeCount != 0);
}
