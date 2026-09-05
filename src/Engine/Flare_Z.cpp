#include "Flare_Z.h"
#include "ClassManager_Z.h"
#include "MaterialAnim_Z.h"
#include "Sys_Z.h"

Flare_Z::Flare_Z() {
    SetGeometryType(FLARE_Z);
    EnableFlag(FL_OBJECT_NO_DISPLAY | FL_OBJECT_NO_SEADCOLLIDE);
    EnableFlag(FL_OBJECT_NO_SEADDISPLAY);
    m_Active = TRUE;
}

Bool Flare_Z::MarkHandles() {
    if (!Object_Z::MarkHandles()) {
        return FALSE;
    }

    for (S32 i = 0; i < m_Flares.GetSize(); i++) {
        m_Flares[i].m_MaterialAnimHdl->MarkHandles();
    }

    return TRUE;
}

void Flare_Z::Clean() {
}

void Flare_Z::Load(void** i_Data) {
    Object_Z::Load(i_Data);
    LOAD_Z(m_Active);

    S32 l_Nb;
    LOAD_Z(l_Nb);
    m_Flares.SetSize(l_Nb, FALSE);
    S32 i;
    FlareDesc_Z* l_Flare = m_Flares.GetArrayPtr();
    for (i = 0; i < l_Nb; i++, l_Flare++) {
        LOADRANGE_Z(l_Flare->m_Color, l_Flare->m_UVs[3]);
        gData.ClassMgr->LoadLink(l_Flare->m_MaterialAnimHdl, i_Data);
    }
}

void Flare_Z::EndLoad() {
    Object_Z::EndLoad();
    for (S32 i = 0; i < m_Flares.GetSize(); i++) {
        gData.ClassMgr->UpdateLink(m_Flares[i].m_MaterialAnimHdl);
    }
}

void Flare_Z::AfterEndLoad() {
    for (S32 i = 0; i < m_Flares.GetSize(); i++) {
        m_Flares[i].m_MaterialHdl = m_Flares[i].m_MaterialAnimHdl->GetMaterial();
    }
}

void Flare_Z::GetFarPos(const Vec3f& i_Pos, Float i_FarClip, Vec3f& o_FarPos) {
    o_FarPos = i_Pos;
    Float l_Distance = o_FarPos.GetNorm();
    o_FarPos /= l_Distance;

    const Vec3f& l_Front = VEC3F_FRONT;
    Float l_FarDistance = -(Vec3f(0.0f, 0.0f, i_FarClip) * l_Front);
    l_FarDistance = -l_FarDistance / (l_Front * o_FarPos);
    if (l_FarDistance > l_Distance) {
        l_FarDistance = l_Distance;
    }
    o_FarPos.x *= l_FarDistance;
    o_FarPos.y *= l_FarDistance;
    o_FarPos.z *= l_FarDistance;
}

void Flare_Z::GetFlareParams(DrawInfo_Z& i_DrawInfo, Vec4f& a2, Vec4f& a3, Float* a4, Float* a5, Float* a6) { }
