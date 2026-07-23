#ifndef _UNPACK_Z_H_
#define _UNPACK_Z_H_
#include "Types_Z.h"

class UnPack_Z {
    UnPack_Z(const U8*, U8);
    ~UnPack_Z();
    void Flush();
    U8* DecodeRS();
};

#endif
