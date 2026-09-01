#include "Omni_Z.h"
#include "MaterialAnim_Z.h"
#include "Math_Z.h"
#include "MathTools_Z.h"
#include "Node_Z.h"
#include "Program_Z.h"
#include "Sys_Z.h"
#include "SystemDatas_Z.h"

Omni_Z::Omni_Z() {
    SetGeometryType(OMNI_Z);
    m_Color.x = 1.0f;
    m_Color.y = 1.0f;
    m_Color.z = 1.0f;
    m_Intensity = 1.0f;
    Changed();

    EnableFlag(FL_IS_LIGHT_ACTIVE | FL_OBJECT_NO_SEADCOLLIDE);
    DisableFlag(FL_OBJECT_UNK_0x10 | (FL_OBJECT_OMNI_REJECT_ALL & ~FL_OBJECT_OMNI_REJECT_CHANNEL_10));
    DisableFlag(FL_OBJECT_OMNI_REJECT_CHANNEL_10);

    m_SpotAtt0 = 0.0f;
    m_SpotAtt1 = 0.0f;
    m_Start = 0.0f;
    m_End = 1.0f;
    m_BSphereLocal.Center = VEC3F_NULL;
    m_BSphereLocal.Radius = m_End;
    SetMaterialAnim(gData.SystemDatas->GetDefaultMaterialAnim());
}

Bool Omni_Z::MarkHandles() {
    if (!Object_Z::MarkHandles()) {
        return FALSE;
    }
    MarkHandle_Z(m_MaterialAnimHdl);
    return TRUE;
}

void Omni_Z::SetMaterialAnim(const MaterialAnim_ZHdl& i_MaterialAnim) {
    if (!i_MaterialAnim.IsValid()) {
        m_MaterialAnimHdl = gData.SystemDatas->GetDefaultMaterialAnim();
    }
    else {
        m_MaterialAnimHdl = i_MaterialAnim;
    }

    m_MaterialHdl = m_MaterialAnimHdl->GetMaterial();
    if (!m_MaterialHdl.IsValid()) {
        SetMaterialAnim(gData.SystemDatas->GetDefaultMaterialAnim());
    }
}

void Omni_Z::UpdateObject(Node_Z* i_Node, ObjectDatas_Z* i_Data) {
    Object_Z::UpdateObject(i_Node, i_Data);

    Mat3x3& l_Rotation = i_Node->GetRotInWorldMatrix();
    Vec3f l_Direction(-l_Rotation.m.m[2][0], -l_Rotation.m.m[2][1], -l_Rotation.m.m[2][2]);
    const Vec3f& l_Position = i_Node->GetWorldTranslation();
    Vec3f l_Up(l_Rotation.m.m[1][0], l_Rotation.m.m[1][1], l_Rotation.m.m[1][2]);
    Vec3f l_Target = l_Position + l_Direction;

    Mat4x4 l_LookAt;
    BuildLookAtLH(l_Position, l_Target, l_Up, l_LookAt);

    Float l_InvTan = 1.0f / O_Tan(m_SpotOuterHalfAngleRad);
    Float l_Near = 1.0f;
    Mat4x4 l_Projection;
    l_Projection.SetNull();
    l_Projection.m[0][0] = l_InvTan;
    l_Projection.m[1][1] = l_InvTan;
    l_Projection.m[2][2] = 1.010101f;
    l_Projection.m[3][2] = -l_Projection.m[2][2] * l_Near;
    l_Projection.m[2][3] = l_Near;

    Mat4x4 l_ViewProjection = l_Projection * l_LookAt;

    Mat4x4 l_Texture;
    l_Texture.SetNull();
    l_Texture.m[0][0] = 0.5f;
    l_Texture.m[1][1] = -0.5f;
    l_Texture.m[2][2] = 0.0f;
    l_Texture.m[3][0] = 0.5f;
    l_Texture.m[3][1] = 0.5f;
    l_Texture.m[3][2] = 1.0f;
    l_Texture.m[3][3] = 1.0f;

    Mat4x4 l_TextureProjection = l_Texture * l_ViewProjection;
    l_TextureProjection.Transp(m_TextureProjMat);
}

void Omni_Z::Changed() {
    m_ScaledColor.x = Min<Float>(m_Color.x, 1.0f);
    m_ScaledColor.y = Min<Float>(m_Color.y, 1.0f);
    m_ScaledColor.z = Min<Float>(m_Color.z, 1.0f);
    m_ScaledColor *= m_Intensity;
}

void Omni_Z::SetEnd(Float i_End) {
    m_End = i_End;
    m_BSphereLocal.Radius = m_End;
}

void Omni_Z::Load(void** i_Data) {
    Object_Z::Load(i_Data);
    LOADRANGE_Z(m_TextureProjMat, m_End);
    gData.ClassMgr->LoadLink(m_MaterialAnimHdl, i_Data);
}

void Omni_Z::EndLoad() {
    Object_Z::EndLoad();
    gData.ClassMgr->UpdateLink(m_MaterialAnimHdl);
}

void Omni_Z::AfterEndLoad() {
    SetMaterialAnim(m_MaterialAnimHdl);
}
