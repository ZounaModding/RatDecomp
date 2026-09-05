#include "RotShape_Z.h"
#include "AnimationManager_Z.h"
#include "ClassManager_Z.h"
#include "MaterialAnim_Z.h"
#include "Program_Z.h"
#include "Sys_Z.h"

void RotShape_Z::Load(void** i_Data) {
    S32 l_Nb;
    S32 i;

    Points_Z::Load(i_Data);

    LOADDYNARRAYSIZE_Z(m_MatIndices);
    LOADARRAY_Z(m_MatIndices.GetArrayPtr(), l_Nb);

    if (IsFlag(FL_OBJECT_SKINNED)) {
        for (i = 0; i < l_Nb; i++) {
            LOAD_Z(m_MatIndices[i]);
        }
    }

    LOADDYNARRAYSIZE_Z(m_LocalVertices);
    LOADARRAY_Z(m_LocalVertices.GetArrayPtr(), l_Nb);

    LOADDYNARRAYSIZE_Z(m_LocalUVs);
    LOADARRAY_Z(m_LocalUVs.GetArrayPtr(), l_Nb);

    LOADDYNARRAYSIZE_Z(m_MaterialHdls);
    for (S32 j = 0; j < l_Nb; j++) {
        gData.ClassMgr->LoadLink(m_MaterialHdls[j], i_Data);
    }

    LOADRANGE_Z(m_Scale, m_Type);
}

void RotShape_Z::EndLoad() {
    Object_Z::EndLoad();

    for (S32 i = 0; i < m_MaterialHdls.GetSize(); i++) {
        gData.ClassMgr->UpdateLink(m_MaterialHdls[i]);
    }
}

void RotShape_Z::AfterEndLoad() {
    S32 i;

    for (i = 0; i < m_MaterialHdls.GetSize(); i++) {
        m_MaterialHdls[i] = ((MaterialAnim_Z*)GETPTR(m_MaterialHdls[i]))->GetMaterial();
    }

    if (IsFlag(FL_OBJECT_SKINNED)) {
        for (S32 j = 0; j < m_MatIndices.GetSize(); j++) {
            Name_Z l_Name;
            gData.ClassMgr->GetNameFromId(m_MatIndices[j], l_Name);
            m_MatIndices[j] = gData.AnimMgr->GetMaterialByName(l_Name);
        }
    }
}

void RotShapeData_Z::Load(void** i_Data) {
    S32 l_Nb;

    ObjectDatas_Z::Load(i_Data);

    LOAD_Z(l_Nb);
    m_FlagsDA.SetSize(l_Nb);
    m_RotShapeDatas.SetSize(l_Nb);
    LOADARRAY_Z(m_FlagsDA.GetArrayPtr(), l_Nb);
    LOADARRAY_Z(m_RotShapeDatas.GetArrayPtr(), l_Nb);
}
