// $SABE: Having to add these pragmas is a bit disgusting but it gets it matching, fine for now at least
#pragma use_lmw_stmw off
#pragma optimize_for_size off
#include "Handle_Z.h"
#pragma optimize_for_size on
#include "SystemDatas_Z.h"
#pragma use_lmw_stmw on
#include "ClassManager_Z.h"
#include "Math_Z.h"
#include "Material_Z.h"
#include "MaterialAnim_Z.h"
#include "MaterialManager_Z.h"
#include "LightData_Z.h"
#include "MaterialObj_Z.h"
#include "Fonts_Z.h"
#include "Main_Z.h"
#include "Console_Z.h"
#include "GameObj_Z.h"
#include "Rtc_Z.h"
#include "KSys_Z.h"

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
    for (S32 i = 0; i < m_ExtGameObjHdls.GetSize(); i++) {
        m_ExtGameObjHdls[i]->MarkHandles();
    }
    for (S32 i = 0; i < m_GameObjHdls.GetSize(); i++) {
        m_GameObjHdls[i]->MarkHandles();
    }
    for (S32 i = 0; i < m_MaterialObjHdls.GetSize(); i++) {
        m_MaterialObjHdls[i]->MarkHandles();
    }
    for (S32 i = 0; i < m_FontHdls.GetSize(); i++) {
        m_FontHdls[i]->MarkHandles();
    }
    for (S32 i = 0; i < m_RtcHdls.GetSize(); i++) {
        m_RtcHdls[i]->MarkHandles();
    }

    if (m_ShadowMaterialHdl.IsValid()) {
        m_ShadowMaterialHdl->MarkHandles();
    }
    if (m_BloomMaterialHdl.IsValid()) {
        m_BloomMaterialHdl->MarkHandles();
    }
    if (m_StencilMaterialHdl.IsValid()) {
        m_StencilMaterialHdl->MarkHandles();
    }
    if (m_OverdrawMaterialHdl.IsValid()) {
        m_OverdrawMaterialHdl->MarkHandles();
    }
    if (m_NoTextureMaterialHdl.IsValid()) {
        m_NoTextureMaterialHdl->MarkHandles();
    }
    if (m_FlashMaterialHdl.IsValid()) {
        m_FlashMaterialHdl->MarkHandles();
    }
    if (m_SonarMaterialHdl.IsValid()) {
        m_SonarMaterialHdl->MarkHandles();
    }
    if (m_RadialMotionBlurMaterialHdl.IsValid()) {
        m_RadialMotionBlurMaterialHdl->MarkHandles();
    }
    m_DefaultMaterialHdl->MarkHandles();
    m_DefaultMaterialAnimHdl->MarkHandles();
    m_WhiteBitmapHdl->MarkHandles();
    m_BlackBitmapHdl->MarkHandles();
    m_BloomBitmapHdl->MarkHandles();
    m_DefaultLightDataHdl->MarkHandles();
    m_SubDefaultLightDataHdl->MarkHandles();
    m_BlackMaterialHdl->MarkHandles();
    if (m_UnkMaterialHdl_0x60.IsValid()) {
        m_UnkMaterialHdl_0x60->MarkHandles();
    }
    if (m_UnkBitmapHdl_0x64.IsValid()) {
        m_UnkBitmapHdl_0x64->MarkHandles();
    }
}

void SystemDatas_Z::Minimize() {
    m_ExtGameObjHdls.Minimize();
    m_GameObjHdls.Minimize();
    m_MaterialObjHdls.Minimize();
}

void SystemDatas_Z::AddExtGameObj(const GameObj_ZHdl& i_GameObjHdl) {
    m_ExtGameObjHdls.Add(i_GameObjHdl);
}

void SystemDatas_Z::RemoveExtGameObj(const GameObj_ZHdl& i_GameObjHdl) {
    for (S32 i = 0; i < m_ExtGameObjHdls.GetSize(); i++) {
        if (m_ExtGameObjHdls[i] == i_GameObjHdl) {
            m_ExtGameObjHdls.Remove(i);
            break;
        }
    }
}

GameObj_ZHdl SystemDatas_Z::GetGameObjectByObjectName(const Name_Z& i_Name, S16 i_GameObjId) {
    for (S32 i = 0; i < m_GameObjHdls.GetSize(); i++) {
        Node_ZHdl l_NodeHdl = m_GameObjHdls[i]->GetObjectByName(i_Name, i_GameObjId);
        if (l_NodeHdl.IsValid()) {
            return m_GameObjHdls[i];
        }
    }
    for (S32 i = 0; i < m_ExtGameObjHdls.GetSize(); i++) {
        Node_ZHdl l_NodeHdl = m_ExtGameObjHdls[i]->GetObjectByName(i_Name, i_GameObjId);
        if (l_NodeHdl.IsValid()) {
            return m_ExtGameObjHdls[i];
        }
    }
    return GameObj_ZHdl(0);
}

Node_ZHdl SystemDatas_Z::GetObjectByName(const Name_Z& i_Name, S16 i_GameObjId) {
    Node_ZHdl l_NodeHdl(0);
    for (S32 i = 0; i < m_GameObjHdls.GetSize(); i++) {
        l_NodeHdl = m_GameObjHdls[i]->GetObjectByName(i_Name, i_GameObjId);
        if (l_NodeHdl.IsValid()) {
            return l_NodeHdl;
        }
    }
    for (S32 i = 0; i < m_ExtGameObjHdls.GetSize(); i++) {
        l_NodeHdl = m_ExtGameObjHdls[i]->GetObjectByName(i_Name, i_GameObjId);
        if (l_NodeHdl.IsValid()) {
            break;
        }
    }
    return l_NodeHdl;
}

Rtc_ZHdl SystemDatas_Z::GetSysRtcByObjectName(const Name_Z& i_Name) {
    for (S32 i = 0; i < m_RtcHdls.GetSize(); i++) {
        if (m_RtcHdls[i]->GetName() == i_Name) {
            return m_RtcHdls[i];
        }
    }
    return Rtc_ZHdl(0);
}

// $SABE: Added this to force HandleManager_Z::GetHandleName before SystemDatas_Z::GetMaterialByName
static Name_Z FixOrder() {
    BaseObject_ZHdl l_Handle;
    Name_Z l_HandleName = gData.ClassMgr->GetHandleName(l_Handle);
    return l_HandleName;
}

MaterialAnim_ZHdl SystemDatas_Z::GetMaterialByName(const Name_Z& i_Name) {
    MaterialAnim_ZHdl l_MaterialHdl;
    for (S32 i = 0; !l_MaterialHdl.IsValid() && i < m_MaterialObjHdls.GetSize(); i++) {
        l_MaterialHdl = m_MaterialObjHdls[i]->GetMaterialByName(i_Name);
    }
    if (!l_MaterialHdl.IsValid()) {
        gData.MaterialMgr->GetMaterialByName(i_Name, l_MaterialHdl);
    }
    return l_MaterialHdl;
}

Bool SystemDatas_Z::LoadObjects(const Char* i_FileName) {
    GameObj_ZHdl l_GameObjHdl = gData.ClassMgr->GetResourceObject(i_FileName);
    if (!l_GameObjHdl.IsValid()) {
        return FALSE;
    }
    for (S32 i = 0; i < m_GameObjHdls.GetSize(); i++) {
        if (m_GameObjHdls[i] == l_GameObjHdl) {
            return TRUE;
        }
    }
    m_GameObjHdls.Add(l_GameObjHdl);
    return TRUE;
}

Bool SystemDatas_Z::RemoveObjects(const Char* i_FileName) {
    String_Z<ARRAY_CHAR_MAX> l_FileName;
    PathKDBToSys(i_FileName, l_FileName);
    String_Z<ARRAY_CHAR_MAX> l_DbFileName;
    PathSysToDB(l_FileName, l_DbFileName);
    const Name_Z l_ResourceName(l_DbFileName);

    for (S32 i = 0; i < m_GameObjHdls.GetSize(); i++) {
        const Name_Z& l_HandleName = gData.ClassMgr->GetHandleName(m_GameObjHdls[i]);
        if (l_HandleName == l_ResourceName) {
            m_GameObjHdls.Remove(i);
            return TRUE;
        }
    }
    return FALSE;
}

Bool SystemDatas_Z::LoadMaterialLib(const Char* i_FileName) {
    MaterialObj_ZHdl l_MaterialObjHdl = gData.ClassMgr->GetResourceObject(i_FileName);
    if (!l_MaterialObjHdl.IsValid()) {
        return FALSE;
    }

    S32 i;
    for (i = 0; i < m_MaterialObjHdls.GetSize(); i++) {
        if (m_MaterialObjHdls[i] == l_MaterialObjHdl) {
            break;
        }
    }
    if (i == m_MaterialObjHdls.GetSize()) {
        m_MaterialObjHdls.Add(l_MaterialObjHdl);
    }

    MaterialAnim_ZHdl l_ShadowMaterialAnimHdl = GetMaterialByName(Name_Z("SHADOW"));
    if (l_ShadowMaterialAnimHdl.IsValid()) {
        m_ShadowMaterialHdl = l_ShadowMaterialAnimHdl->GetMaterial();
    }
    return l_MaterialObjHdl.IsValid();
}

Bool SystemDatas_Z::RemoveMaterialLib(const Char* i_FileName) {
    String_Z<ARRAY_CHAR_MAX> l_FileName;
    PathKDBToSys(i_FileName, l_FileName);
    String_Z<ARRAY_CHAR_MAX> l_DbFileName;
    PathSysToDB(l_FileName, l_DbFileName);
    const Name_Z l_ResourceName(l_DbFileName);

    for (S32 i = 0; i < m_MaterialObjHdls.GetSize(); i++) {
        const Name_Z& l_HandleName = gData.ClassMgr->GetHandleName(m_MaterialObjHdls[i]);
        if (l_HandleName == l_ResourceName) {
            m_MaterialObjHdls.Remove(i);
            return TRUE;
        }
    }
    return FALSE;
}

Bool SystemDatas_Z::LoadSysRtc(const Char* i_FileName) {
    Rtc_ZHdl l_RtcHdl = gData.ClassMgr->GetResourceObject(i_FileName);
    if (!l_RtcHdl.IsValid()) {
        return FALSE;
    }

    S32 i;
    for (i = 0; i < m_RtcHdls.GetSize(); i++) {
        if (m_RtcHdls[i] == l_RtcHdl) {
            break;
        }
    }
    if (i == m_RtcHdls.GetSize()) {
        m_RtcHdls.Add(l_RtcHdl);
        return TRUE;
    }
    return FALSE;
}

Bool SystemDatas_Z::RemoveSysRtc(const Char* i_FileName) {
    String_Z<ARRAY_CHAR_MAX> l_FileName;
    PathKDBToSys(i_FileName, l_FileName);
    String_Z<ARRAY_CHAR_MAX> l_DbFileName;
    PathSysToDB(l_FileName, l_DbFileName);
    const Name_Z l_ResourceName(l_DbFileName);

    for (S32 i = 0; i < m_RtcHdls.GetSize(); i++) {
        const Name_Z& l_HandleName = gData.ClassMgr->GetHandleName(m_RtcHdls[i]);
        if (l_HandleName == l_ResourceName) {
            m_RtcHdls.Remove(i);
            return TRUE;
        }
    }
    return FALSE;
}

S32 SystemDatas_Z::GetFontId(const Name_Z& i_Name) {
    for (S32 i = 0; i < m_FontHdls.GetSize(); i++) {
        if (m_FontHdls[i]->GetName() == i_Name) {
            return i;
        }
    }
    return -1;
}

static const Char s_TextureDebugMat[] = "TextureDebugMat";

Bool LoadMaterialLib() {
    if (gData.Cons->GetNbParam() != 2) {
        return TRUE;
    }
    String_Z<ARRAY_CHAR_MAX> l_FileName;
    l_FileName.StrCpy("DB:>");
    l_FileName.StrCat(gData.Cons->GetParamStr(1));
    l_FileName.StrCat(".TOTEMBITMAP");
    return gData.SystemDatas->LoadMaterialLib(l_FileName);
}

Bool LoadObjectLib() {
    if (gData.Cons->GetNbParam() != 2) {
        return TRUE;
    }
    String_Z<ARRAY_CHAR_MAX> l_FileName;
    l_FileName.StrCpy("DB:>");
    l_FileName.StrCat(gData.Cons->GetParamStr(1));
    l_FileName.StrCat(".TGAMEOBJ");
    return gData.SystemDatas->LoadObjects(l_FileName);
}

Bool RemoveObjectLib() {
    if (gData.Cons->GetNbParam() != 2) {
        return TRUE;
    }
    String_Z<ARRAY_CHAR_MAX> l_FileName;
    l_FileName.StrCpy("DB:>");
    l_FileName.StrCat(gData.Cons->GetParamStr(1));
    l_FileName.StrCat(".TGAMEOBJ");
    return gData.SystemDatas->RemoveObjects(l_FileName);
}

Bool RemoveMaterialLib() {
    if (gData.Cons->GetNbParam() != 2) {
        return TRUE;
    }
    String_Z<ARRAY_CHAR_MAX> l_FileName;
    l_FileName.StrCpy("DB:>");
    l_FileName.StrCat(gData.Cons->GetParamStr(1));
    l_FileName.StrCat(".TOTEMBITMAP");
    return gData.SystemDatas->RemoveMaterialLib(l_FileName);
}

Bool LoadSysRtc() {
    if (gData.Cons->GetNbParam() != 2) {
        return TRUE;
    }
    String_Z<ARRAY_CHAR_MAX> l_FileName;
    l_FileName.StrCpy("DB:>");
    l_FileName.StrCat(gData.Cons->GetParamStr(1));
    l_FileName.StrCat("_RTC.TRTC");
    return gData.SystemDatas->LoadSysRtc(l_FileName);
}

Bool RemoveSysRtc() {
    if (gData.Cons->GetNbParam() != 2) {
        return TRUE;
    }
    String_Z<ARRAY_CHAR_MAX> l_FileName;
    l_FileName.StrCpy("DB:>");
    l_FileName.StrCat(gData.Cons->GetParamStr(1));
    l_FileName.StrCat("_RTC.TRTC");
    return gData.SystemDatas->RemoveSysRtc(l_FileName);
}
