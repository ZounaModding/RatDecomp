#include "BoneNode_Z.h"
#include "Program_Z.h"
#include "MatrixCache_Z.h"

BoneNode_Z::BoneNode_Z() {
    SetWorldMatrixId(gData.MatrixBuffer->GetNewMatrix());
    SetInverseWorldMatrixId(gData.MatrixBuffer->GetNewMatrix());
    SetWorldMatrixPtr(gData.MatrixBuffer->GetMatrix(GetWorldMatrixId()));
    SetInverseWorldMatrixPtr(gData.MatrixBuffer->GetMatrix(GetInverseWorldMatrixId()));
}

BoneNode_Z::~BoneNode_Z() {
    S16 l_WorldMatId = GetWorldMatrixId();
    S16 l_InvWorldMatId = GetInverseWorldMatrixId();
    if (gData.MatrixBuffer) {
        gData.MatrixBuffer->RemoveMatrix(l_WorldMatId);
        gData.MatrixBuffer->RemoveMatrix(l_InvWorldMatId);
        SetWorldMatrixId(0);
        SetInverseWorldMatrixId(0);
    }
}

void BoneNode_Z::MarkHandles() {
    if (m_UserDefineHdl.IsValid()) {
        m_UserDefineHdl->MarkHandles();
    }

    for (BoneNode_Z* l_Son = GetHeadSon(); l_Son; l_Son = l_Son->GetNext()) {
        l_Son->MarkHandles();
    }
}

BoneNode_Z* BoneNode_Z::GetRoot() {
    BoneNode_Z* l_Root = this;
    while (l_Root->GetParent()) {
        l_Root = l_Root->GetParent();
    }
    return l_Root;
}

S32 BoneNode_Z::GetNodeCount(S32 i_NodeCount) {
    i_NodeCount++;
    for (BoneNode_Z* l_Son = GetHeadSon(); l_Son; l_Son = l_Son->GetNext()) {
        i_NodeCount = l_Son->GetNodeCount(i_NodeCount);
    }
    return i_NodeCount;
}

void BoneNode_Z::GetAllSons(BoneNodePtr_ZDA& o_BoneNodes, Bool i_AddNext) const {
    o_BoneNodes.Add((BoneNode_Z*)this);
    if (GetHeadSon()) {
        GetHeadSon()->GetAllSons(o_BoneNodes, TRUE);
    }
    if (i_AddNext && GetNext()) {
        GetNext()->GetAllSons(o_BoneNodes, TRUE);
    }
}

void BoneNode_Z::GetAllNodes(BoneNodePtr_ZDA& o_BoneNodes) {
    GetAllSons(o_BoneNodes, TRUE);
}

void BoneNode_Z::DisableBoneFlag(U32 i_Flag, Bool i_Recursive) {
    DisableBoneFlag(i_Flag);
    if (i_Recursive) {
        for (BoneNode_Z* l_Son = GetHeadSon(); l_Son; l_Son = l_Son->GetNext()) {
            l_Son->DisableBoneFlag(i_Flag, TRUE);
        }
    }
}

void BoneNode_Z::AddSon(BoneNode_Z* i_Son, Bool i_IsWorldRelative) {
    i_Son->SetParent(this);
    i_Son->SetNext(GetHeadSon());
    i_Son->SetPrev(NULL);
    if (GetHeadSon()) {
        GetHeadSon()->SetPrev(i_Son);
    }
    SetHeadSon(i_Son);

    if (i_IsWorldRelative) {
        Mat4x4 l_LocalMatrix = GetInverseWorldMatrix() * i_Son->GetWorldMatrix();
        Quat l_LocalRotation = GetInverseRotInWorld();
        l_LocalRotation *= i_Son->GetRotInWorld();
        Vec3f l_Scale;
        l_LocalMatrix.GetScale(l_Scale);
        i_Son->SetScale(l_Scale);
        i_Son->SetRotation(l_LocalRotation);
        i_Son->SetTranslation(l_LocalMatrix.GetMatrixTrans());
    }
}

BoneNode_Z* BoneNode_Z::Clone(BoneNode_Z** io_BoneNode, Bool i_CloneSons) {
    BoneNode_Z* l_Clone = *io_BoneNode;
    *io_BoneNode = l_Clone + 1;
    l_Clone->SetHeadSon(NULL);
    l_Clone->SetParent(NULL);
    l_Clone->SetPrev(NULL);
    l_Clone->SetNext(NULL);
    l_Clone->SetTranslation(GetTranslation());
    l_Clone->SetRotation(GetRotation());
    l_Clone->SetScale(GetScale());
    l_Clone->m_Trs.m_Flag = m_Trs.m_Flag;
    l_Clone->SetRotInWorld(GetRotInWorld());
    Mat4x4* l_WorldMatrix = GetWorldMatrixPtr();
    l_Clone->SetWorldMatrixPtr(gData.MatrixBuffer->GetMatrix(l_Clone->GetWorldMatrixId()));
    l_Clone->SetWorldMatrix(*l_WorldMatrix);
    l_Clone->EnableBoneFlag(FL_BONENODE_UNK_0x1);
    l_Clone->SetName(GetName());
    l_Clone->m_UserDefineHdl = m_UserDefineHdl;

    if (i_CloneSons) {
        for (BoneNode_Z* l_Son = GetHeadSon(); l_Son; l_Son = l_Son->GetNext()) {
            BoneNode_Z* l_ClonedSon = l_Son->Clone(io_BoneNode, TRUE);
            l_Clone->AddSon(l_ClonedSon, FALSE);
        }
    }
    return l_Clone;
}

void BoneNode_Z::Update(BoneNode_Z* i_Parent) {
    UpdateTM(i_Parent);
    for (BoneNode_Z* l_Son = GetHeadSon(); l_Son; l_Son = l_Son->GetNext()) {
        l_Son->Update(this);
    }
}

void BoneNode_Z::InitNode() {
    if (GetParent()) {
        UpdateTM(GetParent());
    }
    else {
        Mat4x4 l_LocalMatrix;
        l_LocalMatrix.SetTRS(m_Trs.m_Translation, m_Trs.m_Rotation, m_Trs.m_Scale);
        *GetWorldMatrixPtr() = l_LocalMatrix;
        SetRotInWorld(m_Trs.m_Rotation);
    }
    EnableBoneFlag(FL_BONENODE_UNK_0x1 | FL_BONENODE_INVALIDROT);
    if (GetHeadSon()) {
        GetHeadSon()->InitNode();
    }
    if (GetNext()) {
        GetNext()->InitNode();
    }
    DisableBoneFlag(FL_BONENODE_UNK_0x8 | FL_BONENODE_UNK_0x10);
}

// TODO: Finish matching
void BoneNode_Z::BuildSameLink(const BoneNodePtr_ZDA& i_BoneNodes, BoneNode_Z* i_BoneNode) {
    if (GetParent()) {
        i_BoneNode->SetParent(GetBoneNode(i_BoneNodes, GetParent()->GetName()));
    }
    if (GetNext()) {
        i_BoneNode->SetNext(GetBoneNode(i_BoneNodes, GetNext()->GetName()));
    }
    if (GetPrev()) {
        i_BoneNode->SetPrev(GetBoneNode(i_BoneNodes, GetPrev()->GetName()));
    }
    if (GetHeadSon()) {
        i_BoneNode->SetHeadSon(GetBoneNode(i_BoneNodes, GetHeadSon()->GetName()));

        BoneNode_Z* l_Son = GetHeadSon();
        BoneNode_Z* l_NewSon = i_BoneNode->GetHeadSon();
        while (l_Son) {
            l_Son->BuildSameLink(i_BoneNodes, l_NewSon);
            l_Son = l_Son->GetNext();
            l_NewSon = l_NewSon->GetNext();
        }
    }
}
