#include "Object_Z.h"
#include "BaseObject_Z.h"
#include "ResourceObject_Z.h"
#include "Sys_Z.h"
#include "Node_Z.h"

Object_Z::Object_Z() {
    SetGeometryType(UNDEFINED_GEOMETRY);
    m_Flag = 0xffe30;
    Clean();
}

void Object_Z::Clean() {
    m_FadeOutDistance = 0.0f;
}

Bool Object_Z::MarkHandles() {
    if (ResourceObject_Z::MarkHandles() == FALSE) {
        return FALSE;
    }
    else {
        if (gData.ClassMgr->GetPtr(m_ObjectDataHdl) != NULL) {
            gData.ClassMgr->GetPtr(m_ObjectDataHdl)->MarkHandles();
        }
        return TRUE;
    }
}

void Object_Z::UpdateObject(Node_Z* i_Node, ObjectDatas_Z* i_Data) {
    i_Node->SetBSphere((*(Mat4x4*)i_Node->GetRotInWorldMatrix().m.m13.dummy.i32) * GetBSphere());
}

void Object_Z::LoadLinks(void** i_Data) {
    BaseObject_Z::LoadLinks(i_Data);
    m_ResObjLink.Load(i_Data);
    gData.ClassMgr->LoadLink(m_ObjectDataHdl, i_Data);
    LOADRANGE_Z(m_BSphereLocal, m_Type);
}

void Object_Z::EndLoadLinks() { }

void Object_Z::Load(void** i_Data) { }

void Object_Z::EndLoad() {
    m_ResObjLink.EndLoad();
    gData.ClassMgr->UpdateLink(m_ObjectDataHdl);
}

void Object_Z::Draw(DrawInfo_Z& a1, ObjectDatas_Z* i_Data) { }

ObjectDatas_Z::ObjectDatas_Z() {
    m_Flag = 0;
    m_DefaultColor.r = 1.0f;
    m_DefaultColor.g = 1.0f;
    m_DefaultColor.b = 1.0f;
    m_DefaultColor.a = 1.0f;
}

void ObjectDatas_Z::Clone(ObjectDatas_ZHdl& o_ObjectDatasHdl, Object_ZHdl& o_ObjectHdl) {
    ASSERTLE_Z(o_ObjectDatasHdl.IsValid(), "", 126, "_ObjectDatasHdl.IsValid()");
    ObjectDatas_Z* l_ObjectDatas = (ObjectDatas_Z*)GETPTR(o_ObjectDatasHdl);
    l_ObjectDatas->m_DefaultColor = m_DefaultColor;
    l_ObjectDatas->m_Flag = m_Flag | FL_OBJECTDATAS_CLONED;
    l_ObjectDatas->SetName(m_Name, false);
}

void ObjectDatas_Z::Load(void** i_Data) {
    LOADBYTES_Z(&m_Flag, 0x14);
}

Bool ObjectDatas_Z::MarkHandles() {
    U32 l_Result = ResourceObject_Z::MarkHandles();
    return l_Result;
}

void ObjectDatas_Z::Draw(DrawInfo_Z& i_DrawInfo) { }
