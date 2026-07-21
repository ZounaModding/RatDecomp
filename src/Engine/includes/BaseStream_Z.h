#ifndef _BASESTREAM_Z_H_
#define _BASESTREAM_Z_H_

#include "Types_Z.h"

union StreamID {
    int GblID;

    struct {
        int m_StreamId : 16,
            m_ReadId : 16;
    } Ref;
};

class BaseStream_Z {
private:
    StreamID m_ID;

public:
    BaseStream_Z() {
        m_ID.Ref.m_StreamId = -1;
        m_ID.Ref.m_ReadId = -1;
    }

    virtual void StreamOpened(S32 i_FileSize, S32 i_UserDefined) {
        Opened(0, i_FileSize, i_UserDefined);
    }

    virtual void StreamReaded(S32 i_NbBytesRead, S32 i_UserDefined) {
        Readed(0, i_NbBytesRead, i_UserDefined);
    }

    virtual void Opened(S32 i_ErrorCode, S32 i_FileSize, S32 i_UserDefined) { }

    virtual void Readed(S32 i_ErrorCode, S32 i_NbBytesRead, S32 i_UserDefined) { }

    const StreamID& GetID() const {
        return m_ID;
    }

    void SetStreamId(S16 i_StreamId) {
        m_ID.Ref.m_StreamId = i_StreamId;
    }

    void SetReadId(S16 i_ReadId) {
        m_ID.Ref.m_ReadId = i_ReadId;
    }

    BaseStream_Z& operator=(const BaseStream_Z& i_Other);
};

#endif
