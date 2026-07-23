#ifndef _SKINGC_Z_H_
#define _SKINGC_Z_H_
#include "Skin_Z.h"

class SkinGC_Z : public Skin_Z {
public:
    SkinGC_Z();

    virtual ~SkinGC_Z() { }

    virtual void Draw(DrawInfo_Z& i_DrawInfo, ObjectDatas_Z* i_Data);

    virtual void DrawReceiveShadow(DrawInfo_Z& i_DrawInfo, ObjectDatas_Z* i_Data, S32 i_ObjID) { }

    virtual void DrawCastingShadow(DrawInfo_Z& i_DrawInfo, ObjectDatas_Z* i_Data);
    virtual void UpdateBSphere(const ObjectDatas_ZHdl& i_DataHdl);

    static BaseObject_Z* NewObject() {
        return NewL_Z(26) SkinGC_Z;
    }
};

// clang-format off

// GQR2/GQR4 = S16 scale 12 for paired-single quantized loads/stores. (signed 4.12 fixed-point format)
// GQR3      = S8 scale 6 for paired-single quantized loads/stores. (signed 2.6 fixed-point format)
static inline void SetupSkinnedVertexQuantization() {
#ifdef __MWERKS__
    register U32 arg_lo;
    register U32 arg_hi;

    asm {
        lis arg_lo, 0x0C07
        addi arg_hi, arg_lo, 0x0C07
        mtspr GQR2, arg_hi
        lis arg_lo, 0x0606
        addi arg_lo, arg_lo, 0x0606
        mtspr GQR3, arg_lo
        mtspr GQR4, arg_hi
    }
#endif
}

// clang-format on

#endif // _SKINGC_Z_H_
