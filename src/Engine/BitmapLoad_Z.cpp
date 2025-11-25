#include "Bitmap_Z.h"
#include "Color_Z.h"
#include "GCMain_Z.h"
#include "Memory_Z.h"
#include "Sys_Z.h"
#include "File_Z.h"
#include "Types_Z.h"

void Bitmap_Z::Load(void** i_Data) {
    S32 l_DataSize;
    S32 l_AmountOfColors;
    S32 l_PalSize;
    LOADRANGE_Z(m_SizeX, m_Flag);
    if (m_PrecalculatedSize) {
        m_Datas = (U8*)AllocContiguousAlignCL_Z(m_PrecalculatedSize, "BITMAP_DATA_ALLOC", 13, 128);
        LOADBYTES_Z(m_Datas, m_PrecalculatedSize);
    }
    else {
        l_DataSize = GetDataSize();
        m_Datas = (U8*)AllocContiguousAlignCL_Z(l_DataSize, "BITMAP_DATA_ALLOC", 18, 128);
        LOADBYTES_Z(m_Datas, l_DataSize);
    }
    l_AmountOfColors = GetPalSize();
    if (l_AmountOfColors != 0) {
        l_PalSize = 4 * l_AmountOfColors;
        m_Palette = (U8*)AllocAlignCL_Z(l_PalSize, "BITMAP_PAL_ALLOC", 26, 128);
        LOADBYTES_Z(m_Palette, l_PalSize);
    }
}

void Bitmap_Z::Clean() {
    U8 l_Format = m_Format;
    Reset();
    m_Format = l_Format;
    m_TrueFormat = l_Format;
}

void SaveTGA(const Char* i_FileName, U8* i_Data, S32 i_SizeX, S32 i_SizeY, S32 i_Bpp) {
    U8 l_Header[18];
    File_Z l_File;
    if (!l_File.Open(i_FileName, 0x1a)) {
        return;
    }
    else {
        // create header
        l_Header[0] = 0;                   // IDLength
        l_Header[1] = 0;                   // Color map type
        l_Header[2] = 2;                   // Image type code
        l_Header[3] = 0;                   // low color map origin
        l_Header[4] = 0;                   // high color map origin
        l_Header[5] = 0;                   // low color map length
        l_Header[6] = 0;                   // high color map length
        l_Header[7] = 0;                   // depth of color map entries
        l_Header[8] = 0;                   // low X origin
        l_Header[9] = 0;                   // high X origin
        l_Header[10] = 0;                  // low Y origin
        l_Header[11] = 0;                  // high Y origin
        l_Header[12] = (U8)i_SizeX;        // low width of image
        l_Header[13] = (U8)(i_SizeX >> 8); // high width of image
        l_Header[14] = (U8)i_SizeY;        // low height of image
        l_Header[15] = (U8)(i_SizeY >> 8); // high height of image
        l_Header[16] = (U8)i_Bpp;          // Image pixel size
        l_Header[17] = 32;                 // Image descriptor byte
        l_File.Write(l_Header, sizeof(l_Header));
        for (int j = 0; j < i_SizeY; j++) {
            l_File.Write((&i_Data[j * i_SizeX * (i_Bpp >> 3)]), i_SizeX * (i_Bpp >> 3));
        }
        l_File.Flush();
        l_File.Close();
    }
}

void Bitmap_Z::Save(const Char* i_FileName) {
    switch (m_Format) {
        case BM_8888:
            SaveTGA(i_FileName, (U8*)m_Datas, m_SizeX, m_SizeY, 32);
            break;
        case BM_888:
            SaveTGA(i_FileName, (U8*)m_Datas, m_SizeX, m_SizeY, 24);
            break;
        case BM_565:
        case BM_5551:
        case BM_4444:
        case BM_1555:
            SaveTGA(i_FileName, (U8*)m_Datas, m_SizeX, m_SizeY, 16);
            break;
    };
}
