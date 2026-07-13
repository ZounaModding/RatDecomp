#ifndef _TEXTURE_MANAGER_Z_H
#define _TEXTURE_MANAGER_Z_H
#include "HoleArray_Z.h"
#include "Bitmap_Z.h"
#include "dolphin/GX/GXStruct.h"
#include "Types_Z.h"
#include "gx/GXEnum.h"

class Texture_Z {

public:
    GXTexObj m_Tex;
    GXTlutObj m_Tlut;
    GXTexObj m_SecondaryTex;
    Bitmap_Z* m_Bmap;
    Bool m_Used;
    Bool m_Palette;
    Bool m_SecondaryPalette;

    Bitmap_Z* GetBmap() { return m_Bmap; }
};

class TextureManager_Z {
public:
    TextureManager_Z();
    ~TextureManager_Z();
    Bool Init(void);
    void Shut(void);
    void LoadTexture(Bitmap_Z* i_Bmap, GXTexWrapMode i_WrapS, GXTexWrapMode i_WrapT, GXTexMapID i_TexMapID = GX_TEXMAP0);
    void RemoveTexture(U32 i_TexId);
    void Clean();
    Bool Minimize();
    void BeginRender();
    void EndRender(S32&, S32&);

private:
    HoleArray_Z<Texture_Z, 8> m_TextureHA;
};

#endif
