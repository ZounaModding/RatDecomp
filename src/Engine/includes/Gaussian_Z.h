#ifndef _GAUSSIAN_Z_H_
#define _GAUSSIAN_Z_H_
#include "Types_Z.h"
#include "Math_Z.h"

// $SABE: Fake name
struct MipmapGCEle {
    Float m_Weight;
    S32 m_Level;
    Bool m_Enabled;
};

struct MipmapGC {
    MipmapGCEle m_Eles[16];
};

S32 Compute_MipmapedGaussConvol(MipmapGC* i_Mip, int i_SizeX, int i_SizeY, Float i_UnkFloat, Float i_UnkFloat2); // $VIOLET: Maybe i_Sigma and i_Alpha? idrk

#endif
