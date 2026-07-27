#ifndef _DEBUGFONTSTRING_Z_H_
#define _DEBUGFONTSTRING_Z_H_
#include "Types_Z.h"
#include "Material_ZHdl.h"

#define DEBUG_FONT_LINE_ADVANCE 10.0f

struct ScanCode_Z {
    Float m_U1;
    Float m_V1;
    Float m_U2;
    Float m_V2;
    Float m_SizeX;
    Float m_SizeY;
    Bool m_Valid;
};

struct DebugFontString_Z {
    Material_ZHdl m_MaterialHdl;
    ScanCode_Z m_Characters[256];

    void MarkHandles();
    void Init();
};

#endif
