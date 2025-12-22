#ifndef _STREAMING_Z_H_
#define _STREAMING_Z_H_
#include "Stream_Z.h"

class GCStream_Z : public Stream_Z {
public:
    virtual ~GCStream_Z(); /* 0x1C */

    virtual void Opened(S32 i_ErrorCode, S32 i_FileSize, S32 i_UserDefined);    /* 0x08 */
    virtual void Readed(S32 i_ErrorCode, S32 i_NbBytesRead, S32 i_UserDefined); /* 0x0C */
    virtual void Open(const Char* i_Buffer);                                    /* 0x10 */
    virtual void Process(void* o_Buffer, S32 i_Offset, S32 i_Size);             /* 0x14 */
    virtual void Close();                                                       /* 0x18 */
};

typedef GCStream_Z StreamX_Z;

#endif // _STREAMING_Z_H_
