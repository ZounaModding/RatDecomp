#ifndef _RANDOM_Z_H_
#define _RANDOM_Z_H_
#include "Random_Z.h"
#include "Types_Z.h"

#define IA 16807
#define IM 0x7FFFFFFF
#define IQ 127773
#define IR 2836
#define NTAB 32
#define NDIV (1 + ((IM - 1) / NTAB)) // 0x4000000
#define AM (1.0f / IM)               // 4.6566129e-10
#define EPS 1.2e-7f
#define RNMX (1.0f - EPS) // 0.99999988

struct gQRandomStruct_Z {
public:
    gQRandomStruct_Z() {
        gQRandom1 = 2.0f;
        gQRandom2 = 0.5;
    }

    Float gQRandom1;
    Float gQRandom2;
};

Float ran1(S32* i_Seed);
void InitRandom(S32 i_Seed);
S32 Alea(S32 i_Min, S32 i_Max);
Float Random();

#endif
