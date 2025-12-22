#include "MathGC_Z.h"

// clang-format off
asm void ConcatMat4x4(const Mat4x4& a1, const Mat4x4& a2, Mat4x4& a3)
{
    nofralloc

    stwu    r1, -0x140(r1)
    stfd    f31, 0x130(r1)
    psq_st  f31, 0x138(r1), 0, 0
    stfd    f30, 0x120(r1)
    psq_st  f30, 0x128(r1), 0, 0
    stfd    f29, 0x110(r1)
    psq_st  f29, 0x118(r1), 0, 0
    stfd    f28, 0x100(r1)
    psq_st  f28, 0x108(r1), 0, 0
    stfd    f27, 0xF0(r1)
    psq_st  f27, 0xF8(r1),  0, 0
    stfd    f26, 0xE0(r1)
    psq_st  f26, 0xE8(r1),  0, 0
    stfd    f25, 0xD0(r1)
    psq_st  f25, 0xD8(r1),  0, 0
    stfd    f24, 0xC0(r1)
    psq_st  f24, 0xC8(r1),  0, 0
    stfd    f23, 0xB0(r1)
    psq_st  f23, 0xB8(r1),  0, 0
    stfd    f22, 0xA0(r1)
    psq_st  f22, 0xA8(r1),  0, 0
    stfd    f21, 0x90(r1)
    psq_st  f21, 0x98(r1),  0, 0
    stfd    f20, 0x80(r1)
    psq_st  f20, 0x88(r1),  0, 0
    stfd    f19, 0x70(r1)
    psq_st  f19, 0x78(r1),  0, 0
    stfd    f18, 0x60(r1)
    psq_st  f18, 0x68(r1),  0, 0
    stfd    f17, 0x50(r1)
    psq_st  f17, 0x58(r1),  0, 0
    stfd    f16, 0x40(r1)
    psq_st  f16, 0x48(r1),  0, 0
    stfd    f15, 0x30(r1)
    psq_st  f15, 0x38(r1),  0, 0
    stfd    f14, 0x20(r1)
    psq_st  f14, 0x28(r1),  0, 0

    stw     r3, 0x08(r1)     // a1
    stw     r4, 0x0C(r1)     // a2
    stw     r5, 0x10(r1)     // a3

    psq_l   f2,  0x00(r3), 0, 0
    psq_l   f3,  0x08(r3), 0, 0
    psq_l   f4,  0x10(r3), 0, 0
    psq_l   f5,  0x18(r3), 0, 0
    psq_l   f6,  0x20(r3), 0, 0
    psq_l   f7,  0x28(r3), 0, 0
    psq_l   f10, 0x00(r4), 0, 0
    psq_l   f11, 0x08(r4), 0, 0
    psq_l   f17, 0x10(r4), 0, 0
    psq_l   f18, 0x18(r4), 0, 0
    psq_l   f8,  0x30(r3), 0, 0
    psq_l   f9,  0x38(r3), 0, 0

    ps_merge00 f12, f10, f10
    ps_merge11 f13, f10, f10
    ps_merge00 f14, f11, f11
    ps_merge00 f19, f17, f17
    ps_merge11 f20, f17, f17
    ps_merge00 f21, f18, f18
    ps_mul    f15, f2, f12
    ps_mul    f16, f3, f12
    ps_mul    f22, f2, f19
    ps_mul    f23, f3, f19
    ps_madd   f15, f4, f13, f15
    ps_madd   f16, f5, f13, f16
    ps_madd   f22, f4, f20, f22
    ps_madd   f23, f5, f20, f23
    ps_madd   f15, f6, f14, f15
    ps_madd   f16, f7, f14, f16
    ps_madd   f22, f6, f21, f22
    ps_madd   f23, f7, f21, f23
    psq_l     f10, 0x20(r4), 0, 0
    psq_l     f11, 0x28(r4), 0, 0
    psq_l     f17, 0x30(r4), 0, 0
    psq_l     f18, 0x38(r4), 0, 0
    psq_st    f15, 0x00(r5), 0, 0
    psq_st    f16, 0x08(r5), 0, 0
    psq_st    f22, 0x10(r5), 0, 0
    psq_st    f23, 0x18(r5), 0, 0
    ps_merge00 f12, f10, f10
    ps_merge11 f13, f10, f10
    ps_merge00 f14, f11, f11
    ps_merge00 f19, f17, f17
    ps_merge11 f20, f17, f17
    ps_merge00 f21, f18, f18
    ps_mul    f15, f2, f12
    ps_mul    f16, f3, f12
    ps_madd   f22, f2, f19, f8
    ps_madd   f23, f3, f19, f9
    ps_madd   f15, f4, f13, f15
    ps_madd   f16, f5, f13, f16
    ps_madd   f22, f4, f20, f22
    ps_madd   f23, f5, f20, f23
    ps_madd   f15, f6, f14, f15
    ps_madd   f16, f7, f14, f16
    ps_madd   f22, f6, f21, f22
    ps_madd   f23, f7, f21, f23
    psq_st    f15, 0x20(r5), 0, 0
    psq_st    f16, 0x28(r5), 0, 0
    psq_st    f22, 0x30(r5), 0, 0
    psq_st    f23, 0x38(r5), 0, 0
    psq_l     f31, 0x138(r1), 0, 0
    lfd       f31, 0x130(r1)
    psq_l   f30, 0x128(r1), 0, 0
    lfd     f30, 0x120(r1)
    psq_l   f29, 0x118(r1), 0, 0
    lfd     f29, 0x110(r1)
    psq_l   f28, 0x108(r1), 0, 0
    lfd     f28, 0x100(r1)
    psq_l   f27, 0xF8(r1), 0, 0
    lfd     f27, 0xF0(r1)
    psq_l   f26, 0xE8(r1), 0, 0
    lfd     f26, 0xE0(r1)
    psq_l   f25, 0xD8(r1), 0, 0
    lfd     f25, 0xD0(r1)
    psq_l   f24, 0xC8(r1), 0, 0
    lfd     f24, 0xC0(r1)
    psq_l   f23, 0xB8(r1), 0, 0
    lfd     f23, 0xB0(r1)
    psq_l   f22, 0xA8(r1), 0, 0
    lfd     f22, 0xA0(r1)
    psq_l   f21, 0x98(r1), 0, 0
    lfd     f21, 0x90(r1)
    psq_l   f20, 0x88(r1), 0, 0
    lfd     f20, 0x80(r1)
    psq_l   f19, 0x78(r1), 0, 0
    lfd     f19, 0x70(r1)
    psq_l   f18, 0x68(r1), 0, 0
    lfd     f18, 0x60(r1)
    psq_l   f17, 0x58(r1), 0, 0
    lfd     f17, 0x50(r1)
    psq_l   f16, 0x48(r1), 0, 0
    lfd     f16, 0x40(r1)
    psq_l   f15, 0x38(r1), 0, 0
    lfd     f15, 0x30(r1)
    psq_l   f14, 0x28(r1), 0, 0
    lfd     f14, 0x20(r1)

    addi    r1, r1, 0x140
    blr

}

asm void ConcatMat4x4AndConvert(const Mat4x4& a1, const Mat4x4& a2, Mat3x3& a3)
{
    nofralloc

    stwu    r1, -0x140(r1)
    stfd    f31, 0x130(r1)
    psq_st  f31, 0x138(r1), 0, 0
    stfd    f30, 0x120(r1)
    psq_st  f30, 0x128(r1), 0, 0
    stfd    f29, 0x110(r1)
    psq_st  f29, 0x118(r1), 0, 0
    stfd    f28, 0x100(r1)
    psq_st  f28, 0x108(r1), 0, 0
    stfd    f27, 0xF0(r1)
    psq_st  f27, 0xF8(r1),  0, 0
    stfd    f26, 0xE0(r1)
    psq_st  f26, 0xE8(r1),  0, 0
    stfd    f25, 0xD0(r1)
    psq_st  f25, 0xD8(r1),  0, 0
    stfd    f24, 0xC0(r1)
    psq_st  f24, 0xC8(r1),  0, 0
    stfd    f23, 0xB0(r1)
    psq_st  f23, 0xB8(r1),  0, 0
    stfd    f22, 0xA0(r1)
    psq_st  f22, 0xA8(r1),  0, 0
    stfd    f21, 0x90(r1)
    psq_st  f21, 0x98(r1),  0, 0
    stfd    f20, 0x80(r1)
    psq_st  f20, 0x88(r1),  0, 0
    stfd    f19, 0x70(r1)
    psq_st  f19, 0x78(r1),  0, 0
    stfd    f18, 0x60(r1)
    psq_st  f18, 0x68(r1),  0, 0
    stfd    f17, 0x50(r1)
    psq_st  f17, 0x58(r1),  0, 0
    stfd    f16, 0x40(r1)
    psq_st  f16, 0x48(r1),  0, 0
    stfd    f15, 0x30(r1)
    psq_st  f15, 0x38(r1),  0, 0
    stfd    f14, 0x20(r1)
    psq_st  f14, 0x28(r1),  0, 0

    stw     r3, 0x08(r1)     // a1
    stw     r4, 0x0C(r1)     // a2
    stw     r5, 0x10(r1)     // a3

    psq_l   f2,  0x00(r3), 0, 0
    psq_l   f3,  0x08(r3), 0, 0
    psq_l   f4,  0x10(r3), 0, 0
    psq_l   f5,  0x18(r3), 0, 0
    psq_l   f6,  0x20(r3), 0, 0
    psq_l   f7,  0x28(r3), 0, 0
    psq_l   f10, 0x00(r4), 0, 0
    psq_l   f11, 0x08(r4), 0, 0
    psq_l   f17, 0x10(r4), 0, 0
    psq_l   f18, 0x18(r4), 0, 0
    psq_l   f8,  0x30(r3), 0, 0
    psq_l   f9,  0x38(r3), 0, 0

    ps_merge00 f12, f10, f10
    ps_merge11 f13, f10, f10
    ps_merge00 f14, f11, f11
    ps_merge00 f19, f17, f17
    ps_merge11 f20, f17, f17
    ps_merge00 f21, f18, f18
    ps_mul    f15, f2, f12
    ps_mul    f16, f3, f12
    ps_mul    f22, f2, f19
    ps_mul    f23, f3, f19
    ps_madd   f15, f4, f13, f15
    ps_madd   f16, f5, f13, f16
    ps_madd   f22, f4, f20, f22
    ps_madd   f23, f5, f20, f23
    ps_madd   f15, f6, f14, f15
    ps_madd   f16, f7, f14, f16
    ps_madd   f22, f6, f21, f22
    ps_madd   f23, f7, f21, f23
    psq_l     f10, 0x20(r4), 0, 0
    psq_l     f11, 0x28(r4), 0, 0
    psq_l     f17, 0x30(r4), 0, 0
    psq_l     f18, 0x38(r4), 0, 0

    ps_merge00 f12, f10, f10
    ps_merge11 f13, f10, f10
    ps_merge00 f14, f11, f11
    ps_merge00 f19, f17, f17
    ps_merge11 f20, f17, f17
    ps_merge00 f21, f18, f18

    ps_mul    f24, f2, f12
    ps_mul    f25, f3, f12
    ps_madd   f26, f2, f19, f8
    ps_madd   f27, f3, f19, f9
    ps_madd   f24, f4, f13, f24
    ps_madd   f25, f5, f13, f25
    ps_madd   f26, f4, f20, f26
    ps_madd   f27, f5, f20, f27
    ps_madd   f24, f6, f14, f24
    ps_madd   f25, f7, f14, f25
    ps_madd   f26, f6, f21, f26
    ps_madd   f27, f7, f21, f27

    ps_merge00 f2, f15, f22
    ps_merge11 f4, f15, f22
    ps_merge00 f3, f24, f26
    ps_merge11 f5, f24, f26

    psq_st    f2,  0x00(r5), 0, 0
    psq_st    f4,  0x10(r5), 0, 0

    ps_merge00 f6, f16, f23
    ps_merge00 f7, f25, f27

    psq_st    f3,  0x08(r5), 0, 0
    psq_st    f5,  0x18(r5), 0, 0
    psq_st    f6,  0x20(r5), 0, 0
    psq_st    f7, 0x28(r5), 0, 0
    psq_l     f31, 0x138(r1), 0, 0
    lfd       f31, 0x130(r1)
    psq_l     f30, 0x128(r1), 0, 0
    lfd       f30, 0x120(r1)
    psq_l     f29, 0x118(r1), 0, 0
    lfd       f29, 0x110(r1)
    psq_l     f28, 0x108(r1), 0, 0
    lfd       f28, 0x100(r1)
    psq_l     f27, 0xF8(r1), 0, 0
    lfd       f27, 0xF0(r1)
    psq_l     f26, 0xE8(r1), 0, 0
    lfd       f26, 0xE0(r1)
    psq_l     f25, 0xD8(r1), 0, 0
    lfd       f25, 0xD0(r1)
    psq_l     f24, 0xC8(r1), 0, 0
    lfd       f24, 0xC0(r1)
    psq_l     f23, 0xB8(r1), 0, 0
    lfd       f23, 0xB0(r1)
    psq_l     f22, 0xA8(r1), 0, 0
    lfd       f22, 0xA0(r1)
    psq_l     f21, 0x98(r1), 0, 0
    lfd       f21, 0x90(r1)
    psq_l     f20, 0x88(r1), 0, 0
    lfd       f20, 0x80(r1)
    psq_l     f19, 0x78(r1), 0, 0
    lfd       f19, 0x70(r1)
    psq_l     f18, 0x68(r1), 0, 0
    lfd       f18, 0x60(r1)
    psq_l     f17, 0x58(r1), 0, 0
    lfd       f17, 0x50(r1)
    psq_l     f16, 0x48(r1), 0, 0
    lfd       f16, 0x40(r1)
    psq_l     f15, 0x38(r1), 0, 0
    lfd       f15, 0x30(r1)
    psq_l     f14, 0x28(r1), 0, 0
    lfd       f14, 0x20(r1)

    addi      r1, r1, 0x140
    blr

}

// clang-format on
