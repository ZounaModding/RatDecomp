#include "Light_Z.h"
#include "LightData_Z.h"
#include "ClassManager_Z.h"
#include "Node_Z.h"

Light_Z::Light_Z() {
    SetGeometryType(LIGHT_Z);
    m_Color.Set(1.0f, 1.0f, 1.0f);
    m_Direction.z = 1.0f;
    m_Direction.x = 0.0f;
    m_Direction.y = 0.0f;
    m_Translation.Set(0.0f, 0.0f, 0.0f);
    m_Intensity = 1.0f;
    m_AmbientColor.Set(0.0f, 0.0f, 0.0f);
    m_Rotation = QUAT_NULL;
    GetBSphere().Set(VEC3F_NULL, 1.0f);
    EnableFlag(FL_IS_LIGHT_ACTIVE | FL_OBJECT_NO_DISPLAY | FL_OBJECT_UNK_0x10 | FL_OBJECT_NO_SEADCOLLIDE);
    EnableFlag(FL_OBJECT_NO_SEADDISPLAY);
}

void Light_Z::Clean() {
}

// TODO: Finish matching
void Light_Z::UpdateObject(Node_Z* i_Node, ObjectDatas_Z* i_Data) {
    if (i_Node && i_Data) {
        GetBSphere().Set(m_Translation, 1.0f);

        Object_Z::UpdateObject(i_Node, i_Data);

        m_Direction = m_Rotation * Vec3f(0.0f, 0.0f, 1.0f);

        Vec3f l_Color = m_Color * m_Intensity;
        Float l_Red = Min(l_Color.x, 1.0f);
        Float l_Green = Min(l_Color.y, 1.0f);
        Float l_Blue = Min(l_Color.z, 1.0f);

        LightData_Z* l_LightData = (LightData_Z*)i_Data;
        Bool l_Found = FALSE;
        S32 i = m_UserNodeHdls.GetSize() - 1;

        for (;;) {
            if (i_Node && l_LightData) {
                l_LightData->m_Direction = i_Node->GetRotInWorldMatrix() * m_Direction;
                l_LightData->m_UnkVec3f_0x48 = i_Node->GetWorldMatrix() * m_Translation;
                l_LightData->m_Color.Set(l_Red, l_Green, l_Blue);
                l_LightData->m_Ambient = m_AmbientColor;
                l_LightData->m_LightFlag = GetFlag();
            }

            if (i < 0) {
                break;
            }

            if (i_Node == m_UserNodeHdls[i]) {
                l_Found = TRUE;
            }

            i_Node = m_UserNodeHdls[i];
            l_LightData = (LightData_Z*)i_Node->GetObjectDatas();
            i--;
        }

        if (!l_Found) {
            m_UserNodeHdls.Add(i_Node->GetHandle());
        }
    }
}

void Light_Z::SetAmbiant(const Vec3f& i_Ambient) {
    m_AmbientColor.x = Min(i_Ambient.x, 1.0f);
    m_AmbientColor.y = Min(i_Ambient.y, 1.0f);
    m_AmbientColor.z = Min(i_Ambient.z, 1.0f);
}

LightData_Z::LightData_Z() {
    m_LightFlag = FL_IS_LIGHT_ACTIVE;
    m_Direction = VEC3F_NULL;
    m_Ambient = VEC3F_NULL;
    m_Color = VEC3F_ONE;
}

// TODO: Finish matching
void LightData_Z::Clone(ObjectDatas_ZHdl& o_ObjectDatasHdl, Object_ZHdl& o_ObjectHdl) {
    Name_Z l_ClassName(Name_Z::GetID("LightData_Z", 0));
    o_ObjectDatasHdl = gData.ClassMgr->NewObject(l_ClassName, GetName());
    ObjectDatas_Z::Clone(o_ObjectDatasHdl, o_ObjectHdl);

    ObjectDatas_ZHdl l_DataHdl;

    l_DataHdl = o_ObjectDatasHdl;
    LightData_Z* l_Data = (LightData_Z*)GETPTR(l_DataHdl);

    l_Data->m_Direction = m_Direction;
    l_Data->m_Color = m_Color;
    l_Data->m_UnkVec3f_0x48 = m_UnkVec3f_0x48;
    l_Data->m_LightFlag = m_LightFlag;
    l_Data->m_Ambient = m_Ambient;
}
