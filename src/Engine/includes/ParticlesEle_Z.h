#ifndef _PARTICLESELE_Z_H_
#define _PARTICLESELE_Z_H_
#include "DynArray_Z.h"

struct ParticlesEle_Z {
    U8 m_Pad_0x0[168];
};

typedef DynArray_Z<ParticlesEle_Z, 1> ParticlesEle_ZDA;

class ParticlesDataEle_Z {
public:
    U8 m_Unk_0x0[288];
};

typedef DynArray_Z<ParticlesDataEle_Z, 1> ParticlesDataEle_ZDA;

#endif // _PARTICLESELE_Z_H_
