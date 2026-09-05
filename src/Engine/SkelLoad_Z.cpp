#include "Skel_Z.h"
#include "AnimationManager_Z.h"
#include "BoneNode_Z.h"
#include "ClassManager_Z.h"
#include "DynamicAnim2_Z.h"
#include "Main_Z.h"
#include "Material_Z.h"
#include "Memory_Z.h"
#include "Mesh_Z.h"
#include "MeshMorph_Z.h"
#include "Sys_Z.h"

// TODO: Finish matching
void Skel_Z::Load(void** i_Data) {
    S32 l_Nb;
    S32 i;

    ObjectDatas_Z::Load(i_Data);

    LOAD_Z(m_BSphere);
    LOAD_Z(l_Nb);

    m_Bones.SetReserve(gData.AnimMgr->GetNbNodes());
    m_Bones.SetSize(l_Nb, TRUE);

    m_SkelRoot = NewL_Z(25) BoneNode_Z[l_Nb];

    BoneNode_Z* l_Bone = m_SkelRoot;

    for (i = 0; i < l_Nb; i++, l_Bone++) {
        S32 l_Ids[4];
        m_Bones[i] = l_Bone;
        gData.ClassMgr->LoadLink(l_Bone->GetUserDefine(), i_Data);
        U16 l_WorldMatrixId = l_Bone->GetWorldMatrixId();
        U16 l_InverseWorldMatrixId = l_Bone->GetInverseWorldMatrixId();
        LOADRANGE_Z(l_Bone->m_Trs, l_Bone->m_InverseRotInWorldMatrix);
        l_Bone->SetWorldMatrixId(l_WorldMatrixId);
        l_Bone->SetInverseWorldMatrixId(l_InverseWorldMatrixId);
        l_Bone->SetWorldMatrixPtr(gData.MatrixBuffer->GetMatrix(l_WorldMatrixId));
        l_Bone->SetInverseWorldMatrixPtr(gData.MatrixBuffer->GetMatrix(l_InverseWorldMatrixId));
        LOADBYTES_Z(l_Bone->GetWorldMatrixPtr(), sizeof(Mat4x4));
        LOAD_Z(l_Ids);
        if (l_Ids[0] < 0) {
            l_Bone->SetHeadSon(NULL);
        }
        else {
            l_Bone->SetHeadSon(m_SkelRoot + l_Ids[0]);
        }
        if (l_Ids[1] < 0) {
            l_Bone->SetParent(NULL);
        }
        else {
            l_Bone->SetParent(m_SkelRoot + l_Ids[1]);
        }
        if (l_Ids[2] < 0) {
            l_Bone->SetNext(NULL);
        }
        else {
            l_Bone->SetNext(m_SkelRoot + l_Ids[2]);
        }
        if (l_Ids[3] < 0) {
            l_Bone->SetPrev(NULL);
        }
        else {
            l_Bone->SetPrev(m_SkelRoot + l_Ids[3]);
        }
        gData.ClassMgr->LoadName(l_Bone->m_Name, i_Data);
    }
    LOAD_Z(l_Nb);
    m_MaterialHdls.SetSize(l_Nb);
    for (i = 0; i < l_Nb; i++) {
        gData.ClassMgr->LoadLink(m_MaterialHdls[i], i_Data);
    }
    LOAD_Z(l_Nb);
    m_ObjectDatasHdls.SetSize(l_Nb);
    for (i = 0; i < l_Nb; i++) {
        gData.ClassMgr->LoadLink(m_ObjectDatasHdls[i], i_Data);
    }
    LOAD_Z(l_Nb);
    m_ObjectDatasBoneNodeIds.SetSize(l_Nb);
    S32 l_NbId;
    Name_Z l_Name;
    for (i = 0; i < l_Nb; i++) {
        LOAD_Z(l_NbId);
        m_ObjectDatasBoneNodeIds[i].SetSize(l_NbId);
        for (S32 j = 0; j < l_NbId; j++) {
            gData.ClassMgr->LoadName(l_Name, i_Data);
            m_ObjectDatasBoneNodeIds[i][j] = gData.AnimMgr->GetNodeByName(l_Name);
        }
    }
    LOAD_Z(l_Nb);
    m_MorphTargets.SetSize(l_Nb);
    for (i = 0; i < l_Nb; i++) {
        gData.ClassMgr->LoadName(m_MorphTargets[i].m_MeshName, i_Data);
    }
    Load(i_Data, m_HitSpheres);
    Load(i_Data, m_AllSpheres);
    Load(i_Data, m_AllBoxes);
}

void Skel_Z::EndLoad() {
    ObjectDatas_Z::EndLoad();
    S32 i;
    for (i = 0; i < m_ObjectDatasHdls.GetSize(); i++) {
        gData.ClassMgr->UpdateLink(m_ObjectDatasHdls[i]);
    }
    for (i = 0; i < m_MaterialHdls.GetSize(); i++) {
        gData.ClassMgr->UpdateLink(m_MaterialHdls[i]);
    }
    for (i = 0; i < m_Bones.GetSize(); i++) {
        gData.ClassMgr->UpdateLink(m_Bones[i]->GetUserDefine());
    }
    RegisterAndRemap();
}

void Skel_Z::RegisterAndRemap() {
    S32 i;
    m_SkelRoot->InitNode();
    for (i = 0; i < m_Bones.GetSize(); i++) {
        if (m_Bones[i]) {
            gData.AnimMgr->GetNodeByName(m_Bones[i]->GetName());
        }
    }
    const Name_Z& l_SkelName = gData.ClassMgr->GetHandleName(GetHandle());

    m_SkelId = gData.AnimMgr->AddSkelId(l_SkelName);
    SkelNodeArray& l_OriginalNodes = gData.AnimMgr->GetOriginalNode(m_SkelId);
    S32 l_NbNodes = gData.AnimMgr->GetNbNodes();
    if (l_OriginalNodes.m_SkelNodes.GetSize()) {
        l_NbNodes = l_OriginalNodes.m_SkelNodes.GetSize();
    }
    BoneNodePtr_ZDA l_Nodes;

    l_Nodes.SetSize(l_NbNodes);

    l_Nodes.Null();

    BoneNode_Z** l_NodePtr = l_Nodes.GetArrayPtr();

    for (i = 0; i < m_Bones.GetSize(); i++) {
        if (m_Bones[i]) {
            S16 l_NodeId = gData.AnimMgr->GetNodeByName(m_Bones[i]->GetName());
            l_NodePtr[l_NodeId] = m_Bones[i];
        }
    }

    m_Bones = l_Nodes;

    if (!l_OriginalNodes.m_SkelNodes.GetSize()) {
        S32 l_NbBone = m_SkelRoot->GetNodeCount(0);

        BoneNodePtr_ZDA l_Clones;

        l_Clones.SetSize(l_NbBone);

        BoneNode_Z* l_RootBone = NULL;
        BoneNode_Z* l_OrgBones = NewL_Z(158) BoneNode_Z[l_NbBone];
        BoneNode_Z* l_BonePtr = l_OrgBones;

        l_OriginalNodes.m_SkelNodes.SetSize(m_Bones.GetSize());

        S32 j;

        for (j = i = 0; i < m_Bones.GetSize(); i++) {
            if (m_Bones[i]) {
                l_OriginalNodes.m_SkelNodes[i] = m_Bones[i]->Clone(&l_BonePtr, FALSE);
                l_Clones[j++] = l_OriginalNodes.m_SkelNodes[i];

                if (m_Bones[i] == m_SkelRoot) {
                    l_RootBone = l_OriginalNodes.m_SkelNodes[i];
                }
            }
            else {
                l_OriginalNodes.m_SkelNodes[i] = NULL;
            }
        }

        m_SkelRoot->BuildSameLink(l_Clones, l_RootBone);
        gData.AnimMgr->SetOriginalArray(m_SkelId, l_OrgBones);
    }

    m_Bones.Minimize();

    for (i = 0; i < gData.AnimMgr->GetNbNodes(); i++) {
        BoneNode_Z* l_Bone;
        int l_BoneId = i;
        if (i < m_Bones.GetSize()) {
            l_Bone = GetBone(l_BoneId);

            if (l_Bone && DynamicAnim2_Z::IsNodeDynamic(l_Bone, l_BoneId)) {
                l_Bone->EnableBoneFlag(FL_BONENODE_UNK_0x4 | FL_BONENODE_UNK_0x40);
            }
        }
    }

    for (i = 0; i < m_MaterialHdls.GetSize(); i++) {
        if (m_MaterialHdls[i]) {
            gData.AnimMgr->GetMaterialByName(m_MaterialHdls[i]->GetName());
        }
    }

    Material_ZHdlDA l_Materials;

    l_Materials.SetSize(gData.AnimMgr->GetNbMaterials());

    for (i = 0; i < m_MaterialHdls.GetSize(); i++) {
        if (m_MaterialHdls[i]) {
            S32 l_MatId = gData.AnimMgr->GetMaterialByName(m_MaterialHdls[i]->GetName());

            l_Materials[l_MatId] = m_MaterialHdls[i];
        }
    }

    m_MaterialHdls = l_Materials;

    if (m_MorphTargets.GetSize()) {
        for (i = 0; i < m_MorphTargets.GetSize(); i++) {
            gData.AnimMgr->GetMeshByName(m_MorphTargets[i].m_MeshName);
        }

        m_MorphTargetIds.SetSize(gData.AnimMgr->GetNbMeshes());

        for (i = 0; i < m_MorphTargetIds.GetSize(); i++) {
            m_MorphTargetIds[i] = -1;
        }

        for (i = 0; i < m_MorphTargets.GetSize(); i++) {
            m_MorphTargetIds[gData.AnimMgr->GetMeshByName(m_MorphTargets[i].m_MeshName)] = i;
        }

        S32 l_Size = m_MorphTargetIds.GetSize();

        while (m_MorphTargetIds[l_Size - 1] < 0) {
            l_Size--;
        }

        m_MorphTargetIds.SetSize(l_Size);
        m_MorphTargetIds.Minimize();
    }
    else {
        m_MorphTargetIds.Empty();
    }
}

void Skel_Z::Load(void** i_Data, SphereColBone_ZDA& o_Spheres) {
    Name_Z l_Name;
    S32 l_NbSphere;

    LOAD_Z(l_NbSphere);
    o_Spheres.SetSize(l_NbSphere);

    for (S32 i = 0; i < l_NbSphere; i++) {
        Mesh_Z::Load(i_Data, o_Spheres[i]);
        gData.ClassMgr->LoadName(l_Name, i_Data);
        o_Spheres[i].m_BoneNode = FindNodeByName(l_Name);
    }
}

void Skel_Z::Load(void** i_Data, BoxColBone_ZDA& o_Boxes) {
    Name_Z l_Name;
    S32 l_NbBox;

    LOAD_Z(l_NbBox);
    o_Boxes.SetSize(l_NbBox);

    for (S32 i = 0; i < l_NbBox; i++) {
        Mesh_Z::Load(i_Data, o_Boxes[i]);
        gData.ClassMgr->LoadName(l_Name, i_Data);
        o_Boxes[i].m_BoneNode = FindNodeByName(l_Name);
    }
}
