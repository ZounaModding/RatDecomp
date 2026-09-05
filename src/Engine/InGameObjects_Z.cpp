#include "InGameObjects_Z.h"
#include "Camera_Z.h"
#include "ClassManager_Z.h"
#include "Node_Z.h"

InGameObjects_Z::InGameObjects_Z() {
    m_SizeY = 0.3f;
    m_SizeX = 0.3f;
    m_PosY = 0.1f;
    m_PosX = 0.1f;
    m_ViewportId = 0;

    m_CameraNodeHdl = gData.ClassMgr->NewObject(Name_Z(Name_Z::GetID("Node_Z", 0)), Name_Z(Name_Z::GetID("InGameCameraNode", 0)));
    m_CameraHdl = gData.ClassMgr->NewObject(Name_Z(Name_Z::GetID("Camera_Z", 0)), Name_Z(Name_Z::GetID("InGameCamera", 0)));

    m_CameraNodeHdl->SetObject(m_CameraHdl);

    SetNearClip(0.1f);
    SetFarClip(20.0f);
}

InGameObjects_Z::~InGameObjects_Z() {
}

void InGameObjects_Z::Init() {
    ManipulatorDraw_Z::Init();
    SetGroup(ag_draw);
}

Bool InGameObjects_Z::MarkHandles() {
    if (!BaseObject_Z::MarkHandles()) {
        return FALSE;
    }

    if (m_RootNodeHdl.IsValid()) {
        m_RootNodeHdl->MarkHandles();
    }

    if (m_LightNodeHdl.IsValid()) {
        m_LightNodeHdl->MarkHandles();
    }

    for (S32 i = 0; i < 3; i++) {
        if (m_LightNodeHdls[i].IsValid()) {
            m_LightNodeHdls[i]->MarkHandles();
        }
    }

    if (m_CameraNodeHdl.IsValid()) {
        m_CameraNodeHdl->MarkHandles();
    }

    return TRUE;
}

void InGameObjects_Z::SetLights(Node_ZHdl& i_LightNodeHdl1, Node_ZHdl& i_LightNodeHdl2, Node_ZHdl& i_LightNodeHdl3, Node_ZHdl& i_LightNodeHdl4) {
    m_LightNodeHdl = i_LightNodeHdl1;
    m_LightNodeHdls[0] = i_LightNodeHdl2;
    m_LightNodeHdls[1] = i_LightNodeHdl3;
    m_LightNodeHdls[2] = i_LightNodeHdl4;
}

void InGameObjects_Z::SetRoot(Node_ZHdl& i_NodeHdl) {
    m_RootNodeHdl = i_NodeHdl;

    Mat4x4* l_Matrix = m_RootNodeHdl->GetWorldMatrixPtr();

    memset(l_Matrix, 0, sizeof(Mat4x4));

    l_Matrix->m[0][0] = 1.0f;
    l_Matrix->m[1][1] = 1.0f;
    l_Matrix->m[2][2] = 1.0f;
    l_Matrix->m[3][3] = 1.0f;
}

void InGameObjects_Z::GetAllNodes() {
    m_Nodes.Empty();
    m_RootNodeHdl->GetAllNodes(m_Nodes);
}

void InGameObjects_Z::SetNearClip(Float i_Near) {
    m_CameraHdl->SetNearClip(i_Near);
}

void InGameObjects_Z::SetFarClip(Float i_Far) {
    m_CameraHdl->SetFarClip(i_Far);
}

void InGameObjects_Z::SetFov(Float i_Fov) {
    m_CameraHdl->SetFov(i_Fov);
}

void InGameObjects_Z::SetPosAndSize(Float i_PosX, Float i_PosY, Float i_SizeX, Float i_SizeY) {
    m_PosX = i_PosX;
    m_PosY = i_PosY;
    m_SizeX = i_SizeX;
    m_SizeY = i_SizeY;
}
