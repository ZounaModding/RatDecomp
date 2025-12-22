#include "AnimFrame_Z.h"
#include "Node_Z.h"
#include "Light_Z.h"
#include "HFog_Z.h"
#include "Omni_Z.h"
#include "AnimationManager_Z.h"
#include "MaterialAnim_Z.h"
#include "Random_Z.h"
#include "Animation_ZHdl.h"
#include "SoundManager_Z.h"
#include "ABC_ScriptManager.h"
#include "Sound_Z.h"
#include "ParticlesAgent_Z.h"
#include "Particles_Z.h"
#include "PlayAnim_Z.h"
#include "AnimatedAgent_Z.h"
#include "Spline_Z.h"

class Sound_Z;

AnimFrame_Z::AnimFrame_Z() {
    m_PlayFlag = 0;
    m_CurTime = 0.0f;
    m_StartTime = 0.0f;
    m_PrevTime = 0.0f;
}

AnimFrame_Z::~AnimFrame_Z() { }

void AnimFrame_Z::ResetFlag() {
    m_PlayFlag &= ~(FL_ANIM_PLAYED | FL_ANIM_PLAY | FL_ANIM_STARTED | FL_ANIM_READMESSAGE | FL_ANIM_MESSAGE | FL_ANIM_PAUSED);

    if (m_PlayFlag & FL_ANIM_AUTOSTART) {
        m_PlayFlag |= (FL_ANIM_PLAY | FL_ANIM_STARTED | FL_ANIM_READMESSAGE);
    }
}

void AnimFrame_Z::ResetAnim() {
    Node_Z* l_Node = m_AnimatedNodeHdl;

    S32 l_NodeFlag = FL_NODE_ANIMATED;

    Light_Z* l_Light;
    HFog_Z* l_Fog;
    Omni_Z* l_Omni;

    GetAnimatedObjects(l_Node, &l_Light, &l_Fog, &l_Omni);

    S32 l_ChangedFlag = FL_NODE_NONE;
    Vec3f l_TransValue;
    Vec3f l_ScaleValue;
    Quat l_RotValue;
    m_StartTime = 0.0f;
    m_PrevTime = 0.0f;
    m_CurTime = Float_Eps;

    if (l_Light || l_Fog) {
        if (m_TransKfr.GetNbKeys()) {
            m_TransKfr.GetValue(m_CurTime, l_TransValue);

            if (l_Light) {
                l_Light->SetTranslation(l_TransValue);
            }
            else {
                l_Fog->SetTranslation(l_TransValue);
            }

            l_ChangedFlag = FL_NODE_UPDATE_OBJECT;
        }
        if (m_RotKfr.GetNbKeys()) {
            m_RotKfr.GetValue(m_CurTime, l_RotValue);

            if (l_Light) {
                l_Light->SetRotation(l_RotValue);
            }
            else {
                l_Fog->SetRotation(l_RotValue);
            }

            l_ChangedFlag = FL_NODE_UPDATE_OBJECT;
        }
        if (l_Fog && m_ScaleKfr.GetNbKeys()) {
            m_ScaleKfr.GetValue(m_CurTime, l_ScaleValue);
            l_Fog->SetScale(l_ScaleValue);
            l_ChangedFlag = FL_NODE_UPDATE_OBJECT;
        }

        if (m_FollowSplineKfr.GetNbKeys()) {
            Vec3f l_TransValue2;
            Quat l_RotValue2;
            Float l_PlayTime = m_CurTime;
            Bool l_Inc = TRUE;
            if (m_TimeKfr.GetNbKeys()) {
                m_TimeKfr.GetValue(m_CurTime, l_PlayTime, l_Inc);

                if (l_PlayTime > 1.0f) {
                    l_PlayTime = 1.0f;
                }

                if (l_PlayTime < 0.0f) {
                    l_PlayTime = 0.0f;
                }
            }

            Bool l_Target = m_FollowSplineKfr.GetValue(l_PlayTime * m_MaxTime, m_MaxTime, l_TransValue2, l_RotValue2, l_Inc);

            if (m_TransKfr.GetNbKeys()) {
                Node_Z* l_Parent = l_Node->GetParent();
                l_TransValue += l_Parent->GetInverseWorldMatrix() * l_TransValue2;
            }
            else {
                Node_Z* l_Parent = l_Node->GetParent();
                l_TransValue = l_Parent->GetInverseWorldMatrix() * l_TransValue2;
            }

            if (l_Light) {
                l_Light->SetTranslation(l_TransValue);
            }
            else {
                l_Fog->SetTranslation(l_TransValue);
            }

            if (l_Target) {
                Node_Z* l_Parent = l_Node->GetParent();
                Quat l_QuatRot;

                l_QuatRot = l_Parent->GetInverseRotInWorld();
                l_QuatRot *= l_RotValue2;

                l_RotValue2 = l_QuatRot;

                if (m_RotKfr.GetNbKeys()) {
                    l_RotValue2 *= l_RotValue;
                }

                if (l_Light) {
                    l_Light->SetRotation(l_RotValue2);
                }
                else {
                    l_Fog->SetRotation(l_RotValue2);
                }
            }

            l_ChangedFlag = FL_NODE_UPDATE_OBJECT;
        }
    }
    else {
        if (m_TransKfr.GetNbKeys()) {
            m_TransKfr.GetValue(m_CurTime, l_TransValue);
            l_Node->SetTranslation(l_TransValue);
            l_ChangedFlag = FL_NODE_UPDATE;
        }
        if (m_RotKfr.GetNbKeys()) {
            m_RotKfr.GetValue(m_CurTime, l_RotValue);
            l_Node->SetRotation(l_RotValue);
            l_ChangedFlag = FL_NODE_UPDATE;
        }
        if (m_ScaleKfr.GetNbKeys()) {
            m_ScaleKfr.GetValue(m_CurTime, l_ScaleValue);
            l_Node->SetScale(l_ScaleValue.x);
            l_ChangedFlag = FL_NODE_UPDATE;
        }

        if (m_FollowSplineKfr.GetNbKeys()) {
            Vec3f l_TransValue2;
            Quat l_RotValue2;
            Float l_PlayTime = m_CurTime;
            Bool l_Inc = TRUE;
            if (m_TimeKfr.GetNbKeys()) {
                m_TimeKfr.GetValue(m_CurTime, l_PlayTime, l_Inc);

                if (l_PlayTime > 1.0f) {
                    l_PlayTime = 1.0f;
                }

                if (l_PlayTime < 0.0f) {
                    l_PlayTime = 0.0f;
                }
            }

            Bool l_Target = m_FollowSplineKfr.GetValue(l_PlayTime * m_MaxTime, m_MaxTime, l_TransValue2, l_RotValue2, l_Inc);

            if (m_TransKfr.GetNbKeys()) {
                Node_Z* l_Parent = l_Node->GetParent();
                l_TransValue += l_Parent->GetInverseWorldMatrix() * l_TransValue2;
            }
            else {
                Node_Z* l_Parent = l_Node->GetParent();
                l_TransValue = l_Parent->GetInverseWorldMatrix() * l_TransValue2;
            }

            l_Node->SetTranslation(l_TransValue);

            if (l_Target) {
                Node_Z* l_Parent = l_Node->GetParent();
                Quat l_QuatRot;

                l_QuatRot = l_Parent->GetInverseRotInWorld();
                l_QuatRot *= l_RotValue2;

                l_RotValue2 = l_QuatRot;

                if (m_RotKfr.GetNbKeys()) {
                    l_RotValue2 *= l_RotValue;
                }

                l_Node->SetRotation(l_RotValue2);
            }
            l_ChangedFlag = FL_NODE_UPDATE;
        }
    }

    if (m_ColorKfr.GetNbKeys()) {
        Vec3f l_ColorValue;
        m_ColorKfr.GetValue(m_CurTime, l_ColorValue);

        if (l_Light) {
            l_Light->SetColor(l_ColorValue);
        }

        if (l_Fog) {
            l_Fog->SetColor(l_ColorValue);
        }

        if (l_Omni) {
            l_Omni->SetColor(l_ColorValue);
        }

        l_ChangedFlag |= FL_NODE_UPDATE_OBJECT;
    }

    if (m_AmbientKfr.GetNbKeys()) {
        Vec3f l_ColorValue;
        m_AmbientKfr.GetValue(m_CurTime, l_ColorValue);

        if (l_Light) {
            l_Light->SetAmbiant(l_ColorValue);
        }

        l_ChangedFlag |= FL_NODE_UPDATE_OBJECT;
    }

    if (l_ChangedFlag) {
        m_AnimatedNodeHdl->Changed(l_ChangedFlag);
    }
    l_Node->EnableFlag(l_NodeFlag, TRUE);
    l_Node->EnableFlag(l_NodeFlag);
    l_Node->DisableFlag(FL_NODE_SEQUENCED_ABORT);

    InitNoteTrack(l_Node);
    m_CurTime = 0.0f;
    m_PrevTime = 0.0f;
    l_Node->Update();
    ResetFlag();
}

Bool AnimFrame_Z::MarkHandles() {
    if (!ResourceObject_Z::MarkHandles()) {
        return FALSE;
    }

    if (m_AnimatedNodeHdl.IsValid()) {
        m_AnimatedNodeHdl->MarkHandles();
    }

    m_FollowSplineKfr.MarkHandles();
    m_StartStopAnimKfr.MarkHandles();
    m_MsgKfr.MarkHandles();

    return TRUE;
}

void AnimFrame_Z::Update() {
    m_PlayFlag &= ~FL_ANIM_MESSAGE;
    if ((m_PlayFlag & FL_ANIM_ON_PLAY) != FL_ANIM_ON_PLAY) {
        return;
    }
    if (m_PlayFlag & FL_ANIM_PLAYED) {
        m_PlayFlag &= ~FL_ANIM_STARTED;
    }
    if (m_PlayFlag & FL_ANIM_UNK_0x200) {
        return;
    }

    Node_Z* l_Node = m_AnimatedNodeHdl;
    if (!l_Node) {
        return;
    }

    if (l_Node->IsFlagEnable(FL_NODE_SEQUENCED_ABORT)) {
        return;
    }

    Light_Z* l_Light;
    HFog_Z* l_Fog;
    Omni_Z* l_Omni;
    S32 l_ChangedFlag = FL_NODE_NONE;

    GetAnimatedObjects(l_Node, &l_Light, &l_Fog, &l_Omni);

    Vec3f l_TransValue;
    Quat l_RotValue;
    Vec3f l_ScaleValue;
    if (l_Light || l_Fog) {
        if (m_TransKfr.GetNbKeys()) {
            m_TransKfr.GetValue(m_CurTime, l_TransValue);

            if (l_Light) {
                l_Light->SetTranslation(l_TransValue);
            }
            else {
                l_Fog->SetTranslation(l_TransValue);
            }

            l_ChangedFlag = FL_NODE_UPDATE_OBJECT;
        }
        if (m_RotKfr.GetNbKeys()) {
            m_RotKfr.GetValue(m_CurTime, l_RotValue);

            if (l_Light) {
                l_Light->SetRotation(l_RotValue);
            }
            else {
                l_Fog->SetRotation(l_RotValue);
            }

            l_ChangedFlag = FL_NODE_UPDATE_OBJECT;
        }
        if (l_Fog && m_ScaleKfr.GetNbKeys()) {
            m_ScaleKfr.GetValue(m_CurTime, l_ScaleValue);
            l_Fog->SetScale(l_ScaleValue);
            l_ChangedFlag = FL_NODE_UPDATE_OBJECT;
        }

        if (m_FollowSplineKfr.GetNbKeys()) {
            Vec3f l_TransValue2;
            Quat l_RotValue2;
            Float l_PlayTime = 1.0f;
            Bool l_Inc = TRUE;
            if (m_TimeKfr.GetNbKeys()) {
                m_TimeKfr.GetValue(m_CurTime, l_PlayTime, l_Inc);

                if (l_PlayTime > 1.0f) {
                    l_PlayTime = 1.0f;
                }

                if (l_PlayTime < 0.0f) {
                    l_PlayTime = 0.0f;
                }
            }

            Bool l_Target = m_FollowSplineKfr.GetValue(l_PlayTime * m_MaxTime, m_MaxTime, l_TransValue2, l_RotValue2, l_Inc);

            if (m_TransKfr.GetNbKeys()) {
                Node_Z* l_Parent = l_Node->GetParent();
                l_TransValue += l_Parent->GetInverseWorldMatrix() * l_TransValue2;
            }
            else {
                Node_Z* l_Parent = l_Node->GetParent();
                l_TransValue = l_Parent->GetInverseWorldMatrix() * l_TransValue2;
            }

            if (l_Light) {
                l_Light->SetTranslation(l_TransValue);
            }
            else {
                l_Fog->SetTranslation(l_TransValue);
            }

            if (l_Target) {
                Node_Z* l_Parent = l_Node->GetParent();
                Quat l_QuatRot;

                l_QuatRot = l_Parent->GetInverseRotInWorld();
                l_QuatRot *= l_RotValue2;

                l_RotValue2 = l_QuatRot;

                if (m_RotKfr.GetNbKeys()) {
                    l_RotValue2 *= l_RotValue;
                }

                if (l_Light) {
                    l_Light->SetRotation(l_RotValue2);
                }
                else {
                    l_Fog->SetRotation(l_RotValue2);
                }
            }

            l_ChangedFlag = FL_NODE_UPDATE_OBJECT;
        }
    }
    else {
        if (m_TransKfr.GetNbKeys()) {
            m_TransKfr.GetValue(m_CurTime, l_TransValue);
            l_Node->SetTranslation(l_TransValue);
            l_ChangedFlag = FL_NODE_UPDATE;
        }
        if (m_RotKfr.GetNbKeys()) {
            m_RotKfr.GetValue(m_CurTime, l_RotValue);
            l_Node->SetRotation(l_RotValue);
            l_ChangedFlag = FL_NODE_UPDATE;
        }
        if (m_ScaleKfr.GetNbKeys()) {
            m_ScaleKfr.GetValue(m_CurTime, l_ScaleValue);
            l_Node->SetScale(l_ScaleValue.x);
            l_ChangedFlag = FL_NODE_UPDATE;
        }

        if (m_FollowSplineKfr.GetNbKeys()) {
            Vec3f l_TransValue2;
            Quat l_RotValue2;
            Float l_PlayTime = 1.0f;
            Bool l_Inc = TRUE;
            if (m_TimeKfr.GetNbKeys()) {
                m_TimeKfr.GetValue(m_CurTime, l_PlayTime, l_Inc);

                if (l_PlayTime > 1.0f) {
                    l_PlayTime = 1.0f;
                }

                if (l_PlayTime < 0.0f) {
                    l_PlayTime = 0.0f;
                }
            }

            Bool l_Target = m_FollowSplineKfr.GetValue(l_PlayTime * m_MaxTime, m_MaxTime, l_TransValue2, l_RotValue2, l_Inc);

            if (m_TransKfr.GetNbKeys()) {
                Node_Z* l_Parent = l_Node->GetParent();
                l_TransValue += l_Parent->GetInverseWorldMatrix() * l_TransValue2;
            }
            else {
                Node_Z* l_Parent = l_Node->GetParent();
                l_TransValue = l_Parent->GetInverseWorldMatrix() * l_TransValue2;
            }

            l_Node->SetTranslation(l_TransValue);

            if (l_Target) {
                Node_Z* l_Parent = l_Node->GetParent();
                Quat l_QuatRot;

                l_QuatRot = l_Parent->GetInverseRotInWorld();
                l_QuatRot *= l_RotValue2;

                l_RotValue2 = l_QuatRot;

                if (m_RotKfr.GetNbKeys()) {
                    l_RotValue2 *= l_RotValue;
                }

                l_Node->SetRotation(l_RotValue2);
            }
            l_ChangedFlag = FL_NODE_UPDATE;
        }
    }

    if (m_ColorKfr.GetNbKeys()) {
        Vec3f l_ColorValue;
        m_ColorKfr.GetValue(m_CurTime, l_ColorValue);

        if (l_Light) {
            l_Light->SetColor(l_ColorValue);
        }

        if (l_Fog) {
            l_Fog->SetColor(l_ColorValue);
        }

        if (l_Omni) {
            l_Omni->SetColor(l_ColorValue);
        }

        l_ChangedFlag |= FL_NODE_UPDATE_OBJECT;
    }

    if (m_AmbientKfr.GetNbKeys()) {
        Vec3f l_ColorValue;
        m_AmbientKfr.GetValue(m_CurTime, l_ColorValue);

        if (l_Light) {
            l_Light->SetAmbiant(l_ColorValue);
        }

        l_ChangedFlag |= FL_NODE_UPDATE_OBJECT;
    }

    if (l_ChangedFlag) {
        l_Node->Changed(l_ChangedFlag);
    }
}

void AnimFrame_Z::UpdateMessage(Float i_DeltaTime) {
    if (m_PlayFlag & (FL_ANIM_PAUSED | FL_ANIM_MESSAGE)) {
        return;
    }

    if (!(m_PlayFlag & FL_ANIM_PLAY)) {
        return;
    }

    if (!(m_PlayFlag & FL_ANIM_STARTED)) {
        return;
    }

    Node_Z* l_Node = m_AnimatedNodeHdl;

    if (l_Node->IsFlagEnable(FL_NODE_SEQUENCED_ABORT)) {
        return;
    }

    if (m_CurTime > m_MaxTime) {
        m_CurTime = 0.0f;
    }

    m_PrevTime = m_CurTime;
    m_CurTime += i_DeltaTime;

    if (m_StartTime) {
        if (m_CurTime > m_StartTime) {
            m_PrevTime = 0;
            Start(m_CurTime - m_StartTime);
        }
        else {
            return;
        }
    }

    if (m_CurTime > m_MaxTime) {
        if (m_PlayFlag & FL_ANIM_PLAYONCE) {
            m_PlayFlag |= FL_ANIM_PLAYED;
            m_CurTime = m_MaxTime;
        }
        while (m_CurTime > m_MaxTime) {
            m_CurTime -= m_MaxTime;
        }
    }

    if ((m_PlayFlag & FL_ANIM_READMESSAGE) && m_StartStopAnimKfr.GetNbKeys()) {
        if (!m_StartStopAnimKfr.GetValue(m_PrevTime, m_CurTime, m_MaxTime, this)) {
            U16 l_TempFlag = m_PlayFlag;
            m_PlayFlag = l_TempFlag | FL_ANIM_ON_PLAY;
            Update();
            m_PlayFlag = l_TempFlag;
        }
    }

    m_PlayFlag |= FL_ANIM_MESSAGE;
    UpdateNoteTrack(l_Node);
}

void AnimFrame_Z::DoStart(Float i_StartTime, Float i_CurTime, Float i_PrevTime) {
    if (
        (!IsOn() && (m_PlayFlag & (FL_ANIM_PLAYED | FL_ANIM_PLAYONCE)) != (FL_ANIM_PLAYED | FL_ANIM_PLAYONCE))
        || (m_PlayFlag & (FL_ANIM_PLAYED | FL_ANIM_NEVERAGAIN | FL_ANIM_PLAYONCE)) == (FL_ANIM_PLAYED | FL_ANIM_PLAYONCE)
    ) {
        m_PlayFlag |= (FL_ANIM_STARTED | FL_ANIM_READMESSAGE);
        m_PlayFlag &= ~FL_ANIM_PLAYED;

        if (m_PlayFlag & FL_ANIM_PAUSED) {
            m_PlayFlag ^= FL_ANIM_PAUSED;
            m_PlayFlag |= FL_ANIM_PLAY;
            return;
        }

        if (i_PrevTime >= 0) {
            m_PrevTime = i_PrevTime;
        }

        m_CurTime = i_CurTime;
        m_StartTime = i_StartTime;

        if (m_StartTime == 0.0f) {
            m_PlayFlag |= FL_ANIM_PLAY;
        }
    }
}

void AnimFrame_Z::Start(Float i_DeltaTime) {
    if (
        (!IsOn() && (m_PlayFlag & (FL_ANIM_PLAYED | FL_ANIM_PLAYONCE)) != (FL_ANIM_PLAYED | FL_ANIM_PLAYONCE))
        || (m_PlayFlag & (FL_ANIM_PLAYED | FL_ANIM_NEVERAGAIN | FL_ANIM_PLAYONCE)) == (FL_ANIM_PLAYED | FL_ANIM_PLAYONCE)
    ) {
        m_PlayFlag |= (FL_ANIM_PLAY | FL_ANIM_STARTED | FL_ANIM_READMESSAGE);
        m_PlayFlag &= ~FL_ANIM_PLAYED;

        if (m_PlayFlag & FL_ANIM_PAUSED) {
            m_PlayFlag ^= FL_ANIM_PAUSED;
        }
        else {
            m_PlayFlag &= ~FL_ANIM_MESSAGE;
            m_StartTime = 0.0f;
            m_CurTime = 0.0f;

            UpdateMessage(i_DeltaTime);
        }
    }
}

void AnimFrame_Z::Stop(Float i_DeltaTime) {
    if (IsOn()) {
        UpdateMessage(i_DeltaTime);

        Update();

        m_PlayFlag |= FL_ANIM_PLAYED;
        m_PlayFlag ^= (FL_ANIM_PLAY | FL_ANIM_READMESSAGE);
    }
}

void AnimFrame_Z::Pause(Float i_DeltaTime) {
    if (IsOn()) {
        UpdateMessage(i_DeltaTime);

        Update();

        m_PlayFlag |= FL_ANIM_PAUSED;
        m_PlayFlag ^= (FL_ANIM_PLAY | FL_ANIM_READMESSAGE);
    }
}

void AnimFrame_Z::SetAnimOnTime(Float i_DeltaTime) {
    gData.AnimMgr->SetUpdateFlag(FL_ANIM_ON_PLAY);
    m_CurTime = i_DeltaTime;
    m_PlayFlag |= FL_ANIM_ON_PLAY;

    Update();

    m_PrevTime = 0.0f;
    m_CurTime = 0.0f;
    m_PlayFlag |= FL_ANIM_STARTED;
    m_PlayFlag &= ~(FL_ANIM_PAUSED | FL_ANIM_MESSAGE | FL_ANIM_PLAYED | FL_ANIM_READMESSAGE);

    UpdateMessage(i_DeltaTime);

    m_PlayFlag &= ~FL_ANIM_MESSAGE;
    gData.AnimMgr->SetUpdateFlag(FL_ANIM_NONE);
}

void AnimFrame_Z::InitNoteTrack(Node_Z* i_Node) {
    if (!m_MsgKfr.GetNbKeys()) {
        return;
    }

    Light_Z* l_Light;
    HFog_Z* l_Fog;
    Omni_Z* l_Omni;

    GetAnimatedObjects(i_Node, &l_Light, &l_Fog, &l_Omni);

    if (!m_MsgKfr.GetNbKeys()) {
        return;
    }

    S32 l_KeyNb = m_MsgKfr.GetNbKeys();
    RegMessage_Z l_MsgBuffer[ANIMFRAME_MAX_MSG_ARRAY_SIZE];
    S32 i;
    RegMessage_Z* l_CurMsg = l_MsgBuffer;
    S32 l_TempNbMsg = 0;

    m_MsgKfr.GetValue(m_PrevTime, m_CurTime, m_MaxTime, l_CurMsg, l_TempNbMsg);

    ASSERTLE_Z(l_TempNbMsg <= ANIMFRAME_MAX_MSG_ARRAY_SIZE, "Increase ANIMFRAME_MAX_MSG_ARRAY_SIZE", 810, "TempNbMsg<=ANIMFRAME_MAX_MSG_ARRAY_SIZE");

    for (i = 0; i < l_TempNbMsg; i++, l_CurMsg++) {
        if (l_CurMsg->m_Id == msg_anim_hide) {
            i_Node->Hide();
        }
        else if (l_CurMsg->m_Id == msg_anim_unhide) {
            i_Node->UnHide();
        }
        else if (l_CurMsg->m_Id == msg_anim_activate_animated_object || l_CurMsg->m_Id == msg_anim_deactivate_animated_object) {
            Bool l_Activate = l_CurMsg->m_Id == msg_anim_activate_animated_object;
            if (l_Light) {
                l_Light->SetActive(l_Activate);
                i_Node->LightingChanged();
            }
            else if (l_Fog) {
                l_Fog->SetActive(l_Activate);
                i_Node->LightingChanged();
            }
            else if (l_Omni) {
                l_Omni->SetActive(l_Activate);
            }
        }
        else if (l_CurMsg->m_Id == msg_anim_hide_node || l_CurMsg->m_Id == msg_anim_unhide_node) {
            Node_Z* l_Node = (Node_Z*)GETPTR(gData.ClassMgr->U32ToHandle(l_CurMsg->m_U32Param));
            if (l_Node) {
                if (l_CurMsg->m_Id == msg_anim_hide_node) {
                    l_Node->Hide();
                }
                else {
                    l_Node->UnHide();
                }
            }
        }
    }

    for (i = 0; i < l_KeyNb; i++) {
        KeyMessage_Z& l_CurKey = m_MsgKfr.m_Keys[i];
        for (S32 j = 0; j < l_CurKey.GetNb(); j++) {
            Message_Z& l_Msg = l_CurKey.Get(j);
            if (l_Msg.m_Id == msg_anim_start_material_anim || l_Msg.m_Id == msg_anim_stop_material_anim || l_Msg.m_Id == msg_anim_pause_material_anim) {
                MaterialAnim_Z* l_MaterialAnim = (MaterialAnim_Z*)GETPTR(gData.ClassMgr->U32ToHandle(l_Msg.m_U32Param));
                l_MaterialAnim->Reset();
            }
        }
    }
}

void AnimFrame_Z::UpdateNoteTrack(Node_Z* i_Node) {
    if (!m_MsgKfr.GetNbKeys()) {
        return;
    }

    S32 l_PrevCount = gData.AnimMgr->GetRegMessageNb();

    RegMessage_Z* l_Msg = gData.AnimMgr->GetRegMessage();

    m_MsgKfr.GetValue(
        m_PrevTime,
        m_CurTime,
        m_MaxTime,
        l_Msg,
        gData.AnimMgr->GetRegMessageNb()
    );

    S32 l_CurCount = gData.AnimMgr->GetRegMessageNb();
    if (l_PrevCount == l_CurCount) {
        return;
    }

    l_Msg = &l_Msg[l_PrevCount];

    Light_Z* l_Light;
    HFog_Z* l_Fog;
    Omni_Z* l_Omni;
    S32 l_ObjType = GetAnimatedObjects(i_Node, &l_Light, &l_Fog, &l_Omni);

    Agent_Z* l_Agent = m_AnimatedNodeHdl->GetAgent();

    U32 l_UpdateFlag = gData.AnimMgr->GetUpdateFlag();

    for (; l_PrevCount < l_CurCount; l_PrevCount++, l_Msg++) {
        l_Msg->m_SkelPtr = NULL;
        l_Msg->m_BoneNodePtr = NULL;
        l_Msg->m_NodePtr = NULL;

        S32 l_ClearCount = 3;
        U32 l_ClearPtr = (U32)l_Msg + 0xc;
        while (l_ClearPtr -= 4, l_ClearCount--) {
            *(U32*)(l_ClearPtr + 0x18) = 0;
        }

        l_Msg->m_NodePtr = i_Node;

        // $SABE: Stupid hack but whatever
        switch (*(abc_message*)l_Msg) {
            case msg_anim_play_sound:
                if (!(l_UpdateFlag & 1)) {
                    if (!(l_Msg->m_FlagParam & 0x80) || !(Random() > 0.5f)) {
                        Sound_ZHdl l_SoundHdl = gData.ClassMgr->U32ToHandle(l_Msg->m_U32Param);

                        gData.SoundMgr->PlaySound(
                            l_SoundHdl,
                            l_Msg->m_FlagParam,
                            m_AnimatedNodeHdl
                        );
                        gData.SoundMgr->SetVolumeFactor(
                            l_Msg->m_FloatParam,
                            l_SoundHdl,
                            m_AnimatedNodeHdl
                        );
                        break;
                    }
                }
                continue;
            case msg_anim_stop_sound:
                if (!(l_UpdateFlag & 1)) {
                    Sound_ZHdl l_SoundHdl = gData.ClassMgr->U32ToHandle(l_Msg->m_U32Param);

                    if (l_SoundHdl.IsValid()) {
                        if (l_SoundHdl->IsFlag(FL_SOUND_LOOPING)) {
                            gData.SoundMgr->StopSound(
                                l_SoundHdl,
                                m_AnimatedNodeHdl,
                                1.0f
                            );
                        }
                        else {
                            gData.SoundMgr->StopSound(
                                l_SoundHdl,
                                m_AnimatedNodeHdl,
                                -1.0f
                            );
                        }
                    }
                    break;
                }
                continue;
            case msg_anim_hide:
                i_Node->Hide();
                break;
            case msg_anim_unhide:
                i_Node->UnHide();
                break;
            case msg_anim_activate_animated_object:
            case msg_anim_deactivate_animated_object: {
                Bool l_Activate = l_Msg->m_Id == msg_anim_activate_animated_object;

                if (l_Light) {
                    l_Light->SetActive(l_Activate);
                    i_Node->LightingChanged();
                }
                else if (l_Fog) {
                    l_Fog->SetActive(l_Activate);
                    i_Node->LightingChanged();
                }
                else if (l_Omni) {
                    l_Omni->SetActive(l_Activate);
                }
                break;
            }
            case msg_anim_hide_node:
            case msg_anim_unhide_node: {
                Node_Z* l_TargetNode = (Node_Z*)GETPTR(
                    gData.ClassMgr->U32ToHandle(l_Msg->m_U32Param)
                );

                if (l_TargetNode) {
                    Agent_Z* l_TargetAgent = l_TargetNode->GetAgent();

                    if (l_Msg->m_Id == msg_anim_hide_node) {
                        l_TargetNode->Hide();
                    }
                    else {
                        l_TargetNode->UnHide();
                    }

                    if (l_TargetAgent) {
                        Send(l_Msg->m_Id, l_TargetAgent, l_PrevCount);
                    }
                }
                break;
            }
            case msg_anim_set_anim:
                if (l_Agent && l_Agent->HasFlag(0x80)) {
                    PlayAnim_Z* l_PlayAnim = ((AnimatedAgent_Z*)l_Agent)->GetPlayAnim();
                    l_PlayAnim->SetAnim(gData.ClassMgr->U32ToHandle(l_Msg->m_U32Param), 0, TRUE, 0.0f);
                }
                break;
            case msg_anim_start_material_anim: {
                MaterialAnim_Z* l_MatAnim = (MaterialAnim_Z*)GETPTR(
                    gData.ClassMgr->U32ToHandle(l_Msg->m_U32Param)
                );
                l_MatAnim->DoStart();
                break;
            }
            case msg_anim_stop_material_anim: {
                MaterialAnim_Z* l_MatAnim = (MaterialAnim_Z*)GETPTR(
                    gData.ClassMgr->U32ToHandle(l_Msg->m_U32Param)
                );
                l_MatAnim->Stop();
                break;
            }
            case msg_anim_pause_material_anim:
                GETPTR(gData.ClassMgr->U32ToHandle(l_Msg->m_U32Param));
                break;
        }

        if (l_ObjType == PARTICLES_Z && l_Agent) {
            Particles_Z* l_Particles = (Particles_Z*)m_AnimatedNodeHdl->GetObjectA();
            if (!(l_UpdateFlag & 2) || l_Particles->HasEmittersFlag(1)) {
                switch (l_Msg->m_Id) {
                    case msg_anim_reset:
                        ((ParticlesAgent_Z*)l_Agent)->Reset();
                        break;
                    case msg_anim_replay:
                        ((ParticlesAgent_Z*)l_Agent)->Replay();
                        break;
                    case msg_anim_pause:
                        ((ParticlesAgent_Z*)l_Agent)->Pause();
                        break;
                    case msg_anim_resume:
                        ((ParticlesAgent_Z*)l_Agent)->Resume();
                        break;
                }
            }
            else {
                continue;
            }
        }

        if (l_Agent) {
            Send(l_Msg->m_Id, l_Agent, l_PrevCount);
        }
        if (gData.ScriptMgr) {
            gData.ScriptMgr->InterpKeyframeMsg(*l_Msg);
        }
    }
}

S32 AnimFrame_Z::GetAnimatedObjects(Node_Z* i_Node, Light_Z** o_Light, HFog_Z** o_Fog, Omni_Z** o_Omni) {
    *o_Light = NULL;
    *o_Fog = NULL;
    *o_Omni = NULL;

    Object_Z* l_Object = i_Node->GetObjectA();

    if (!l_Object) {
        return -1;
    }

    S32 l_Type = l_Object->GetGeometryType();
    if (l_Type == LIGHT_Z) {
        *o_Light = (Light_Z*)l_Object;
        return l_Type;
    }
    if (l_Type == HFOG_Z) {
        *o_Fog = (HFog_Z*)l_Object;
        return l_Type;
    }
    if (l_Type == OMNI_Z) {
        *o_Omni = (Omni_Z*)l_Object;
        return l_Type;
    }
    return l_Type;
}

// TODO: Finish matching
Bool KeyframerFollow_Z::GetValue(Float i_Time, Float i_MaxTime, Vec3f& o_Value, Quat& o_Rot, Bool i_Inc) {
    S32 l_Idx = 0;
    while ((l_Idx < m_Keys.GetSize()) && (m_Keys[l_Idx].GetTime() < i_Time)) {
        l_Idx++;
    }

    --l_Idx;
    if (l_Idx < 0) {
        l_Idx = m_Keys.GetSize() - 1;
    }

    Node_Z* l_Node = m_Keys[l_Idx].GetNode();
    Spline_Z* l_Spline = (Spline_Z*)l_Node->GetObjectA();
    l_Spline->GetValueOnTrack(i_Time / i_MaxTime, o_Value);
    o_Value = l_Node->GetWorldMatrix() * o_Value;

    if (m_Keys[l_Idx].GetOrient() && m_Keys[l_Idx].GetAvance() > 0) {
        Float l_TotalAdvance = i_Time;

        if (i_Inc) {
            l_TotalAdvance += m_Keys[l_Idx].GetAvance();
        }
        else {
            l_TotalAdvance -= m_Keys[l_Idx].GetAvance();
        }

        Float l_Ratio = l_TotalAdvance / i_MaxTime;

        Vec3f l_Target;

        if ((l_Ratio >= 1.0f || l_Ratio < 0.0f) && !l_Spline->IsFlag(FL_OBJECT_IS_SPLINE_TRACK_LOOP)) {
            l_Spline->GetValueOutTrack(l_Ratio, l_Target);
        }
        else {
            while (l_Ratio > 1.0f) {
                l_Ratio -= 1.0f;
            }
            while (l_Ratio < 0.0f) {
                l_Ratio += 1.0f;
            }
            l_Spline->GetValueOnTrack(l_Ratio, l_Target);
        }

        l_Target = l_Node->GetWorldMatrix() * l_Target;
        Vec3f l_LookAt = l_Target - o_Value;
        l_LookAt.Normalize();

        const Vec3f& l_Axis = m_Keys[l_Idx].GetAxis();

        if (l_Axis.x != 0) {
            l_LookAt.x = 0.0f;
        }
        if (l_Axis.y != 0) {
            l_LookAt.y = 0.0f;
        }
        if (l_Axis.z != 0) {
            l_LookAt.z = 1.0f;
        }

        l_LookAt.Normalize();

        Vec3f l_Up(0.0f, 1.0f, 0.0f);
        BuildLookAtRotation(l_LookAt, l_Up, o_Rot);
        return TRUE;
    }
    return FALSE;
}

void KeyframerFollow_Z::MarkHandles() {
    for (S32 i = 0; i < GetNbKeys(); i++) {
        if (m_Keys[i].m_NodeHdl.IsValid()) {
            Node_Z* l_Node = m_Keys[i].m_NodeHdl;
            l_Node->MarkHandles();
        }
    }
}

// TODO: Finish matching
Bool KeyframerStartStop_Z::GetValue(Float i_PrevTime, Float& i_CurTime, Float i_MaxTime, AnimFrame_Z* i_CurAnim) {
    S32 l_KeyIndex0 = -1;
    S32 l_KeyIndex1 = -1;
    S32 l_Index;

    if (i_CurTime < i_PrevTime) {
        if (GetValue(i_PrevTime, i_MaxTime, i_MaxTime, i_CurAnim)) {
            return GetValue(0, i_CurTime, i_MaxTime, i_CurAnim);
        }
        else {
            i_CurTime = i_MaxTime;
            return FALSE;
        }
    }
    else {
        for (l_Index = 0; l_Index < GetNbKeys(); l_Index++) {
            if (m_Keys[l_Index].GetTime() >= i_PrevTime) {
                l_KeyIndex0 = l_Index;
                break;
            }
        }

        for (l_Index = GetNbKeys() - 1; l_Index >= 0; l_Index--) {
            if (m_Keys[l_Index].GetTime() <= i_CurTime) {
                l_KeyIndex1 = l_Index;
                break;
            }
        }

        if (l_KeyIndex1 >= 0 && l_KeyIndex0 >= 0) {
            for (l_Index = l_KeyIndex0; l_Index <= l_KeyIndex1; l_Index++) {
                S32 i;
                for (i = 0; i < m_Keys[l_Index].GetNb(); i++) {
                    StartStop_Z& l_StSt = m_Keys[l_Index].Get(i);
                    AnimFrame_Z* l_Anim = l_StSt.m_AnimHdl;
                    U32 l_Value = l_StSt.m_Value;

                    if (l_Value == VL_STARTSTOP_START) {
                        l_Anim->Start(i_CurTime - m_Keys[l_Index].GetTime());
                    }
                    else if (l_Value == VL_STARTSTOP_STOP) {
                        if (l_Anim == i_CurAnim) {
                            i_CurAnim->m_PlayFlag |= FL_ANIM_PLAYED;
                            i_CurAnim->m_PlayFlag ^= FL_ANIM_PLAY;
                            return FALSE;
                        }
                        l_Anim->Stop(m_Keys[l_Index].GetTime() - i_PrevTime);
                    }
                    else if (l_Value == VL_STARTSTOP_PAUSE) {
                        if (l_Anim == i_CurAnim) {
                            i_CurAnim->m_PlayFlag |= FL_ANIM_PAUSED;
                            i_CurAnim->m_PlayFlag ^= FL_ANIM_PLAY;
                            return FALSE;
                        }
                        l_Anim->Pause(m_Keys[l_Index].GetTime() - i_PrevTime + Float_Eps);
                    }
                }
            }
        }
    }
    return TRUE;
}

void KeyframerStartStop_Z::MarkHandles() {
    for (S32 i = 0; i < GetNbKeys(); i++) {
        KeyStartStop_Z& l_Key = m_Keys[i];
        for (S32 j = 0; j < l_Key.GetNb(); j++) {
            StartStop_Z& l_StartStop = l_Key.m_StartStops[j];
            if (l_StartStop.m_AnimHdl.IsValid()) {
                AnimFrame_Z* l_AnimFrame = l_StartStop.m_AnimHdl;
                l_AnimFrame->MarkHandles();
            }
        }
    }
}
