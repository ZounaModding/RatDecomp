#include "Mesh_Z.h"

Mesh_Z::Mesh_Z() {
    SetGeometryType(MESH_Z);
    m_DrawingStartDistance = 0.0f;
    m_DrawingCutoffDistance = 0.0f;
    EnableFlag(FL_IS_MESH_UNK_0x100000 | FL_IS_MESH_UNK_0x200000);
}

Bool MeshData_Z::IsBSphere(Node_Z* i_Node, Frustrum_Z& i_Frustrum, DrawInfo_Z& i_DrawInfo) {
    return FALSE;
}

void MeshData_Z::Clone(ObjectDatas_ZHdl& o_ObjectDatasHdl, Object_ZHdl& o_ObjectHdl) {
    Name_Z l_ClassName(Name_Z::GetID("MeshData_Z", 0));
    (BaseObject_ZHdl&)o_ObjectDatasHdl = gData.ClassMgr->NewObject(l_ClassName, GetName());
    MeshData_Z* l_Data = (MeshData_Z*)GETPTR(o_ObjectDatasHdl);
    l_Data->m_Flag = m_Flag;
    l_Data->m_MeshVolume.Clone(m_MeshVolume);
}
