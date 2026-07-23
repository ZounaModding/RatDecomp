#ifndef _COLOR_Z_H_
#define _COLOR_Z_H_
#include "Types_Z.h"
#include "DynArray_Z.h"

struct Color {
    Color() { }

    Color(Float i_Red, Float i_Green, Float i_Blue, Float i_Alpha) {
        r = i_Red;
        g = i_Green;
        b = i_Blue;
        a = i_Alpha;
    }

    Color& operator=(const Color& i_Color) {
        r = i_Color.r;
        g = i_Color.g;
        b = i_Color.b;
        a = i_Color.a;
        return *this;
    }

    void Set(Float i_Red = 1.0f, Float i_Green = 1.0f, Float i_Blue = 1.0f, Float i_Alpha = 1.0f) {
        r = i_Red;
        g = i_Green;
        b = i_Blue;
        a = i_Alpha;
    }

    Float r;
    Float g;
    Float b;
    Float a;
};

Extern_Z const Color COLOR_BLACK;

struct ColorU8 {
    ColorU8() { }

    ColorU8(U8 i_Red, U8 i_Green, U8 i_Blue, U8 i_Alpha = 255) {
        r = i_Red;
        g = i_Green;
        b = i_Blue;
        a = i_Alpha;
    }

    inline U32 GetARGB() const { return (a << 24) | (r << 16) | (g << 8) | b; }

    ColorU8& operator=(const Color& i_Color) {
        r = (U8)(i_Color.r * 255.0f);
        g = (U8)(i_Color.g * 255.0f);
        b = (U8)(i_Color.b * 255.0f);
        a = (U8)(i_Color.a * 255.0f);
        return *this;
    }

    U8 r;
    U8 g;
    U8 b;
    U8 a;
};

class ByteColor {
public:
    ByteColor() { }

    U8 r;
    U8 g;
    U8 b;
    U8 a;
};

typedef DynArray_Z<ByteColor, 32, FALSE, FALSE> ByteColorDA;

#endif
