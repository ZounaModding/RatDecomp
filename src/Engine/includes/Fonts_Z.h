#ifndef _FONTS_Z_H_
#define _FONTS_Z_H_
#include "Material_ZHdl.h"
#include "DynArray_Z.h"
#include "ResourceObject_Z.h"
#include "Types_Z.h"
#include "Math_Z.h"
#include "HashTable_Z.h"

S32 GetUTF8CharBytes(const Char* a1);
U32 GetUTF8CharCode(const Char* a1);

class FontGlyphHash_Z {
public:
    U32 m_ID;
    S32 m_MaterialIndex;
    Float m_Descent;
    Vec2f m_TexCoordTopLeft;
    Vec2f m_TexCoordBottomRight;
    S32 m_Ref;

    FontGlyphHash_Z() { }

    FontGlyphHash_Z(U32 i_ID) { m_ID = i_ID; }

    inline S32 HashBase() const { return m_ID; }

    inline void SetShadow() { m_Ref = -1; }

    inline Bool IsEmpty() { return !m_Ref; }

    inline S32 HashIncrement() const {
        return HashBase() == 0;
    }

    Bool operator==(const FontGlyphHash_Z& i_Elem) { return i_Elem.m_ID == m_ID; }

    Bool operator!=(const FontGlyphHash_Z& i_Elem) { return i_Elem.m_ID != m_ID; }

    FontGlyphHash_Z& operator=(const FontGlyphHash_Z& i_Elem) {
        m_ID = i_Elem.m_ID;
        m_MaterialIndex = i_Elem.m_MaterialIndex;
        m_Descent = i_Elem.m_Descent;
        m_TexCoordTopLeft = i_Elem.m_TexCoordTopLeft;
        m_TexCoordBottomRight = i_Elem.m_TexCoordBottomRight;
        m_Ref = i_Elem.m_Ref;
        return *this;
    }
};

struct CharDesc_Z {
    Material_ZHdl m_MatHdl;
    Float m_Descent;
    Vec2f m_TexCoordTopLeft;
    Vec2f m_TexCoordBottomRight;
};

struct FontParam_Z {
    FontParam_Z() {
        m_BorderColor = COLOR_BLACK;
        m_MarginMult2 = 1.0f;
        m_HasBorder = FALSE;
        m_BorderOffset = 1.0f;
        m_BottomBoundY = -1.0f;
        m_TopBoundY = -1.0f;
        m_ZOffset = 1.0f;
    }

    Char* m_Text;
    Bool m_HasBorder;
    Float m_BorderOffset;
    Color m_BorderColor;
    Float m_TopBoundY;
    Float m_BottomBoundY;
    Vec2f m_BottomLeftPos;
    Color m_BottomColor;
    Color m_TopColor;
    Float m_MarginMult1;
    Float m_MarginMult2;
    Float m_ZOffset;
    Vec2f m_FinalBottomLeftPos;
};

class Fonts_Z : public ResourceObject_Z {
public:
    Fonts_Z() { }

    virtual ~Fonts_Z() { }

    virtual void Load(void** i_Data);
    virtual void EndLoad();
    virtual void AfterEndLoad();
    virtual Bool MarkHandles();
    Bool GetCharDesc(const Char* i_Char, CharDesc_Z& o_CharDesc);
    S32 GetSizeChar(const Char* i_Char);
    U32 GetSizeStr(const Char* i_Text);
    void DrawString(FontParam_Z& i_Params);
    void DrawString(Char* i_Text, S32 i_X, S32 i_Y, const Color& i_TopColor, const Color& i_BottomColor, Float i_Scale, Float i_Z);

    static BaseObject_Z* NewObject() { return NewL_Z(161) Fonts_Z; }

private:
    Material_ZHdlDA m_MaterialDA;
    HashTableBase_Z<FontGlyphHash_Z> m_FontGlyphHashHT;
};

#endif
