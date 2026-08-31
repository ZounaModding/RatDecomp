#ifndef _UNPACK_Z_H_
#define _UNPACK_Z_H_
#include "Types_Z.h"

class UnPack_Z {
public:
    UnPack_Z(const U8* i_PackedData, U8* o_UnpackedData);
    ~UnPack_Z();
    void Flush();
    U8* DecodeRS();

private:
    U8* m_UnPackedBuffer;
    U32 m_UnPackedSize;
    U32 m_PackedSize;
    Bool m_AllocatedUnPackedBuffer;
    Bool m_UnkBool_0xd;
};

#endif
