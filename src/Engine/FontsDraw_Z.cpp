#include "Fonts_Z.h"
#include "Main_Z.h"
#include "Material_Z.h"
#include "Renderer_Z.h"
#include <stdlib.h>

// TODO: Finish matching
void Fonts_Z::DrawString(FontParam_Z& i_Params) {
    i_Params.m_FinalBottomLeftPos = i_Params.m_BottomLeftPos;

    Color l_BorderColor;
    Vec2f l_LeftBorderOffset;
    Vec2f l_BottomBorderOffset;
    Vec2f l_RightBorderOffset;
    Vec2f l_TopBorderOffset;

    if (i_Params.m_HasBorder) {
        l_BorderColor = i_Params.m_BorderColor;
        l_BorderColor.a *= i_Params.m_BottomColor.a;
        l_BorderColor.a *= l_BorderColor.a;
        l_LeftBorderOffset.Set(-i_Params.m_BorderOffset, 0.0f);
        l_BottomBorderOffset.Set(0.0f, -i_Params.m_BorderOffset);
        l_RightBorderOffset.Set(i_Params.m_BorderOffset, 0.0f);
        l_TopBorderOffset.Set(0.0f, i_Params.m_BorderOffset);
    }

    Float l_Width;
    Float l_InvBitmapWidth = 1.0f;
    Float l_InvBitmapHeight = 1.0f;

    Char* l_Text = i_Params.m_Text;
    U32 l_MaterialIndex = -1;

    while (*l_Text) {
        U32 l_CharCode = GetUTF8CharCode(l_Text);

        if (l_CharCode == '~') {
            l_Text++;
            continue;
        }

        if (l_CharCode == 0xc2a7) {
            Char l_Margin[4];
            l_Margin[0] = l_Text[2];
            l_Margin[1] = l_Text[3];
            l_Margin[2] = l_Text[4];
            l_Margin[3] = 0;
            i_Params.m_MarginMult2 = atof(l_Margin) / 100.0;
            l_Text += 5;
            continue;
        }

        if (l_CharCode == '^') {
            l_Text += 4;
            continue;
        }

        U8 l_FirstByte = *l_Text;

        l_Text += GetUTF8CharBytes(l_Text);

        if (l_CharCode == 0xc2b2) {
            l_CharCode = GetUTF8CharCode(" ");
        }

        FontGlyphHash_Z l_GlyphId(l_CharCode);
        const FontGlyphHash_Z* l_Glyph = m_GlyphsHT.Search(l_GlyphId);

        if (!l_Glyph) {
            continue;
        }

        if (l_MaterialIndex != l_Glyph->m_MaterialIndex) {
            l_MaterialIndex = l_Glyph->m_MaterialIndex;
            gData.MainRdr->SetActiveMaterial(
                m_MaterialDA[l_MaterialIndex]
            );
            Bitmap_Z* l_Bitmap = m_MaterialDA[l_MaterialIndex]->GetBitmap();
            l_InvBitmapWidth = 1.0f / l_Bitmap->GetSizeX();
            l_InvBitmapHeight = 1.0f / l_Bitmap->GetSizeY();
        }

        Bool l_Clipped = FALSE;
        Float l_Margin = i_Params.m_MarginMult2 * i_Params.m_MarginMult1;
        Float l_Height = (l_Glyph->m_TexCoordBottomRight.y - l_Glyph->m_TexCoordTopLeft.y) * l_Margin;
        l_Width = (l_Glyph->m_TexCoordBottomRight.x - l_Glyph->m_TexCoordTopLeft.x) * l_Margin;
        Float l_Descent = l_Glyph->m_Descent * l_Margin;
        Float l_BottomLeftY = i_Params.m_FinalBottomLeftPos.y;
        Vec2f l_BottomLeft;
        Vec2f l_TopRight;

        l_BottomLeft.x = i_Params.m_FinalBottomLeftPos.x;
        l_BottomLeft.y = l_BottomLeftY + l_Descent;

        l_TopRight.x = l_BottomLeft.x + l_Width;
        l_TopRight.y = l_Descent + (l_BottomLeftY + l_Height);

        Vec2f l_UvBottomLeft;
        Vec2f l_UvTopRight;

        l_UvBottomLeft.x = l_InvBitmapWidth * (0.25f + l_Glyph->m_TexCoordTopLeft.x);
        l_UvBottomLeft.y = l_InvBitmapHeight * (0.25f + l_Glyph->m_TexCoordTopLeft.y);

        l_UvTopRight.x = l_InvBitmapWidth * (0.25f + l_Glyph->m_TexCoordBottomRight.x);
        l_UvTopRight.y = l_InvBitmapHeight * (0.25f + l_Glyph->m_TexCoordBottomRight.y);

        if (i_Params.m_TopBoundY >= 0.0f && i_Params.m_BottomBoundY >= 0.0f) {
            if (l_BottomLeft.y > i_Params.m_BottomBoundY || l_TopRight.y < i_Params.m_TopBoundY) {
                l_Clipped = TRUE;
            }
            else {
                if (l_BottomLeft.y < i_Params.m_TopBoundY) {
                    l_UvBottomLeft.y = ((i_Params.m_TopBoundY - l_BottomLeft.y) / (l_TopRight.y - l_BottomLeft.y)) * (l_UvTopRight.y - l_UvBottomLeft.y) + l_UvBottomLeft.y;
                    l_BottomLeft.y = i_Params.m_TopBoundY;
                }

                if (l_TopRight.y > i_Params.m_BottomBoundY) {
                    l_UvTopRight.y = ((i_Params.m_BottomBoundY - l_BottomLeft.y) / (l_TopRight.y - l_BottomLeft.y)) * (l_UvTopRight.y - l_UvBottomLeft.y) + l_UvBottomLeft.y;
                    l_TopRight.y = i_Params.m_BottomBoundY;
                }
            }
        }

        if (!l_Clipped) {
            if (i_Params.m_HasBorder) {
                gData.MainRdr->Draw2DQuad(
                    l_BottomLeft + l_LeftBorderOffset,
                    l_TopRight + l_LeftBorderOffset,
                    l_UvBottomLeft,
                    l_UvTopRight,
                    l_BorderColor,
                    l_BorderColor,
                    i_Params.m_ZOffset
                );

                gData.MainRdr->Draw2DQuad(
                    l_BottomLeft + l_BottomBorderOffset,
                    l_TopRight + l_BottomBorderOffset,
                    l_UvBottomLeft,
                    l_UvTopRight,
                    l_BorderColor,
                    l_BorderColor,
                    i_Params.m_ZOffset
                );

                gData.MainRdr->Draw2DQuad(
                    l_BottomLeft + l_RightBorderOffset,
                    l_TopRight + l_RightBorderOffset,
                    l_UvBottomLeft,
                    l_UvTopRight,
                    l_BorderColor,
                    l_BorderColor,
                    i_Params.m_ZOffset
                );

                gData.MainRdr->Draw2DQuad(
                    l_BottomLeft + l_TopBorderOffset,
                    l_TopRight + l_TopBorderOffset,
                    l_UvBottomLeft,
                    l_UvTopRight,
                    l_BorderColor,
                    l_BorderColor,
                    i_Params.m_ZOffset
                );
            }

            if (l_FirstByte != 0 && l_FirstByte < 20) {
                if (i_Params.m_TopColor.r < 0.01f && i_Params.m_TopColor.g < 0.01f && i_Params.m_TopColor.b < 0.01f && i_Params.m_TopColor.a < 0.01f) {
                    gData.MainRdr->Draw2DQuad(
                        l_BottomLeft,
                        l_TopRight,
                        l_UvBottomLeft,
                        l_UvTopRight,
                        i_Params.m_BottomColor,
                        i_Params.m_TopColor,
                        i_Params.m_ZOffset
                    );
                }
                else {
                    Color l_BottomColor(
                        0.5f,
                        0.5f,
                        0.5f,
                        i_Params.m_BottomColor.a
                    );

                    Color l_TopColor(
                        0.5f,
                        0.5f,
                        0.5f,
                        i_Params.m_TopColor.a
                    );

                    gData.MainRdr->Draw2DQuad(
                        l_BottomLeft,
                        l_TopRight,
                        l_UvBottomLeft,
                        l_UvTopRight,
                        l_BottomColor,
                        l_TopColor,
                        i_Params.m_ZOffset
                    );
                }
            }
            else {
                gData.MainRdr->Draw2DQuad(
                    l_BottomLeft,
                    l_TopRight,
                    l_UvBottomLeft,
                    l_UvTopRight,
                    i_Params.m_BottomColor,
                    i_Params.m_TopColor,
                    i_Params.m_ZOffset
                );
            }
        }

        i_Params.m_FinalBottomLeftPos.x += l_Width;
    }
}

void Fonts_Z::DrawString(Char* i_Text, S32 i_X, S32 i_Y, const Color& i_TopColor, const Color& i_BottomColor, Float i_Scale, Float i_Z) {
    FontParam_Z l_Params;
    l_Params.m_Text = i_Text;
    l_Params.m_BottomColor = i_TopColor;
    l_Params.m_TopColor = i_BottomColor;
    l_Params.m_BottomLeftPos.Set((Float)i_X, (Float)i_Y);
    l_Params.m_MarginMult1 = i_Scale;
    l_Params.m_ZOffset = i_Z;
    DrawString(l_Params);
}
