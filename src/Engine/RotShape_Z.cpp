#include "RotShape_Z.h"
#include "MaterialAnim_Z.h"
#include "MaterialManager_Z.h"
#include "Program_Z.h"
#include "ClassManager_Z.h"

RotShape_Z::RotShape_Z() {
    SetGeometryType(ROTSHAPE_Z);
    m_Type = 0;
    m_Scale = 0.0f;
}

Bool RotShape_Z::MarkHandles() {
    if (!Object_Z::MarkHandles()) {
        return FALSE;
    }

    for (S32 i = 0; i < m_MaterialHdls.GetSize(); i++) {
        m_MaterialHdls[i]->MarkHandles();
        gData.MaterialMgr->GetMaterialAnimFromMaterial(m_MaterialHdls[i])->MarkHandles();
    }

    return TRUE;
}

RotShapeData_Z::RotShapeData_Z() {
}

RotShapeData_Z::~RotShapeData_Z() {
}

void RotShapeData_Z::UnHideObject(S32 i_Index) {
    if (i_Index >= 0) {
        m_FlagsDA[i_Index] &= ~FL_ROTSHAPE_HIDE;
        return;
    }
    S32 i;
    for (i = 0; i < m_RotShapeDatas.GetSize(); i++) {
        m_FlagsDA[i] &= ~FL_ROTSHAPE_HIDE;
    }
}

void RotShapeData_Z::HideObject(S32 i_Index) {
    if (i_Index >= 0) {
        m_FlagsDA[i_Index] |= FL_ROTSHAPE_HIDE;
        return;
    }
    S32 i;
    for (i = 0; i < m_RotShapeDatas.GetSize(); i++) {
        m_FlagsDA[i] |= FL_ROTSHAPE_HIDE;
    }
}

void RotShapeData_Z::Clone(ObjectDatas_ZHdl& o_ObjectDatasHdl, Object_ZHdl& o_ObjectHdl) {
    Name_Z l_ClassName(Name_Z::GetID("RotShapeData_Z", 0));
    o_ObjectDatasHdl = gData.ClassMgr->NewObject(l_ClassName, GetName());

    PointsDatas_ZHdl l_DataHdl;

    l_DataHdl = o_ObjectDatasHdl;

    ObjectDatas_Z* l_Data = l_DataHdl;

    PointsDatas_Z::Clone(o_ObjectDatasHdl, o_ObjectHdl);

    // $SABE: Ugly as hell but it gets it to 100%
    ((RotShapeData_Z*)l_Data)->m_Flag = m_Flag;
    ((RotShapeData_Z*)l_Data)->m_RotShapeDatas = m_RotShapeDatas;
    ((RotShapeData_Z*)l_Data)->m_FlagsDA = m_FlagsDA;
    ((RotShapeData_Z*)l_Data)->UpdatePointers(o_ObjectHdl, 0);
}

void RotShapeData_Z::SetShadowData(Node_Z* i_Node, const Vec3f& i_Origin, const ColLineResult_Z& i_Result, Float i_Radius) {
}
