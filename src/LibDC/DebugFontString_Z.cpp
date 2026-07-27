#include "Font_Z.h"
#include "DCRenderer_Z.h"
#include "DebugFontString_Z.h"
#include "Bitmap_Z.h"
#include "Material_Z.h"

void DebugFontString_Z::Init() {
    Material_Z* l_Material = m_MaterialHdl;
    if (l_Material) {
        return;
    }

    U8* l_BitmapData = (U8*)AllocContiguousAlignCL_Z(65536, "BITMAP_DATA_ALLOC", 15, 128);

    for (S32 i = 0; i < 256; i++) {
        m_Characters[i].m_Valid = FALSE;
    }

    S32 x;
    S32 y;
    for (S32 i = 0; i < 223; i++) {
        S32 l_X = (i & 15) * 8;
        S32 l_Y = (i >> 4) * 8;

        for (y = 0; y < 8; y++) {
            U8* l_Destination = l_BitmapData + (l_X + (l_Y + y) * 128) * 2;
            for (x = 0; x < 8; x++) {
                if (Font[i][y][x]) {
                    *(U16*)l_Destination = 65535;
                }
                else {
                    *(U16*)l_Destination = 0;
                }
                l_Destination += 2;
            }
        }

        ScanCode_Z& l_Character = m_Characters[i + 32];
        l_Character.m_U1 = (l_X + 0.05f) / 128.0f;
        l_Character.m_U2 = ((Float)(l_X + 8) - 0.3f) / 128.0f;
        l_Character.m_V1 = (l_Y + 0.05f) / 128.0f;
        l_Character.m_V2 = ((Float)(l_Y + 8) - 0.3f) / 128.0f;
        l_Character.m_Valid = TRUE;
        l_Character.m_SizeX = 8.0f;
        l_Character.m_SizeY = 8.0f;
    }

    Name_Z l_MaterialName(Name_Z::GetID("ShadowMaterial", 0));
    Name_Z l_MaterialClass(Name_Z::GetID("Material_Z", 0));
    m_MaterialHdl = gData.ClassMgr->NewObject(l_MaterialClass, l_MaterialName);

    Name_Z l_BitmapName(Name_Z::GetID("ShadowBitmap", 0));
    Name_Z l_BitmapClass(Name_Z::GetID("Bitmap_Z", 0));
    Bitmap_ZHdl l_BitmapHdl = gData.ClassMgr->NewObject(l_BitmapClass, l_BitmapName);
    Bitmap_Z* l_Bitmap = l_BitmapHdl;
    l_Bitmap->InitBmap(128, 128, BM_4444, l_BitmapData, NULL);
    l_Bitmap->EnableFlag(FL_BITMAP_UNK_0x8);
    l_Bitmap->DisableFlag(FL_BITMAP_BILINEAR | FL_BITMAP_UNK_0x10 | FL_BITMAP_UNK_0x20);
    l_Bitmap->SetTransp(BM_TRANSP_ONE);

    m_MaterialHdl->SetBitmap(l_BitmapHdl);
    m_MaterialHdl->EnableRenderFlag(FL_MTL_RDR_TEX_ADDRESS_CLAMP_U | FL_MTL_RDR_TEX_ADDRESS_CLAMP_V);

    if (l_BitmapData) {
        FreeContiguous_Z(l_BitmapData);
    }
}

void DebugFontString_Z::MarkHandles() {
    m_MaterialHdl->MarkHandles();
}
