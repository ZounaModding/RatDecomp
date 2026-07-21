#include "MathTools_Z.h"
#include "Math_Z.h"
#define PI 3.14159265358979323846
#define PI2 (PI / 2.0)
#define TABLESIZE_COS 32
#define BITCOUNT_INVSQRT 7
#define BITCOUNT_ACOS 6
#define TABLESIZE_INVSQRT (1 << BITCOUNT_INVSQRT)
#define TABLESIZE_ACOS (1 << BITCOUNT_ACOS)
static Float gTableAcos[TABLESIZE_ACOS];
static Float gTableCos[TABLESIZE_COS + 1];
static U8 gTable1InvSqrt[TABLESIZE_INVSQRT];

static inline Float O_CosCore(Float i_Angle) {
    i_Angle = 20.371833f * i_Angle;
    Float l_TablePosition = fabsf(i_Angle);
    S32 l_PositionIndex = (S32)l_TablePosition;
    U32 l_TableIndex = l_PositionIndex & (TABLESIZE_COS - 1);
    Float l_InverseRatio;
    Float l_Ratio = l_TablePosition - l_PositionIndex;
    l_InverseRatio = 1.0f - l_Ratio;
    Float l_Result;

    if (l_PositionIndex & TABLESIZE_COS) {
        l_TableIndex = TABLESIZE_COS - l_TableIndex;
        l_Result = l_InverseRatio * gTableCos[l_TableIndex] + l_Ratio * gTableCos[l_TableIndex - 1];
    }
    else {
        l_Result = l_InverseRatio * gTableCos[l_TableIndex] + l_Ratio * gTableCos[l_TableIndex + 1];
    }

    if (((l_PositionIndex ^ ((S32)l_PositionIndex >> 1)) & TABLESIZE_COS) != 0) {
        l_Result = -l_Result;
    }
    return l_Result;
}

static inline Float O_SinCore(Float i_Angle) {
    i_Angle = 20.371833f * i_Angle;
    Float l_TablePosition = fabsf(i_Angle);
    S32 l_PositionIndex = (S32)l_TablePosition;
    U32 l_TableIndex = l_PositionIndex & (TABLESIZE_COS - 1);
    Float l_Ratio = l_TablePosition - l_PositionIndex;
    Float l_InverseRatio = 1.0f - l_Ratio;
    Float l_Result;

    if (l_PositionIndex & TABLESIZE_COS) {
        l_TableIndex = TABLESIZE_COS - l_TableIndex;
        l_Result = l_InverseRatio * gTableCos[l_TableIndex] + l_Ratio * gTableCos[l_TableIndex - 1];
    }
    else {
        l_Result = l_InverseRatio * gTableCos[l_TableIndex] + l_Ratio * gTableCos[l_TableIndex + 1];
    }

    if (((l_PositionIndex ^ ((S32)l_PositionIndex >> 1)) & TABLESIZE_COS) != 0) {
        l_Result = -l_Result;
    }
    return l_Result;
}

void MathInitTables() {
    int i;

    for (i = 0; i <= TABLESIZE_COS; ++i) {
        Float angle = i * (PI2 / TABLESIZE_COS);
        gTableCos[i] = cos(angle);
    }

    for (i = 0; i < TABLESIZE_INVSQRT; ++i) {
        U32 l_UnkNum = (127UL << 23) + (i << (23 - BITCOUNT_INVSQRT));
        Float l_FloatNum = *(Float*)&l_UnkNum;
        l_FloatNum = (Float)(1.0 / sqrtf(l_FloatNum));
        l_UnkNum = *(U32*)&l_FloatNum;
        gTable1InvSqrt[i] = (U8)((l_UnkNum >> (23 - 8)) & 0xffUL);
    }

    for (i = 0; i < TABLESIZE_ACOS; ++i) {
        U32 l_UnkNum = (0x7UL << 27) | (i << (27 - BITCOUNT_ACOS));
        Float l_FloatNum = *(Float*)&l_UnkNum;
        gTableAcos[i] = (Float)acos(1.F - l_FloatNum);
    }
}

Float O_Atan2(Float i_X, Float i_Y) {
    Float l_Norm2 = i_X * i_X + i_Y * i_Y;
    Float l_InvNorm = InvSqrt(l_Norm2);
    l_InvNorm = l_InvNorm * (0.5f * (1.0f - l_Norm2 * l_InvNorm * l_InvNorm)) + l_InvNorm;
    i_Y *= l_InvNorm;
    i_X *= l_InvNorm;
    Float l_Result = O_ACos(i_Y);
    if (i_X < 0.0f) {
        l_Result = -l_Result;
    }
    return l_Result;
}

Float O_Cos(Float i_Angle) {
    return O_CosCore(i_Angle);
}

Float O_Sin(Float i_Angle) {
    return O_SinCore(i_Angle - 1.5707963267948966);
}

Float O_Tan(Float i_Angle) {
    Float l_SinAngle = i_Angle - 1.5707963267948966;
    Float l_Sin = O_SinCore(l_SinAngle);
    Float l_Cos = O_CosCore(i_Angle);
    return l_Sin / l_Cos;
}

void O_SinCos(Vec2f& o_Result, Float i_Angle) {
    Float l_SinAngle = i_Angle - 1.5707963267948966;
    o_Result.x = O_SinCore(l_SinAngle);
    o_Result.y = O_CosCore(i_Angle);
}

Float O_ACos(Float i_Value) {
    Float l_ClampedValue = Clamp<Float>(i_Value, -1.0f, 1.0f);
    Float l_Delta = 1.0f - (Float)Abs(l_ClampedValue);
    U32 l_DeltaBits = *(U32*)&l_Delta;
    l_DeltaBits &= 0x7fffffff;
    Float l_Radicand = l_Delta + l_Delta - l_Delta * l_Delta;
    Float l_InvSqrt = InvSqrt(l_Radicand);

    Float l_TableDelta;
    Float l_TableValue;
    if (l_DeltaBits < 0x38000000) {
        l_InvSqrt = 0.5f * l_InvSqrt * -(l_InvSqrt * l_Radicand * l_InvSqrt - 1.0f) + l_InvSqrt;
        l_TableValue = 0.0f;
        l_TableDelta = l_TableValue;
    }
    else {
        U32 l_TableIndex = (l_DeltaBits >> 21) & (TABLESIZE_ACOS - 1);
        l_DeltaBits = (l_DeltaBits & 0x07e00000) | 0x38000000;
        l_TableDelta = *(Float*)&l_DeltaBits;
        l_TableValue = gTableAcos[l_TableIndex];
    }

    l_TableValue = l_InvSqrt * (l_Delta - l_TableDelta) + l_TableValue;
    if (l_ClampedValue < 0.0f) {
        l_TableValue = Pi - l_TableValue;
    }
    return l_TableValue;
}
