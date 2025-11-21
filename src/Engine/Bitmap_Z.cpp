#include "Bitmap_Z.h"
#include "Memory_Z.h"
#include "Program_Z.h"
#include "Renderer_Z.h"
Extern_Z "C" void memcpy(void* dest, const void* src, int n);
#pragma dont_inline on

Bitmap_Z::Bitmap_Z() {
    Init();
}

Bitmap_Z::Bitmap_Z(S32 i_Width, S32 i_Height, U8 i_Format, U8* i_Datas) {
    Init();
    InitBmap(i_Width, i_Height, i_Format, i_Datas, 0);
}

Bitmap_Z::~Bitmap_Z() {
    Reset();
}

void Bitmap_Z::Init() {
    m_Flag = 0;
    Bitmap_Z::EnableFlag(0x14);
    m_Datas = 0;
    m_Palette = 0;
    m_SizeX = 0;
    m_SizeY = 0;
    m_TrueFormat = 0;
    m_UnkU8_0x31 = 4;
    m_TexID = INVALID_TEXID;
    m_Transp = -1;
    m_MipmapCount = 0;
    m_PrecalculatedSize = 0;
    m_PalFormat = PAL_8888;
}

void Bitmap_Z::Invalidate() {
    if ((gData.MainRdr != NULL) && (m_TexID != INVALID_TEXID)) {
        gData.MainRdr->FreeTexture(m_TexID);
    }
    m_TexID = INVALID_TEXID;
}

void Bitmap_Z::Reset() {
    Invalidate();
    if (m_Datas) {
        FreeContiguous_Z(m_Datas);
    }
    m_Datas = 0;

    if (m_Palette) {
        Delete_Z m_Palette;
    }
    m_Palette = 0;

    Init();
}

void Bitmap_Z::InitBmap(S32 i_SizeX, S32 i_SizeY, U8 i_Format, U8* i_Datas, U8* i_Palette) {
    S32 l_PaletteSize;
    S32 l_BytePalleteSize;

    m_TexID = INVALID_TEXID;
    m_SizeX = i_SizeX;
    m_SizeY = i_SizeY;
    m_Format = i_Format;
    m_TrueFormat = i_Format;
    m_MipmapCount = 0;

    l_PaletteSize = GetPalSize();
    GetBytePerPixel();

    if (i_Format == BM_4 || i_Format == BM_8) {
        l_BytePalleteSize = 4 * l_PaletteSize;
        m_Palette = (U8*)AllocAlignCL_Z(l_BytePalleteSize, "BITMAP_PAL_ALLOC", 74, 128);
        if (i_Palette)
            memcpy(m_Palette, i_Palette, l_BytePalleteSize);
    }
    else {
        m_Palette = 0;
    }

    if (i_Datas) {
        SetUniversal(i_Datas);
    }
    else {
        m_Datas = (U8*)AllocContiguousAlignCL_Z(GetDataSize(), "BITMAP_DATA_ALLOC", 92, 128);

        Clear(Color(0.0, 0.0, 0.0, 0.0));
    }
}

Float Bitmap_Z::GetBytePerPixel() {
    Float l_Result; // st7

    switch (m_Format) {
        case BM_8888:
            l_Result = 4.0;
            break;
        case BM_888:
            l_Result = 3.0;
            break;
        case BM_4:
            l_Result = 0.5;
            break;
        case BM_8:
        case BM_I4A4:
            l_Result = 1.0;
            break;
        case BM_5551:
        case BM_565:
        case BM_4444:
        case BM_1555:
            l_Result = 2.0;
            break;
        case BM_CMPR:
            l_Result = 0.0;
            break;
        default:
            ASSERTL_Z(FALSE, "Bitmap_Z::GetBytePerPixel", 117);
            l_Result = 0.0;
            break;
    }
    return l_Result;
}

S32 Bitmap_Z::GetPalSize() {
    switch (GetFormat()) {
        case BM_4:
            return 16;
        case BM_8:
            return 256;
        case BM_5551:
        case BM_565:
        case BM_4444:
        case BM_1555:
        case BM_8888:
        case BM_888:
        case BM_CMPR:
        case BM_I4A4:
            return 0;
        default:
            ASSERTL_Z(FALSE, "Bitmap_Z::GetPalSize", 138);
            return 0;
    }
}

S32 Bitmap_Z::GetDataSize() {
    S32 l_MipSize;
    S32 l_DataSize;
    U8 l_MipmapCount;
    l_DataSize = (GetBytePerPixel() * ((Float)m_SizeY * m_SizeX));
    l_MipmapCount = m_MipmapCount;
    for (l_MipSize = l_DataSize; l_MipmapCount--; l_MipSize += (l_DataSize + 127) & ~127) {
        l_DataSize >>= 2;
    }
    return l_MipSize;
}

void Bitmap_Z::SetDatas(U8* i_Datas) {
    if (i_Datas != m_Datas) {
        if (m_Datas) {
            FreeContiguous_Z(m_Datas);
        }
        m_Datas = 0;
        m_Datas = i_Datas;
    }
}

// $VIOLET: This function might make me become religious
U16 Bitmap_Z::GetColor(const Color& i_Color) {
    U16 l_Red = (i_Color.r * 255.0f);
    U16 l_Green = (i_Color.g * 255.0f);
    U16 l_Blue = (i_Color.b * 255.0f);
    U16 l_Alpha = (i_Color.a * 255.0f);

    U16 l_Color;

    switch (m_Format) {
        case BM_565:
            l_Blue = l_Blue >> 3;
            l_Green = l_Green >> 2;
            l_Red = l_Red >> 3;
            l_Color = (l_Red << 11) + (l_Green << 5) + l_Blue;
            break;
        case BM_5551:
            l_Blue = l_Blue >> 3;
            l_Green = l_Green >> 3;
            l_Red = l_Red >> 3;
            l_Alpha = l_Alpha >> 7;
            l_Color = (l_Alpha << 15) + (l_Red << 10) + (l_Green << 5) + l_Blue;
            break;
        case BM_4444:
            l_Blue = l_Blue >> 4;
            l_Green = l_Green >> 4;
            l_Red = l_Red >> 4;
            l_Alpha = l_Alpha >> 4;
            l_Color = (l_Alpha << 12) + (l_Red << 8) + (l_Green << 4) + l_Blue;
            break;
        case BM_1555:
            l_Blue = l_Blue >> 3;
            l_Green = l_Green >> 3;
            l_Red = l_Red >> 3;
            l_Alpha = l_Alpha >> 7;
            l_Color = (l_Red << 11) + (l_Green << 6) + (l_Blue << 1) + l_Alpha;
            break;
        case BM_4:
        case BM_8:
            return GetBestPalEntry(l_Red, l_Green, l_Blue, l_Alpha);
    }
    return l_Color;
}

U8 Bitmap_Z::GetBestPalEntry(U8 i_Red, U8 i_Green, U8 i_Blue, U8 i_Alpha) {
    Float l_CmpValue = 1099511600000.0f;
    Float l_Match = 0;
    U8 l_BestMatch = -1;
    U8 l_Red, l_Green, l_Blue, l_Alpha = 0;

    for (S32 i = 0; i < GetNbEntries(); i++) {
        if (m_PalFormat == PAL_8888) {
            U8* l_Palette = (U8*)m_Palette;
            l_Red = l_Palette[4 * i];
            l_Green = l_Palette[4 * i + 1];
            l_Blue = l_Palette[4 * i + 2];
            l_Alpha = l_Palette[4 * i + 3];
        }
        else if (m_PalFormat == PAL_565) {
            U16* l_Palette = (U16*)m_Palette;
            l_Red = (l_Palette[i] >> 8) & 0xF8;
            l_Green = (l_Palette[i] >> 3) & 0xFC;
            l_Blue = (l_Palette[i] & 0x1F) << 3;
            l_Alpha = -1;
        }
        else {
            U16* l_Palette = (U16*)m_Palette;
            l_Red = (l_Palette[i] >> 4) & 0xF0;
            l_Green = (l_Palette[i]) & 0xF0;
            l_Blue = (l_Palette[i] << 4) & 0xF0;
            l_Alpha = (l_Palette[i] >> 7) & 0xE0;
        }

        l_Match = ((Float)(l_Red - i_Red) * (Float)(l_Red - i_Red)) + ((l_Green - i_Green) * (l_Green - i_Green)) + ((l_Blue - i_Blue) * (l_Blue - i_Blue)) + ((l_Alpha - i_Alpha) * (l_Alpha - i_Alpha));

        if (l_Match < l_CmpValue) {
            l_CmpValue = l_Match;
            l_BestMatch = i;
        }
    }

    return l_BestMatch;
}

void Bitmap_Z::Clear(Color i_Color) {
    switch (m_Format) {
        case BM_4: {
            U8* l_Datas8 = (U8*)m_Datas;
            U8 l_Color8 = GetColor(i_Color);
            for (S32 i = 0; i < m_SizeX * m_SizeY / 2; i++) {
                l_Datas8[i] = ((l_Color8 & 0xf) + (l_Color8 << 4)) & 0xff;
            }
        } break;
        case BM_8: {
            U8* l_Datas8 = (U8*)m_Datas;
            U8 l_Color8 = GetColor(i_Color);
            for (S32 i = 0; i < m_SizeX * m_SizeY; i++) {
                l_Datas8[i] = l_Color8;
            }
        } break;
        case BM_5551:
        case BM_565:
        case BM_4444:
        case BM_1555: {
            U16* l_Datas16 = (U16*)m_Datas;
            U16 l_Color16 = GetColor(i_Color);
            for (S32 i = 0; i < m_SizeX * m_SizeY; i++) {
                l_Datas16[i] = l_Color16;
            }
        } break;
        case BM_888: {
            U8* l_Data = (U8*)m_Datas;
            U8 l_Red = i_Color.r * 255.0f;
            U8 l_Green = i_Color.g * 255.0f;
            U8 l_Blue = i_Color.b * 255.0f;
            S32 i;
            for (i = 0; i < m_SizeX * m_SizeY; i++) {
                l_Data[3 * i] = l_Red;
                l_Data[3 * i] = l_Green;
                l_Data[3 * i] = l_Blue;
            }
        } break;
        case BM_8888: {
            U32* l_Data = (U32*)m_Datas;
            Float l_Red = i_Color.r * 255.0f;
            Float l_Green = i_Color.g * 255.0f;
            Float l_Blue = i_Color.b * 255.0f;
            Float l_Alpha = i_Color.a * 255.0f;
            for (S32 i = 0; i < m_SizeX * m_SizeY; i++) {
                l_Data[i] = ((int)(l_Alpha) * 0x1000000) + (((int)(l_Red) & 0xFF) * 0x10000) + (((int)(l_Green) & 0xFF) * 0x100) + ((int)(l_Blue) & 0xFF);
            }
        } break;
        default:
            break;
    }
}

// $VIOLET: TODO: Fix this
void Bitmap_Z::SetPoint(S32 i_X, S32 i_Y, const Color& i_Color) {
    if ((i_X > -1) && (i_X < m_SizeX) && (i_Y > -1) && (i_Y < m_SizeY)) {
        SetPoint(&((U8*)m_Datas)[i_X + i_Y * m_SizeX * (int)GetBytePerPixel()], GetFormat(), i_X, i_Y, i_Color);
    }
}

void Bitmap_Z::SetPoint(U8* i_Datas, U8 i_Format, S32 i_X, S32 i_Y, const Color& i_Color) {
    U8 l_Red = i_Color.r * 255.0f;
    U8 l_Green = i_Color.g * 255.0f;
    U8 l_Blue = i_Color.b * 255.0f;
    U8 l_Alpha = i_Color.a * 255.0f;
    switch (i_Format) {
        case BM_8888:
            i_Datas[0] = l_Blue;
            i_Datas[1] = l_Green;
            i_Datas[2] = l_Red;
            i_Datas[3] = l_Alpha;
            break;
        case BM_888:
            i_Datas[0] = l_Blue;
            i_Datas[1] = l_Green;
            i_Datas[2] = l_Red;
    }
}

#pragma dont_inline off
