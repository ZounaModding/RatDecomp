#ifndef _BITMAP_Z_H_
#define _BITMAP_Z_H_
#include "ResourceObject_Z.h"
#include "Types_Z.h"
#include "Assert_Z.h"
#include "Color_Z.h"
#define INVALID_TEXID -1

#define FL_BITMAP_NONE (0 << 0)
#define FL_BITMAP_UNK_0x1 (1 << 0)
#define FL_BITMAP_BILINEAR (1 << 2)
#define FL_BITMAP_UNK_0x8 (1 << 3)
#define FL_BITMAP_UNK_0x10 (1 << 4)
#define FL_BITMAP_UNK_0x20 (1 << 5)

enum BmFormat_Z {
    BM_4 = 0x01,    // 4 Bits Indexed RGB
    BM_8 = 0x02,    // 8 Bits Indexed RGB
    BM_5551 = 0x07, // 16 RGBA
    BM_565 = 0x08,  // 16 RGB
    BM_4444 = 0x0A, // 16 RGBA
    BM_1555 = 0x0B, //.TGA compatibility
    BM_8888 = 0x0C, // 32 RGBA - Is DXT5 on PC
    BM_888 = 0x0D,  // 24 RGB - Is DXT1 on PC
    BM_CMPR = 0x0E, // S3TC
    BM_I8 = 0x0F,   // I8
    BM_I8A8 = 0x10, // I8A8
};

enum PalFormat_Z {
    PAL_3444 = 0x01,  // 16 ARGB
    PAL_565 = 0x02,   // 16 RGB
    PAL_8888 = 0x03,  // 32 RGBA
    PAL_LUM = 0x04,   // Luminance
    PAL_ALPHA = 0x05, // Alpha
    PAL_DXT1 = 0x11,  // DXT1 (OPAQUE)
    PAL_DXT3 = 0x12,  // DXT3 (ALPHA)
    PAL_DXT5 = 0x13,  // DXT5 (ALPHA)
};

enum BmTransp {
    BM_NO_TRANSP = 0,
    BM_TRANSP_ONE = 1,
    BM_TRANSP = 2
};

class Bitmap_Z : public ResourceObject_Z {
public:
    Bitmap_Z();
    Bitmap_Z(S32 i_Width, S32 i_Height, U8 i_Format, U8* i_Datas);
    virtual void Init();
    virtual ~Bitmap_Z();
    virtual void Load(void** i_Data);
    virtual void Clean();
    void InitBmap(S32 i_SizeX, S32 i_SizeY, U8 l_Format, U8* i_Datas, U8* i_Palette);

    static BaseObject_Z* NewObject() { return NewL_Z(196) Bitmap_Z; }

    S16 GetTexId() { return m_TexID; }

    S32 GetSizeX() { return m_SizeX; }

    S32 GetSizeY() { return m_SizeY; }

    U8* GetPalette() { return m_Palette; }

    U8 GetPalFormat() { return m_PalFormat; }

    void SetTexId(S16 i_TexId) { m_TexID = i_TexId; }

    void EnableFlag(U16 i_Flag) { m_Flag |= i_Flag; }

    void DisableFlag(U16 i_Flag) { m_Flag &= ~i_Flag; }

    U8 GetFormat() { return m_Format; }

    U8* GetDatas() { return m_Datas; }

    U8 GetMipMapCount() { return m_MipmapCount; }

    Bool IsFlagEnable(U16 i_Flag) { return (m_Flag & i_Flag) != 0; }

    void SetDatas(U8* i_Datas);

    inline void SetTransp(U8 i_Transp) {
        m_Transp = i_Transp;
    }

    inline U8 GetTransp() {
        return m_Transp;
    }

    inline Bool IsTransp(U8 i_Transp) {
        return m_Transp == i_Transp;
    }

    void Clear(Color i_Color);
    U8 GetBestPalEntry(U8 i_Red, U8 i_Green, U8 i_Blue, U8 i_Alpha);
    U16 GetColor(const Color& i_Color);
    void Reset();
    void Invalidate();
    Float GetBytePerPixel();
    S32 GetPalSize();
    void SetPoint(S32 i_X, S32 i_Y, const Color& i_Color);
    void SetPoint(U8* i_Data, U8 i_Forma, S32 i_X, S32 i_Y, const Color& i_Color);

    S32 GetPrecalculatedSize() {
        return m_PrecalculatedSize;
    }

    S32 GetSize() {
        if (m_PrecalculatedSize) {
            return m_PrecalculatedSize;
        }
        return GetDataSize();
    }

    S32 GetDataSize();

    S32 GetNbEntries() {
        if (m_Format == BM_4) {
            return 16;
        }
        else if (m_Format == BM_8) {
            return 256;
        }
        else {
            return 0;
        }
    }

    void SetUniversal(U8* i_Datas);
    void Save(const Char* i_FileName);

private:
    U8* m_Datas;
    U8* m_Palette;
    S16 m_TexID;
    S32 m_SizeX;
    S32 m_SizeY;
    S32 m_PrecalculatedSize;
    U8 m_Format;
    U8 m_TrueFormat;
    U8 m_PalFormat;
    U8 m_Transp;
    U8 m_MipmapCount;
    U8 m_UnkU8_0x31; // Always set to 4
    U16 m_Flag;
};

void SaveTGA(const Char* i_FileName, U8* i_Data, S32 i_SizeX, S32 i_SizeY, S32 i_Bpp);

#endif
