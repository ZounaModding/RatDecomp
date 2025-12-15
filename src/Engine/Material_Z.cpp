#include "Material_Z.h"
#include "Math_Z.h"
#include "ResourceObject_Z.h"
#include "Types_Z.h"
#include "WaterHeightMap_Z.h"
U32 Material_Z::DefaultRdrFlag = FL_MAT_RDR_UNK23;

Material_Z::Material_Z() {
    m_RdrFlag2 = 0;
    m_WaterHeightMap = NULL;
    m_RdrFlag = DefaultRdrFlag;
    m_ColFlag = FL_MAT_COL_UNK0 | FL_MAT_COL_COLLIDABLE | FL_MAT_COL_UNK2;
    m_ObjectFlag = 0;
    m_Flag = 0;

    m_UVTransform.m.m[0][0] = 1.0f;
    m_UVTransform.m.m[0][1] = 0.0f;
    m_UVTransform.m.m[0][2] = 0.0f;
    m_UVTransform.m.m[0][3] = 0.0f;
    m_UVTransform.m.m[1][0] = 0.0f;
    m_UVTransform.m.m[1][1] = 1.0f;
    m_UVTransform.m.m[1][2] = 0.0f;
    m_UVTransform.m.m[1][3] = 0.0f;
    m_UVTransform.m.m[2][0] = 0.0f;
    m_UVTransform.m.m[2][1] = 0.0f;
    m_UVTransform.m.m[2][2] = 1.0f;
    m_UVTransform.m.m[2][3] = 0.0f;

    m_DiffuseOpacity = 1.0f;
    m_EmissiveColor = Vec3f(0.5f, 0.5f, 0.5f);
    m_DiffuseColor = Vec3f(0.5f, 0.5f, 0.5f);
    m_Rotation = -1.0f;
}

Material_Z::~Material_Z() {
    // $VIOLET: TODO: Implement this
}

Bool Material_Z::MarkHandles() {
    if (!ResourceObject_Z::MarkHandles()) {
        return FALSE;
    }
    else {
        for (S32 i = 0; i < mtl_nb_params; i++) {
            if (gData.ClassMgr->GetPtr(m_BmapHdl[i]) != NULL) {
                gData.ClassMgr->GetPtr(m_BmapHdl[i])->MarkHandles();
            }
        }
        return TRUE;
    }
}

void Material_Z::Clean() {
    if (m_WaterHeightMap != NULL) {
        m_WaterHeightMap->Release();
        Delete_Z m_WaterHeightMap;
        m_WaterHeightMap = NULL;
        m_RdrFlag2 = 0;
    }
}

Extern_Z void O_SinCos(Vec2f, Float);

void Material_Z::UpdateTMatrix() {
    Vec2f m_SinCos;
    O_SinCos(m_SinCos, m_Rotation);

    m_UVTransform.m.m[0][0] = m_SinCos.x * m_Scale.x;
    m_UVTransform.m.m[1][0] = m_SinCos.y * m_Scale.x;
    m_UVTransform.m.m[0][1] = -m_SinCos.y * m_Scale.y;
    m_UVTransform.m.m[1][1] = m_SinCos.x * m_Scale.y;

    if (m_Flag & FL_MATERIAL_PAWAP) {
        m_UVTransform.m.m[2][0] = m_Translation.x - (m_UVTransform.m.m[0][0] + m_UVTransform.m.m[1][0]) * 0.5f + 0.5f;
        m_UVTransform.m.m[2][1] = m_Translation.y - (m_UVTransform.m.m[0][1] + m_UVTransform.m.m[1][1]) * 0.5f + 0.5f;
    }
    else {
        m_UVTransform.m.m[2][0] = (m_UVTransform.m.m[0][0] + m_UVTransform.m.m[1][0]) * (m_Translation.x - 0.5f) + 0.5f;
        m_UVTransform.m.m[2][1] = (m_UVTransform.m.m[0][1] + m_UVTransform.m.m[1][1]) * (m_Translation.y - 0.5f) + 0.5f;
    }
}
