#include "WarpGC_Z.h"

WarpGC_Z::WarpGC_Z() {
    m_TextureCoordinates[0].Set(0.0f, 0.0f);
    m_TextureCoordinates[1].Set(1.0f, 0.0f);
    m_TextureCoordinates[2].Set(0.0f, 1.0f);
    m_TextureCoordinates[3].Set(1.0f, 1.0f);
}

WarpGC_Z::~WarpGC_Z() {
}

void WarpGC_Z::EndLoad() {
    Warp_Z::EndLoad();
    BuildDisplayList();
}

void WarpGC_Z::BuildDisplayList() {
}
