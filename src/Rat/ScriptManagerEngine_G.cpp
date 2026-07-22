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
#include "TextGameDraw_G.h"
#include "WanderingPath_G.h"
#include "Node_Z.h"

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

void ScriptManager_G::StreamDone(const Game_ZHdl& i_GameHdl, S32 i_StreamId) {
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

void ScriptManager_G::UpdateOmnis(Float i_DeltaTime) {
    for (S32 i = 0; i < m_FXOmnis.GetSize(); ++i) {
        FXOmni& l_FXOmni = m_FXOmnis[i];
        if (!l_FXOmni.m_IsActive) {
            continue;
        }

        if (l_FXOmni.m_Duration != 0.0f) {
            if (l_FXOmni.m_Time > l_FXOmni.m_Duration) {
                l_FXOmni.m_OmniHdl->SetActive(FALSE);
                l_FXOmni.m_IsActive = FALSE;
            }
            else {
                Vec3f l_Color = l_FXOmni.m_Color;
                Float l_Ratio = l_FXOmni.m_Time / l_FXOmni.m_Duration;

                switch (l_FXOmni.m_Type) {
                    case FX_OMNI_INTERP_LINEAR: {
                        Float l_Factor = 1.0f - l_Ratio;
                        l_Color *= l_Factor;
                        break;
                    }
                    case FX_OMNI_INTERP_SINE: {
                        Float l_Factor = O_Sin(0.5f * Pi * (1.0f - l_Ratio));
                        l_Color *= l_Factor;
                        break;
                    }
                    case FX_OMNI_INTERP_DOUBLE_SINE: {
                        Float l_Factor = O_Sin(0.5f * Pi * (1.0f - l_Ratio));
                        l_Factor = O_Sin(0.5f * Pi * l_Factor);
                        l_Color *= l_Factor;
                        break;
                    }
                    case FX_OMNI_INTERP_DOUBLE_SINE_BLINK: {
                        Float l_Blink = 0.0f;
                        if (l_Ratio < FX_OMNI_BLINK_FIRST_END || (l_Ratio >= FX_OMNI_BLINK_SECOND_START && l_Ratio < FX_OMNI_BLINK_SECOND_END) || l_Ratio >= FX_OMNI_BLINK_FINAL_START) {
                            l_Blink = 1.0f;
                        }
                        Float l_Factor = O_Sin(0.5f * Pi * (1.0f - l_Ratio));
                        l_Factor = O_Sin(0.5f * Pi * l_Factor);
                        l_Color *= l_Factor * l_Blink;
                        break;
                    }
                }

                l_FXOmni.m_OmniHdl->SetColor(l_Color);
            }

            l_FXOmni.m_NodeHdl->Changed();
            l_FXOmni.m_NodeHdl->Update();
        }

        l_FXOmni.m_Time += i_DeltaTime;
    }
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
