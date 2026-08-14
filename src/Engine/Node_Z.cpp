#include "Node_Z.h"
#include "MatrixCache_Z.h"
#include "World_Z.h"

// TODO: Finish matching
Node_Z::Node_Z()
    : m_RotInWorld(1.0f, 0.0f, 0.0f, 0.0f)
    , m_Rotation(1.0f, 0.0f, 0.0f, 0.0f) {
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
    return FALSE;
}

Node_ZHdl& Node_Z::GetRoot() {
}

void Node_Z::AddSon(const Node_ZHdl& i_Son, Bool i_IsWorldRelative, Bool i_Changed) {
}

void Node_Z::Remove(Bool a1, Bool a2) {
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
            Ref(l_WorldMgr->GetWorld(l_WorldId), GetObject(FALSE));
        }
    }
    if (i_Recursive) {
        UnHideSons();
    }
}

void Node_Z::Update() {
}

void Node_Z::Changed() {
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

void Node_Z::SetFromWorldTransRot(const Vec3f& i_Trans, const Quat& i_Rot) {
}

void Node_Z::SetFromWorldTrans(const Vec3f& i_Trans) {
}

void Node_Z::SetFromWorldTransScale(const Vec3f& i_Trans, Float i_Scale) {
}

void Node_Z::SetFromWorldScale(Float i_Scale) {
}

void Node_Z::SetFromWorldTransRotScale(const Vec3f& i_Trans, const Quat& i_Rot, const Float& i_Scale) {
}
