#include "Material_Z.h"
#include "Math_Z.h"
#include "ResourceObject_Z.h"
#include "Types_Z.h"
#include "MaterialManager_Z.h"

U32 Material_Z::DefaultRdrFlag = FL_MTL_RDR_UNK_0x800000;

Material_Z::Material_Z() {
    m_RdrFlag = DefaultRdrFlag;
    m_ColFlag = FL_MTL_COL_CLING | FL_MTL_COL_COLLISION | FL_MTL_COL_COLLISIONCAM;
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
    Clean();
}

Bool Material_Z::MarkHandles() {
    if (!ResourceObject_Z::MarkHandles()) {
        return FALSE;
    }
    else {
        for (S32 i = 0; i < mtl_nb_params; i++) {
            if (gData.ClassMgr->GetPtr(m_BmapHdls[i]) != NULL) {
                gData.ClassMgr->GetPtr(m_BmapHdls[i])->MarkHandles();
            }
        }
        return TRUE;
    }
}

void Material_Z::Clean() {
    m_MaterialUser.Clean();
}

Extern_Z void O_SinCos(Vec2f&, Float);

void Material_Z::UpdateTMatrix() {
    Vec2f l_SinCos;
    O_SinCos(l_SinCos, m_Rotation);

    Float l_Cos = l_SinCos.y;
    Float l_Sin = l_SinCos.x;

    m_UVTransform.m.m[0][0] = l_Cos * m_Scale.x;
    m_UVTransform.m.m[1][0] = l_Sin * m_Scale.x;
    m_UVTransform.m.m[0][1] = -l_Sin * m_Scale.y;
    m_UVTransform.m.m[1][1] = l_Cos * m_Scale.y;

    if (m_Flag & FL_MATERIAL_PAWAP) {
        m_UVTransform.m.m[2][0] = m_Translation.x - (m_UVTransform.m.m[0][0] + m_UVTransform.m.m[1][0]) * 0.5f + 0.5f;
        m_UVTransform.m.m[2][1] = m_Translation.y - (m_UVTransform.m.m[0][1] + m_UVTransform.m.m[1][1]) * 0.5f + 0.5f;
    }
    else {
        m_UVTransform.m.m[2][0] = (0.5f + m_Translation.x) * -(m_UVTransform.m.m[0][0] + m_UVTransform.m.m[1][0]) - 0.5f;
        m_UVTransform.m.m[2][1] = (0.5f + m_Translation.y) * -(m_UVTransform.m.m[0][1] + m_UVTransform.m.m[1][1]) - 0.5f;
    }
}

void Material_Z::Changed() {
    m_Flag &= FL_TEX_RESET;

    if (m_BmapHdls[mtl_diffuse].IsValid()) {
        m_Flag = m_Flag | FL_TEX_DIFFUSE;
    }
    if (m_BmapHdls[mtl_envmap].IsValid()) {
        m_Flag = m_Flag | FL_TEX_ENV;
    }
    if (m_BmapHdls[mtl_bumpmap].IsValid()) {
        m_Flag = m_Flag | FL_TEX_BUMP;
    }
    if (m_BmapHdls[mtl_specularmap].IsValid()) {
        m_Flag = m_Flag | FL_TEX_SPECULAR;
    }
    m_RdrFlag &= FL_MTL_RDR_TRANSP_RESET;
    if (m_DiffuseOpacity < 1.0f || m_RdrFlag & (FL_MTL_RDR_ADDITIF | FL_MTL_RDR_SOUSTRACTIF)) {
        m_RdrFlag = m_RdrFlag | FL_MTL_RDR_IS_ALPHABLENDED;
    }
    else {
        UpdateTransp();
    }

    U32 l_Code = GetCode();
    if (l_Code && m_MaterialUser.m_Code != l_Code) {
        m_MaterialUser.Clean();
        m_MaterialUser.m_Code = GetCode();
        m_MaterialUser.m_MatUser = gData.MaterialMgr->GetMaterialUser(GetCode());
    }
}

void Material_Z::Clone(Material_ZHdl& o_MaterialHdl) {
    Name_Z l_ClassName = Name_Z::GetID("Material_Z");

    String_Z<ARRAY_CHAR_MAX> l_DebugNameString;
    l_DebugNameString.Sprintf("%s_CL", GetDebugRscName());

    o_MaterialHdl = gData.ClassMgr->NewObject(l_ClassName, Name_Z::GetID(l_DebugNameString));
    o_MaterialHdl->SetName(GetName(), FALSE);

    Material_Z* l_Material = o_MaterialHdl;

    l_Material->m_Translation.x = m_Translation.x;
    l_Material->m_Translation.y = m_Translation.y;
    l_Material->m_Scale.x = m_Scale.x;
    l_Material->m_Scale.y = m_Scale.y;
    l_Material->m_Rotation = m_Rotation;

    l_Material->m_DiffuseColor.x = m_DiffuseColor.x;
    l_Material->m_DiffuseColor.y = m_DiffuseColor.y;
    l_Material->m_DiffuseColor.z = m_DiffuseColor.z;

    l_Material->m_EmissiveColor.x = m_EmissiveColor.x;
    l_Material->m_EmissiveColor.y = m_EmissiveColor.y;
    l_Material->m_EmissiveColor.z = m_EmissiveColor.z;

    l_Material->m_Params.x = m_Params.x;
    l_Material->m_Params.y = m_Params.y;
    l_Material->m_Params.z = m_Params.z;
    l_Material->m_Params.w = m_Params.w;

    l_Material->m_Specular.w = m_Specular.w;
    Float l_SpecularY = m_Specular.y;
    Float l_SpecularZ = m_Specular.z;
    l_Material->m_Specular.x = m_Specular.x;
    l_Material->m_Specular.y = l_SpecularY;
    l_Material->m_Specular.z = l_SpecularZ;

    l_Material->m_ColFlag = m_ColFlag;
    l_Material->m_ObjectFlag = m_ObjectFlag;
    l_Material->m_Flag = m_Flag;
    l_Material->m_DiffuseOpacity = m_DiffuseOpacity;
    l_Material->m_RdrFlag = m_RdrFlag;

    // $SABE: This is ugly but it's what gets it matching
    Material_Z* l_SourceBmapMaterial = this;
    Material_Z* l_DestBmapMaterial = l_Material;
    for (S32 i = 0; i < mtl_nb_params; i++) {
        l_DestBmapMaterial->m_BmapHdls[0] = l_SourceBmapMaterial->m_BmapHdls[0];
        l_Material->Changed();

        l_SourceBmapMaterial = (Material_Z*)((U8*)l_SourceBmapMaterial + sizeof(Bitmap_ZHdl));
        l_DestBmapMaterial = (Material_Z*)((U8*)l_DestBmapMaterial + sizeof(Bitmap_ZHdl));
    }

    l_Material->UpdateTMatrix();
}
