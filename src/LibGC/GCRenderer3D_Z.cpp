#include "GCRenderer_Z.h"
#include <dolphin.h>

void Draw3D_Z::End() {
    m_CurDisplayList = NULL;
    PPCSync();
}

void Draw3D_Z::EndRender() {
    m_CurDisplayList = NULL;
    PPCSync();
    m_Vtx3DBufferDA[0].Empty();
    m_Vtx3DBufferDA[1].Empty();
}
