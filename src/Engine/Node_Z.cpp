#include "Node_Z.h"
#include "Agent_Z.h"
#include "Assert_Z.h"
#include "Light_Z.h"
#include "Lod_Z.h"
#include "MatrixCache_Z.h"
#include "World_Z.h"

// TODO: Finish matching
Node_Z::Node_Z() {
    m_RotInWorld.SetIdentity();
    m_Rotation.SetIdentity();
    m_RotInWorldMatrix.SetIdentity();
    m_Parent = NULL;
    m_HeadSon = NULL;
    m_Prev = NULL;
    m_Next = NULL;
    m_Object = NULL;
    m_ObjectDatas = NULL;
    m_UserDefine = NULL;
    m_LightData = NULL;
    m_HFogData = NULL;
    m_RadiosityBitmap = NULL;
    m_RadiosityNormalBitmap = NULL;
    m_Flag = FL_NODE_COLLIDE;
    m_DisplaySeadRect.Set(-1, -1, -1, -1);
    m_CollideSeadRect.Set(-1, -1, -1, -1);
    SetCollideSeadId(-1);
    SetDisplaySeadId(-1);
    ClrOccluderZoneId();
    SetWorldId(-1);
    SetStart(0.0f);
    SetEnd(0.0f);
    m_Color.Set(1.0f, 1.0f, 1.0f, 1.0f);
    m_Scale = 1.0f;
    m_UniformScale = 1.0f;
    m_InverseUniformScale = 1.0f;
    m_BSphere.Set(VEC3F_NULL, -1.0f);
    SetTranslation(VEC3F_NULL);
    SetWorldMatrixId(gData.MatrixBuffer->GetNewMatrix());
    SetWorldMatrixPtr();
}

Node_Z::~Node_Z() {
    if (gData.MatrixBuffer) {
        gData.MatrixBuffer->RemoveMatrix(GetWorldMatrixId());
    }
}

Bool Node_Z::MarkHandles() {
    if (!ResourceObject_Z::MarkHandles()) {
        return FALSE;
    }

    if (m_Object) {
        m_Object->MarkHandles();
    }

    if (m_ObjectDatas) {
        m_ObjectDatas->MarkHandles();
    }

    if (m_HeadSon) {
        m_HeadSon->MarkHandles();
    }

    if (m_Next) {
        m_Next->MarkHandles();
    }

    if (m_UserDefine) {
        m_UserDefine->MarkHandles();
    }

    if (m_LightData) {
        m_LightData->MarkHandles();
    }

    if (m_HFogData) {
        m_HFogData->MarkHandles();
    }

    if (m_Agent.IsValid()) {
        m_Agent->MarkHandles();
    }

    if (m_RadiosityBitmap) {
        m_RadiosityBitmap->MarkHandles();
    }

    if (m_RadiosityNormalBitmap) {
        m_RadiosityNormalBitmap->MarkHandles();
    }

    return TRUE;
}

Node_ZHdl& Node_Z::GetRoot() {
}

void Node_Z::AddSon(const Node_ZHdl& i_Son, Bool i_IsWorldRelative, Bool i_Changed) {
    ASSERTL_Z(!gData.ClassMgr->IsAsynchDelHandle(GetHandle()), "Node_Z::AddSon during ASynchCheckHandles", 0x89);

    Node_Z* l_Son = i_Son;
    l_Son->SetWorldId(GetWorldId(), TRUE);
    l_Son->m_Parent = this;
    l_Son->m_Next = m_HeadSon;
    l_Son->m_Prev = NULL;
    if (m_HeadSon) {
        m_HeadSon->m_Prev = l_Son;
    }
    m_HeadSon = l_Son;

    if (i_IsWorldRelative) {
        const Mat4x4& l_InverseWorldMatrix = GetInverseWorldMatrix();
        Mat4x4 l_LocalMatrix;
        l_LocalMatrix = l_InverseWorldMatrix * l_Son->GetWorldMatrix();
        Quat l_LocalRotation = GetInverseRotInWorld();
        l_LocalRotation *= l_Son->GetRotInWorld();
        l_Son->SetRotation(l_LocalRotation);
        l_Son->SetTranslation(l_LocalMatrix.GetMatrixTrans());
        Vec3f l_Scale;
        l_LocalMatrix.GetScale(l_Scale);
        l_Son->SetScale(l_Scale.x);
        l_Scale.x *= m_UniformScale;
        l_Son->m_UniformScale = l_Scale.x;
        l_Son->m_InverseUniformScale = 1.0f / l_Scale.x;
    }

    if (i_Changed) {
        l_Son->Changed();
    }
}

void Node_Z::Remove(Bool a1, Bool a2) {
}

void Node_Z::SetWorldId(S16 i_Id, Bool i_Recursive) {
    SetWorldId(i_Id);
    if (i_Recursive) {
        if (GetHeadSon()) {
            GetHeadSon()->SetWorldId(i_Id, i_Recursive);
        }
        if (GetNext()) {
            GetNext()->SetWorldId(i_Id, i_Recursive);
        }
    }
}

void Node_Z::GetByObjectType(Node_ZHdlDA& o_Nodes, S32 i_Type, Bool i_Next) {
    Object_Z* l_Object = GetObject();
    if (l_Object && i_Type == l_Object->GetGeometryType()) {
        o_Nodes.Add(GetHandle());
    }
    if (GetHeadSon()) {
        GetHeadSon()->GetByObjectType(o_Nodes, i_Type, TRUE);
    }
    if (i_Next && GetNext()) {
        GetNext()->GetByObjectType(o_Nodes, i_Type, TRUE);
    }
}

void Node_Z::GetByName(const Name_Z& i_Name, Node_ZHdl& o_Node, Bool i_Next) {
    if (GetName() == i_Name) {
        o_Node = GetHandle();
    }
    if (!o_Node.IsValid()) {
        if (GetHeadSon()) {
            GetHeadSon()->GetByName(i_Name, o_Node, TRUE);
        }
        if (i_Next && GetNext()) {
            GetNext()->GetByName(i_Name, o_Node, TRUE);
        }
    }
}

void Node_Z::GetNodesFromFlag(Node_ZHdlDA& o_Nodes, U32 i_Flag, U32 i_NoFlag, Bool i_Next) {
    if (HasFlag(i_Flag) && !HasFlag(i_NoFlag)) {
        o_Nodes.Add(GetHandle());
    }
    if (GetHeadSon()) {
        GetHeadSon()->GetNodesFromFlag(o_Nodes, i_Flag, i_NoFlag, TRUE);
    }
    if (i_Next && GetNext()) {
        GetNext()->GetNodesFromFlag(o_Nodes, i_Flag, i_NoFlag, TRUE);
    }
}

void Node_Z::HideSons() {
    for (Node_Z* l_Node = m_HeadSon; l_Node; l_Node = l_Node->m_Next) {
        l_Node->Hide(TRUE);
    }
}

void Node_Z::Hide(Bool i_Recursive) {
    if (!(m_Flag & FL_NODE_HIDE)) {
        UnRef();
        m_Flag |= FL_NODE_HIDE;
    }
    if (i_Recursive) {
        HideSons();
    }
}

void Node_Z::UnHideSons() {
    for (Node_Z* l_CurNode = m_HeadSon; l_CurNode; l_CurNode = l_CurNode->m_Next) {
        l_CurNode->UnHide(TRUE);
    }
}

// TODO: Finish matching
void Node_Z::UnHide(Bool i_Recursive) {
    WorldManager_Z* l_WorldMgr = gData.WorldMgr;
    S32 l_WorldId;
    if (m_Flag & FL_NODE_HIDE) {
        m_Flag &= ~FL_NODE_HIDE;
        l_WorldId = GetWorldId();
        if (l_WorldId != -1) {
            l_WorldMgr = gData.WorldMgr;
            Ref(l_WorldMgr->GetWorld(l_WorldId), GetObject());
        }
    }
    if (i_Recursive) {
        UnHideSons();
    }
}

void Node_Z::UnRef() {
    S32 l_WorldId = GetWorldId();
    if (l_WorldId != -1) {
        Object_Z* l_Object = m_Object;
        World_Z* l_World = gData.WorldMgr->GetWorld(l_WorldId);
        if (l_Object) {
            l_World->GetSeadDisplay().UnRef(GetDisplaySeadId(), m_DisplaySeadRect);
            l_World->GetSeadCollide().UnRef(GetCollideSeadId(), m_CollideSeadRect);
        }
        else {
            l_World->GetSeadCollide().UnRef(GetCollideSeadId(), m_CollideSeadRect);
        }
    }
}

void Node_Z::GetAllSons(Node_ZHdlDA& o_Sons, Bool i_Next) const {
    o_Sons.Add(GetHandle());
    if (GetHeadSon()) {
        GetHeadSon()->GetAllSons(o_Sons, TRUE);
    }
    if (i_Next && GetNext()) {
        GetNext()->GetAllSons(o_Sons, TRUE);
    }
}

void Node_Z::GetAllNodes(Node_ZHdlDA& o_Nodes) const {
    GetAllSons(o_Nodes, TRUE);
}

void Node_Z::Clone(Node_ZHdl& o_NodeHdl, Bool i_BuildNonShared) {
    o_NodeHdl = gData.ClassMgr->NewObject(Name_Z(Name_Z::GetID("Node_Z")), gData.ClassMgr->GetHandleName(GetHandle()));
    Node_Z* l_Node = o_NodeHdl;
    l_Node->m_LightData = m_LightData;
    l_Node->m_HFogData = m_HFogData;
    l_Node->SetTranslation(m_Translation);
    l_Node->SetRotation(m_Rotation);
    l_Node->SetScale(m_Scale);
    l_Node->SetFlag(m_Flag & ~FL_NODE_NO_UNSHARED);
    l_Node->m_RotInWorld = m_RotInWorld;
    l_Node->m_RotInWorld.GetMatrix(l_Node->m_RotInWorldMatrix);
    l_Node->EnableFlag(FL_NODE_INVALIDROT);
    Mat4x4* l_WorldMatrix = GetWorldMatrixPtr();
    l_Node->SetWorldMatrixPtr();
    l_Node->SetWorldMatrix(*l_WorldMatrix);
    l_Node->EnableFlag(FL_NODE_INVALIDMAT);
    l_Node->m_Color = m_Color;
    l_Node->Changed();
    l_Node->SetObject(m_Object);
    if (i_BuildNonShared) {
        l_Node->BuildNonShared(FALSE, FALSE);
    }
    else {
        l_Node->m_ObjectDatas = m_ObjectDatas;
    }
}

void Node_Z::BuildNonShared(Bool i_Recursive, Bool i_Next) {
    if (GetObject() && !IsFlagEnable(FL_NODE_NO_UNSHARED) && GetObject()->GetNonShared().IsValid() && !GetObjectDatas()) {
        ObjectDatas_ZHdl l_ObjectDatasHdl;
        Object_ZHdl l_ObjectHdl;
        if (GetObject()) {
            l_ObjectHdl = GetObject()->GetHandle();
        }
        else {
            l_ObjectHdl = HANDLE_NULL;
        }

        GetObject()->GetNonShared()->Clone(l_ObjectDatasHdl, l_ObjectHdl);
        ASSERTLE_Z(l_ObjectDatasHdl.IsValid(), "", 0x248, "ObjectDatasHdl.IsValid()");
        ASSERTLE_Z(
            gData.ClassMgr->GetClassName(l_ObjectDatasHdl) == gData.ClassMgr->GetClassName(GetObject()->GetNonShared()),
            "",
            0x249,
            "gData.ClassMgr->GetClassName(ObjectDatasHdl)==gData.ClassMgr->GetClassName(GetObject()->GetNonShared())"
        );
        m_ObjectDatas = l_ObjectDatasHdl;
        GetObjectDatas()->SetDfltColor(m_Color);
        GetObject()->UpdateObject(this, l_ObjectDatasHdl);
        GetObjectDatas()->UpdatePointers(GetObject(), 0);
    }

    if (i_Recursive) {
        if (GetHeadSon()) {
            GetHeadSon()->BuildNonShared(TRUE, TRUE);
        }
        Node_Z* l_Next = GetNext();
        if (i_Next && l_Next) {
            l_Next->BuildNonShared(TRUE, TRUE);
        }
    }
}

void Node_Z::Update() {
    World_Z* l_World;
    S32 l_WorldId = GetWorldId();
    if (l_WorldId != -1) {
        l_World = gData.WorldMgr->GetWorld(l_WorldId);
    }
    else {
        l_World = NULL;
    }
    Update(l_World, m_Parent);
}

void Node_Z::Update(World_Z* i_World, Node_Z* i_Parent) {
    if (m_Flag & FL_NODE_UPDATE) {
        SetWorldMatrixPtr();
        if (i_Parent) {
            UpdateTM(i_Parent);
        }
        else {
            UpdateRootTM();
        }
        EnableFlag(FL_NODE_INVALIDMAT | FL_NODE_INVALIDROT);

        if (m_Object) {
            m_Object->UpdateObject(this, m_ObjectDatas);
        }
        for (Node_Z* l_Son = m_HeadSon; l_Son; l_Son = l_Son->m_Next) {
            l_Son->EnableFlag(FL_NODE_UPDATE);
            l_Son->Update(i_World, this);
        }
        if (i_World && m_Object) {
            U32 l_ObjectFlag = m_Object->GetFlag();
            if (!(m_Flag & FL_NODE_NO_SEADDISPLAY) && !(l_ObjectFlag & FL_OBJECT_NO_SEADDISPLAY)) {
                i_World->GetSeadDisplay().Ref(GetDisplaySeadId(), this, m_DisplaySeadRect);
            }
            if (!(m_Flag & FL_NODE_NO_SEADCOLLIDE) && !(l_ObjectFlag & FL_OBJECT_NO_SEADCOLLIDE)) {
                i_World->GetSeadCollide().Ref(GetCollideSeadId(), this, m_CollideSeadRect);
            }
        }
        m_Flag &= ~(FL_NODE_UPDATE | FL_NODE_UPDATE_OBJECT | FL_NODE_SCAN);
    }
    else if (m_Flag & FL_NODE_UPDATE_OBJECT) {
        if (m_Object) {
            m_Object->UpdateObject(this, m_ObjectDatas);
        }
        for (Node_Z* l_Son = m_HeadSon; l_Son; l_Son = l_Son->m_Next) {
            l_Son->Update(i_World, this);
        }
        if (i_World && m_Object) {
            U32 l_ObjectFlag = m_Object->GetFlag();
            if (!(m_Flag & FL_NODE_NO_SEADDISPLAY) && !(l_ObjectFlag & FL_OBJECT_NO_SEADDISPLAY)) {
                i_World->GetSeadDisplay().Ref(GetDisplaySeadId(), this, m_DisplaySeadRect);
            }
            if (!(m_Flag & FL_NODE_NO_SEADCOLLIDE) && !(l_ObjectFlag & FL_OBJECT_NO_SEADCOLLIDE)) {
                i_World->GetSeadCollide().Ref(GetCollideSeadId(), this, m_CollideSeadRect);
            }
        }
        m_Flag &= ~(FL_NODE_UPDATE_OBJECT | FL_NODE_SCAN);
    }
    else if (m_Flag & FL_NODE_SCAN) {
        for (Node_Z* l_Son = m_HeadSon; l_Son; l_Son = l_Son->m_Next) {
            l_Son->Update(i_World, this);
        }
        DisableFlag(FL_NODE_SCAN);
    }
}

void Node_Z::SetLighting(LightData_Z* i_LightData, HFogData_Z* i_HFogData, Bool i_KeepExisting) {
    if (i_KeepExisting) {
        if (!m_LightData) {
            m_LightData = i_LightData;
        }
        if (!m_HFogData) {
            m_HFogData = i_HFogData;
        }
    }
    else {
        m_LightData = i_LightData;
        m_HFogData = i_HFogData;
    }

    Object_Z* l_Object = m_Object;
    Node_Z* l_HeadSon = m_HeadSon;
    Bool l_PropagateLighting = TRUE;
    if (l_Object) {
        S32 l_Type = l_Object->GetGeometryType();
        switch (l_Type) {
            case LIGHT_Z:
                if (((Light_Z*)l_Object)->IsActive() && l_HeadSon) {
                    l_PropagateLighting = FALSE;
                    l_HeadSon->SetLighting((LightData_Z*)m_ObjectDatas, m_HFogData, i_KeepExisting);
                }
                break;
            case HFOG_Z:
                if (((HFog_Z*)l_Object)->IsActive() && l_HeadSon) {
                    l_PropagateLighting = FALSE;
                    l_HeadSon->SetLighting(m_LightData, (HFogData_Z*)m_ObjectDatas, i_KeepExisting);
                }
                break;
            case LOD_Z: {
                LodData_Z* l_LodData = (LodData_Z*)m_ObjectDatas;
                LightData_ZHdl l_LightDataHdl;
                l_LodData->SetLightingData(l_LightDataHdl);
                break;
            }
        }
    }
    if (l_PropagateLighting && l_HeadSon) {
        l_HeadSon->SetLighting(m_LightData, m_HFogData, i_KeepExisting);
    }
    if (m_Next) {
        m_Next->SetLighting(m_LightData, m_HFogData, i_KeepExisting);
    }
}

// TODO: Implement this function
LightData_Z* Node_Z::GetFirstDLight() {
    return NULL;
}

// TODO: Implement this function
HFogData_Z* Node_Z::GetFirstHFog() {
    return NULL;
}

void Node_Z::Changed() {
    Changed(FL_NODE_UPDATE);
}

void Node_Z::Changed(S32 i_Flag) {
    EnableFlag(i_Flag);
    Node_Z* l_Parent = GetParent();
    if (!l_Parent) {
        return;
    }

    if (!(l_Parent->GetFlag() & FL_NODE_SCAN)) {
        l_Parent->Changed(FL_NODE_SCAN);
    }
}

void Node_Z::SetTranslation(const Vec3f& i_Translation) {
    m_Translation = i_Translation;
}

void Node_Z::SetRotation(const Quat& i_Rotation) {
    m_Rotation = i_Rotation;
}

void Node_Z::SetScale(Float i_Scale) {
    m_Scale = i_Scale;
}

void Node_Z::SetFromWorldTransEpsilon(Node_Z* i_Parent, const Vec3f& i_Trans) {
    if ((GetTranslation() - i_Trans).GetNorm2() > 1.e-8f) {
        m_Translation = i_Parent->GetInverseWorldMatrix() * i_Trans;
    }
    Changed();
}

void Node_Z::SetFromWorldTransRot(const Vec3f& i_Trans, const Quat& i_Rot) {
}

void Node_Z::SetFromWorldTrans(const Vec3f& i_Trans) {
    SetFromWorldTransEpsilon(m_Parent, i_Trans);
}

void Node_Z::SetFromWorldTransScale(const Vec3f& i_Trans, Float i_Scale) {
}

void Node_Z::SetFromWorldScale(Float i_Scale) {
}

void Node_Z::SetFromWorldTransRotScale(const Vec3f& i_Trans, const Quat& i_Rot, const Float& i_Scale) {
}

void Node_Z::EnableFlag(U32 i_Flag, Bool i_Recursive) {
    EnableFlag(i_Flag);
    if (i_Recursive) {
        for (Node_Z* l_Node = GetHeadSon(); l_Node; l_Node = l_Node->GetNext()) {
            l_Node->EnableFlag(i_Flag, TRUE);
        }
    }
}
