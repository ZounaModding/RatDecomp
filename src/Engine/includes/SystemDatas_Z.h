#ifndef _SYSTEMDATAS_Z_H_
#define _SYSTEMDATAS_Z_H_
#include "Light_ZHdl.h"
#include "Material_ZHdl.h"
#include "Fonts_ZHdl.h"
#include "Rtc_ZHdl.h"
#include "GameObj_ZHdl.h"
#include "MaterialObj_ZHdl.h"
#include "MaterialAnim_ZHdl.h"
#include "Bitmap_ZHdl.h"
#include "Node_ZHdl.h"

class SystemDatas_Z {
public:
    SystemDatas_Z();
    ~SystemDatas_Z();

    MaterialAnim_ZHdl GetMaterialByName(const Name_Z& i_Name);
    S32 GetFontId(const Name_Z& i_Name);
    void MarkHandles();
    void Minimize();
    void AddExtGameObj(const GameObj_ZHdl& i_GameObjHdl);
    void RemoveExtGameObj(const GameObj_ZHdl& i_GameObjHdl);
    GameObj_ZHdl GetGameObjectByObjectName(const Name_Z& i_Name, S16 i_GameObjId);
    Node_ZHdl GetObjectByName(const Name_Z& i_Name, S16 i_GameObjId);
    Rtc_ZHdl GetSysRtcByObjectName(const Name_Z& i_Name);
    Bool LoadObjects(const Char* i_FileName);
    Bool RemoveObjects(const Char* i_FileName);
    Bool LoadMaterialLib(const Char* i_FileName);
    Bool RemoveMaterialLib(const Char* i_FileName);
    Bool LoadSysRtc(const Char* i_FileName);
    Bool RemoveSysRtc(const Char* i_FileName);

    Fonts_Z* GetFont(S32 i_FontId) const {
        if (i_FontId < 0 || i_FontId >= m_FontHdls.GetSize()) {
            return NULL;
        }
        return m_FontHdls[i_FontId];
    }

    inline LightData_ZHdl& GetDefaultLight() {
        return m_DefaultLightDataHdl;
    }

    inline Material_ZHdl& GetDefaultMaterial() {
        return m_DefaultMaterialHdl;
    }

    inline const MaterialAnim_ZHdl& GetDefaultMaterialAnim() const {
        return m_DefaultMaterialAnimHdl;
    }

    inline Material_ZHdl& GetBloomMaterial() {
        return m_BloomMaterialHdl;
    }

    inline Bitmap_ZHdl& GetWhiteBitmap() {
        return m_WhiteBitmapHdl;
    }

    inline Material_ZHdl& GetNoTextureMaterial() {
        return m_NoTextureMaterialHdl;
    }

private:
    GameObj_ZHdlDA m_ExtGameObjHdls;
    GameObj_ZHdlDA m_GameObjHdls;
    MaterialObj_ZHdlDA m_MaterialObjHdls;
    Fonts_ZHdlDA m_FontHdls;
    Rtc_ZHdlDA m_RtcHdls;
    Material_ZHdl m_ShadowMaterialHdl;
    Material_ZHdl m_BloomMaterialHdl;
    Material_ZHdl m_StencilMaterialHdl;
    Material_ZHdl m_SonarMaterialHdl;
    Material_ZHdl m_DefaultMaterialHdl;
    MaterialAnim_ZHdl m_DefaultMaterialAnimHdl;
    Bitmap_ZHdl m_WhiteBitmapHdl;
    Material_ZHdl m_BlackMaterialHdl;
    Bitmap_ZHdl m_BlackBitmapHdl;
    Bitmap_ZHdl m_BloomBitmapHdl;
    Material_ZHdl m_RadialMotionBlurMaterialHdl;
    Material_ZHdl m_OverdrawMaterialHdl;
    Material_ZHdl m_NoTextureMaterialHdl;
    Material_ZHdl m_FlashMaterialHdl;
    Material_ZHdl m_UnkMaterialHdl_0x60;
    Bitmap_ZHdl m_UnkBitmapHdl_0x64;
    LightData_ZHdl m_DefaultLightDataHdl;
    LightData_ZHdl m_SubDefaultLightDataHdl;
};
#endif // _SYSTEMDATAS_Z_H_
