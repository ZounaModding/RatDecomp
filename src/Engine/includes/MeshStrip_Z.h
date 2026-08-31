#ifndef _MESHSTRIP_Z_H_
#define _MESHSTRIP_Z_H_

#include "DynArray_Z.h"

#define FL_STRIP_CW (1 << 0)
#define FL_STRIP_CCW (1 << 1)
#define INVALID_MATERIAL_ID (U32_MINUS_ONE)

class Vtx_Z {
public:
    U16 m_UVId;
    U16 m_NormalId;
    U16 m_ColorId;
    U16 m_VertexId;
};

class Strip_Z {
public:
    Strip_Z() {
        FIXDEBUGINLINING_Z();
        m_Flags = 0;
    }

    DynArray_Z<U16, 32, FALSE, FALSE> m_VertexIndices;
    U32 m_MaterialId;
    U32 m_Flags;
};

class StripExt_Z {
public:
    StripExt_Z() {
        FIXDEBUGINLINING_Z();
    }

    DynArray_Z<Vtx_Z, 32, FALSE, FALSE> m_Vertices;
};

typedef DynArray_Z<Strip_Z, 64> Strip_ZDA;
typedef DynArray_Z<StripExt_Z, 64> StripExt_ZDA;

#endif
