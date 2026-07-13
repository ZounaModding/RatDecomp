#ifndef _STREAMFILE_Z_H_
#define _STREAMFILE_Z_H_
#include "Manipulator_Z.h"
#include "StreamAgent_ZHdl.h"
#include "Streaming_Z.h"
#include "ClassManager_Z.h"
#include "DynPtrArray_Z.h"
#include "Types_Z.h"

#define STR_FILE_LOAD_STAGE_ABORT -1
#define STR_FILE_LOAD_STAGE_INIT 0
#define STR_FILE_LOAD_STAGE_READ_HEADER 1
#define STR_FILE_LOAD_STAGE_PARSE_HEADER 2
#define STR_FILE_LOAD_STAGE_READ_BLOCK 3
#define STR_FILE_LOAD_STAGE_LOAD_RESOURCES 4
#define STR_FILE_LOAD_STAGE_ENDLOAD 5
#define STR_FILE_LOAD_STAGE_AFTERENDLOAD 6
#define STR_FILE_LOAD_STAGE_LOADDONE 7
#define STR_FILE_LOAD_STAGE_FINISHED 8

#define STR_LOAD_STAGE_INIT 0
#define STR_LOAD_STAGE_PROCESSING 1
#define STR_LOAD_STAGE_UNK_2 2
#define STR_LOAD_STAGE_ERROR 3

#define STR_FILE_ERROR_NONE 0

#define BF_HEADER_FLAG_COMPRESSED (1 << 0) // Does the bigfile have compressed objects

struct BlockDescription_Z {
    S32 m_ObjectCount;
    S32 m_PaddedSize;
    U32 m_DataSize;
    U32 m_WorkingBufferOffset;
    Name_Z m_FirstResourceName;
    U32 m_BlockHash;
};

struct BigFileHeader_Z {
    Char m_VersionString[ARRAY_CHAR_MAX];
    U32 m_Flag;
    S32 m_BlockCount;
    U32 m_WorkingBufferCapacityEven;
    U32 m_WorkingBufferCapacityOdd;
    U32 m_PaddedSize;
    U32 m_VersionPatch;
    U32 m_VersionMinor;
    U32 m_UnkS32_0x11c;
    BlockDescription_Z m_Blocks[64];
    U32 m_UnkS32_0x720;
    U32 m_PoolManifestPaddedSize;
} Aligned_Z(2048);

#define g_BigFileHeader ((BigFileHeader_Z&)StreamFile_Z::gBigFileHeader)

class StreamFile_Z : public Manipulator_Z {
public:
    StreamFile_Z() { }

    virtual ~StreamFile_Z();
    virtual void Init();
    virtual void Update(Float i_DeltaTime);

    static BaseObject_Z* NewObject() { return NewL_Z(56) StreamFile_Z; }

    void Start(const Char* i_FilePath, const StreamAgent_ZHdl& i_AgentHdl);
    void Resume();
    Bool GetHeader();
    void Read(S32 i_BlockIdx);
    void End();
    void Aborted();

    Stream_Z& GetStream() { return m_Str; }

    void SetLoadStage(S32 i_LoadStage) { m_LoadStage = i_LoadStage; }

    void ResetBuffers() {
        if (m_Buf[0].m_RealPtr) {
            Free_Z(m_Buf[0].m_RealPtr);
            m_Buf[0].m_RealPtr = NULL;
        }
        if (m_Buf[1].m_RealPtr) {
            Free_Z(m_Buf[1].m_RealPtr);
            m_Buf[1].m_RealPtr = NULL;
        }
    }

private:
    S32 m_LoadStage;
    S32 m_LoadStageBeforeResume;
    S32 m_BlockOffsetToRead;
    StreamX_Z m_Str;
    StrBuffer m_Buf[2];
    S32 m_CurBlockIdx;
    BigFileRsc_Z m_CurResource;
    StreamAgent_ZHdl m_AgentHdl;
    String_Z<ARRAY_CHAR_MAX> m_BfPath;
    DynPtrArray_Z<BaseObject_Z*, 32> m_Resources;
    S32 m_CurResourceIdx;
    S32 m_UnkS32_0x174;
    Bool m_RestoreVsync;

    static U8 gBigFileHeader[sizeof(BigFileHeader_Z)];
};

#endif // _STREAMFILE_Z_H_
