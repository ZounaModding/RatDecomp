#include "Mesh_Z.h"
#include "AnimationManager_Z.h"
#include "ClassManager_Z.h"
#include "Main_Z.h"
#include "Sys_Z.h"

void Mesh_Z::Load(void** i_Data) {
    Points_Z::Load(i_Data);

    S32 i;
    S32 j;
    S32 l_Nb;
    LOADDYNARRAY_Z(m_TextureCoordinates);
    LOADDYNARRAY_Z(m_Normals);

    LOAD_Z(l_Nb);
    m_Strips.SetSize(l_Nb);
    for (i = 0; i < l_Nb; i++) {
        S32 l_VertexNb;
        LOAD_Z(l_VertexNb);
        m_Strips[i].m_VertexIndices.SetSize(l_VertexNb);
        LOADARRAY_Z(m_Strips[i].m_VertexIndices.GetArrayPtr(), l_VertexNb);
        LOADBYTES_Z(&m_Strips[i].m_MaterialId, sizeof(U32) * 2);
    }
    if (IsFlag(FL_OBJECT_SKINNED)) {
        for (j = 0; j < l_Nb; j++) {
            LOAD_Z(m_Strips[j].m_MaterialId);
        }
    }

    LOAD_Z(l_Nb);
    m_StripExtensions.SetSize(l_Nb);
    for (i = 0; i < l_Nb; i++) {
        S32 l_VertexNb;
        LOAD_Z(l_VertexNb);
        m_StripExtensions[i].m_Vertices.SetSize(l_VertexNb);
        LOADARRAY_Z(m_StripExtensions[i].m_Vertices.GetArrayPtr(), l_VertexNb);
    }

    LOAD_Z(l_Nb);
    m_Materials.SetSize(l_Nb);
    for (i = 0; i < m_Materials.GetSize(); i++) {
        gData.ClassMgr->LoadLink(m_Materials[i], i_Data);
    }

    LOADRANGE_Z(m_DrawingStartDistance, m_UnkCount4);
    Load(i_Data, m_SphereCollisions);
    Load(i_Data, m_BoxCollisions);
    Load(i_Data, m_CylindreCollisions);
    m_AABBCol.Load(i_Data);
    LOADDYNARRAY_Z(m_OptimizedVertices);
}

void Mesh_Z::EndLoad() {
    Object_Z::EndLoad();
    for (S32 i = 0; i < m_Materials.GetSize(); i++) {
        gData.ClassMgr->UpdateLink(m_Materials[i]);
    }
}

void Mesh_Z::AfterEndLoad() {
    if (IsFlag(FL_OBJECT_SKINNED)) {
        for (S32 i = 0; i < m_Strips.GetSize(); i++) {
            Name_Z l_MaterialName;
            gData.ClassMgr->GetNameFromId(m_Strips[i].m_MaterialId, l_MaterialName);
            m_Strips[i].m_MaterialId = gData.AnimMgr->GetMaterialByName(l_MaterialName);
        }
    }
}

void Mesh_Z::EndLoadLinks() {
    Object_Z::EndLoadLinks();
    for (S32 i = 0; i < m_Materials.GetSize(); i++) {
        m_ResObjLink.UpdateLinkFromId(m_Materials[i], m_Materials[i].GetGlobalID());
    }
}

void Mesh_Z::Clean() {
    Points_Z::Clean();
    m_Normals.Flush();
    m_TextureCoordinates.Flush();
    m_Strips.Flush();
    m_StripExtensions.Flush();
    m_Materials.Flush();
    m_SphereCollisions.Flush();
    m_BoxCollisions.Flush();
    m_CylindreCollisions.Flush();
    m_OptimizedVertices.Flush();
    m_AABBCol.Flush();
}

void Mesh_Z::Load(void** i_Data, SphereCol_Z& o_SphereCol) {
    LOAD_Z(o_SphereCol.m_Sphere);
    LOAD_Z(o_SphereCol.m_Flag);
    gData.ClassMgr->LoadName(o_SphereCol.m_Name, i_Data);
}

void Mesh_Z::Load(void** i_Data, BoxCol_Z& o_BoxCol) {
    LOAD_Z(o_BoxCol.m_Box);
    LOAD_Z(o_BoxCol.m_Flag);
    gData.ClassMgr->LoadName(o_BoxCol.m_Name, i_Data);
}

void Mesh_Z::Load(void** i_Data, CylindreCol_Z& o_CylindreCol) {
    LOAD_Z(o_CylindreCol.m_Cylindre);
    LOAD_Z(o_CylindreCol.m_Flag);
    gData.ClassMgr->LoadName(o_CylindreCol.m_Name, i_Data);
}

void Mesh_Z::Load(void** i_Data, SphereCol_ZDA& o_SphereCols) {
    S32 l_Nb;
    LOAD_Z(l_Nb);
    o_SphereCols.SetSize(l_Nb);
    for (S32 i = 0; i < l_Nb; i++) {
        Load(i_Data, o_SphereCols[i]);
    }
}

void Mesh_Z::Load(void** i_Data, BoxCol_ZDA& o_BoxCols) {
    S32 l_Nb;
    LOAD_Z(l_Nb);
    o_BoxCols.SetSize(l_Nb);
    for (S32 i = 0; i < l_Nb; i++) {
        Load(i_Data, o_BoxCols[i]);
    }
}

void Mesh_Z::Load(void** i_Data, CylindreCol_ZDA& o_CylindreCols) {
    S32 l_Nb;
    LOAD_Z(l_Nb);
    o_CylindreCols.SetSize(l_Nb);
    for (S32 i = 0; i < l_Nb; i++) {
        Load(i_Data, o_CylindreCols[i]);
    }
}

void Mesh_Z::Load(void** i_Data, SphereColNode_Z** o_SphereColNode) {
    U32 l_LoadSphere;
    LOAD_Z(l_LoadSphere);
    if (l_LoadSphere) {
        *o_SphereColNode = NewL_Z(0xd9) SphereColNode_Z;
        LOAD_Z((*o_SphereColNode)->m_BoneNodeId);
        Load(i_Data, *(SphereCol_Z*)*o_SphereColNode);
    }
}

void MeshData_Z::Load(void** i_Data) {
    ObjectDatas_Z::Load(i_Data);
    m_MeshVolume.Load(i_Data);
}
