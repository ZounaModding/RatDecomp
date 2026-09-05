#include "Fonts_Z.h"
#include "Main_Z.h"
#include "Program_Z.h"
#include "Material_Z.h"
#include "Console_Z.h"
#include "SystemDatas_Z.h"

static const Char* s_Unused_2647 = "Fonts_Z.cpp";
static const Char* s_Unused_2648 = "Unknown UTF-8 pattern";

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
        if (m_MaterialDA[i].IsValid()) {
            m_MaterialDA[i]->MarkHandles();
        }
    }
    return ResourceObject_Z::MarkHandles();
}

Bool Fonts_Z::GetCharDesc(const Char* i_Char, CharDesc_Z& o_CharDesc) {
    S32 l_CharCode = GetUTF8CharCode(i_Char);
    const FontGlyphHash_Z l_HashElt(l_CharCode);
    const FontGlyphHash_Z* l_Result = m_GlyphsHT.Search(l_HashElt);
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
    const FontGlyphHash_Z* l_Result = m_GlyphsHT.Search(l_HashElt);
    if (l_Result == NULL) {
        return -1;
    }
    return (S32)((l_Result->m_TexCoordBottomRight.x - l_Result->m_TexCoordTopLeft.x));
}

void Fonts_Z::GetSizeYStr(const Char* i_Text, S32& o_MinY1, S32& o_MaxY, S32& o_MinY2) {
    o_MinY1 = 1024;
    o_MinY2 = 1024;
    o_MaxY = -1;

    if ((S32)strlen(i_Text) == 0) {
        return;
    }

    while (*i_Text) {
        const U8* l_CharByteU8Ptr = (const U8*)i_Text;
        Char l_CharSigned = *i_Text;
        U32 l_CharFirstByte = (U8)l_CharSigned;
        S32 l_HighBit = l_CharSigned & 0x80;
        S32 l_CharCode;
        if (l_HighBit == 0) {
            l_CharCode = l_CharFirstByte;
        }
        else if ((l_CharSigned & 0xE0) == 0xC0) {
            l_CharCode = ((l_CharFirstByte & 0xff) << 8) | (l_CharByteU8Ptr[1] & 0xff);
        }
        else if ((l_CharSigned & 0xF0) == 0xE0) {
            l_CharCode = ((l_CharFirstByte & 0xff) << 16) | ((l_CharByteU8Ptr[1] & 0xff) << 8) | (l_CharByteU8Ptr[2] & 0xff);
        }
        else if ((l_CharSigned & 0xF8) == 0xF0) {
            l_CharCode = ((l_CharFirstByte & 0xff) << 24) | ((l_CharByteU8Ptr[1] & 0xff) << 16) | ((l_CharByteU8Ptr[2] & 0xff) << 8) | (l_CharByteU8Ptr[3] & 0xff);
        }
        else {
            l_CharCode = 1;
        }
        switch (l_CharCode) {
            case '~':
                i_Text++;
                break;
            case 0xc2a7: // '§'
                i_Text += 5;
                break;
            case '^':
                i_Text += 4;
                break;
            default:
                S32 l_CharBytes;
                if (l_HighBit == 0) {
                    l_CharBytes = 1;
                }
                else if ((l_CharSigned & 0xE0) == 0xC0) {
                    l_CharBytes = 2;
                }
                else if ((l_CharSigned & 0xF0) == 0xE0) {
                    l_CharBytes = 3;
                }
                else if ((l_CharSigned & 0xF8) == 0xF0) {
                    l_CharBytes = 4;
                }
                else {
                    l_CharBytes = 1;
                }
                i_Text += l_CharBytes;
                const FontGlyphHash_Z l_HashElt(l_CharCode);
                const FontGlyphHash_Z* l_Result = m_GlyphsHT.Search(l_HashElt);
                if (l_Result) {
                    if (l_Result->m_Descent < (Float)o_MinY2) {
                        o_MinY2 = (S32)l_Result->m_Descent;
                    }
                    if (l_Result->m_Descent < (Float)o_MinY1) {
                        o_MinY1 = (S32)l_Result->m_Descent;
                    }
                    if (l_Result->m_Descent + (l_Result->m_TexCoordBottomRight.y - l_Result->m_TexCoordTopLeft.y) > (Float)o_MaxY) {
                        o_MaxY = (S32)(l_Result->m_Descent + ((Float)(S32)l_Result->m_TexCoordBottomRight.y - l_Result->m_TexCoordTopLeft.y));
                    }
                }
                break;
        }
    }
}

U32 Fonts_Z::GetSizeStr(const Char* i_Text) {
    if ((S32)strlen(i_Text) == 0) {
        return 0;
    }

    S32 l_Size = 0;
    while (*i_Text) {
        const U8* l_CharByteU8Ptr = (const U8*)i_Text;
        Char l_CharSigned = *i_Text;
        U32 l_CharFirstByte = (U8)l_CharSigned;
        S32 l_HighBit = l_CharSigned & 0x80;
        S32 l_CharCode;
        if (l_HighBit == 0) {
            l_CharCode = l_CharFirstByte;
        }
        else if ((l_CharSigned & 0xE0) == 0xC0) {
            l_CharCode = ((l_CharFirstByte & 0xff) << 8) | (l_CharByteU8Ptr[1] & 0xff);
        }
        else if ((l_CharSigned & 0xF0) == 0xE0) {
            l_CharCode = ((l_CharFirstByte & 0xff) << 16) | ((l_CharByteU8Ptr[1] & 0xff) << 8) | (l_CharByteU8Ptr[2] & 0xff);
        }
        else if ((l_CharSigned & 0xF8) == 0xF0) {
            l_CharCode = ((l_CharFirstByte & 0xff) << 24) | ((l_CharByteU8Ptr[1] & 0xff) << 16) | ((l_CharByteU8Ptr[2] & 0xff) << 8) | (l_CharByteU8Ptr[3] & 0xff);
        }
        else {
            l_CharCode = 1;
        }
        switch (l_CharCode) {
            case '~':
                i_Text++;
                break;
            case 0xc2a7: // '§'
                i_Text += 5;
                break;
            case '^':
                i_Text += 4;
                break;
            default:
                S32 l_CharBytes;
                if (l_HighBit == 0) {
                    l_CharBytes = 1;
                }
                else if ((l_CharSigned & 0xE0) == 0xC0) {
                    l_CharBytes = 2;
                }
                else if ((l_CharSigned & 0xF0) == 0xE0) {
                    l_CharBytes = 3;
                }
                else if ((l_CharSigned & 0xF8) == 0xF0) {
                    l_CharBytes = 4;
                }
                else {
                    l_CharBytes = 1;
                }
                i_Text += l_CharBytes;
                const FontGlyphHash_Z l_HashElt(l_CharCode);
                const FontGlyphHash_Z* l_Result = m_GlyphsHT.Search(l_HashElt);
                if (l_Result) {
                    l_Size = l_Size + (l_Result->m_TexCoordBottomRight.x - l_Result->m_TexCoordTopLeft.x);
                }
                break;
        }
    }
    return l_Size;
}

void Fonts_Z::Load(void** i_Data) {
    m_GlyphsHT.Flush();

    S32 l_Nb;
    LOAD_Z(l_Nb);
    for (S32 i = 0; i < l_Nb; i++) {
        FontGlyph_Z l_Glyph;
        LOAD_Z(l_Glyph);
        m_GlyphsHT.Insert(FontGlyphHash_Z(l_Glyph));
    }

    LOAD_Z(l_Nb);
    m_MaterialDA.SetSize(l_Nb);
    for (S32 i = 0; i < l_Nb; i++) {
        gData.ClassMgr->LoadLink(m_MaterialDA[i], i_Data);
    }
}

void Fonts_Z::EndLoad() {
    ResourceObject_Z::EndLoad();
    for (S32 i = 0; i < m_MaterialDA.GetSize(); i++) {
        gData.ClassMgr->UpdateLink(m_MaterialDA[i]);
    }
}

void Fonts_Z::AfterEndLoad() {
}

Bool LoadFont() {
    if (gData.Cons->GetNbParam() != 2) {
        return TRUE;
    }

    Char l_FileName[264];
    l_FileName[0] = '\0';
    strcpy(l_FileName, "DB:>");
    strcat(l_FileName, gData.Cons->GetParamStr(1));
    strcat(l_FileName, ".TFONT");

    SystemDatas_Z* l_SystemDatas = gData.SystemDatas;
    BaseObject_ZHdl l_ResourceHdl = gData.ClassMgr->GetResourceObject(l_FileName);
    Fonts_ZHdl l_FontHdl(l_ResourceHdl);
    if (!l_FontHdl.IsValid()) {
        return FALSE;
    }

    Fonts_ZHdlDA& l_Fonts = l_SystemDatas->GetFonts();
    for (S32 i = 0; i < l_Fonts.GetSize(); i++) {
        if (l_Fonts[i] == l_FontHdl) {
            return TRUE;
        }
    }
    l_Fonts.Add(l_FontHdl);
    return TRUE;
}

Bool RemoveFont() {
    return FALSE;
}
