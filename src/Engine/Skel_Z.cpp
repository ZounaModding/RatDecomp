#include "Skel_Z.h"
#include "AnimationManager_Z.h"
#include "Material_Z.h"
#include "Points_Z.h"

MorphTarget_Z Skel_Z::DfltMorphTarget;

Skel_Z::Skel_Z() {
    m_SkelId = -1;
    m_BSphere.Center = VEC3F_NULL;
    m_BSphere.Radius = 1.0f;
    m_SkelRoot = NULL;
}

Skel_Z::~Skel_Z() {
    Delete_Z[] m_SkelRoot;
    if (gData.AnimMgr && m_SkelId >= 0) {
        gData.AnimMgr->RemoveSkelNodes(m_SkelId);
    }
    m_SkelRoot = NULL;
}

Bool Skel_Z::MarkHandles() {
    if (!ObjectDatas_Z::MarkHandles()) {
        return FALSE;
    }

    m_SkelRoot->MarkHandles();
    S32 i;
    for (i = 0; i < m_MaterialHdls.GetSize(); i++) {
        if (m_MaterialHdls[i]) {
            m_MaterialHdls[i]->MarkHandles();
        }
    }
    for (i = 0; i < m_ObjectDatasHdls.GetSize(); i++) {
        m_ObjectDatasHdls[i]->MarkHandles();
    }
    return TRUE;
}

void Skel_Z::Reset() {
    ResetBoneNodes();
    ResetMorphTarget();
    DefaultObjectsVisibility();
}

void Skel_Z::ResetBoneNodes() {
    SkelNodeArray& l_OriginalNodes = gData.AnimMgr->GetOriginalNode(m_SkelId);
    for (S32 i = 0; i < m_Bones.GetSize() && i < l_OriginalNodes.m_SkelNodes.GetSize(); i++) {
        BoneNode_Z* l_Bone = m_Bones[i];
        BoneNode_Z* l_OriginalBone = l_OriginalNodes.m_SkelNodes[i];
        if (l_Bone && l_OriginalBone) {
            l_Bone->SetTranslation(l_OriginalBone->GetTranslation());
            l_Bone->SetRotation(l_OriginalBone->GetRotation());
            l_Bone->SetScale(l_OriginalBone->GetScale());
            l_Bone->DisableBoneFlag(FL_BONENODE_UNK_0x80);
        }
    }
}

BoneNode_Z* Skel_Z::GetOrgBoneNode(S32 i_Index) {
    SkelNodeArray& l_OriginalNodes = gData.AnimMgr->GetOriginalNode(m_SkelId);
    if (i_Index >= l_OriginalNodes.m_SkelNodes.GetSize() || i_Index >= m_Bones.GetSize()) {
        return NULL;
    }
    return l_OriginalNodes.m_SkelNodes[i_Index];
}

MorphTarget_Z* Skel_Z::GetMorphTargetObject(S16 i_Index) {
    if (i_Index >= m_MorphTargetIds.GetSize()) {
        return &DfltMorphTarget;
    }
    S16 l_MorphTargetId = m_MorphTargetIds[i_Index];
    if (l_MorphTargetId < 0) {
        return &DfltMorphTarget;
    }
    return &m_MorphTargets[l_MorphTargetId];
}

void Skel_Z::ResetMorphTarget() {
    for (S32 i = 0; i < m_MorphTargets.GetSize(); i++) {
        m_MorphTargets[i].m_Morph = 0.0f;
    }
}

// TODO: Finish matching
void Skel_Z::Update() {
    BoneNode_Z* l_Children[32];
    BoneNode_Z* l_Parents[32];
    BoneNode_Z** l_ParentPtr;
    BoneNode_Z* l_Parent = m_SkelRoot;
    BoneNode_Z* l_BoneNode = l_Parent->GetHeadSon();
    S32 l_Depth = 0;

    do {
        if (l_Depth) {
            l_Depth--;
            l_BoneNode = l_Children[l_Depth];
            l_Parent = l_Parents[l_Depth];
        }

        l_ParentPtr = &l_Parents[l_Depth];
        do {
            if (l_BoneNode->GetHeadSon()) {
                *l_ParentPtr++ = l_BoneNode;
                l_Children[l_Depth++] = l_BoneNode->GetHeadSon();
            }

            U16 l_MatrixId = l_BoneNode->GetWorldMatrixId();
            Mat4x4* l_WorldMatrix = gData.MatrixBuffer->GetMatrix(l_MatrixId);
            l_BoneNode->SetWorldMatrixPtr(l_WorldMatrix);
            l_BoneNode->UpdateTM(l_Parent);
            l_BoneNode->EnableBoneFlag(FL_BONENODE_UNK_0x1 | FL_BONENODE_INVALIDROT);
        } while ((l_BoneNode = l_BoneNode->GetNext()) != NULL);
    } while (l_Depth);
}

// TODO: Finish matching
void Skel_Z::Update(BoneNode_Z* i_BoneNode, Bool i_UpdateSiblings) {
    BoneNode_Z* l_Parent = i_BoneNode->GetParent();
    BoneNode_Z* l_TrueChildren[32];
    BoneNode_Z* l_TrueParents[32];

    if (!i_UpdateSiblings) {
        i_BoneNode->SetWorldMatrixPtr(gData.MatrixBuffer->GetMatrix(i_BoneNode->GetWorldMatrixId()));
        i_BoneNode->UpdateTM(l_Parent);
        i_BoneNode->EnableBoneFlag(FL_BONENODE_UNK_0x1 | FL_BONENODE_INVALIDROT);

        BoneNode_Z* l_Child = i_BoneNode->GetHeadSon();
        if (l_Child) {
            BoneNode_Z* l_Parent = l_Child->GetParent();
            BoneNode_Z* l_Children[32];
            BoneNode_Z* l_Parents[32];
            BoneNode_Z** l_ParentPtr;
            S32 l_Depth = 0;

            do {
                if (l_Depth) {
                    l_Depth--;
                    i_BoneNode = l_Children[l_Depth];
                    l_Parent = l_Parents[l_Depth];
                }

                l_ParentPtr = &l_Parents[l_Depth];
                do {
                    if (i_BoneNode->GetHeadSon()) {
                        *l_ParentPtr++ = i_BoneNode;
                        l_Children[l_Depth++] = i_BoneNode->GetHeadSon();
                    }

                    i_BoneNode->SetWorldMatrixPtr(gData.MatrixBuffer->GetMatrix(i_BoneNode->GetWorldMatrixId()));
                    i_BoneNode->UpdateTM(l_Parent);
                    i_BoneNode->EnableBoneFlag(FL_BONENODE_UNK_0x1 | FL_BONENODE_INVALIDROT);
                } while ((i_BoneNode = i_BoneNode->GetNext()) != NULL);
            } while (l_Depth);
        }
    }
    else {
        BoneNode_Z** l_ParentPtr;
        S32 l_Depth = 0;

        do {
            if (l_Depth) {
                l_Depth--;
                i_BoneNode = l_TrueChildren[l_Depth];
                l_Parent = l_TrueParents[l_Depth];
            }

            l_ParentPtr = &l_TrueParents[l_Depth];
            do {
                if (i_BoneNode->GetHeadSon()) {
                    *l_ParentPtr++ = i_BoneNode;
                    l_TrueChildren[l_Depth++] = i_BoneNode->GetHeadSon();
                }

                U16 l_MatrixId = i_BoneNode->GetWorldMatrixId();
                Mat4x4* l_WorldMatrix = gData.MatrixBuffer->GetMatrix(l_MatrixId);
                i_BoneNode->SetWorldMatrixPtr(l_WorldMatrix);
                i_BoneNode->UpdateTM(l_Parent);
                i_BoneNode->EnableBoneFlag(FL_BONENODE_UNK_0x1 | FL_BONENODE_INVALIDROT);
            } while ((i_BoneNode = i_BoneNode->GetNext()) != NULL);
        } while (l_Depth);
    }
}

SphereColBone_Z* Skel_Z::ItSphere(S32& io_Index, const Name_Z& i_Name) const {
    S32 l_Index = io_Index;
    S32 l_SphereNb = m_AllSpheres.GetSize();
    if (l_Index < 0 || l_Index >= l_SphereNb) {
        return NULL;
    }

    SphereColBone_Z* l_Sphere = m_AllSpheres.GetArrayPtr() + l_Index;
    while (io_Index < l_SphereNb) {
        io_Index++;
        if (l_Sphere->m_Name == i_Name) {
            return l_Sphere;
        }
        l_Sphere++;
    }
    return NULL;
}

Bool Skel_Z::GetLocalMatrix(S32 i_Index, Mat4x4& o_Matrix) {
    BoneNode_Z* l_BoneNode = GetBone(i_Index);
    if (!l_BoneNode) {
        return FALSE;
    }
    o_Matrix = *l_BoneNode->GetWorldMatrixPtr();
    return TRUE;
}

Bool Skel_Z::GetLocalRot(S32 i_Index, Quat& o_Rotation) {
    BoneNode_Z* l_BoneNode = GetBone(i_Index);
    if (!l_BoneNode) {
        return FALSE;
    }
    o_Rotation = l_BoneNode->GetRotInWorld();
    return TRUE;
}

BoneNode_Z* Skel_Z::FindNodeByName(const Name_Z& i_Name) {
    for (S32 i = 0; i < m_Bones.GetSize(); i++) {
        if (GetBone(i)) {
            if (GetBone(i)->GetName() == i_Name) {
                return GetBone(i);
            }
        }
    }

    return NULL;
}

S32 Skel_Z::GetNodeIndex(const BoneNode_Z* i_BoneNode) {
    for (S32 i = 0; i < m_Bones.GetSize(); i++) {
        if (GetBone(i) == i_BoneNode) {
            return i;
        }
    }
    return -1;
}

// TODO: Finish matching
void Skel_Z::Clone(ObjectDatas_ZHdl& o_ObjectDatasHdl, Object_ZHdl& o_ObjectHdl) {
    S32 i;
    Name_Z l_ClassName(Name_Z::GetID("Skel_Z", 0));
    Name_Z& l_SkelName = gData.ClassMgr->GetHandleName(GetHandle());
    o_ObjectDatasHdl = gData.ClassMgr->NewObject(l_ClassName, l_SkelName);
    Skel_Z* l_Skel = (Skel_Z*)GETPTR(o_ObjectDatasHdl);
    ObjectDatas_Z::Clone(o_ObjectDatasHdl, o_ObjectHdl);

    l_Skel->m_BSphere = m_BSphere;
    S32 l_BoneCount = m_SkelRoot->GetNodeCount(0);
    BoneNode_Z* l_BoneNodes = NewL_Z(404) BoneNode_Z[l_BoneCount];
    l_Skel->m_SkelRoot = l_BoneNodes;
    m_SkelRoot->Clone(&l_BoneNodes, TRUE);
    l_Skel->m_SkelRoot->GetAllNodes(l_Skel->m_Bones);

    BoneNodePtr_ZDA l_OrderedBones;
    l_OrderedBones.SetSize(m_Bones.GetSize());
    l_OrderedBones.Null();
    for (i = 0; i < l_Skel->m_Bones.GetSize(); i++) {
        S32 l_NodeId = gData.AnimMgr->GetNodeByName(l_Skel->m_Bones[i]->GetName());
        l_OrderedBones[l_NodeId] = l_Skel->m_Bones[i];
    }
    l_Skel->m_Bones = l_OrderedBones;

    l_Skel->m_SkelId = gData.AnimMgr->AddSkelId(l_SkelName);
    ASSERTLE_Z(m_SkelId == l_Skel->m_SkelId, "", 433, "SkelId==Skel->SkelId");
    l_Skel->m_Bones.SetSize(gData.AnimMgr->GetOriginalNode(m_SkelId).m_SkelNodes.GetSize());

    l_Skel->m_ObjectDatasBoneNodeIds = m_ObjectDatasBoneNodeIds;
    l_Skel->m_ObjectDatasHdls.SetSize(m_ObjectDatasHdls.GetSize());
    for (i = 0; i < m_ObjectDatasHdls.GetSize(); i++) {
        m_ObjectDatasHdls[i]->Clone(l_Skel->m_ObjectDatasHdls[i], o_ObjectHdl);
    }

    l_Skel->m_AllSpheres = m_AllSpheres;
    for (i = 0; i < l_Skel->m_AllSpheres.GetSize(); i++) {
        if (l_Skel->m_AllSpheres[i].m_BoneNode) {
            l_Skel->m_AllSpheres[i].m_BoneNode = l_Skel->FindNodeByName(m_AllSpheres[i].m_BoneNode->GetName());
        }
    }

    l_Skel->m_HitSpheres = m_HitSpheres;
    for (i = 0; i < l_Skel->m_HitSpheres.GetSize(); i++) {
        if (l_Skel->m_HitSpheres[i].m_BoneNode) {
            l_Skel->m_HitSpheres[i].m_BoneNode = l_Skel->FindNodeByName(m_HitSpheres[i].m_BoneNode->GetName());
        }
    }

    l_Skel->m_AllBoxes = m_AllBoxes;
    for (i = 0; i < l_Skel->m_AllBoxes.GetSize(); i++) {
        if (l_Skel->m_AllBoxes[i].m_BoneNode) {
            l_Skel->m_AllBoxes[i].m_BoneNode = l_Skel->FindNodeByName(m_AllBoxes[i].m_BoneNode->GetName());
        }
    }

    l_Skel->m_MaterialHdls.SetSize(m_MaterialHdls.GetSize());
    for (i = 0; i < m_MaterialHdls.GetSize(); i++) {
        if (m_MaterialHdls[i]) {
            m_MaterialHdls[i]->Clone(l_Skel->m_MaterialHdls[i]);
        }
    }

    l_Skel->m_MorphTargets = m_MorphTargets;
    l_Skel->m_MorphTargetIds = m_MorphTargetIds;
}

// TODO: Finish matching
Bool Skel_Z::InsertNode(const Name_Z& i_NodeName, const Name_Z& i_ParentName) {
    BoneNodePtr_ZDA l_BoneNodes;
    m_SkelRoot->GetAllNodes(l_BoneNodes);

    DynArray_Z<Name_Z, 32, FALSE> l_CollisionBoneNames;
    l_CollisionBoneNames.SetSize(m_AllSpheres.GetSize() + m_HitSpheres.GetSize() + m_AllBoxes.GetSize());

    S32 i;
    S32 l_CollisionBoneIndex = 0;
    for (i = 0; i < m_AllSpheres.GetSize(); i++) {
        l_CollisionBoneNames[l_CollisionBoneIndex++] = m_AllSpheres[i].m_BoneNode->GetName();
    }
    for (i = 0; i < m_HitSpheres.GetSize(); i++) {
        l_CollisionBoneNames[l_CollisionBoneIndex++] = m_HitSpheres[i].m_BoneNode->GetName();
    }
    for (i = 0; i < m_AllBoxes.GetSize(); i++) {
        l_CollisionBoneNames[l_CollisionBoneIndex++] = m_AllBoxes[i].m_BoneNode->GetName();
    }

    if (!BoneNode_Z::InsertNode(l_BoneNodes, i_NodeName, i_ParentName)) {
        return FALSE;
    }

    m_SkelRoot = l_BoneNodes[0]->GetRoot();
    m_Bones.SetSize(gData.AnimMgr->GetNbNodes());
    if (m_Bones.GetArrayPtr()) {
        memset(m_Bones.GetArrayPtr(), 0, m_Bones.GetSize() * sizeof(BoneNode_Z*));
    }
    for (i = 0; i < l_BoneNodes.GetSize(); i++) {
        S16 l_NodeId = (S16)gData.AnimMgr->GetNodeByName(l_BoneNodes[i]->GetName());
        m_Bones[l_NodeId] = l_BoneNodes[i];
    }

    l_CollisionBoneIndex = 0;
    for (i = 0; i < m_AllSpheres.GetSize(); i++) {
        m_AllSpheres[i].m_BoneNode = FindNodeByName(l_CollisionBoneNames[l_CollisionBoneIndex++]);
    }
    for (i = 0; i < m_HitSpheres.GetSize(); i++) {
        m_HitSpheres[i].m_BoneNode = FindNodeByName(l_CollisionBoneNames[l_CollisionBoneIndex++]);
    }
    for (i = 0; i < m_AllBoxes.GetSize(); i++) {
        m_AllBoxes[i].m_BoneNode = FindNodeByName(l_CollisionBoneNames[l_CollisionBoneIndex++]);
    }
    return TRUE;
}

// TODO: Finish matching
void Skel_Z::DefaultObjectsVisibility() {
    S16 i;
    S16 j;

    for (i = 0; i < m_ObjectDatasHdls.GetSize(); i++) {
        S16DA& l_BoneNodeIds = m_ObjectDatasBoneNodeIds[i];

        for (j = 0; j < l_BoneNodeIds.GetSize(); j++) {
            m_Bones[l_BoneNodeIds[j]]->DisableBoneFlag(FL_BONENODE_UNK_0x20);
        }

        ObjectDatas_Z* l_ObjectDatas = m_ObjectDatasHdls[i];
        if (l_ObjectDatas) {
            l_ObjectDatas->DisableFlag(
                FL_OBJECTDATAS_UNK_0x4 | FL_OBJECTDATAS_VP0_HIDE | FL_OBJECTDATAS_VP1_HIDE | FL_OBJECTDATAS_VP2_HIDE | FL_OBJECTDATAS_VP3_HIDE
            );
        }
    }
}

Bool Skel_Z::IsBSphere(Node_Z* i_Node, Frustrum_Z& i_Frust, DrawInfo_Z& i_DrawInfo) {
    for (S32 i = 0; i < m_ObjectDatasHdls.GetSize(); i++) {
    }
    return FALSE;
}
