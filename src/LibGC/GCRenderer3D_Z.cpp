#include "GCRenderer_Z.h"
#include <dolphin.h>

void GCRenderer_Z::DrawFace(Vertex3D& a1, Vertex3D& a2, Vertex3D& a3) {
}

void GCRenderer_Z::DrawStrip(Vertex3D* a1, U32 a2, Bool a3) {
}

void GCRenderer_Z::DrawStripAfterScene(Vertex3D* a1, U32 a2, Bool a3) {
}

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
