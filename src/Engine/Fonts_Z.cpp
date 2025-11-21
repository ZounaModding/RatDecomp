#include "Fonts_Z.h"
#include "Main_Z.h"
#include "Program_Z.h"

S32 GetUTF8CharBytes(const Char* i_CharBytePtr) {
    if ((*i_CharBytePtr & 0x80) == 0) {
        return 1;
    }
    else if ((*i_CharBytePtr & 0xE0) == 0xC0) {
        return 2;
    }
    else if ((*i_CharBytePtr & 0xF0) == 0xE0) {
        return 3;
    }
    else if ((*i_CharBytePtr & 0xF8) == 0xF0) {
        return 4;
    }
    return 1;
}

U32 GetUTF8CharCode(const Char* i_CharBytePtr) {
    const U8* l_CharByteU8Ptr = (const U8*)i_CharBytePtr;
    Char l_CharFirstByte = *l_CharByteU8Ptr;

    if ((l_CharFirstByte & 0x80) == 0) {
        return l_CharByteU8Ptr[0];
    }
    else if ((l_CharFirstByte & 0xE0) == 0xC0) {
        return ((l_CharByteU8Ptr[0] & 0xff) << 8) | (l_CharByteU8Ptr[1] & 0xff);
    }
    else if ((l_CharFirstByte & 0xF0) == 0xE0) {
        return ((l_CharByteU8Ptr[0] & 0xff) << 16) | ((l_CharByteU8Ptr[1] & 0xff) << 8) | (l_CharByteU8Ptr[2] & 0xff);
    }
    else if ((l_CharFirstByte & 0xF8) == 0xF0) {
        return ((l_CharByteU8Ptr[0] & 0xff) << 24) | ((l_CharByteU8Ptr[1] & 0xff) << 16) | ((l_CharByteU8Ptr[2] & 0xff) << 8) | (l_CharByteU8Ptr[3] & 0xff);
    }

    return 1;
}

Bool Fonts_Z::MarkHandles() {
    for (S32 i = 0; i < m_MaterialDA.GetSize(); i++) {
        if (gData.ClassMgr->GetPtr(m_MaterialDA[i]) != NULL) {
            gData.ClassMgr->GetPtr(m_MaterialDA[i])->MarkHandles();
        }
    }
    return ResourceObject_Z::MarkHandles();
}

Bool Fonts_Z::GetCharDesc(const Char* i_Char, CharDesc_Z& o_CharDesc) {
    S32 l_CharCode = GetUTF8CharCode(i_Char);
    const FontGlyphHash_Z l_HashElt(l_CharCode);
    const FontGlyphHash_Z* l_Result = m_FontGlyphHashHT.Search(l_HashElt);
    if (!l_Result) {
        return FALSE;
    }
    o_CharDesc.m_TexCoordBottomRight = l_Result->m_TexCoordBottomRight;
    o_CharDesc.m_TexCoordTopLeft = l_Result->m_TexCoordTopLeft;
    o_CharDesc.m_Descent = l_Result->m_Descent;
    o_CharDesc.m_MatHdl = m_MaterialDA[l_Result->m_MaterialIndex];
    return TRUE;
}

S32 Fonts_Z::GetSizeChar(const Char* i_Char) {
    S32 l_CharCode = GetUTF8CharCode(i_Char);
    const FontGlyphHash_Z l_HashElt(l_CharCode);
    const FontGlyphHash_Z* l_Result = m_FontGlyphHashHT.Search(l_HashElt);
    if (l_Result == NULL) {
        return -1;
    }
    return (S32)((l_Result->m_TexCoordBottomRight.x - l_Result->m_TexCoordTopLeft.x));
}
