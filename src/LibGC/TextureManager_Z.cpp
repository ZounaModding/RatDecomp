#include "TextureManager_Z.h"
#include "os/OSCache.h"
#include "gx/GXTexture.h"

TextureManager_Z::TextureManager_Z() { }

TextureManager_Z::~TextureManager_Z() {
    Shut();
}

Bool TextureManager_Z::Init(void) {
    return TRUE;
}

void TextureManager_Z::Shut(void) {
    S32 i = m_TextureHA.FindFirst();
    while (i != -1) {
        RemoveTexture(i);
        i = m_TextureHA.FindNext(i);
    }
}

// TODO: Finish matching
void TextureManager_Z::LoadTexture(
    Bitmap_Z* i_Bmap,
    GXTexWrapMode i_WrapS,
    GXTexWrapMode i_WrapT,
    GXTexMapID i_TexMapID
) {
    Texture_Z* l_Tex;
    S32 l_Size;
    S32 l_SizeX;
    S32 l_SizeY;

    if (i_Bmap->GetTexId() == INVALID_TEXID) {
        l_Size = i_Bmap->GetSize();

        DCStoreRange(i_Bmap->GetDatas(), l_Size);

        i_Bmap->SetTexId(m_TextureHA.Add());

        l_SizeX = i_Bmap->GetSizeX();
        l_SizeY = i_Bmap->GetSizeY();

        l_Tex = &m_TextureHA[i_Bmap->GetTexId()];

        switch (i_Bmap->GetFormat()) {
            case BM_4:
                GXInitTexObjCI(
                    &l_Tex->m_Tex,
                    i_Bmap->GetDatas(),
                    l_SizeX,
                    l_SizeY,
                    GX_TF_C4,
                    (GXTexWrapMode)i_WrapS,
                    (GXTexWrapMode)i_WrapT,
                    GX_FALSE,
                    GX_TLUT0
                );

                if (i_Bmap->GetPalFormat() == PAL_565) {
                    GXInitTlutObj(
                        &l_Tex->m_Tlut,
                        i_Bmap->GetPalette(),
                        GX_TL_RGB565,
                        i_Bmap->GetNbEntries()
                    );
                }
                else {
                    GXInitTlutObj(
                        &l_Tex->m_Tlut,
                        i_Bmap->GetPalette(),
                        GX_TL_RGB5A3,
                        i_Bmap->GetNbEntries()
                    );
                }

                l_Tex->m_Palette = TRUE;
                l_Tex->m_SecondaryPalette = FALSE;
                break;

            case BM_8:
                GXInitTexObjCI(
                    &l_Tex->m_Tex,
                    i_Bmap->GetDatas(),
                    l_SizeX,
                    l_SizeY,
                    GX_TF_C8,
                    (GXTexWrapMode)i_WrapS,
                    (GXTexWrapMode)i_WrapT,
                    GX_FALSE,
                    GX_TLUT0
                );

                if (i_Bmap->GetPalFormat() == PAL_565) {
                    GXInitTlutObj(
                        &l_Tex->m_Tlut,
                        i_Bmap->GetPalette(),
                        GX_TL_RGB565,
                        i_Bmap->GetNbEntries()
                    );
                }
                else {
                    GXInitTlutObj(
                        &l_Tex->m_Tlut,
                        i_Bmap->GetPalette(),
                        GX_TL_RGB5A3,
                        i_Bmap->GetNbEntries()
                    );
                }

                l_Tex->m_SecondaryPalette = FALSE;
                l_Tex->m_Palette = TRUE;
                break;

            case BM_565:
                GXInitTexObj(
                    &l_Tex->m_Tex,
                    i_Bmap->GetDatas(),
                    l_SizeX,
                    l_SizeY,
                    GX_TF_RGB565,
                    (GXTexWrapMode)i_WrapS,
                    (GXTexWrapMode)i_WrapT,
                    GX_FALSE
                );

                l_Tex->m_Palette = FALSE;
                l_Tex->m_SecondaryPalette = FALSE;
                break;

            case BM_5551:
            case BM_4444:
                GXInitTexObj(
                    &l_Tex->m_Tex,
                    i_Bmap->GetDatas(),
                    l_SizeX,
                    l_SizeY,
                    GX_TF_RGB5A3,
                    (GXTexWrapMode)i_WrapS,
                    (GXTexWrapMode)i_WrapT,
                    GX_FALSE
                );

                l_Tex->m_Palette = FALSE;
                l_Tex->m_SecondaryPalette = FALSE;
                break;

            case BM_8888:
            case BM_888:
                GXInitTexObj(
                    &l_Tex->m_Tex,
                    i_Bmap->GetDatas(),
                    l_SizeX,
                    l_SizeY,
                    GX_TF_RGBA8,
                    (GXTexWrapMode)i_WrapS,
                    (GXTexWrapMode)i_WrapT,
                    GX_FALSE
                );

                l_Tex->m_Palette = FALSE;
                l_Tex->m_SecondaryPalette = FALSE;
                break;

            case BM_I8:
                GXInitTexObj(
                    &l_Tex->m_Tex,
                    i_Bmap->GetDatas(),
                    l_SizeX,
                    l_SizeY,
                    GX_TF_I8,
                    (GXTexWrapMode)i_WrapS,
                    (GXTexWrapMode)i_WrapT,
                    GX_FALSE
                );

                l_Tex->m_Palette = FALSE;
                l_Tex->m_SecondaryPalette = FALSE;
                break;

            case BM_CMPR:
                GXInitTexObj(
                    &l_Tex->m_Tex,
                    i_Bmap->GetDatas(),
                    l_SizeX,
                    l_SizeY,
                    GX_TF_CMPR,
                    (GXTexWrapMode)i_WrapS,
                    (GXTexWrapMode)i_WrapT,
                    GX_FALSE
                );

                l_Tex->m_Palette = FALSE;
                l_Tex->m_SecondaryPalette = FALSE;
                break;
        }

        if (i_Bmap->IsFlagEnable(FL_BITMAP_BILINEAR)) {
            GXInitTexObjLOD(
                &l_Tex->m_Tex,
                GX_LINEAR,
                GX_LINEAR,
                0.0f,
                0.0f,
                0.0f,
                GX_FALSE,
                GX_FALSE,
                GX_ANISO_1
            );
        }
        else {
            GXInitTexObjLOD(
                &l_Tex->m_Tex,
                GX_NEAR,
                GX_NEAR,
                0.0f,
                0.0f,
                0.0f,
                GX_FALSE,
                GX_FALSE,
                GX_ANISO_1
            );
        }
    }
    else {
        l_Tex = &m_TextureHA[i_Bmap->GetTexId()];
    }

    l_Tex->m_Bmap = i_Bmap;
    l_Tex->m_Used = TRUE;

    if (l_Tex->m_Palette) {
        GXLoadTlut(&l_Tex->m_Tlut, GX_TLUT0);
    }

    GXLoadTexObj(&l_Tex->m_Tex, i_TexMapID);

    if (l_Tex->m_SecondaryPalette) {
        GXLoadTexObj(&l_Tex->m_SecondaryTex, GX_TEXMAP1);
    }
}

void TextureManager_Z::RemoveTexture(U32 i_TexId) {
    m_TextureHA[i_TexId].GetBmap()->SetTexId((S16)INVALID_TEXID);
    m_TextureHA.Remove(i_TexId);
}

void TextureManager_Z::Clean() {
    S32 i = m_TextureHA.FindFirst();
    while (i != -1) {
        if (m_TextureHA[i].m_Used)
            m_TextureHA[i].m_Used = FALSE;
        else
            RemoveTexture(i);

        i = m_TextureHA.FindNext(i);
    }
}

// TODO: Finish matching
Bool TextureManager_Z::Minimize() {
    Clean();
    m_TextureHA.Minimize();
    return TRUE;
}

void TextureManager_Z::BeginRender() {
    S32 i = m_TextureHA.FindFirst();
    while (i != -1) {
        m_TextureHA[i].m_Used = FALSE;
        i = m_TextureHA.FindNext(i);
    }
}

void TextureManager_Z::EndRender(S32& o_NbTex, S32& o_NbPal) {
    o_NbTex = 0;
    o_NbPal = 0;
}
