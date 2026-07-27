#ifndef _STREAMING_Z_H_
#define _STREAMING_Z_H_
#include "Stream_Z.h"

class DCStream_Z : public Stream_Z {
public:
    virtual ~DCStream_Z() {
        Close();
    }

    virtual void Opened(S32 i_ErrorCode, S32 i_FileSize, S32 i_UserDefined);
    virtual void Readed(S32 i_ErrorCode, S32 i_NbBytesRead, S32 i_UserDefined);
    virtual void Open(const Char* i_Buffer);
    virtual void Process(void* o_Buffer, S32 i_Offset, S32 i_BlockCount);
    virtual void Close();
};

typedef DCStream_Z StreamX_Z;

#endif // _STREAMING_Z_H_
