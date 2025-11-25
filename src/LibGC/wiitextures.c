#include "wiitextures.h"
#include "GXTexture.h"
#include "OSCache.h"

static U32 nextpow2(U32 x) {
    return (1 << (32 - __cntlzw(x)));
}

static U32 divisible128(U32 val) {
    return ((val + 127) & ~127);
}

static void fill_offset_buffer(U8* buffer, U32 sourcewidth, U32 offwidth) {
    U32 finalwidth = sourcewidth / 8;

    S32 srcx, srcy, x, y;

    srcx = 0;
    srcy = 0;

    for (y = 0; y < 4; ++y) {
        for (x = 0; x < finalwidth; ++x) {
            U32 texoffset = (y * offwidth + x) * 2;
            S32 xoffset = srcx - x;
            S32 yoffset = srcy - y;

            buffer[texoffset] = (U8)(xoffset + 128);
            buffer[texoffset + 1] = (U8)(yoffset + 128);

            if (++srcy == 4) {
                srcy = 0;
                ++srcx;
            }
        }
    }
}

static void swizzle_offsets(U8* swizz, U8 const* offsets, U32 sourcewidth, U32 sourceheight, U32 bytesperpixel) {

    U32 tilex, tiley;
    U32 pitch = sourcewidth * bytesperpixel;
    U32 tilesw = pitch / 8;
    U32 tilesh = sourceheight / 4;
    U32 swizzoffset = 0;

    for (tiley = 0; tiley < tilesh; ++tiley) {
        for (tilex = 0; tilex < tilesw; ++tilex) {
            U32 row;
            U32 x = tilex * 8;
            U32 y = tiley * 4;
            U32 sourceoffset = y * pitch + x;

            for (row = 0; row < 4; ++row) {
                U32* dest = (U32*)(swizz + swizzoffset + (row * 8));
                U32* src = (U32*)(offsets + sourceoffset + (row * pitch));

                dest[0] = src[0];
                dest[1] = src[1];
            }
            swizzoffset += 32;
        }
    }
}

static S32 make_texture(U32 pitch, U32 height, GXTexFmt format, GXTexWrapMode type, GXTexObj* texture, void* ptr) {
    GXInitTexObj(texture, ptr, (U16)pitch, (U16)height, format, type, type, GX_FALSE);

    GXInitTexObjLOD(texture, GX_NEAR, GX_NEAR, 0, 0, 0.0f, GX_FALSE, 0, GX_ANISO_1);
    return (1);
}

static U32 swizz_div(U32 sourcewidth) {
    U32 offwidth;

    offwidth = ((sourcewidth / 8) + 3) & ~3;

    if (offwidth & (offwidth - 1)) {
        offwidth = nextpow2(offwidth);
    }

    return (offwidth);
}

static U32 make_swizzler(void* data, GXTexObj* obj, unsigned int* out_swidth, unsigned int* out_sheight, unsigned int sourcewidth) {
    U32 offwidth, offsize;
    U8 buffer[256 * 4 * 2];

    offwidth = swizz_div(sourcewidth);

    *out_swidth = offwidth;
    *out_sheight = 4;

    offsize = divisible128(offwidth * 2 * 4);

    fill_offset_buffer(buffer, sourcewidth, offwidth);
    swizzle_offsets((U8*)data, buffer, offwidth, 4, 2);

    DCFlushRange(data, offsize);

    make_texture(offwidth, 4, GX_TF_IA8, GX_REPEAT, obj, data);

    return (offsize);
}
