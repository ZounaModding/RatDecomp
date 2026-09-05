#include "ObjectMove_Z.h"
#include "ObjectMoveCollFlag_Z.h"
#include "Math_Z.h"
#include "Spline_Z.h"
#include "Collision_Z.h"
#include "MaterialFlag_Z.h"
#include "ABC_Message_Engine.h"
#include "Agent_Z.h"

void ObjectMove_Z::Init() {
    Manipulator_Z::Init();
    m_TotalSpeed = VEC3F_NULL;
    m_Accel = VEC3F_NULL;
    m_CollisionBools = COL_BOL_DEFAULT;
    m_Flag = 0;
    m_ShadowOldRayLen = -1.0f;
    m_NodeHdl = HANDLE_NULL;
    m_LinkedNodeHdl = HANDLE_NULL;
    m_OwnerHdl = HANDLE_NULL;
    m_StartingLocalPosDA.SetSize(1);
    m_StartingLocalRotDA.SetSize(1);
    m_StartingLocalScaleDA.SetSize(1);
    m_StartingNodeParentHdlDA.SetSize(1);
    m_CollideDataDA.SetSize(1);
    SetGroup(ag_move_first);
    m_CurrentAlignRot = QUAT_NULL;
}

void ObjectMove_Z::Reset() {
    RestoreStartingPos(0);
    m_TotalSpeed = VEC3F_NULL;
    m_Accel = VEC3F_NULL;
    m_LinkedNodeHdl = HANDLE_NULL;
    m_OwnerHdl = HANDLE_NULL;
    m_CollisionBools &= INT_BOL_KEEP;
    EnableFlag(COL_BOL_DEFAULT);
    m_Flag = 0;
    m_ShadowOldRayLen = -1.0f;
    StaticArray_Z<Node_ZHdl, 8>* l_VolumeMemory = GetVolumeMemory(0);
    l_VolumeMemory->Empty();
    m_SplineNodeHdl = HANDLE_NULL;
    m_UnkNodeHdl_0x190 = HANDLE_NULL;
    m_CurrentAlignRot = QUAT_NULL;
}

void ObjectMove_Z::ComputeNewPos(const Vec3f& i_CurPos, Vec3f& o_NewPos, Vec3f& o_Dir, Float i_DeltaTime, S32 i_Index) {
    DisableFlag(~COL_BOL_CLEAN);
    if (IsFlagEnable(COL_BOL_GOTO)) {
        ComputeGotoPos(i_CurPos, o_NewPos, o_Dir, i_DeltaTime);
    }
    else {
        o_NewPos = i_CurPos + m_TotalSpeed * i_DeltaTime;
        o_Dir = GetNode(0)->GetRotInWorldMatrix() * VEC3F_FRONT;
    }
}

void ObjectMove_Z::Update(Float i_DeltaTime) {
    m_TotalSpeed = VEC3F_NULL;
}

void ObjectMove_Z::ResetPosAndRot(const Vec3f& i_Pos, const Quat& i_Rot) {
    SetPosAndRot(i_Pos, i_Rot, 0);
    GetNode(0)->Update();
}

void ObjectMove_Z::SetNodeObject(const Node_ZHdl& i_NodeHdl, S32 i_Index) {
    m_NodeHdl = i_NodeHdl;
    m_AgentHdl = i_NodeHdl->GetAgent();
    StoreStartingPos(i_Index);
}

Bool ObjectMove_Z::ParentLink(S32 i_Index) {
    Node_ZHdl& l_NodeHdl = GetNode(i_Index);
    Node_Z* l_Node = l_NodeHdl;
    Node_Z* l_Parent = m_StartingNodeParentHdlDA[i_Index];
    Node_Z* l_CurParent = l_Node->GetParent();
    if (!l_Parent) {
        return FALSE;
    }
    if (l_CurParent != l_Parent) {
        l_Node->Remove(FALSE, FALSE);
        l_Parent->AddSon(l_NodeHdl, TRUE, TRUE);
    }
    return TRUE;
}

void ObjectMove_Z::RestoreStartingPos(S32 i_Index) {
    m_TotalSpeed = VEC3F_NULL;
    if (ParentLink(i_Index)) {
        Node_Z* l_Node = GetNode(i_Index);
        l_Node->SetTranslation(m_StartingLocalPosDA[i_Index]);
        l_Node->SetRotation(m_StartingLocalRotDA[i_Index]);
        l_Node->SetScale(m_StartingLocalScaleDA[i_Index]);
        l_Node->Changed();
        l_Node->Update();
    }
}

void ObjectMove_Z::StoreStartingPos(S32 i_Index) {
    Node_Z* l_Node = GetNode(i_Index);
    // $SABE: This is ugly as fuck... but it matches so we ball
    m_StartingLocalPosDA.GetArrayPtr()[i_Index] = l_Node->GetTranslation();
    m_StartingLocalRotDA.GetArrayPtr()[i_Index] = l_Node->GetRotation();
    m_StartingLocalScaleDA.GetArrayPtr()[i_Index] = l_Node->GetScale();
    m_StartingNodeParentHdlDA.GetArrayPtr()[i_Index] = l_Node->GetParent()->GetHandle();
}

void ObjectMove_Z::ChangeLink(const Node_ZHdl& i_NodeHdl, Node_Z* i_Parent, Bool i_Update, S32 i_Index) {
    Node_Z* l_Node = i_NodeHdl;
    if (!(l_Node->GetFlag() & FL_NODE_USER_LOCK)) {
        l_Node->Remove(FALSE, FALSE);
        i_Parent->AddSon(i_NodeHdl, TRUE, TRUE);
        if (i_Update) {
            l_Node->Update();
        }
    }
}

void ObjectMove_Z::DeadCodeUseUndefinedFValue() {
    Send(msg_no_message, m_AgentHdl);
}

void ObjectMove_Z::FatherUnLinked(S32 i_Index) {
    Node_ZHdl& l_NodeHdl = GetNode(0);
    Node_Z* l_Node = l_NodeHdl;
    Node_Z* l_Root = l_Node->GetRoot();
    ChangeLink(l_NodeHdl, l_Root, TRUE, 0);
    GetLastEffectiveAccel(0).Set(0.0f, -9.8f, 0.0f);
}

const Vec3f& ObjectMove_Z::GetPos(S32 i_Index) {
    return GetNode(0)->GetWorldTranslation();
}

Float ObjectMove_Z::GetScale(S32 i_Index) {
    return GetNode(0)->GetUniformScale();
}

Quat& ObjectMove_Z::GetRot(S32 i_Index) {
    return GetNode(0)->GetRotInWorld();
}

void ObjectMove_Z::SetPos(const Vec3f& i_Pos, S32 i_Index) {
    GetNode(0)->SetFromWorldTrans(i_Pos);
}

void ObjectMove_Z::SetPosAndRot(const Vec3f& i_Pos, const Quat& i_Rot, S32 i_Index) {
    GetNode(0)->SetFromWorldTransRot(i_Pos, i_Rot);
}

void ObjectMove_Z::SetPosAndScale(const Vec3f& i_Pos, Float i_Scale, S32 i_Index) {
    GetNode(0)->SetFromWorldTransScale(i_Pos, i_Scale);
}

void ObjectMove_Z::SetScale(Float i_Scale, S32 i_Index) {
    GetNode(0)->SetFromWorldScale(i_Scale);
}

void ObjectMove_Z::SetPosRotAndScale(const Vec3f& i_Pos, const Quat& i_Rot, Float i_Scale, S32 i_Index) {
    GetNode(0)->SetFromWorldTransRotScale(i_Pos, i_Rot, i_Scale);
}

void ObjectMove_Z::Update(const Vec3f& i_CurPos, const Vec3f& i_NewPos, const Vec3f& i_Dir, Float i_DeltaTime, S32 i_Index) {
    SetPos(i_NewPos, 0);
}

void ObjectMove_Z::Update(SeadZone_Z& i_Zone, const Vec3f& i_CurPos, const Vec3f& i_NewPos, const Vec3f& i_Dir, Float i_DeltaTime, S32 i_Index) {
    Update(i_CurPos, i_NewPos, i_Dir, i_DeltaTime, 0);
}

void ObjectMove_Z::SetCollideData(const ObjectMoveCollData_Z& i_CollideData, S32 i_Index) {
    m_CollideDataDA[i_Index] = i_CollideData;
}

Float ObjectMove_Z::ComputeGotoPos(const Vec3f& i_CurPos, Vec3f& o_NewPos, Vec3f& o_Dir, Float i_DeltaTime) {
    Float l_Len;
    Float l_Ratio;
    Bool l_Finished = FALSE;
    Quat l_Quat;

    if (m_GotoPosTime < 0.0f) {
        l_Ratio = 1.0f;
        l_Finished = TRUE;
    }
    else {
        l_Ratio = m_GotoPosTime / m_AnimTime;
    }

    if (l_Ratio >= 1.0f) {
        m_GotoPosTime = -1.0f;
        l_Ratio = 0.999999f;
    }
    else if (l_Ratio < 0.001f) {
        l_Ratio = 0.001f;
    }

    if (m_SplineNodeHdl.IsValid()) {
        Node_Z* l_SplineNode = m_SplineNodeHdl;
        Spline_Z* l_Spline = (Spline_Z*)l_SplineNode->GetObjectA();
        l_Len = l_Spline->GetValueOnTrack(m_SplineNodeHdl, l_Ratio, o_NewPos);
        if ((U8)((m_CollisionBools & COL_BOL_GOTOREL_SMOOTH) ? TRUE : FALSE)) {
            Float l_Remain = 1.0f - Min(m_GotoPosTime, 1.0f);
            o_NewPos -= m_SplineWorldDelta * l_Remain;
        }
        else if (IsFlagEnable(COL_BOL_GOTOREL)) {
            o_NewPos -= m_SplineWorldDelta;
        }
        if (!IsFlagEnable(COL_BOL_GOTO_ID) && l_Spline->GetSplineId(l_Ratio) != 0) {
            EnableFlag(COL_BOL_GOTO_ID);
            Send(msg_goto_2nd_vertex, m_AgentHdl);
        }
    }
    else {
        o_NewPos = m_PosD + l_Ratio * (m_PosC + l_Ratio * (m_PosB + l_Ratio * m_PosA));
        l_Len = Spline_Z::CalcSplineLen(m_PosA, m_PosB, m_PosC, m_PosD, l_Ratio);
    }

    if (IsFlagEnable(COL_BOL_GOTOROT)) {
        o_Dir = (o_NewPos - i_CurPos).Normalize();
    }
    else {
        if (m_Omega) {
            Float l_SinInv = O_Sin((1.0f - l_Ratio) * m_Omega);
            Float l_Sin = O_Sin(l_Ratio * m_Omega);
            l_Quat.v = (m_RotStart.v * l_SinInv + m_RotEnd.v * l_Sin) * m_Sinh;
            l_Quat.w = m_Sinh * (m_RotStart.w * l_SinInv + m_RotEnd.w * l_Sin);
        }
        else {
            l_Quat = m_RotStart;
        }
        o_Dir = l_Quat * Vec3f(0.0f, 0.0f, 1.0f);
    }

    m_GotoPosTime += i_DeltaTime;
    if (l_Finished) {
        Abort();
    }
    else {
        Send(msg_goto_curlength, m_AgentHdl, l_Len);
    }
    return l_Len;
}

void ObjectMove_Z::Abort() {
    if (IsFlagEnable(COL_BOL_GOTO)) {
        Send(msg_goto_end, m_AgentHdl);
        if (m_UnkNodeHdl_0x190.IsValid()) {
            if (m_UnkNodeHdl_0x190->GetAgent().IsValid()) {
                Agent_Z* l_Agent = m_UnkNodeHdl_0x190->GetAgent();
                Agent_Z* l_Sender = m_AgentHdl;
                l_Sender->Send(msg_goto_end, l_Agent);
            }
        }
    }
    if (IsFlagEnable(COL_BOL_THROWN)) {
        Send(msg_thrown_end, m_AgentHdl);
    }
    DisableFlag(COL_BOL_GOTO | COL_BOL_THROWN);
    m_Accel = VEC3F_NULL;
    m_TotalSpeed = m_Accel;
    m_SplineNodeHdl = HANDLE_NULL;
    m_UnkNodeHdl_0x190 = HANDLE_NULL;
}

Bool ObjectMove_Z::UpdateCollisionLineFlag(const ColLineResult_Z& i_Result, const Vec3f& i_Pos, S32 i_Index) {
    UpdateCollisionMsg(i_Result.m_Node, i_Result.m_Intersection, i_Result.m_Flag, 0);
    return TRUE;
}

Bool ObjectMove_Z::UpdateCollisionSphereFlag(const ColSphereResult_Z& i_Result, const Vec3f& i_Pos) {
    UpdateCollisionMsg(i_Result.m_Node, i_Result.m_Intersection, i_Result.m_Flag, 0);
    return TRUE;
}

void ObjectMove_Z::UpdateCollisionMsg(Node_Z* i_Node, const Vec3f& i_Pos, U64 i_Flag, S32 i_Index) {
    Agent_Z* l_Agent = m_AgentHdl;
    if (!(i_Flag & FL_MTL_COL_DEPTH)) {
        l_Agent->Send(msg_decor_collide, l_Agent);
    }
}
