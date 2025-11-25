#ifndef _MESHMORPH_Z_H_
#define _MESHMORPH_Z_H_
#include "Name_Z.h"
#include "Math_Z.h"

struct MorphTarget_Z {
    Name_Z m_MeshName;
    Float m_Morph;
};

// $SABE: Probably wrong, no way to tell
class MorphValue_Z {
public:
    MorphValue_Z() { }

private:
    U32 m_VertexIndex;
    Vec3f m_Delta;
};

class MorphTargetDesc_Z {
public:
    MorphTargetDesc_Z()
        : m_MeshName() { }

    void Load(void** i_Data);

private:
    Name_Z m_MeshName;
    U16 m_MeshID;
    DynArray_Z<MorphValue_Z, 64> m_MorphValues;
};

class Morpher_Z {
public:
    Morpher_Z() { }

    void Load(void** i_Data);

private:
    DynArray_Z<MorphValue_Z, 64> m_MorphValues;
    DynArray_Z<MorphTargetDesc_Z, 64> m_MorphTargetDescDA;
};

#endif // _MESHMORPH_Z_H_
