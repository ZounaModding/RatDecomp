#include "Bitmap_Z.h"
#include "Memory_Z.h"
#include <string.h>

/* 
    Linear/iterative twiddling from Marcus' tatest, the same one pvr_txr_load_ex
    uses internally. LoadTexture copies the data directly, and shipped textures will
    be pre-twiddled, so bitmaps built at runtime have to be twiddled here instead.
*/
#define TWIDTAB(x) ((x & 1) | ((x & 2) << 1) | ((x & 4) << 2) | ((x & 8) << 3) | ((x & 16) << 4) | ((x & 32) << 5) | ((x & 64) << 6) | ((x & 128) << 7) | ((x & 256) << 8) | ((x & 512) << 9))
#define TWIDOUT(x, y) (TWIDTAB(y) | (TWIDTAB(x) << 1))

static Bool Is16BppFormat(U8 i_Format) {
    return i_Format == BM_4444 || i_Format == BM_565 || i_Format == BM_5551 || i_Format == BM_1555;
}

// Reached only by runtime-built and compiled-in bitmaps
void Bitmap_Z::SetUniversal(U8* i_Datas) {
    S32 l_Min = m_SizeX < m_SizeY ? m_SizeX : m_SizeY;
    S32 l_Mask = l_Min - 1;

    // The PVR has no 32bpp texture format, so BM_8888 is
    // narrowed to BM_4444 and the format updated to match.
    if (m_Format == BM_8888) {
        U16* l_Pixels = (U16*)AllocContiguousAlignCL_Z(m_SizeX * m_SizeY * 2, "BITMAP_DATA_ALLOC", 29, 128);
        U32* l_Source = (U32*)i_Datas;
        // clang-format off
        for (S32 y = 0; y < m_SizeY; y++) {
            for (S32 x = 0; x < m_SizeX; x++) {
                U32 l_Pixel = l_Source[y * m_SizeX + x];
                l_Pixels[
                    TWIDOUT(x & l_Mask, y & l_Mask) +
                    (x / l_Min + y / l_Min) * l_Min * l_Min
                ] = ((l_Pixel << 8) & 0xF000)  |
                    ((l_Pixel >> 20) & 0x0F00) |
                    ((l_Pixel >> 16) & 0x00F0) |
                    ((l_Pixel >> 12) & 0x000F);
            }
        }
        // clang-format on
        m_Format = BM_4444;
        SetDatas((U8*)l_Pixels);
        return;
    }

    S32 l_DataSize = GetDataSize();
    U8* l_Datas = (U8*)AllocContiguousAlignCL_Z(l_DataSize, "BITMAP_DATA_ALLOC", 50, 128);

    if (Is16BppFormat(m_Format)) {
        U16* l_Source = (U16*)i_Datas;
        U16* l_Pixels = (U16*)l_Datas;
        for (S32 y = 0; y < m_SizeY; y++) {
            for (S32 x = 0; x < m_SizeX; x++) {
                l_Pixels[TWIDOUT(x & l_Mask, y & l_Mask) + (x / l_Min + y / l_Min) * l_Min * l_Min] = l_Source[y * m_SizeX + x];
            }
        }
    }
    else {
        // TODO: the 4bpp and 8bpp palettised formats need their own twiddle passes
        memcpy(l_Datas, i_Datas, l_DataSize);
    }

    SetDatas(l_Datas);
}
