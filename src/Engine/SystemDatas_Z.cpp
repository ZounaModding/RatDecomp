#include "SystemDatas_Z.h"
#include "ClassManager_Z.h"
#include "Math_Z.h"
#include "Material_Z.h"
#include "MaterialAnim_Z.h"
#include "MaterialManager_Z.h"
#include "LightData_Z.h"
#include "MaterialObj_Z.h"
#include "Fonts_Z.h"
#include "Main_Z.h"

SystemDatas_Z::SystemDatas_Z() {
    m_DefaultMaterialAnimHdl = gData.ClassMgr->NewResource(Name_Z("MaterialAnim_Z"), Name_Z("DefaultMaterialAnim"));
    m_DefaultMaterialHdl = gData.ClassMgr->NewResource(Name_Z("Material_Z"), Name_Z("DefaultMaterial"));
    m_BloomMaterialHdl = gData.ClassMgr->NewResource(Name_Z("Material_Z"), Name_Z("BloomMaterial"));
    m_StencilMaterialHdl = gData.ClassMgr->NewResource(Name_Z("Material_Z"), Name_Z("StencilMaterial"));
    m_SonarMaterialHdl = gData.ClassMgr->NewResource(Name_Z("Material_Z"), Name_Z("SonarMaterial"));
    m_RadialMotionBlurMaterialHdl = gData.ClassMgr->NewResource(Name_Z("Material_Z"), Name_Z("RadialMBlurMaterial"));
    m_FlashMaterialHdl = gData.ClassMgr->NewResource(Name_Z("Material_Z"), Name_Z("FlashMaterial"));
    m_BlackMaterialHdl = gData.ClassMgr->NewResource(Name_Z("Material_Z"), Name_Z("BlackMaterial"));
    m_WhiteBitmapHdl = gData.ClassMgr->NewResource(Name_Z("Bitmap_Z"), Name_Z("WhiteTexture"));
    m_BlackBitmapHdl = gData.ClassMgr->NewResource(Name_Z("Bitmap_Z"), Name_Z("BlackTexture"));
    m_BloomBitmapHdl = gData.ClassMgr->NewResource(Name_Z("Bitmap_Z"), Name_Z("BloomTexture"));
    m_OverdrawMaterialHdl = gData.ClassMgr->NewResource(Name_Z("Material_Z"), Name_Z("OverdrawMaterial"));

    Bitmap_Z* l_Bitmap = m_WhiteBitmapHdl;
    l_Bitmap->InitBmap(64, 64, BM_5551, NULL, NULL);
    l_Bitmap->EnableFlag(FL_BITMAP_UNK_0x8);
    memset(l_Bitmap->GetDatas(), 0xFF, 0x2000);
    l_Bitmap->SetTransp(BM_NO_TRANSP);

    m_DefaultMaterialAnimHdl->SetMaterial(m_DefaultMaterialHdl);
    m_DefaultMaterialHdl->SetBitmap(m_WhiteBitmapHdl, 0);
    gData.MaterialMgr->AddMaterialAnim(m_DefaultMaterialAnimHdl);

    l_Bitmap = m_BlackBitmapHdl;
    l_Bitmap->InitBmap(64, 64, BM_5551, 0, 0);
    l_Bitmap->EnableFlag(FL_BITMAP_UNK_0x8);
    l_Bitmap->SetTransp(BM_NO_TRANSP);
    m_BlackMaterialHdl->SetBitmap(m_BlackBitmapHdl, 0);

    l_Bitmap = m_BloomBitmapHdl;
    l_Bitmap->InitBmap(32, 32, BM_5551, 0, 0);
    l_Bitmap->EnableFlag(FL_BITMAP_UNK_0x8);
    memset(l_Bitmap->GetDatas(), 0xFF, 0x800);
    l_Bitmap->SetTransp(BM_NO_TRANSP);

    m_StencilMaterialHdl->SetCode(FL_MTL_CODE_STENCIL);
    m_StencilMaterialHdl->EnableRenderFlag(FL_MTL_RDR_ADDITIF);

    m_BloomMaterialHdl->SetCode(FL_MTL_CODE_BLOOM);
    m_BloomMaterialHdl->SetDiffuse(Vec3f(0.35f, 0.35f, 0.35f));
    m_BloomMaterialHdl->SetOpacity(1.0f);

    m_SonarMaterialHdl->SetCode(FL_MTL_CODE_SONAR);
    m_SonarMaterialHdl->SetBitmap(m_BloomBitmapHdl, 0);
    m_SonarMaterialHdl->SetDiffuse(Vec3f(0.5f, 0.5f, 0.5f));
    m_SonarMaterialHdl->SetEmissive(Vec3f(0.75f, 0.75f, 1.0f));
    m_SonarMaterialHdl->GetParams().Set(96.0f, 80.0f, 50.0f, 30.0f);

    m_RadialMotionBlurMaterialHdl->SetDiffuse(Vec3f(0.5f, 0.5f, 0.5f));
    m_RadialMotionBlurMaterialHdl->SetEmissive(Vec3f(1.0f, 0.75f, 0.75f));
    m_RadialMotionBlurMaterialHdl->GetParams().Set(64.0f, 64.0f, 64.0f, 64.0f);

    m_OverdrawMaterialHdl->SetBitmap(m_WhiteBitmapHdl, Material_Z::mtl_diffuse);
    m_OverdrawMaterialHdl->SetDiffuse(Vec3f(0.0f, 0.0f, 0.0f));
    m_OverdrawMaterialHdl->SetEmissive(Vec3f(0.05f, 0.0f, 0.05f));
    m_OverdrawMaterialHdl->EnableRenderFlag(FL_MTL_RDR_IS_ALPHABLENDED | FL_MTL_RDR_ADDITIF);
    m_OverdrawMaterialHdl->Changed();

    m_DefaultLightDataHdl = gData.ClassMgr->NewResource(Name_Z("LightData_Z"), Name_Z("DefaultLight"));
    m_DefaultLightDataHdl->SetColor(VEC3F_NULL);
    m_DefaultLightDataHdl->SetAmbiant(VEC3F_NULL);
    m_DefaultLightDataHdl->SetDir(VEC3F_FRONT);

    m_SubDefaultLightDataHdl = gData.ClassMgr->NewResource(Name_Z("LightData_Z"), Name_Z("SubDefaultLight"));
    m_SubDefaultLightDataHdl->SetColor(VEC3F_NULL);
    m_SubDefaultLightDataHdl->SetAmbiant(Vec3f(0.3f, 0.3f, 0.3f));
    m_SubDefaultLightDataHdl->SetDir(VEC3F_FRONT);
}

SystemDatas_Z::~SystemDatas_Z() { }

void SystemDatas_Z::MarkHandles() {
}

Bool SystemDatas_Z::Minimize() {
    return FALSE;
}

MaterialAnim_ZHdl SystemDatas_Z::GetMaterialByName(const Name_Z& i_Name) {
    MaterialAnim_ZHdl l_MatHdl;
    for (S32 i = 0; !l_MatHdl.IsValid() && i < m_MaterialObjHdls.GetSize(); i++) {
        l_MatHdl = m_MaterialObjHdls[i]->GetMaterialByName(i_Name);
    }
    if (!l_MatHdl.IsValid()) {
        gData.MaterialMgr->GetMaterialByName(i_Name, l_MatHdl);
    }
    return l_MatHdl;
}

S32 SystemDatas_Z::GetFontId(const Name_Z& i_Name) {
    for (S32 i = 0; i < m_FontHdls.GetSize(); i++) {
        if (m_FontHdls[i]->GetName() == i_Name) {
            return i;
        }
    }
    return -1;
}

Bool LoadMaterialLib() {
    return FALSE;
}

Bool LoadObjectLib() {
    return FALSE;
}

Bool RemoveObjectLib() {
    return FALSE;
}

Bool RemoveMaterialLib() {
    return FALSE;
}

Bool LoadSysRtc() {
    return FALSE;
}

Bool RemoveSysRtc() {
    return FALSE;
}
