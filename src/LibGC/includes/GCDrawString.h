#ifndef _GCDRAWSTRING_H_
#define _GCDRAWSTRING_H_
#include "Types_Z.h"
#include "Material_ZHdl.h"

struct ScanCode_Z {
    Float m_U1;
    Float m_V1;
    Float m_U2;
    Float m_V2;
    Float m_SizeX;
    Float m_SizeY;
    Bool m_Valid;
};

struct FontString_Z {
    Material_ZHdl m_MaterialHdl;
    ScanCode_Z m_Characters[256];

    void MarkHandles();
    void Init();
};

#endif
