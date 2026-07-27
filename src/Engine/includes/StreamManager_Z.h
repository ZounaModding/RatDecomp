#ifndef _STREAMMANAGER_Z_H_
#define _STREAMMANAGER_Z_H_
#include "Assert_Z.h"
#include <String_Z.h>
#include "FileHdl_Z.h"
#include "Mutex_Z.h"
#include "StaticArray_Z.h"
#include "BaseStream_Z.h"

#define STR_MAX_READ_SIZE 0x100000
#define STR_MAX_OPERATIONS 32

enum StreamError {
    STR_ERROR_NONE = 0,
    STR_ERROR_UNKNOWN = 1
};

class StreamManager_Z {
public:
    StreamManager_Z();
    ~StreamManager_Z();

    static S16 readId;

    struct OpenOp {
        enum Status {
            Start = 0,
            Failed = 1,
            Retry = 2,
            Opened = 3,
            Open = 4,
            CloseRequested = 5,
            Closed = 6,
            Finished = 7
        };

        OpenOp() {
        }

        Bool NeedsProcessing() const {
            return m_Status == CloseRequested || m_Status == Start || m_Status == Failed;
        }

        S32 m_Status;
        String_Z<ARRAY_CHAR_MAX> m_FilePath;
        FileHdl_Z m_FileHdl;
        S32 m_UserDefined;
        S32 m_FileSize;
        BaseStream_Z* m_OpenStreamInterface;
        BaseStream_Z* m_CloseStreamInterface;
        void* m_UnkPtr_0x160;
        S32 m_UnkS32_0x164;
    };

    struct ReadOp {
        enum Status {
            Start = 0,
            Failed = 1,
            Readed = 2,
            CloseRequested = 3,
            Finished = 4,
        };

        ReadOp() {
            m_Status = Start;
        }

        Bool NeedsProcessing() const {
            return m_Status == Start;
        }

        StreamID& GetStreamID() {
            return m_StreamID;
        }

        S32 m_Status;
        StreamID m_StreamID;
        S32 m_UserDefined;
        Char* m_Buffer;
        Char* m_StartOffset;
        S32 m_BytesToRead;
        S32 m_BytesRead;
        BaseStream_Z* m_Stream;
    };

    Bool Init();

    Bool Heartbeat(Float i_DeltaTime);
    void Update(Float i_DeltaTime);
    void Thread();
    static void* ThreadProcVoid(void* i_Param);

    Bool Open(const Char* i_FilePath, BaseStream_Z* i_Stream, void* i_UnkPtr_0x160, S32 i_UnkS32_0x164, S32 i_UserDefined);
    Bool Read(void* i_Buffer, S32 i_StartOffset, S32 i_BytesToRead, BaseStream_Z* i_Stream, S32 i_UserDefined);
    Bool Close(BaseStream_Z* i_Stream, Bool i_CloseAllReadsForFile);

    U32 GetLastError() {
        return m_LastError;
    }

private:
    U32 m_UnkU32_0x0;
    Bool m_Finished;
    Bool m_Retrying;
    Bool m_SuccessfulRetry;
    U32 m_LastError;
    SharedResource_Z m_StreamMutex;
    SharedResource_Z m_ReadMutex;
    Event_Z m_ReadyToCloseEvent;
    Event_Z m_UnkEvent_0x18;
    StaticArray_Z<ReadOp, STR_MAX_OPERATIONS> m_ReadOps;
    StaticArray_Z<OpenOp, STR_MAX_OPERATIONS> m_OpenOps;
};

#endif // _STREAMMANAGER_Z_H_
