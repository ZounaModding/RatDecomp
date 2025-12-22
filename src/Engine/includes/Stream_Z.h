#ifndef _STREAM_Z_H_
#define _STREAM_Z_H_
#include "Types_Z.h"

struct StrBuffer {
    void* m_RealPtr;
    void* m_AlignedPtr;
};

class BaseStream_Z {
public:
    BaseStream_Z()
        : m_OpenId(-1)
        , m_ReadId(-1) {
    }

    virtual void StreamOpened(S32 i_FileSize, S32 i_UserDefined) { Opened(0, i_FileSize, i_UserDefined); }

    virtual void StreamReaded(S32 i_NbBytesRead, S32 i_UserDefined) { Readed(0, i_NbBytesRead, i_UserDefined); }

    virtual void Opened(S32 i_ErrorCode, S32 i_FileSize, S32 i_UserDefined) { }

    virtual void Readed(S32 i_ErrorCode, S32 i_NbBytesRead, S32 i_UserDefined) { }

    BaseStream_Z& operator=(const BaseStream_Z& i_Stream);

private:
    S16 m_OpenId;
    S16 m_ReadId;
};

class Stream_Z : public BaseStream_Z {
public:
    Stream_Z()
        : m_LoadStage(0)
        , m_ErrorCode(0) {
    }

    virtual void Open(const Char* i_Buffer);                        /* 0x18 */
    virtual void Process(void* o_Buffer, S32 i_Offset, S32 i_Size); /* 0x1C */
    virtual void Close();                                           /* 0x20 */
    virtual void Update();                                          /* 0x24 */

    S32 GetErrorCode() const { return m_ErrorCode; }

private:
    S32 m_LoadStage;
    S32 m_ErrorCode;
    S32 m_FileSize;
};

#endif // _STREAM_Z_H_
