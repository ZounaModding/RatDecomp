#include "PlayAnim_Z.h"
#include "AnimFrame_Z.h"
#include "AnimMessage_Z.h"
#include "DynamicAnim2_Z.h"
#include "ClassManager_Z.h"
#include "Lod_Z.h"
#include "Node_Z.h"
#include "Skel_Z.h"
#include "SequenceAnim_Z.h"

RegMessage_Z MsgSERept[64];

void PlayAnim_Z::Init() {
    Manipulator_Z::Init();
    m_UpdateTimer = 0.0f;
    m_UpdateRate = 0.0f;
    SetGroup(ag_play_animation);
    Reset();
}

// TODO: Finish matching
void PlayAnim_Z::Reset() {
    S32 i;
    for (i = 0; i < MAX_MIX_ANIM; i++) {
        m_MixAnims[i].m_CurTime = 0.0f;
        SetStartTime(0.0f, i);
        SetSmoothTime(0.1f, i);
        SetSpeed(1.0f, i);
        m_MixAnims[i].m_Flag = i ? FL_MIX_ANIM_OVERWRITE : FL_MIX_ANIM_NONE;
        m_MixAnims[i].m_Flag |= FL_MIX_ANIM_RESETED;
        m_MixAnims[i].m_AnimHdl = HANDLE_NULL;
    }

    Skel_Z* l_Skel = m_SkelHdl;
    if (l_Skel) {
        l_Skel->Reset();
    }

    ResetDynamic();
}

Bool PlayAnim_Z::MarkHandles() {
    if (!BaseObject_Z::MarkHandles()) {
        return FALSE;
    }

    DynamicAnim2_Z* l_DynamicAnim = m_DynamicAnimHdl;
    if (l_DynamicAnim) {
        l_DynamicAnim->MarkHandles();
    }

    SequenceAnim_Z* l_SequenceManager = m_SequenceManagerHdl;
    if (l_SequenceManager) {
        l_SequenceManager->MarkHandles();
    }

    return TRUE;
}

void PlayAnim_Z::Update(Float i_DeltaTime) {
}

void PlayAnim_Z::SetAnim(const Animation_ZHdl& i_AnimHdl, S32 i_Priority, Bool i_Smooth, Float i_CurTime) {
}

Bool PlayAnim_Z::SetNode(const Node_ZHdl& i_NodeHdl) {
    m_NodeHdl = i_NodeHdl;

    if (m_NodeHdl.IsValid()) {
        Node_Z* l_Node = m_NodeHdl;
        Object_Z* l_Object = l_Node->GetObjectA();
        S32 l_GeometryType = l_Object->GetGeometryType();
        Skel_ZHdl l_SkelHdl;

        if (l_GeometryType == LOD_Z) {
            Lod_Z* l_Lod = (Lod_Z*)l_Object;
            if (l_Lod->GetNbObject() && l_Lod->GetObjectA(0).IsValid() && l_Lod->GetObjectA(0)->GetGeometryType() == SKIN_Z) {
                l_SkelHdl = ((LodData_Z*)l_Node->GetObjectDatas())->GetObjectA(0);
            }
        }
        else if (l_GeometryType == SKIN_Z) {
            ObjectDatas_Z* l_ObjectDatas = l_Node->GetObjectDatas();
            if (l_ObjectDatas) {
                l_SkelHdl = l_ObjectDatas->GetHandle();
            }
            else {
                l_SkelHdl = HANDLE_NULL;
            }
        }

        if (l_SkelHdl.IsValid() && !(l_SkelHdl == m_SkelHdl)) {
            m_SkelHdl = l_SkelHdl;

            Skel_Z* l_Skel = m_SkelHdl;
            l_Skel->Reset();

            if (!m_DynamicAnimHdl.IsValid() && DynamicAnim2_Z::IsSkelDynamic(l_Skel)) {
                m_DynamicAnimHdl = gData.ClassMgr->NewObject(Name_Z(Name_Z::GetID("DynamicAnim2_Z")), GetName());
            }

            DynamicAnim2_Z* l_DynamicAnim = m_DynamicAnimHdl;
            if (l_DynamicAnim) {
                l_DynamicAnim->SetSkel(m_SkelHdl);
                l_DynamicAnim->SetNode(m_NodeHdl);
            }
        }

        return m_SkelHdl.IsValid();
    }
    else {
        m_NodeHdl = HANDLE_NULL;
        m_SkelHdl = HANDLE_NULL;
        return FALSE;
    }
}

void PlayAnim_Z::ActionOnActivate() {
}

void PlayAnim_Z::ActionOnDeactivate() {
}

void PlayAnim_Z::AddConicForce(const Vec3f& a1, const Vec3f& a2, Float a3, Float a4, Float a5, Float a6, Float a7) {
}

void PlayAnim_Z::AddSphericForce(const Sphere_Z& a1, Float a2, Float a3) {
}

void PlayAnim_Z::ResetDynamic() {
    if (m_DynamicAnimHdl.IsValid()) {
        m_DynamicAnimHdl->Reset();
    }
}
